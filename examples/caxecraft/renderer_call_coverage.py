"""Check direct-call coverage of a supplied final-C interval, without admitting a benchmark.

Clang owns C parsing. Extraction rejects variable-length types and binds the
source, transitive headers, compiler, command and AST before graph traversal.
Compiler-owned preprocessing expands pragma operators; only emitted diagnostic
control directives are blanked before parsing the retained preprocessed input.
All semantic pragmas and line markers are preserved.
Unknown calls, indirect dispatch, atomic operations and implicit cleanup fail.
Callers still own complete project/native input selection and reviewed foreign
contracts. This module does not establish C ABI compatibility, library identity,
or full-game coverage; the existing benchmark gate remains authoritative.
"""
from __future__ import annotations

from dataclasses import dataclass
from typing import Iterator
import hashlib
import json
from pathlib import Path
import re
import shlex
import subprocess

from scripts.test.bounded_process import run as run_bounded_process
from scripts.test.c_fixture_harness import C11_STRICT_FLAGS


class CoverageFailure(ValueError):
    """The supplied AST cannot establish complete interval coverage."""


@dataclass(frozen=True)
class Node:
    kind: str
    identifier: str
    name: str
    storage: str
    tls: str
    reference: str
    reference_kind: str
    atomic: bool
    children: tuple[Node, ...]


def decode(value: object) -> Node:
    """Narrow the JSON boundary before any call-graph work."""
    if not isinstance(value, dict):
        raise CoverageFailure("AST node must be an object")
    def field(owner: dict, key: str) -> str:
        item = owner.get(key, "")
        if not isinstance(item, str):
            raise CoverageFailure(f"AST {key} must be text")
        return item
    inner = value.get("inner", [])
    reference = value.get("referencedDecl", {})
    if not isinstance(inner, list) or not isinstance(reference, dict):
        raise CoverageFailure("invalid AST children or declaration reference")
    atomic = False
    # Clang puts sizeof's operand type in argType, separate from its result type.
    # This defense cannot see adjusted parameter bounds; extraction additionally
    # strips diagnostic controls after preprocessing and requires Clang's VLA error.
    for owner in ("type", "argType"):
        raw_type = value.get(owner, {})
        if not isinstance(raw_type, dict):
            raise CoverageFailure("AST type must be an object")
        for name in ("qualType", "desugaredQualType"):
            spelling = field(raw_type, name)
            atomic = atomic or re.search(r"\b_Atomic\b", spelling) is not None
            remainder = re.sub(r"\[\s*[0-9]*\s*\]", "", spelling)
            if "[" in remainder or "]" in remainder:
                raise CoverageFailure("array dimension is not a decimal constant or incomplete")
    if value.get("isVariablyModified") is True or field(value, "kind") == "VariableArrayType":
        raise CoverageFailure("variably modified AST type is unsupported")
    atomic = atomic or field(value, "castKind") in {"AtomicToNonAtomic", "NonAtomicToAtomic"}
    return Node(field(value, "kind"), field(value, "id"),
                field(value, "name"), field(value, "storageClass"), field(value, "tls"),
                field(reference, "id"), field(reference, "kind"), atomic,
                tuple(decode(child) for child in inner))


def walk(node: Node) -> Iterator[Node]:
    yield node
    for child in node.children:
        yield from walk(child)


# A static function belongs to one translation unit; external symbols are shared.
Key = tuple[str, str]


class _Program:
    def __init__(self, units: dict[str, object]):
        self.units = {name: decode(ast) for name, ast in units.items()}
        self.declarations: dict[tuple[str, str], Key] = {}
        self.bodies: dict[Key, tuple[str, Node]] = {}
        self.unsafe_declarations: set[Key] = set()
        self.tls_variables: set[tuple[str, str]] = set()
        for unit, ast in self.units.items():
            if ast.kind != "TranslationUnitDecl":
                raise CoverageFailure("expected complete translation unit")
            for node in walk(ast):
                # Ordinary atomic loads/stores need not be CallExpr or AtomicExpr.
                # Reject the TU until implicit helper effects have their own proof.
                if node.atomic or node.kind == "AtomicExpr":
                    raise CoverageFailure("atomic types or operations have no implicit-call proof")
                if node.kind == "VarDecl" and node.tls:
                    self.tls_variables.add((unit, node.identifier))
                if node.kind != "FunctionDecl":
                    continue
                if not node.identifier or not node.name:
                    raise CoverageFailure("function identity is missing")
                key = (unit if node.storage == "static" else "", node.name)
                self.declarations[unit, node.identifier] = key
                # Name-only admission is invalid for linker aliases and weak or
                # resolver-backed functions. Keep unknown attributes closed too;
                # these admitted attributes affect optimization/diagnostics only.
                harmless = {"BuiltinAttr", "NoThrowAttr", "NoReturnAttr",
                            "WarnUnusedResultAttr", "AlwaysInlineAttr", "NoInlineAttr",
                            "ConstAttr", "PureAttr", "VisibilityAttr"}
                if any(child.kind.endswith("Attr") and child.kind not in harmless
                       for child in node.children):
                    self.unsafe_declarations.add(key)
                bodies = [child for child in node.children if child.kind == "CompoundStmt"]
                if bodies:
                    if len(bodies) != 1 or key in self.bodies:
                        raise CoverageFailure(f"ambiguous definition: {key}")
                    self.bodies[key] = (unit, bodies[0])

    def callee(self, unit: str, call: Node) -> Key:
        if call.kind != "CallExpr" or not call.children:
            raise CoverageFailure("malformed call")
        target = call.children[0]
        while target.kind in {"ImplicitCastExpr", "ParenExpr", "ConstantExpr"}:
            if len(target.children) != 1:
                raise CoverageFailure("ambiguous callee conversion")
            target = target.children[0]
        if target.kind != "DeclRefExpr" or target.reference_kind != "FunctionDecl":
            raise CoverageFailure("indirect call has no complete target proof")
        try:
            key = self.declarations[unit, target.reference]
            if key in self.unsafe_declarations:
                raise CoverageFailure(f"unproved declaration effects or linker identity: {key}")
            return key
        except KeyError as error:
            raise CoverageFailure("callee declaration is absent") from error

    def interval(self, begin: str, end: str) -> tuple[str, tuple[Node, ...]]:
        """Require exactly one pair of plain calls in the same statement block.

        This rejects a marker hidden behind a branch or inside an argument. The
        markers must not themselves prepare rendering arguments before observation.
        """
        markers: list[tuple[str, str]] = []
        matches: list[tuple[str, tuple[Node, ...]]] = []
        for unit, body in self.bodies.values():
            for node in walk(body):
                if node.kind == "CallExpr":
                    # An unrelated indirect call outside observation is allowed.
                    try:
                        target = self.callee(unit, node)
                    except CoverageFailure:
                        continue
                    if target in {("", begin), ("", end)}:
                        markers.append((unit, target[1]))
                        if len(node.children) != 1:
                            raise CoverageFailure("interval markers must have no arguments")
                if node.kind == "CompoundStmt":
                    positions: dict[str, int] = {}
                    for index, statement in enumerate(node.children):
                        if statement.kind == "CallExpr":
                            try:
                                target = self.callee(unit, statement)
                            except CoverageFailure:
                                continue
                            if target in {("", begin), ("", end)}:
                                positions[target[1]] = index
                    if begin in positions and end in positions:
                        if any(child.kind in {"GotoStmt", "IndirectGotoStmt", "AddrLabelExpr", "CleanupAttr"}
                               for child in walk(body)):
                            raise CoverageFailure("enclosing function has unproved control flow or cleanup")
                        first, last = positions[begin], positions[end]
                        if first >= last:
                            raise CoverageFailure("interval markers are reversed")
                        if any(child.kind in {"ReturnStmt", "BreakStmt", "ContinueStmt"}
                               for statement in node.children[first + 1:last]
                               for child in walk(statement)):
                            raise CoverageFailure("control flow can leave the interval")
                        matches.append((unit, node.children[first + 1:last]))
        if len(markers) != 2 or len(matches) != 1:
            raise CoverageFailure("need one complete interval in one block")
        return matches[0]

    def prove(self, begin: str, end: str, forbidden: set[str],
              reviewed_leaves: set[str]) -> dict[str, object]:
        """Explore a closure; leaf admission here is an input, not provenance proof."""
        unit, statements = self.interval(begin, end)
        pending = [(unit, node) for node in statements]
        visited: set[Key] = set()
        leaves: set[str] = set()
        while pending:
            unit, statement = pending.pop()
            for node in walk(statement):
                if (node.kind == "DeclRefExpr" and node.reference_kind == "VarDecl"
                        and (unit, node.reference) in self.tls_variables):
                    raise CoverageFailure("thread-local access needs native runtime proof")
                if (node.kind in {"GCCAsmStmt", "MSAsmStmt", "CleanupAttr", "AtomicExpr"}
                        or node.kind.startswith("CXX")):
                    raise CoverageFailure(f"unsupported implicit operation: {node.kind}")
                if node.kind != "CallExpr":
                    continue
                key = self.callee(unit, node)
                if key[1] in forbidden:
                    raise CoverageFailure(f"prohibited call: {key[1]}")
                if key in visited:
                    continue
                visited.add(key)
                if key in self.bodies:
                    pending.append(self.bodies[key])
                elif not key[0] and key[1] in reviewed_leaves:
                    leaves.add(key[1])
                else:
                    raise CoverageFailure(f"unreviewed external call: {key}")
        return {"functions": sorted(visited), "foreignLeaves": sorted(leaves),
                "claim": "coverage of supplied validated C translation units only; not full-game admission"}


@dataclass(frozen=True)
class ExtractedUnit:
    """Trusted extraction receipt identity, returned only after a complete parse."""
    receipt: Path
    sha256: str


def _sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _flags(flags: tuple[str, ...]) -> None:
    """Keep this C extraction subset explicit; never silently discard build flags."""
    index = 0
    while index < len(flags):
        flag = flags[index]
        if flag in {"-I", "-isystem", "-isysroot", "-arch", "-include"}:
            index += 1
            if index == len(flags) or not flags[index] or flags[index].startswith("-"):
                raise CoverageFailure("missing extraction flag argument")
        elif (flag.startswith(("-I", "-D", "-U")) and len(flag) > 2):
            pass
        elif flag in C11_STRICT_FLAGS or flag in {"-pedantic-errors", "-std=c11", "-std=gnu99", "-std=gnu11", "-ffreestanding",
                      "-fno-strict-aliasing", "-O0", "-O1", "-O2", "-O3", "-g",
                      "-Wall", "-Wextra", "-Werror", "-pedantic",
                      "-Werror=implicit-function-declaration", "-Werror=pointer-arith"}:
            pass
        else:
            raise CoverageFailure(f"unsupported extraction flag: {flag}")
        index += 1


def _dependencies(text: str, cwd: Path) -> tuple[Path, ...]:
    """Read Clang's one-target Make dependency record, including system headers."""
    if not text.startswith("coverage:") or "$" in text or "\r" in text:
        raise CoverageFailure("unsupported compiler dependency record")
    names = shlex.split(text[len("coverage:"):].replace("\\\n", " "))
    if not names:
        raise CoverageFailure("compiler dependency record is empty")
    paths = tuple(sorted({(cwd / name).absolute() for name in names}, key=str))
    if any(not path.is_file() for path in paths):
        raise CoverageFailure("compiler dependency is missing")
    return paths


def _semantic_flags(flags: tuple[str, ...]) -> tuple[str, ...]:
    """Separate preprocessing-only options; retain dialect/target/codegen policy."""
    result = []
    index = 0
    while index < len(flags):
        flag = flags[index]
        if flag in {"-I", "-isystem", "-include"}:
            index += 2
            continue
        if flag.startswith(("-I", "-D", "-U")):
            index += 1
            continue
        result.append(flag)
        if flag in {"-isysroot", "-arch"}:
            index += 1
            result.append(flags[index])
        index += 1
    return tuple(result)


def diagnostic_neutral_text(raw: str) -> tuple[str, list[dict[str, object]]]:
    """Blank emitted warning-control directives, preserving every physical line.

    Input must be compiler-owned -E output, not arbitrary authored C. Clang
    expands macros, splices and _Pragma first. Preserve pack, attributes and
    other semantic pragmas, as well as source/system line markers. Unexpected
    continued diagnostic directives fail instead of guessing their extent.
    """
    lines = raw.split("\n")
    removed = []
    for index, line in enumerate(lines):
        if re.match(r"^[ \t]*#[ \t]*pragma[ \t]+(?:clang|GCC)[ \t]+diagnostic(?:[ \t]|$)", line):
            if line.rstrip().endswith("\\"):
                raise CoverageFailure("continued emitted diagnostic directive is unsupported")
            removed.append({"line": index + 1, "directive": line})
            lines[index] = ""
    return "\n".join(lines), removed


def extract_unit(source: Path, *, compiler: Path, flags: tuple[str, ...],
                 cwd: Path, evidence: Path, preprocessor: Path | None = None) -> ExtractedUnit:
    """Extract one C TU with 20-second tree-cleaned commands and immutable evidence.

    The native build's compiler owns preprocessing; Clang owns AST parsing.
    Keeping those roles separate preserves GCC-specific conditional branches
    instead of silently inspecting Clang's version of the program. The default
    uses the parser for both roles for existing Clang-only controls.
    A dependency pass pins headers before preprocessing. The -E pass emits a
    second dependency list; additions/removals or byte changes fail. Paired
    -include is applied by the native preprocessor during these passes and its resolved headers
    join the same pinned dependency set; it is not reapplied to cpp-output. Only warning
    controls are blanked before parsing exact retained cpp-output bytes. This
    detects stale inputs, not malicious concurrent mutation.
    Evidence must be a new directory; callers retain it after failures.
    """
    evidence = evidence.absolute()
    evidence.mkdir(parents=True, exist_ok=False)
    source, compiler, cwd = source.absolute(), compiler.absolute(), cwd.absolute()
    preprocessor = (preprocessor or compiler).absolute()
    receipt = evidence / "receipt.json"
    record: dict[str, object] = {"schemaVersion": 1, "status": "failed", "commands": [],
                                "source": str(source), "compiler": str(compiler),
                                "preprocessor": str(preprocessor), "flags": list(flags)}

    def command(arguments: list[str], name: str) -> str:
        stdout, stderr, code = b"", b"", None
        outcome = "failed"
        try:
            result = run_bounded_process(arguments, cwd=cwd, capture_output=True, timeout=20,
                                         phase=f"C coverage {name}")
            stdout, stderr, code = result.stdout, result.stderr, result.returncode
            outcome = "success" if code == 0 else "failed"
        except subprocess.TimeoutExpired as error:
            stdout, stderr = error.output or b"", error.stderr or b""
            outcome = "timeout"
        finally:
            (evidence / f"{name}.stdout").write_bytes(stdout)
            (evidence / f"{name}.stderr").write_bytes(stderr)
            record["commands"].append({"argv": arguments, "cwd": str(cwd), "timeout": 20,
                                       "returncode": code, "outcome": outcome})
        if outcome != "success":
            raise CoverageFailure(f"C coverage {name} {outcome}; retained diagnostics: {evidence}")
        return stdout.decode("utf-8")

    try:
        _flags(flags)
        initial = {str(path): _sha(path) for path in (source, compiler, preprocessor)}
        record["initialInputs"] = initial
        prefix = [str(preprocessor), "-x", "c", *flags, "-Wsystem-headers", "-Werror=vla"]
        dependencies = _dependencies(command([*prefix, "-M", "-MT", "coverage", str(source)], "dependencies"), cwd)
        if source not in dependencies:
            raise CoverageFailure("dependency record omitted the source")
        pins = {str(path): _sha(path) for path in (*dependencies, compiler, preprocessor)}
        if any(pins.get(path) != digest for path, digest in initial.items()):
            raise CoverageFailure("source or compiler changed during dependency discovery")
        record["inputs"] = pins
        semantic = _semantic_flags(flags)
        record["semanticFlags"] = list(semantic)
        record["preprocessingFlags"] = list(flags)
        depfile = evidence / "preprocessed.d"
        raw = command([*prefix, "-E", "-MD", "-MF", str(depfile), "-MT", "coverage",
                       str(source)], "preprocess")
        if _dependencies(depfile.read_text(), cwd) != dependencies:
            raise CoverageFailure("preprocess dependency set differs from discovery")
        if any(_sha(Path(path)) != digest for path, digest in pins.items()):
            raise CoverageFailure("extraction input changed during preprocessing")
        derived, removed = diagnostic_neutral_text(raw)
        derived_path = evidence / "diagnostic-neutral.i"
        derived_path.write_bytes(derived.encode("utf-8"))
        ledger = evidence / "removed-diagnostics.json"
        ledger.write_text(json.dumps(removed, indent=2) + "\n")
        artifacts = {str(path): _sha(path) for path in
                     (evidence / "preprocess.stdout", derived_path, ledger, depfile)}
        record["preprocessingArtifacts"] = artifacts
        record["removedDiagnosticDirectives"] = len(removed)
        ast_text = command([str(compiler), "-x", "cpp-output", *semantic,
                            "-Werror=vla", "-Wsystem-headers", "-Xclang", "-ast-dump=json",
                            "-fsyntax-only", str(derived_path)], "ast")
        if any(_sha(Path(path)) != digest for path, digest in {**pins, **artifacts}.items()):
            raise CoverageFailure("extraction input changed during parsing")
        ast = json.loads(ast_text)
        if not isinstance(ast, dict) or ast.get("kind") != "TranslationUnitDecl":
            raise CoverageFailure("compiler output is not a complete translation unit")
        record["ast"] = str(evidence / "ast.stdout")
        record["astSha256"] = _sha(evidence / "ast.stdout")
        record["status"] = "success"
    except Exception as error:
        record["error"] = str(error)
        raise
    finally:
        receipt.write_text(json.dumps(record, indent=2) + "\n")
    return ExtractedUnit(receipt, _sha(receipt))


def verified_ast(unit: ExtractedUnit) -> object:
    """Reject changed receipts, source/header/compiler bytes or AST bytes."""
    if _sha(unit.receipt) != unit.sha256:
        raise CoverageFailure("extraction receipt changed")
    record = json.loads(unit.receipt.read_text())
    if record.get("status") != "success":
        raise CoverageFailure("extraction did not succeed")
    if any(_sha(Path(path)) != digest for path, digest in record["inputs"].items()):
        raise CoverageFailure("extraction input identity changed")
    if any(_sha(Path(path)) != digest for path, digest in record["preprocessingArtifacts"].items()):
        raise CoverageFailure("preprocessing artifact identity changed")
    path = Path(record["ast"])
    if _sha(path) != record["astSha256"]:
        raise CoverageFailure("extracted AST identity changed")
    return json.loads(path.read_text())


def prove_interval(units: dict[str, ExtractedUnit], *, begin: str, end: str,
                   forbidden: set[str], reviewed_leaves: set[str]) -> dict[str, object]:
    """Prove only the supplied units; reviewed leaf semantics remain caller-owned."""
    program = _Program({name: verified_ast(unit) for name, unit in units.items()})
    return program.prove(begin, end, forbidden, reviewed_leaves)
