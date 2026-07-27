#!/usr/bin/env python3
"""Fail-closed native object and link reuse for immutable generated C.

Each C translation unit is compiled with a compiler-produced dependency file.
The cache never guesses which headers matter: it re-hashes that exact closure,
including system headers, before accepting an object. It also inventories the
*names* visible through ordered include roots so adding a new shadowing header
cannot leave an older include resolution cached.

A small mutable index maps the cheap pre-compilation key to an immutable final
entry. The index is only a hint. Missing or malformed metadata, dependencies,
objects, or executables are cache misses; failed compilation and linking never
publish an entry. This module owns native build mechanics only. Haxe/haxe.c
semantics and generated-project ownership remain separate earlier layers.
"""

from __future__ import annotations

import concurrent.futures
import hashlib
import json
import os
import re
import shutil
import subprocess
import tempfile
import uuid
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Mapping, Sequence

from dev_build_state import (
    atomic_write_state,
    canonical_digest,
    sha256_file,
)


OBJECT_KIND = "caxecraft-native-object"
LINK_KIND = "caxecraft-native-link"
INDEX_KIND = "caxecraft-native-object-index"
NATIVE_SCHEMA_VERSION = 1
ENVIRONMENT_KEYS = (
    "CPATH",
    "C_INCLUDE_PATH",
    "CPLUS_INCLUDE_PATH",
    "MACOSX_DEPLOYMENT_TARGET",
    "SDKROOT",
)


class NativeCacheFailure(RuntimeError):
    """A native input, dependency record, compile, or link is unsafe."""


@dataclass(frozen=True)
class IncludeRoot:
    """One ordered include search root with a path-independent logical name."""

    logical_name: str
    path: Path


@dataclass(frozen=True)
class DependencyRoot:
    """One root used to rebind depfile paths to a new generated generation."""

    logical_name: str
    path: Path


@dataclass(frozen=True)
class NativeObject:
    """One validated immutable object selected for the ordered link."""

    source: str
    key: str
    path: Path
    sha256: str
    cache_hit: bool


@dataclass(frozen=True)
class NativeBuildResult:
    """Ordered objects, linked-output status, and concise cache observability."""

    objects: tuple[NativeObject, ...]
    object_hits: int
    object_misses: int
    link_hit: bool


def _logical(value: str, label: str) -> str:
    path = PurePosixPath(value)
    if (
        path.is_absolute()
        or path.as_posix() != value
        or any(part in ("", ".", "..") for part in path.parts)
    ):
        raise NativeCacheFailure(f"{label} must be a normalized relative path")
    return value


def _real_file(path: Path, label: str) -> Path:
    if path.is_symlink() or not path.is_file():
        raise NativeCacheFailure(f"{label} is missing or not a regular file: {path}")
    return path


def _real_directory(path: Path, label: str, *, create: bool = False) -> Path:
    if create:
        path.mkdir(parents=True, exist_ok=True)
    if path.is_symlink() or not path.is_dir():
        raise NativeCacheFailure(f"{label} must be a real directory: {path}")
    return path


def _tool_output(arguments: Sequence[str], label: str) -> str:
    try:
        result = subprocess.run(
            list(arguments),
            check=False,
            capture_output=True,
            text=True,
            timeout=10,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise NativeCacheFailure(f"cannot identify {label}: {error}") from error
    value = "\n".join(
        part.strip() for part in (result.stdout, result.stderr) if part.strip()
    )
    if result.returncode != 0 or not value:
        raise NativeCacheFailure(
            f"cannot identify {label}: exit={result.returncode}, output={value!r}"
        )
    return value


def compiler_identity(compiler: str) -> dict[str, object]:
    """Capture compiler bytes, reported version, target, and relevant environment."""

    candidate = Path(compiler)
    located = candidate if candidate.parent != Path(".") else Path(shutil.which(compiler) or "")
    if not str(located):
        raise NativeCacheFailure(f"C compiler is unavailable: {compiler}")
    executable = _real_file(located.resolve(), "C compiler")
    target = _tool_output((str(executable), "-dumpmachine"), "C compiler target")
    version = _tool_output((str(executable), "--version"), "C compiler version")
    return {
        "schemaVersion": 1,
        "command": compiler,
        "path": str(executable),
        "bytes": executable.stat().st_size,
        "sha256": sha256_file(executable),
        "target": target,
        "version": version,
        "environment": {
            key: os.environ[key]
            for key in ENVIRONMENT_KEYS
            if key in os.environ
        },
    }


def include_path_inventory(roots: Sequence[IncludeRoot]) -> dict[str, object]:
    """Inventory visible path names, not bytes, to detect include shadowing.

    Header contents belong to each object's compiler-produced dependency
    closure. Path names are a separate input because a newly added earlier-root
    header can change which file an unchanged ``#include`` resolves to.
    """

    records: list[dict[str, object]] = []
    names: set[str] = set()
    for order, root in enumerate(roots):
        logical = _logical(root.logical_name, "include-root name")
        if logical in names:
            raise NativeCacheFailure(f"include-root name is duplicated: {logical}")
        names.add(logical)
        directory = _real_directory(root.path, f"include root {logical}")
        paths: list[str] = []
        for candidate in directory.rglob("*"):
            relative = candidate.relative_to(directory).as_posix()
            if candidate.is_symlink():
                raise NativeCacheFailure(
                    f"include root {logical} contains a symlink: {relative}"
                )
            if candidate.is_file():
                paths.append(relative)
        records.append(
            {
                "order": order,
                "name": logical,
                "paths": sorted(paths, key=lambda value: value.encode("utf-8")),
            }
        )
    body: dict[str, object] = {"schemaVersion": 1, "roots": records}
    return {**body, "sha256": canonical_digest(body)}


def _parse_depfile(path: Path) -> tuple[Path, ...]:
    """Parse the Make-style dependency form emitted by GCC and Clang."""

    _real_file(path, "compiler dependency file")
    try:
        text = path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise NativeCacheFailure(f"cannot read compiler dependency file: {error}") from error
    text = text.replace("\\\r\n", "").replace("\\\n", "")
    separator = None
    escaped = False
    for index, character in enumerate(text):
        if escaped:
            escaped = False
        elif character == "\\":
            escaped = True
        elif character == ":":
            separator = index
            break
    if separator is None:
        raise NativeCacheFailure("compiler dependency file omitted its target separator")
    raw = text[separator + 1 :]
    tokens: list[str] = []
    current: list[str] = []
    escaped = False
    for character in raw:
        if escaped:
            current.append(character)
            escaped = False
        elif character == "\\":
            escaped = True
        elif character.isspace():
            if current:
                tokens.append("".join(current))
                current = []
        else:
            current.append(character)
    if escaped:
        raise NativeCacheFailure("compiler dependency file ends with an escape")
    if current:
        tokens.append("".join(current))
    if not tokens:
        raise NativeCacheFailure("compiler dependency file contains no dependencies")
    values: list[Path] = []
    seen: set[Path] = set()
    for token in tokens:
        dependency = Path(token)
        if not dependency.is_absolute():
            dependency = (path.parent / dependency).resolve()
        else:
            dependency = dependency.resolve()
        if dependency not in seen:
            seen.add(dependency)
            values.append(dependency)
    return tuple(values)


def _under(path: Path, root: Path) -> Path | None:
    try:
        return path.relative_to(root)
    except ValueError:
        return None


def dependency_snapshot(
    dependencies: Sequence[Path],
    roots: Sequence[DependencyRoot],
) -> list[dict[str, object]]:
    """Convert depfile paths to stable logical paths and hash every dependency."""

    normalized_roots = [
        DependencyRoot(
            _logical(root.logical_name, "dependency-root name"),
            _real_directory(root.path, f"dependency root {root.logical_name}").resolve(),
        )
        for root in roots
    ]
    normalized_roots.sort(
        key=lambda root: len(root.path.parts),
        reverse=True,
    )
    records: list[dict[str, object]] = []
    seen: set[str] = set()
    for dependency in dependencies:
        resolved = _real_file(dependency.resolve(), "native dependency")
        logical: str | None = None
        source: str | None = None
        for root in normalized_roots:
            relative = _under(resolved, root.path)
            if relative is not None:
                logical = PurePosixPath(root.logical_name, *relative.parts).as_posix()
                break
        if logical is None:
            logical = f"system/{resolved.as_posix().lstrip('/')}"
            source = str(resolved)
        if logical in seen:
            continue
        seen.add(logical)
        record: dict[str, object] = {
            "path": logical,
            "bytes": resolved.stat().st_size,
            "sha256": sha256_file(resolved),
        }
        if source is not None:
            record["source"] = source
        records.append(record)
    records.sort(key=lambda record: str(record["path"]).encode("utf-8"))
    return records


def _current_dependencies(
    stored: object,
    roots: Sequence[DependencyRoot],
) -> list[dict[str, object]]:
    if not isinstance(stored, list):
        raise NativeCacheFailure("object dependency inventory is malformed")
    root_by_name = {
        _logical(root.logical_name, "dependency-root name"): _real_directory(
            root.path, f"dependency root {root.logical_name}"
        )
        for root in roots
    }
    dependencies: list[Path] = []
    for record in stored:
        if not isinstance(record, dict) or not isinstance(record.get("path"), str):
            raise NativeCacheFailure("object dependency record is malformed")
        logical = str(record["path"])
        source = record.get("source")
        if source is not None:
            if not isinstance(source, str):
                raise NativeCacheFailure("system dependency source is malformed")
            dependencies.append(Path(source))
            continue
        matched = False
        for root_name, root_path in root_by_name.items():
            prefix = f"{root_name}/"
            if logical.startswith(prefix):
                relative = PurePosixPath(logical[len(prefix) :])
                if any(part in ("", ".", "..") for part in relative.parts):
                    raise NativeCacheFailure("logical dependency path is malformed")
                dependencies.append(root_path.joinpath(*relative.parts))
                matched = True
                break
        if not matched:
            raise NativeCacheFailure(f"unknown dependency root in {logical}")
    return dependency_snapshot(dependencies, roots)


def _load_json(path: Path, label: str) -> dict[str, object]:
    _real_file(path, label)
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise NativeCacheFailure(f"cannot read {label}: {error}") from error
    if not isinstance(value, dict):
        raise NativeCacheFailure(f"{label} must contain a JSON object")
    digest = value.get("sha256")
    body = {key: item for key, item in value.items() if key != "sha256"}
    if not isinstance(digest, str) or digest != canonical_digest(body):
        raise NativeCacheFailure(f"{label} digest is invalid")
    return value


def _quarantine(cache_root: Path, path: Path) -> None:
    quarantine = _real_directory(
        cache_root / "quarantine", "native-cache quarantine", create=True
    )
    target = quarantine / f"{path.name}-{uuid.uuid4().hex}"
    os.replace(path, target)


def _publish_directory(cache_root: Path, transaction: Path, target: Path) -> None:
    target.parent.mkdir(parents=True, exist_ok=True)
    if target.exists():
        if target.is_symlink() or not target.is_dir():
            _quarantine(cache_root, target)
        else:
            _quarantine(cache_root, target)
    os.replace(transaction, target)


def _atomic_copy(source: Path, target: Path) -> None:
    target.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(
        dir=target.parent, prefix=f".{target.name}.", suffix=".tmp"
    )
    os.close(descriptor)
    temporary = Path(temporary_name)
    try:
        shutil.copy2(source, temporary)
        os.replace(temporary, target)
    finally:
        temporary.unlink(missing_ok=True)


class NativeCache:
    """Compile and link through one per-worktree immutable cache."""

    def __init__(
        self,
        cache_root: Path,
        *,
        compiler: str,
        compile_flags: Sequence[str],
        include_roots: Sequence[IncludeRoot],
        dependency_roots: Sequence[DependencyRoot],
        jobs: int,
        enabled: bool = True,
    ):
        if jobs < 1 or jobs > 32:
            raise NativeCacheFailure("native job count must be between 1 and 32")
        self.cache_root = cache_root
        self.compiler = compiler
        self.compile_flags = tuple(compile_flags)
        self.include_roots = tuple(include_roots)
        self.dependency_roots = tuple(dependency_roots)
        self.jobs = jobs
        self.enabled = enabled
        self.compiler_facts = compiler_identity(compiler)
        self.include_inventory = include_path_inventory(include_roots)
        _real_directory(cache_root, "native cache", create=True)
        for name in ("objects", "object-indexes", "links", "transactions"):
            _real_directory(cache_root / name, f"native cache {name}", create=True)

    def _base_identity(self, source: str, path: Path) -> dict[str, object]:
        _logical(source, "generated source")
        _real_file(path, f"generated source {source}")
        return {
            "schemaVersion": NATIVE_SCHEMA_VERSION,
            "source": source,
            "sourceBytes": path.stat().st_size,
            "sourceSha256": sha256_file(path),
            "compiler": self.compiler_facts,
            "compileFlags": list(self.compile_flags),
            "includeInventory": self.include_inventory,
        }

    def _lookup(self, base: Mapping[str, object]) -> NativeObject | None:
        if not self.enabled:
            return None
        base_key = canonical_digest(base)
        index_path = self.cache_root / "object-indexes" / f"{base_key}.json"
        try:
            index = _load_json(index_path, "native object index")
            if (
                index.get("schemaVersion") != NATIVE_SCHEMA_VERSION
                or index.get("kind") != INDEX_KIND
                or index.get("baseKey") != base_key
                or not isinstance(index.get("objectKey"), str)
            ):
                return None
            object_key = str(index["objectKey"])
            entry = self.cache_root / "objects" / object_key
            metadata = _load_json(entry / "metadata.json", "native object metadata")
            object_path = _real_file(entry / "object.o", "cached native object")
            if (
                metadata.get("schemaVersion") != NATIVE_SCHEMA_VERSION
                or metadata.get("kind") != OBJECT_KIND
                or metadata.get("base") != dict(base)
                or metadata.get("key") != object_key
                or metadata.get("objectSha256") != sha256_file(object_path)
            ):
                return None
            current = _current_dependencies(
                metadata.get("dependencies"), self.dependency_roots
            )
            if current != metadata.get("dependencies"):
                return None
            return NativeObject(
                source=str(base["source"]),
                key=object_key,
                path=object_path,
                sha256=str(metadata["objectSha256"]),
                cache_hit=True,
            )
        except (OSError, NativeCacheFailure):
            return None

    def _compile(self, source: str, path: Path, base: Mapping[str, object]) -> NativeObject:
        transaction = Path(
            tempfile.mkdtemp(
                prefix="object-", dir=self.cache_root / "transactions"
            )
        )
        object_path = transaction / "object.o"
        depfile = transaction / "object.d"
        log_path = transaction / "compile.log"
        arguments = [
            self.compiler,
            *self.compile_flags,
        ]
        for root in self.include_roots:
            arguments.extend(("-I", str(root.path)))
        arguments.extend(
            (
                "-MD",
                "-MF",
                str(depfile),
                "-MT",
                "object.o",
                "-c",
                str(path),
                "-o",
                str(object_path),
            )
        )
        try:
            result = subprocess.run(
                arguments,
                check=False,
                capture_output=True,
                text=True,
                timeout=180,
            )
        except (OSError, subprocess.TimeoutExpired) as error:
            _quarantine(self.cache_root, transaction)
            raise NativeCacheFailure(
                f"native compile of {source} could not finish: {error}"
            ) from error
        log = f"$ {' '.join(arguments)}\n{result.stdout}{result.stderr}"
        log_path.write_text(log, encoding="utf-8")
        if result.returncode != 0:
            _quarantine(self.cache_root, transaction)
            raise NativeCacheFailure(
                f"native compile of {source} failed with exit {result.returncode}\n"
                f"{result.stdout}{result.stderr}"
            )
        _real_file(object_path, f"native object for {source}")
        dependencies = dependency_snapshot(
            _parse_depfile(depfile), self.dependency_roots
        )
        semantic: dict[str, object] = {
            "schemaVersion": NATIVE_SCHEMA_VERSION,
            "kind": OBJECT_KIND,
            "base": dict(base),
            "dependencies": dependencies,
            "objectSha256": sha256_file(object_path),
        }
        object_key = canonical_digest(semantic)
        metadata = {
            **semantic,
            "key": object_key,
        }
        atomic_write_state(
            transaction / "metadata.json",
            {**metadata, "sha256": canonical_digest(metadata)},
        )
        target = self.cache_root / "objects" / object_key
        if self.enabled:
            if target.is_dir() and not target.is_symlink():
                try:
                    existing = _load_json(
                        target / "metadata.json", "existing native object metadata"
                    )
                    existing_object = _real_file(
                        target / "object.o", "existing native object"
                    )
                    if (
                        existing.get("key") == object_key
                        and existing.get("objectSha256") == sha256_file(existing_object)
                    ):
                        shutil.rmtree(transaction)
                    else:
                        _publish_directory(self.cache_root, transaction, target)
                except (OSError, NativeCacheFailure):
                    _publish_directory(self.cache_root, transaction, target)
            else:
                _publish_directory(self.cache_root, transaction, target)
            base_key = canonical_digest(base)
            index_body: dict[str, object] = {
                "schemaVersion": NATIVE_SCHEMA_VERSION,
                "kind": INDEX_KIND,
                "baseKey": base_key,
                "objectKey": object_key,
            }
            atomic_write_state(
                self.cache_root / "object-indexes" / f"{base_key}.json",
                {**index_body, "sha256": canonical_digest(index_body)},
            )
            final_object = target / "object.o"
        else:
            # Cache-off still uses a unique transaction and depfile, but the
            # caller owns its lifetime through the returned temporary object.
            final_object = object_path
        return NativeObject(
            source=source,
            key=object_key,
            path=final_object,
            sha256=sha256_file(final_object),
            cache_hit=False,
        )

    def objects(
        self, sources: Sequence[tuple[str, Path]]
    ) -> tuple[NativeObject, ...]:
        """Reuse or compile sources concurrently, returning canonical input order."""

        planned: list[
            tuple[int, str, Path, dict[str, object], NativeObject | None]
        ] = []
        for index, (source, path) in enumerate(sources):
            base = self._base_identity(source, path)
            planned.append((index, source, path, base, self._lookup(base)))
        results: list[NativeObject | None] = [None] * len(planned)
        misses: list[tuple[int, str, Path, dict[str, object]]] = []
        for index, source, path, base, hit in planned:
            if hit is None:
                misses.append((index, source, path, base))
            else:
                results[index] = hit
        if misses:
            with concurrent.futures.ThreadPoolExecutor(
                max_workers=min(self.jobs, len(misses)),
                thread_name_prefix="caxecraft-native",
            ) as pool:
                futures = {
                    pool.submit(self._compile, source, path, base): index
                    for index, source, path, base in misses
                }
                failures: list[tuple[int, BaseException]] = []
                for future, index in futures.items():
                    try:
                        results[index] = future.result()
                    except BaseException as error:
                        failures.append((index, error))
                if failures:
                    failures.sort(key=lambda item: item[0])
                    raise NativeCacheFailure(str(failures[0][1])) from failures[0][1]
        if any(result is None for result in results):
            raise NativeCacheFailure("native object scheduler omitted a result")
        return tuple(result for result in results if result is not None)

    def link(
        self,
        objects: Sequence[NativeObject],
        *,
        output: Path,
        arguments_after_objects: Sequence[str],
        libraries: Sequence[tuple[str, Path]],
    ) -> bool:
        """Reuse or publish one executable from an ordered, validated link plan."""

        library_records = []
        for logical_name, path in libraries:
            _logical(logical_name, "native library name")
            library = _real_file(path, f"native library {logical_name}")
            library_records.append(
                {
                    "name": logical_name,
                    "path": str(library.resolve()),
                    "bytes": library.stat().st_size,
                    "sha256": sha256_file(library),
                }
            )
        identity: dict[str, object] = {
            "schemaVersion": NATIVE_SCHEMA_VERSION,
            "compiler": self.compiler_facts,
            "objects": [
                {
                    "source": item.source,
                    "key": item.key,
                    "sha256": item.sha256,
                }
                for item in objects
            ],
            "argumentsAfterObjects": list(arguments_after_objects),
            "libraries": library_records,
        }
        link_key = canonical_digest(identity)
        target = self.cache_root / "links" / link_key
        if self.enabled and target.is_dir() and not target.is_symlink():
            try:
                metadata = _load_json(target / "metadata.json", "native link metadata")
                executable = _real_file(
                    target / "caxecraft", "cached native executable"
                )
                if (
                    metadata.get("schemaVersion") == NATIVE_SCHEMA_VERSION
                    and metadata.get("kind") == LINK_KIND
                    and metadata.get("identity") == identity
                    and metadata.get("key") == link_key
                    and metadata.get("executableSha256") == sha256_file(executable)
                ):
                    _atomic_copy(executable, output)
                    return True
            except (OSError, NativeCacheFailure):
                pass

        transaction = Path(
            tempfile.mkdtemp(prefix="link-", dir=self.cache_root / "transactions")
        )
        executable = transaction / "caxecraft"
        arguments = [
            self.compiler,
            *[str(item.path) for item in objects],
            *arguments_after_objects,
            "-o",
            str(executable),
        ]
        try:
            result = subprocess.run(
                arguments,
                check=False,
                capture_output=True,
                text=True,
                timeout=180,
            )
        except (OSError, subprocess.TimeoutExpired) as error:
            _quarantine(self.cache_root, transaction)
            raise NativeCacheFailure(
                f"Caxecraft native link could not finish: {error}"
            ) from error
        (transaction / "link.log").write_text(
            f"$ {' '.join(arguments)}\n{result.stdout}{result.stderr}",
            encoding="utf-8",
        )
        if result.returncode != 0:
            _quarantine(self.cache_root, transaction)
            raise NativeCacheFailure(
                f"Caxecraft native link failed with exit {result.returncode}\n"
                f"{result.stdout}{result.stderr}"
            )
        _real_file(executable, "linked Caxecraft executable")
        metadata: dict[str, object] = {
            "schemaVersion": NATIVE_SCHEMA_VERSION,
            "kind": LINK_KIND,
            "identity": identity,
            "key": link_key,
            "executableSha256": sha256_file(executable),
        }
        atomic_write_state(
            transaction / "metadata.json",
            {**metadata, "sha256": canonical_digest(metadata)},
        )
        if self.enabled:
            _publish_directory(self.cache_root, transaction, target)
            executable = target / "caxecraft"
        _atomic_copy(executable, output)
        if not self.enabled:
            shutil.rmtree(transaction)
            for item in objects:
                if item.path.parent.parent == self.cache_root / "transactions":
                    shutil.rmtree(item.path.parent, ignore_errors=True)
        return False
