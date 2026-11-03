"""Bind proof diagnostics to exact preprocessed bytes and a matching Clang reader.

Native compilation owns quality warnings. Coverage admits only literal GNU access
metadata ignored by Clang; this is explicit-call compatibility, not object-size,
optimization or ABI equivalence. The graph still checks every body and callee.
The ctypes reader runs as a bounded subprocess, never inside the proof process.
"""
from __future__ import annotations

from pathlib import Path
import json
import re
import sys

PARSER_POLICY_VERSION = "literal-gnu-access-clang-21.1.8-v2"

PROOF_FLAGS = ("-Wunknown-pragmas", "-Wignored-pragmas",
               "-Werror=implicit-function-declaration", "-Werror=unknown-warning-option",
               "-Werror=unused-command-line-argument", "-Wunknown-attributes", "-Wignored-attributes", "-Wno-gnu-line-marker",
               "-Werror=vla", "-Wsystem-headers")


def require_matching_version(compiler: object, library: object) -> None:
    """Only the tested location-serialization profile may admit ignored metadata."""
    if not isinstance(compiler, str) or compiler != library:
        raise ValueError("Clang and libclang versions do not match")
    if re.search(r"\bclang version 21\.1\.8(?:\s|$)", compiler) is None:
        raise ValueError("coverage serialized-diagnostic locations require the verified Clang 21.1.8 profile")


def matching_library(compiler: Path) -> Path:
    """Use only a library beside the resolved compiler installation, never loader search."""
    root = compiler.resolve().parent.parent / "lib"
    candidates = [root / name for name in ("libclang.dylib", "libclang.so", "libclang.so.1", "libclang-21.so.1")]
    found = {path.resolve() for path in candidates if path.is_file()}
    if len(found) != 1:
        raise ValueError("coverage requires one matched libclang beside the selected Clang installation")
    return found.pop()


# Token positions are byte offsets, including UTF-8 bytes before an occurrence.
# Strings/comments and preprocessor lines cannot supply attribute tokens.
_TOKEN = re.compile(rb'(?P<skip>\s+|/\*.*?\*/|//[^\n]*|^[ \t]*\#[^\n]*)|'
                    rb'(?P<string>"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\')|'
                    rb'(?P<word>[A-Za-z_][A-Za-z_0-9]*|[0-9]+)|(?P<punct>.)', re.S | re.M)
_MODES = frozenset((b"read_only", b"__read_only__", b"write_only", b"__write_only__",
                    b"read_write", b"__read_write__", b"none", b"__none__"))


def _literal_access(tokens: list[bytes]) -> bool:
    # Each operand is one lexer token: joining first would admit split identifiers
    # or decimal operands such as `read_ only` or `1 2`.
    return (len(tokens) in (6, 8) and tokens[0] in (b"access", b"__access__")
            and tokens[1] == b"(" and tokens[2] in _MODES and tokens[3] == b","
            and re.fullmatch(rb"[1-9][0-9]*", tokens[4]) is not None
            and tokens[-1] == b")" and (len(tokens) == 6 or
                (tokens[5] == b"," and re.fullmatch(rb"[1-9][0-9]*", tokens[6]) is not None)))


def access_occurrences(source: bytes) -> dict[tuple[int, int], str]:
    """Recognize complete literal access entries only inside GNU attribute lists.

    The compiler supplies diagnostic ranges; this lexer verifies their exact
    occurrence and grammar. It does not remove attributes or interpret C types.
    Namespaced, expression-valued, suffixed or ambiguous entries are not admitted.
    """
    tokens = [(m.group(), m.start()) for m in _TOKEN.finditer(source) if m.lastgroup != "skip"]
    admitted: dict[tuple[int, int], str] = {}
    for index, (token, _) in enumerate(tokens):
        if token not in (b"__attribute__", b"__attribute") or [t[0] for t in tokens[index+1:index+3]] != [b"(", b"("]:
            continue
        depth, first = 2, index + 3
        pending: dict[tuple[int, int], str] = {}
        for cursor in range(first, len(tokens)):
            current = tokens[cursor][0]
            if depth == 2 and current in (b",", b")"):
                entry = tokens[first:cursor]
                spelling = b"".join(t[0] for t in entry)
                if entry and _literal_access([t[0] for t in entry]):
                    pending[(entry[0][1], entry[-1][1])] = spelling.decode("ascii")
                first = cursor + 1
            if current == b"(": depth += 1
            elif current == b")": depth -= 1
            if depth < 2:
                if cursor + 1 < len(tokens) and tokens[cursor + 1][0] == b")":
                    admitted.update(pending)
                break
    return admitted


def admit_diagnostics(value: object, source: bytes) -> list[dict[str, object]]:
    """Reject all diagnostics except uniquely located, reviewed access metadata."""
    if not isinstance(value, dict) or set(value) != {"compilerVersion", "libraryVersion", "diagnostics"}:
        raise ValueError("invalid structured diagnostic record")
    require_matching_version(value["compilerVersion"], value["libraryVersion"])
    diagnostics = value["diagnostics"]
    if not isinstance(diagnostics, list): raise ValueError("invalid diagnostic list")
    occurrences = access_occurrences(source)
    decisions: list[dict[str, object]] = []
    seen: set[tuple[int, int]] = set()
    for diagnostic in diagnostics:
        if not isinstance(diagnostic, dict) or set(diagnostic) != {"severity", "option", "offset", "ranges", "children"}:
            raise ValueError("invalid diagnostic entry")
        if (type(diagnostic["severity"]) is not int or diagnostic["severity"] != 2
                or diagnostic["option"] != "-Wunknown-attributes"
                or not isinstance(diagnostic["children"], list)):
            raise ValueError("unclassified coverage diagnostic")
        ranges = diagnostic["ranges"]
        if not isinstance(ranges, list) or len(ranges) != 1 or not isinstance(ranges[0], list) or len(ranges[0]) != 2:
            raise ValueError("ambiguous diagnostic range")
        first, last = ranges[0]
        if type(first) is not int or type(last) is not int or type(diagnostic["offset"]) is not int:
            raise ValueError("invalid diagnostic offsets")
        key = (first, last)
        if diagnostic["offset"] != first or key not in occurrences or key in seen:
            raise ValueError("unreviewed or duplicate ignored attribute occurrence")
        context = diagnostic["children"]
        # LLVM llvmorg-21.1.8 clang/lib/Frontend/SerializedDiagnosticPrinter.cpp
        # HandleDiagnostic emits every non-note at top level; emitNote supplies
        # context children. These fields allow context, not an include-note ID.
        for note in context:
            if (not isinstance(note, dict) or set(note) != {"severity", "option", "category", "location", "ranges", "fixits", "children"}
                    or type(note["severity"]) is not int or note["severity"] != 1
                    or note["option"] != "" or type(note["category"]) is not int or note["category"] != 0
                    or note["ranges"] != [] or type(note["fixits"]) is not int or note["fixits"] != 0
                    or note["children"] != []):
                raise ValueError("unclassified coverage context diagnostic")
            location = note["location"]
            if (not isinstance(location, dict) or set(location) != {"offset", "line", "column"}
                    or any(type(value) is not int or value < 0 for value in location.values())
                    or location["offset"] >= len(source)):
                raise ValueError("invalid context diagnostic location")
        seen.add(key)
        decisions.append({"start": first, "lastToken": last, "attribute": occurrences[key],
                          "decision": "literal-access-explicit-call-compatibility", "contextNotes": context})
    return decisions


def read_serialized(compiler: Path, library: Path, diagnostics: Path) -> dict[str, object]:
    """Read one completed parse with the installation-matched libclang C ABI."""
    import ctypes as c
    import subprocess
    # This process is itself bounded to 20 seconds by the extractor.
    version = subprocess.run([str(compiler), "--version"], capture_output=True, check=True, timeout=5)
    compiler_version = version.stdout.decode("utf-8").splitlines()[0]
    lib = c.CDLL(str(library))
    class String(c.Structure): _fields_ = [("data", c.c_void_p), ("flags", c.c_uint)]
    class Location(c.Structure): _fields_ = [("ptr", c.c_void_p * 2), ("data", c.c_uint)]
    class Range(c.Structure): _fields_ = [("ptr", c.c_void_p * 2), ("begin", c.c_uint), ("end", c.c_uint)]
    def bind(name, arguments, result):
        function = getattr(lib, name); function.argtypes = arguments; function.restype = result
        return function
    string = bind("clang_getCString", [String], c.c_char_p)
    dispose = bind("clang_disposeString", [String], None)
    def text(value):
        raw = string(value); result = raw.decode("utf-8") if raw else ""; dispose(value); return result
    library_version = text(bind("clang_getClangVersion", [], String)())
    require_matching_version(compiler_version, library_version)
    load = bind("clang_loadDiagnostics", [c.c_char_p, c.POINTER(c.c_int), c.POINTER(String)], c.c_void_p)
    count = bind("clang_getNumDiagnosticsInSet", [c.c_void_p], c.c_uint)
    get = bind("clang_getDiagnosticInSet", [c.c_void_p, c.c_uint], c.c_void_p)
    severity = bind("clang_getDiagnosticSeverity", [c.c_void_p], c.c_int)
    option = bind("clang_getDiagnosticOption", [c.c_void_p, c.POINTER(String)], String)
    location = bind("clang_getDiagnosticLocation", [c.c_void_p], Location)
    children = bind("clang_getChildDiagnostics", [c.c_void_p], c.c_void_p)
    range_count = bind("clang_getDiagnosticNumRanges", [c.c_void_p], c.c_uint)
    get_range = bind("clang_getDiagnosticRange", [c.c_void_p, c.c_uint], Range)
    start = bind("clang_getRangeStart", [Range], Location)
    end = bind("clang_getRangeEnd", [Range], Location)
    file_location = bind("clang_getFileLocation", [Location, c.POINTER(c.c_void_p), c.POINTER(c.c_uint),
                                                  c.POINTER(c.c_uint), c.POINTER(c.c_uint)], None)
    # Serialized filenames follow presumed #line names. Physical offsets are
    # meaningful because the owning parse has one retained cpp-output operand,
    # no module inputs, and no preprocessing that could load a second buffer.
    def located(value):
        file = c.c_void_p(); line = c.c_uint(); column = c.c_uint(); position = c.c_uint()
        file_location(value, c.byref(file), c.byref(line), c.byref(column), c.byref(position))
        if not file: raise ValueError("diagnostic lacks a physical offset")
        return {"offset": position.value, "line": line.value, "column": column.value}
    def offset(value):
        return located(value)["offset"]
    error = c.c_int(); detail = String()
    result = load(str(diagnostics).encode(), c.byref(error), c.byref(detail))
    detail_text = text(detail)
    if not result or error.value: raise ValueError("invalid serialized diagnostics: " + detail_text)
    category = bind("clang_getDiagnosticCategory", [c.c_void_p], c.c_uint)
    fixits = bind("clang_getDiagnosticNumFixIts", [c.c_void_p], c.c_uint)
    def context_note(value):
        nested = children(value)
        return {"severity": severity(value), "option": text(option(value, None)),
                "category": category(value), "location": located(location(value)),
                "ranges": [[offset(start(get_range(value, j))), offset(end(get_range(value, j)))]
                           for j in range(range_count(value))], "fixits": fixits(value),
                "children": [context_note(get(nested, j)) for j in range(count(nested))] if nested else []}
    entries = []
    try:
        for index in range(count(result)):
            diagnostic = get(result, index)
            ranges = [get_range(diagnostic, number) for number in range(range_count(diagnostic))]
            child_set = children(diagnostic)
            entries.append({"severity": severity(diagnostic), "option": text(option(diagnostic, None)),
                            "offset": offset(location(diagnostic)),
                            "ranges": [[offset(start(value)), offset(end(value))] for value in ranges],
                            "children": [context_note(get(child_set, j)) for j in range(count(child_set))] if child_set else []})
    finally:
        bind("clang_disposeDiagnosticSet", [c.c_void_p], None)(result)
    return {"compilerVersion": compiler_version, "libraryVersion": library_version, "diagnostics": entries}


if __name__ == "__main__":
    if len(sys.argv) != 4: raise SystemExit("expected compiler, matched library, serialized diagnostics")
    print(json.dumps(read_serialized(*(Path(value) for value in sys.argv[1:]))))
