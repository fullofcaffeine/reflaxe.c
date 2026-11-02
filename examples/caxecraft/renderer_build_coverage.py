"""Bind one terrain call-coverage proof to the exact supplied native build.

The manifest owns generated translation units; the native plan adds only the
explicit interval observer. Extraction and foreign-library verification retain
their existing owners. This module joins their evidence once, then checks hashes
at the end of repeated pilots without parsing every AST again. It does not prove
runtime observations or replace the benchmark's existing acceptance rules.
"""
from __future__ import annotations

from dataclasses import dataclass
import hashlib
import json
from pathlib import Path, PurePosixPath

import renderer_call_coverage as coverage
from renderer_native_contract import NativeTerrainContract
from dev_native_cache import NativeBuildPins, NativeCacheFailure


CoverageFailure = coverage.CoverageFailure
ROOT = Path(__file__).resolve().parents[2]
OBSERVER_NAME = "terrain-observer/interval_observer.c"
OBSERVER_SOURCE = ROOT / "runtime/hxrt/test/interval_observer.c"
FORBIDDEN = frozenset({
    "hxc_alloc", "hxc_realloc", "hxc_gc_allocate", "hxc_gc_collect",
    "malloc", "calloc", "realloc", "aligned_alloc", "posix_memalign",
})


def _sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _json(value: object) -> str:
    return json.dumps(value, sort_keys=True, separators=(",", ":"), allow_nan=False)


def _native_identity(contract: NativeTerrainContract) -> str:
    if not isinstance(contract, NativeTerrainContract):
        raise coverage.CoverageFailure("terrain coverage requires a verified native contract")
    return _json({"identity": contract.identity_report(), "leaves": sorted(contract.leaves)})


def _selected_sources(manifest: Path, sources: tuple[tuple[str, Path], ...]) -> None:
    """Reject missing, extra, aliased or reordered units before any extraction."""
    document = json.loads(manifest.read_text(encoding="utf-8"))
    build = document.get("build") if isinstance(document, dict) else None
    expected = build.get("sources") if isinstance(build, dict) else None
    if (not isinstance(expected, list) or not expected
            or any(not isinstance(name, str) or not name for name in expected)):
        raise coverage.CoverageFailure("manifest omitted generated source selection")
    for name in expected:
        path = PurePosixPath(name)
        if (path.is_absolute() or path.as_posix() != name or ".." in path.parts
                or "\\" in name or path.suffix != ".c" or name == OBSERVER_NAME):
            raise coverage.CoverageFailure("manifest source must be a normalized relative C path")
    names = tuple(name for name, _ in sources)
    if len(set(expected)) != len(expected) or names != (*expected, OBSERVER_NAME):
        raise coverage.CoverageFailure("native plan must select every generated unit once, then the observer")
    paths = tuple(path.resolve() for _, path in sources)
    if len(set(paths)) != len(paths):
        raise coverage.CoverageFailure("native plan repeats a translation unit")
    for (name, path), actual in zip(sources, paths):
        wanted = OBSERVER_SOURCE if name == OBSERVER_NAME else manifest.parent / name
        if actual != wanted.resolve() or not path.is_file():
            raise coverage.CoverageFailure("native source does not match its declared owner")


@dataclass(frozen=True)
class TerrainBuildCoverage:
    """Immutable build proof, valid only while its pinned inputs remain unchanged.

    The caller supplies a freshly verified native contract to verify(), after all
    pilot repetitions and before publishing results. That owner rechecks native
    library/source/policy identity; this owner rechecks the build and extraction
    files. The summary is a fresh JSON value with no host paths.
    """

    sources: tuple[tuple[str, Path], ...]
    flags: tuple[str, ...]
    compiler: Path
    preprocessor: Path
    cwd: Path
    manifest: Path
    executable: Path
    receipt: Path
    receipt_sha256: str
    _pins: tuple[tuple[Path, str], ...]
    _native_json: str
    _summary_json: str
    _native_build_pins: NativeBuildPins

    def summary(self) -> dict[str, object]:
        report = json.loads(self._summary_json)
        report["receiptSha256"] = self.receipt_sha256
        return report

    def verify(self, *, native_contract: NativeTerrainContract) -> None:
        """Recheck evidence without recompiling or rebuilding the call graph."""
        try:
            self._native_build_pins.verify()
            if _native_identity(native_contract) != self._native_json:
                raise CoverageFailure("native terrain contract changed after coverage")
            if _sha(self.receipt) != self.receipt_sha256:
                raise CoverageFailure("terrain coverage receipt changed")
            for path, digest in self._pins:
                if _sha(path) != digest:
                    raise CoverageFailure(f"terrain coverage input changed: {path}")
        except (OSError, ValueError, TypeError, NativeCacheFailure) as error:
            if isinstance(error, CoverageFailure):
                raise
            raise CoverageFailure(f"cannot verify terrain coverage: {error}") from error


def build_terrain_coverage(
    *, sources: tuple[tuple[str, Path], ...], flags: tuple[str, ...],
    compiler: Path, preprocessor: Path, cwd: Path, manifest: Path,
    executable: Path, native_contract: NativeTerrainContract, evidence: Path,
    native_build_pins: NativeBuildPins,
) -> TerrainBuildCoverage:
    """Extract each selected unit once and retain a failed or complete receipt.

    Flags already contain the caller's exact include options. The native compiler
    preprocesses those inputs; the separately selected Clang parser owns the AST.
    No source discovery, flag rewriting or caller-provided leaf list occurs here.
    """
    evidence = evidence.absolute()
    try:
        evidence.mkdir(parents=True, exist_ok=False)
    except OSError as error:
        raise CoverageFailure(f"cannot create terrain coverage evidence: {error}") from error
    receipt = evidence / "receipt.json"
    sources = tuple((name, path.absolute()) for name, path in sources)
    flags = tuple(flags)
    compiler, preprocessor, cwd, manifest, executable = (
        path.absolute() for path in (compiler, preprocessor, cwd, manifest, executable)
    )
    record: dict[str, object] = {
        "schemaVersion": 1, "status": "failed",
        "sources": [{"name": name, "path": str(path)} for name, path in sources],
        "flags": list(flags), "compiler": str(compiler), "preprocessor": str(preprocessor),
        "cwd": str(cwd), "manifest": str(manifest), "executable": str(executable),
        "extractions": [],
    }
    pins: dict[Path, str] = {}

    def pin(path: Path, digest: str | None = None) -> None:
        path = path.resolve()
        expected = _sha(path) if digest is None else digest
        if path in pins and pins[path] != expected:
            raise coverage.CoverageFailure("inconsistent shared extraction input")
        pins[path] = expected

    try:
        _selected_sources(manifest, sources)
        if not isinstance(native_build_pins, NativeBuildPins):
            raise CoverageFailure("terrain coverage requires verified native objects")
        native_build_pins.verify()
        if (native_build_pins.sources != tuple((name, path.resolve()) for name, path in sources)
                or native_build_pins.flags != flags
                or native_build_pins.compiler != preprocessor.resolve()):
            raise CoverageFailure("native build evidence differs from extraction plan")
        for path, digest in native_build_pins.pins:
            pin(path, digest)
        record["nativeObjects"] = [
            {"source": item.source, "key": item.key, "sha256": item.sha256,
             "path": str(item.path), "metadata": str(item.path.parent / "metadata.json")}
            for item in native_build_pins.objects
        ]
        native_json = _native_identity(native_contract)
        record["nativeContract"] = json.loads(native_json)
        for path in (manifest, executable, compiler, preprocessor, Path(__file__),
                     Path(coverage.__file__), *(path for _, path in sources)):
            pin(path)
        record["initialInputs"] = {str(path): digest for path, digest in pins.items()}
        units = {}
        for index, (name, path) in enumerate(sources):
            unit = coverage.extract_unit(
                path, compiler=compiler, preprocessor=preprocessor, flags=flags,
                cwd=cwd, evidence=evidence / f"unit-{index:04d}",
            )
            units[name] = unit
            record["extractions"].append({"source": name, "receipt": str(unit.receipt),
                                          "sha256": unit.sha256})
        proof = coverage.prove_interval(
            units, begin="caxecraft_test_interval_begin", end="caxecraft_test_interval_end",
            forbidden=set(FORBIDDEN), reviewed_leaves=set(native_contract.leaves),
        )
        # prove_interval verified these extractor-owned receipts and ASTs. Freeze
        # their already checked file identities for cheap end-of-pilot rechecks.
        compiled_dependencies = dict(native_build_pins.source_dependencies)
        for name, unit in units.items():
            pin(unit.receipt, unit.sha256)
            extracted = json.loads(unit.receipt.read_text(encoding="utf-8"))
            built_paths = set(compiled_dependencies[name])
            extracted_paths = {Path(path).resolve() for path in extracted["inputs"]}
            tools = {compiler.resolve(), preprocessor.resolve()}
            if extracted_paths - tools != built_paths - tools:
                raise CoverageFailure("extraction dependencies differ from the compiled translation unit")
            for field in ("inputs", "preprocessingArtifacts"):
                for path, digest in extracted[field].items():
                    pin(Path(path), digest)
            pin(Path(extracted["ast"]), extracted["astSha256"])
        for path, digest in pins.items():
            if _sha(path) != digest:
                raise coverage.CoverageFailure("build inputs changed during terrain coverage")
        native_build_pins.verify()
        summary = {
            "schemaVersion": 1, "translationUnits": len(units),
            "reachableFunctions": len(proof["functions"]),
            "foreignLeaves": len(proof["foreignLeaves"]),
            "manifestSha256": pins[manifest.resolve()], "executableSha256": pins[executable.resolve()],
            "nativeContractSha256": hashlib.sha256(native_json.encode()).hexdigest(),
            "claim": "static call coverage of the supplied build; runtime interval evidence remains required",
        }
        record.update(status="success", summary=summary, proof=proof,
                      inputs={str(path): digest for path, digest in pins.items()})
    except Exception as error:
        record["error"] = str(error)
        if isinstance(error, CoverageFailure):
            raise
        raise CoverageFailure(f"cannot establish terrain coverage: {error}") from error
    finally:
        try:
            receipt.write_text(json.dumps(record, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        except OSError as error:
            raise CoverageFailure(f"cannot retain terrain coverage receipt: {error}") from error
    return TerrainBuildCoverage(sources, flags, compiler, preprocessor, cwd, manifest,
                                executable, receipt, _sha(receipt), tuple(pins.items()),
                                native_json, _json(summary), native_build_pins)
