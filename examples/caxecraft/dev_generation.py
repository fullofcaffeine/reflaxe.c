#!/usr/bin/env python3
"""Crash-safe immutable generated projects for the Caxecraft edit loop.

Haxe and haxe.c write many related files. A process failure halfway through
that write must not leave the next native build reading a mixture of old and
new C. This module gives one build a private transaction directory, identifies
the validated result by its complete content digest, and publishes a small
JSON pointer with an atomic rename.

The generated directory becomes read-only *by convention*: later build stages
only read it, and a content check rejects accidental mutation. Publication
does not use a ``current`` symlink because the repository's output policy
rejects symlink traversal. An unreferenced transaction or generation is safe;
the pointer always names either the previous complete generation or the new
complete generation.
"""

from __future__ import annotations

import json
import os
import shutil
import tempfile
import uuid
from dataclasses import dataclass
from pathlib import Path
from typing import Iterator

from dev_build_state import (
    BuildStateFailure,
    InputPath,
    atomic_write_state,
    canonical_digest,
    inventory_inputs,
)


POINTER_KIND = "caxecraft-generated-project"
POINTER_SCHEMA_VERSION = 1


class GenerationFailure(RuntimeError):
    """A generated-project transaction or pointer is unsafe or malformed."""


@dataclass(frozen=True)
class GeneratedTransaction:
    """One private same-filesystem directory into which haxe.c may write."""

    root: Path
    generated: Path


@dataclass(frozen=True)
class PublishedGeneration:
    """One complete immutable generated project and its publication sequence."""

    generation_id: str
    sequence: int
    root: Path
    generated: Path


class VariantLock:
    """Serialize publication for one output variant without deleting lock files.

    The operating system releases the lock if the process exits or is killed,
    so a crash cannot leave a permanent “busy” marker. The tiny file remains as
    a stable lock identity; its contents are not trusted as ownership evidence.
    """

    def __init__(self, path: Path):
        self.path = path
        self._handle = None

    def __enter__(self) -> "VariantLock":
        self.path.parent.mkdir(parents=True, exist_ok=True)
        if self.path.is_symlink():
            raise GenerationFailure(f"variant lock must not be a symlink: {self.path}")
        self._handle = self.path.open("a+b")
        try:
            if os.name == "nt":
                import msvcrt

                self._handle.seek(0)
                if self._handle.read(1) == b"":
                    self._handle.write(b"\0")
                    self._handle.flush()
                self._handle.seek(0)
                msvcrt.locking(self._handle.fileno(), msvcrt.LK_LOCK, 1)
            else:
                import fcntl

                fcntl.flock(self._handle.fileno(), fcntl.LOCK_EX)
        except BaseException:
            self._handle.close()
            self._handle = None
            raise
        return self

    def __exit__(self, _type, _value, _traceback) -> None:
        if self._handle is None:
            return
        try:
            if os.name == "nt":
                import msvcrt

                self._handle.seek(0)
                msvcrt.locking(self._handle.fileno(), msvcrt.LK_UNLCK, 1)
            else:
                import fcntl

                fcntl.flock(self._handle.fileno(), fcntl.LOCK_UN)
        finally:
            self._handle.close()
            self._handle = None


def _real_directory(path: Path, label: str, *, create: bool = False) -> Path:
    if create:
        path.mkdir(parents=True, exist_ok=True)
    if path.is_symlink() or not path.is_dir():
        raise GenerationFailure(f"{label} must be a real directory: {path}")
    return path


def begin_transaction(variant_root: Path) -> GeneratedTransaction:
    """Create one unique transaction below the final generation filesystem."""

    variant_root = _real_directory(variant_root, "variant root")
    transactions = _real_directory(
        variant_root / "transactions", "transaction root", create=True
    )
    root = Path(
        tempfile.mkdtemp(prefix=f"{uuid.uuid4().hex}-", dir=transactions)
    )
    generated = root / "generated"
    generated.mkdir()
    return GeneratedTransaction(root=root, generated=generated)


def discard_transaction(transaction: GeneratedTransaction) -> None:
    """Remove only the private transaction named by ``begin_transaction``."""

    if transaction.root.exists():
        transactions = transaction.root.parent
        if (
            transactions.name != "transactions"
            or transaction.root.parent.parent == transaction.root
            or transaction.root.is_symlink()
        ):
            raise GenerationFailure(
                f"refusing to remove an unrecognized transaction: {transaction.root}"
            )
        shutil.rmtree(transaction.root)


def generated_inventory(generated: Path) -> dict[str, object]:
    """Describe every generated byte with checkout-independent logical paths."""

    try:
        files = inventory_inputs((InputPath("generated", generated),))
    except (OSError, BuildStateFailure) as error:
        raise GenerationFailure(str(error)) from error
    if not files:
        raise GenerationFailure("generated project is empty")
    body: dict[str, object] = {"schemaVersion": 1, "files": files}
    return {**body, "sha256": canonical_digest(body)}


def _load_pointer(path: Path) -> dict[str, object] | None:
    if not path.exists():
        return None
    if path.is_symlink() or not path.is_file():
        raise GenerationFailure(f"generated-project pointer is not a regular file: {path}")
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise GenerationFailure(f"generated-project pointer is unreadable: {error}") from error
    if not isinstance(value, dict):
        raise GenerationFailure("generated-project pointer must be a JSON object")
    if (
        value.get("schemaVersion") != POINTER_SCHEMA_VERSION
        or value.get("kind") != POINTER_KIND
    ):
        raise GenerationFailure("generated-project pointer has an unsupported schema")
    return value


def _pointer_sequence(path: Path) -> int:
    pointer = _load_pointer(path)
    if pointer is None:
        return 0
    sequence = pointer.get("sequence")
    if not isinstance(sequence, int) or isinstance(sequence, bool) or sequence < 1:
        raise GenerationFailure("generated-project pointer has an invalid sequence")
    return sequence


def finalize_transaction(
    variant_root: Path,
    transaction: GeneratedTransaction,
) -> PublishedGeneration:
    """Move a validated transaction into the immutable content-addressed store.

    The caller must hold ``VariantLock`` from before it starts the transaction
    until after it publishes the pointer. That single writer rule is what keeps
    publication sequence monotonic when two watch requests arrive together.
    """

    variant_root = _real_directory(variant_root, "variant root")
    transactions = _real_directory(
        variant_root / "transactions", "transaction root", create=True
    )
    if transaction.root.parent != transactions or transaction.root.is_symlink():
        raise GenerationFailure("generated transaction does not belong to this variant")
    inventory = generated_inventory(transaction.generated)
    generation_id = str(inventory["sha256"])
    generations = _real_directory(
        variant_root / "generations", "generation root", create=True
    )
    target = generations / generation_id
    if target.exists():
        if target.is_symlink() or not target.is_dir():
            raise GenerationFailure(f"generation destination is unsafe: {target}")
        existing = generated_inventory(target / "generated")
        if existing != inventory:
            raise GenerationFailure(
                "an existing generation does not match its content-derived identity"
            )
        discard_transaction(transaction)
    else:
        os.replace(transaction.root, target)
    return PublishedGeneration(
        generation_id=generation_id,
        sequence=0,
        root=target,
        generated=target / "generated",
    )


def publish_pointer(
    variant_root: Path,
    generation: PublishedGeneration,
) -> PublishedGeneration:
    """Atomically select one complete generation after verifying its contents."""

    variant_root = _real_directory(variant_root, "variant root")
    expected_root = variant_root / "generations" / generation.generation_id
    if generation.root != expected_root:
        raise GenerationFailure("generation does not belong to this variant")
    inventory = generated_inventory(generation.generated)
    if inventory.get("sha256") != generation.generation_id:
        raise GenerationFailure("generation contents changed before publication")
    pointer_path = variant_root / "hxc-play-current-generation.json"
    sequence = _pointer_sequence(pointer_path) + 1
    body: dict[str, object] = {
        "schemaVersion": POINTER_SCHEMA_VERSION,
        "kind": POINTER_KIND,
        "sequence": sequence,
        "generationId": generation.generation_id,
        "inventorySha256": inventory["sha256"],
    }
    atomic_write_state(pointer_path, {**body, "sha256": canonical_digest(body)})
    return PublishedGeneration(
        generation_id=generation.generation_id,
        sequence=sequence,
        root=generation.root,
        generated=generation.generated,
    )


def current_generation(variant_root: Path) -> PublishedGeneration:
    """Resolve and revalidate the selected generation without following links."""

    variant_root = _real_directory(variant_root, "variant root")
    pointer_path = variant_root / "hxc-play-current-generation.json"
    pointer = _load_pointer(pointer_path)
    if pointer is None:
        raise GenerationFailure("no generated project has been published")
    digest = pointer.get("sha256")
    body = {key: value for key, value in pointer.items() if key != "sha256"}
    if not isinstance(digest, str) or digest != canonical_digest(body):
        raise GenerationFailure("generated-project pointer digest is invalid")
    generation_id = pointer.get("generationId")
    inventory_digest = pointer.get("inventorySha256")
    sequence = pointer.get("sequence")
    if (
        not isinstance(generation_id, str)
        or len(generation_id) != 64
        or not isinstance(inventory_digest, str)
        or inventory_digest != generation_id
        or not isinstance(sequence, int)
        or isinstance(sequence, bool)
        or sequence < 1
    ):
        raise GenerationFailure("generated-project pointer fields are invalid")
    root = variant_root / "generations" / generation_id
    generated = root / "generated"
    inventory = generated_inventory(generated)
    if inventory.get("sha256") != generation_id:
        raise GenerationFailure("selected generated project is missing or corrupt")
    return PublishedGeneration(
        generation_id=generation_id,
        sequence=sequence,
        root=root,
        generated=generated,
    )


def incomplete_transactions(variant_root: Path) -> Iterator[Path]:
    """List abandoned private transactions; callers may report or quarantine them."""

    transactions = variant_root / "transactions"
    if not transactions.exists():
        return iter(())
    _real_directory(transactions, "transaction root")
    return iter(
        sorted(
            (path for path in transactions.iterdir() if path.is_dir() and not path.is_symlink()),
            key=lambda path: path.name.encode("utf-8"),
        )
    )
