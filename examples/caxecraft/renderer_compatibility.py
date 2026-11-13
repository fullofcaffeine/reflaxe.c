"""Certify parser-only GNU forwarding compatibility outside the measured interval.

GCC headers can contain va_arg_pack forwarding bodies that Clang cannot parse.
Blank only the final pack operand of a closed direct-return call. A separate
conditional grammar also replaces exact pack-length calls with zero placeholders.
These placeholders do not model the native argument count. Every original branch,
statement and non-pack call must remain in the successful AST at its byte offset.
The AST must independently bind each edit to a declaration and parameter identities. Every affected
declaration is excluded from roots, calls, markers and reviewed leaves by the
owning graph before traversal. Native compilation always uses the original bytes.

Allocator association and format warnings have separate occurrence certificates.
Unknown diagnostics and unsuccessful parses remain fatal. This boundary proves
only explicit-call coverage of the remaining graph, never native ABI, optimization,
object-size or arbitrary format-string equivalence. haxe_c-rl9u owns integration.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import re

from renderer_diagnostics import admit_diagnostics, require_matching_version

# LLVM llvmorg-21.1.8: clang/include/clang/Basic/Builtins.td declares these
# fixed counts and PrintfFormat<3>/<4>. BuiltinsBase.td and
# utils/TableGen/ClangBuiltinsEmitter.cpp encode p:I:. Basic/Builtins.cpp
# decodes that index; Sema/SemaDecl.cpp:AddKnownFunctionAttributes constructs
# FormatAttr(I+1, I+2). Thus the format is the last fixed argument in both.
# The caller verifies the exact Clang/libclang version and pins tool bytes.
PRINTF_FIXED_PARAMETERS = {"__builtin___sprintf_chk": 4, "__builtin___snprintf_chk": 5}

PACK = b"__builtin_va_arg_pack"
PACK_LENGTH = b"__builtin_va_arg_pack_len"
OBJECT_SIZE = {b"__builtin_object_size", b"__builtin_dynamic_object_size"}
IDENT = re.compile(b"[A-Za-z_][A-Za-z_0-9]*\\Z")
DECIMAL = re.compile(b"(?:0|[1-9][0-9]*)\\Z")


class CompatibilityFailure(ValueError):
    """An occurrence lacks the complete evidence needed for parser compatibility."""


def _require(ok, message):
    if not ok:
        raise CompatibilityFailure(message)


@dataclass(frozen=True)
class _Token:
    """A token keeps physical byte positions in one immutable parser input."""

    text: bytes
    start: int
    end: int


@dataclass(frozen=True)
class ForwardingEdit:
    """A proposed tail removal; only the later successful AST can certify it."""

    body_start: int
    body_end: int
    call_start: int
    call_end: int
    callee: str
    arguments: tuple[tuple[bytes, ...], ...]
    start: int
    end: int
    conditional: bool = False
    length_ranges: tuple[tuple[int, int], ...] = ()


def _tokens(data):
    """Byte offsets; quoted/comment text cannot create braces or pack identifiers."""
    result = []
    i = 0
    while i < len(data):
        if data[i : i + 1] in b" \t\r\n\x0b\x0c":
            i += 1
            continue
        start = i
        if data[i : i + 2] in (b"//", b"/*"):
            if data[i : i + 2] == b"//":
                end = data.find(b"\n", i + 2)
                i = len(data) if end < 0 else end
            else:
                end = data.find(b"*/", i + 2)
                _require(end >= 0, "unterminated comment")
                i = end + 2
            continue
        if data[i] in (34, 39):
            quote = data[i]
            i += 1
            while i < len(data) and data[i] != quote:
                _require(data[i] != 10, "newline in literal")
                i += 2 if data[i] == 92 else 1
            _require(i < len(data), "unterminated literal")
            i += 1
        elif 65 <= data[i] <= 90 or 97 <= data[i] <= 122 or data[i] == 95:
            i += 1
            while i < len(data) and (
                65 <= data[i] <= 90
                or 97 <= data[i] <= 122
                or 48 <= data[i] <= 57
                or (data[i] == 95)
            ):
                i += 1
        elif 48 <= data[i] <= 57:
            i += 1
            while i < len(data) and (chr(data[i]).isalnum() or data[i] in b"._"):
                i += 1
        else:
            i += 1
        result.append(_Token(data[start:i], start, i))
    return result


def float128_offsets(source: bytes) -> tuple[int, ...]:
    """Return real type-spelling occurrences; comments and literals cannot opt in."""
    if b"_Float128" not in source:
        return ()
    return tuple(token.start for token in _tokens(source) if token.text == b"_Float128")


def require_float128_header(header: bytes) -> None:
    """Require the sole effect-free alias, with no directive or hidden declaration.

    The checked-in header owns its bytes. This grammar check constrains its
    authority; extraction separately hashes the actual file before and after use.
    """
    tokens = _tokens(header)
    _require([token.text for token in tokens] == [b"typedef", b"__float128", b"_Float128", b";"],
             "unsupported Float128 compatibility header")
    cursor = 0
    for token in tokens:
        _require(not header[cursor:token.start].strip(), "non-whitespace in Float128 header")
        cursor = token.end
    _require(not header[cursor:].strip(), "trailing Float128 header content")


def float128_target(target: str, version: str, semantic_flags: tuple[str, ...]) -> str:
    """Select only the native GCC x86_64 GNU/Linux C11 profile already verified.

    These facts come from native object verification, not the machine running
    the parser. Return an explicit parser target rather than use its host default.
    """
    _require(target in ("x86_64-linux-gnu", "x86_64-pc-linux-gnu")
             and re.match(r"(?:gcc(?:-[0-9]+)?|[^\n ]*gcc(?:-[0-9]+)?) ", version) is not None
             and "clang" not in version.lower(), "unsupported native Float128 profile")
    _require("-std=c11" in semantic_flags
             and not any(flag.startswith("-std=") and flag != "-std=c11" for flag in semantic_flags)
             and not any(flag in ("-arch", "-isysroot") for flag in semantic_flags),
             "unsupported Float128 parser dialect or target flags")
    return "x86_64-linux-gnu"


def certify_float128(source: bytes, ast: dict, header: Path) -> dict[str, object]:
    """Bind every original token to a plain external prototype parameter.

    The injected alias must denote Clang's __float128 builtin. No variable,
    expression, cast, body parameter or unmatched occurrence is admitted. This
    certificate grants no call permission or graph exclusion.
    """
    _require(ast.get("kind") == "TranslationUnitDecl", "Float128 requires complete AST")
    aliases = [node for node in _walk(ast) if node.get("kind") == "TypedefDecl"
               and node.get("name") == "_Float128"]
    _require(len(aliases) == 1, "Float128 alias conflict or missing declaration")
    alias = aliases[0]
    _require(alias.get("loc", {}).get("file") == str(header.resolve())
             and alias.get("type", {}).get("qualType") == "__float128"
             and [(node.get("kind"), node.get("type", {}).get("qualType"))
                  for node in alias.get("inner", [])] == [("BuiltinType", "__float128")]
             and _extent(alias) == (0, 28)
             and isinstance(alias.get("id"), str) and bool(alias["id"]),
             "unproved Float128 header alias")
    uses = []
    for function in ast.get("inner", []):
        if function.get("kind") != "FunctionDecl":
            continue
        for param in function.get("inner", []):
            if param.get("kind") != "ParmVarDecl" or param.get("type", {}).get("qualType") != "_Float128":
                continue
            _require(function.get("storageClass", "") in ("", "extern")
                     and not any(child.get("kind") == "CompoundStmt" for child in function.get("inner", [])),
                     "Float128 requires external bodyless prototype")
            type_info = param["type"]
            _require(type_info.get("desugaredQualType") == "__float128"
                     and type_info.get("typeAliasDeclId") == alias["id"], "Float128 parameter alias changed")
            first, end = _extent(param)
            _require(0 <= first < end <= len(source), "Float128 parameter range")
            location = param["range"]["begin"]
            _require(not any(key in location for key in ("spellingLoc", "expansionLoc")),
                     "expanded Float128 parameter")
            spelling = [token.text for token in _tokens(source[first:end])]
            expected = [b"_Float128"] + ([param["name"].encode("ascii")] if param.get("name") else [])
            _require(spelling == expected, "non-plain Float128 parameter")
            uses.append(first)
    _require(bool(uses) and sorted(uses) == list(float128_offsets(source)),
             "unbound or duplicate Float128 occurrence")
    return {"canonicalType": "__float128", "aliasId": alias["id"], "parameterOffsets": sorted(uses)}


def _require_decimal_expression(xs):
    """Only unsuffixed decimal arithmetic, with explicit unary/binary operators."""
    pos = 0

    def atom():
        nonlocal pos
        _require(pos < len(xs), "missing constant operand")
        if xs[pos] in (b"+", b"-"):
            pos += 1
            atom()
            return
        if xs[pos] == b"(":
            pos += 1
            expr()
            _require(pos < len(xs) and xs[pos] == b")", "constant parenthesis")
            pos += 1
            return
        _require(DECIMAL.fullmatch(xs[pos]) is not None, "nondecimal constant")
        pos += 1

    def expr():
        nonlocal pos
        atom()
        while pos < len(xs) and xs[pos] in (b"+", b"-", b"*", b"/", b"%"):
            pos += 1
            atom()

    expr()
    _require(pos == len(xs), "unsupported constant tokens")


def _argument_parameters(xs):
    if len(xs) == 1 and IDENT.fullmatch(xs[0]):
        return {xs[0].decode()}
    if (
        len(xs) == 6
        and xs[0] in OBJECT_SIZE
        and (xs[1] == b"(")
        and IDENT.fullmatch(xs[2])
        and (xs[3] == b",")
        and (xs[4] in (b"0", b"1", b"2", b"3"))
        and (xs[5] == b")")
    ):
        return {xs[2].decode()}
    _require_decimal_expression(xs)
    return set()


@dataclass(frozen=True)
class _ConditionalStatement:
    """An original statement inventory; a later AST must reproduce every node."""

    kind: str
    start: int
    end: int
    children: tuple = ()
    expression: tuple = ()
    callee: str = ""
    arguments: tuple = ()
    call_start: int = 0
    call_end: int = 0
    pack: tuple = ()


def _conditional_integer(token):
    """Only unsuffixed signed-int decimal/octal tokens; bind AST values later."""
    _require(re.fullmatch(b"(?:0|[1-9][0-9]*|0[0-7]+)", token) is not None,
             "unsupported conditional integer token")
    value = int(token, 8 if token.startswith(b"0") else 10)
    _require(value <= 2147483647, "conditional integer outside signed int")
    return value


def _conditional_body(ts):
    """Parse the closed source statement tree without granting graph authority.

    There are no locals, writes, loops, labels or indirect calls. A bare call is
    provisional: only an explicit bodyless void ErrorAttr declaration can later
    certify it. Conditions are independently checked against the successful AST.
    """
    position = 0
    lengths = []

    def take(text):
        nonlocal position
        _require(position < len(ts) and ts[position].text == text,
                 "unsupported conditional statement")
        token = ts[position]
        position += 1
        return token

    def arguments():
        nonlocal position
        take(b"(")
        first = position
        depth = 0
        result = []
        while position < len(ts):
            text = ts[position].text
            if text == b")" and depth == 0:
                if position != first:
                    result.append(tuple(ts[first:position]))
                take(b")")
                return tuple(result)
            if text == b"," and depth == 0:
                _require(position != first, "empty conditional operand")
                result.append(tuple(ts[first:position]))
                position += 1
                first = position
                continue
            _require(text not in (b";", b"{", b"}"), "unclosed conditional operands")
            depth += (text == b"(") - (text == b")")
            _require(depth >= 0, "unbalanced conditional operands")
            position += 1
        raise CompatibilityFailure("unclosed conditional call")

    def condition(tokens):
        _require(bool(tokens), "empty conditional expression")
        for index, token in enumerate(tokens):
            text = token.text
            if text == PACK_LENGTH:
                _require([t.text for t in tokens[index:index + 3]] == [PACK_LENGTH, b"(", b")"],
                         "pack length must be an empty direct call")
                lengths.append((token.start, tokens[index + 2].end))
            elif text[:1].isdigit():
                _conditional_integer(text)
            else:
                _require(IDENT.fullmatch(text) is not None or text in
                         (b"(", b")", b"&", b"|", b"=", b"!", b"<", b">"),
                         "unsupported conditional expression token")

    def statement():
        nonlocal position
        _require(position < len(ts), "conditional statement absent")
        first = ts[position]
        if first.text == b"{":
            take(b"{")
            children = []
            while position < len(ts) and ts[position].text != b"}":
                children.append(statement())
            last = take(b"}")
            return _ConditionalStatement("CompoundStmt", first.start, last.end, tuple(children))
        if first.text == b"if":
            take(b"if")
            expressions = arguments()
            _require(len(expressions) == 1, "one conditional expression required")
            condition(expressions[0])
            children = [statement()]
            if position < len(ts) and ts[position].text == b"else":
                take(b"else")
                children.append(statement())
            return _ConditionalStatement("IfStmt", first.start, children[-1].end,
                                         tuple(children), expressions[0])
        returned = first.text == b"return"
        if returned:
            take(b"return")
        _require(position < len(ts) and IDENT.fullmatch(ts[position].text) is not None,
                 "direct conditional call required")
        callee = ts[position]
        position += 1
        args = arguments()
        call_end = ts[position - 1].end
        take(b";")
        pack = ()
        if returned:
            if args and [t.text for t in args[-1]] == [PACK, b"(", b")"]:
                _require(len(args) > 1, "fixed forwarding operand absent")
                pack = (args[-2][-1].end, args[-1][0].start, args[-1][-1].end)
                args = args[:-1]
            for arg in args:
                _require(len(arg) == 1 and (IDENT.fullmatch(arg[0].text) is not None
                                           or arg[0].text[:1].isdigit()),
                         "conditional return operand must be a parameter or integer")
                if arg[0].text[:1].isdigit():
                    _conditional_integer(arg[0].text)
        else:
            _require(not args, "diagnostic helper must take no arguments")
        _require(not callee.text.startswith(b"__builtin_"), "conditional return/helper builtin")
        return _ConditionalStatement("ReturnStmt" if returned else "CallExpr", first.start,
                                     call_end, callee=callee.text.decode(), arguments=args,
                                     call_start=callee.start, call_end=call_end, pack=pack)

    tree = statement()
    _require(position == len(ts), "trailing conditional body tokens")
    return tree, tuple(lengths)


def _prepare_conditional(ts):
    tree, lengths = _conditional_body(ts)
    _require(bool(lengths), "conditional forwarding needs pack-length evidence")
    edits = []

    def visit(node):
        if node.pack:
            comma, start, last = node.pack
            # The comma must be its own token; no preceding source byte changes.
            between = [t for t in ts if comma <= t.start < start]
            _require([t.text for t in between] == [b","], "forwarding comma absent")
            edits.append(ForwardingEdit(tree.start, tree.end, node.call_start, node.call_end,
                                        node.callee, tuple(tuple(t.text for t in a) for a in node.arguments),
                                        between[0].start, last, True, lengths))
        for child in node.children:
            visit(child)

    visit(tree)
    _require(bool(edits), "conditional forwarding pack absent")
    return edits


def _original_condition_extent(node, lengths):
    first, last = _extent(node)
    for start, end in lengths:
        if last == start + 1:
            last = end
    return first, last


def _certify_condition(node, spelling, params, declarations, original, derived, lengths):
    """Bind the entire original expression to AST nodes, not only placeholders."""
    _require(bool(spelling), "condition spelling absent")
    first, last = spelling[0].start, spelling[-1].end
    _require(_original_condition_extent(node, lengths) == (first, last), "condition extent mismatch")
    _require(node.get("type", {}).get("qualType") in ("int", "const int"),
             "condition must have nonvolatile signed-int type")
    kind = node.get("kind")
    children = node.get("inner", [])
    texts = [t.text for t in spelling]
    if kind == "ImplicitCastExpr":
        _require(node.get("castKind") in ("LValueToRValue", "NoOp") and len(children) == 1,
                 "unsupported condition conversion")
        return _certify_condition(children[0], spelling, params, declarations, original, derived, lengths)
    if kind == "ParenExpr":
        _require(len(children) == 1 and texts[0] == b"(" and texts[-1] == b")", "condition parentheses")
        return _certify_condition(children[0], spelling[1:-1], params, declarations, original, derived, lengths)
    if kind == "IntegerLiteral":
        _require(not children, "literal children")
        if (first, last) in lengths:
            _require(texts == [PACK_LENGTH, b"(", b")"] and node.get("value") == "0"
                     and _extent(node) == (first, first + 1), "pack length placeholder mismatch")
        else:
            _require(len(spelling) == 1 and str(_conditional_integer(texts[0])) == node.get("value"),
                     "integer source/AST value mismatch")
        return
    if kind == "DeclRefExpr":
        ref = node.get("referencedDecl", {})
        _require(len(spelling) == 1 and not children and ref.get("kind") == "ParmVarDecl"
                 and params.get(ref.get("name")) == ref.get("id")
                 and ref.get("name", "").encode() == texts[0], "condition parameter identity mismatch")
        return
    if kind == "BinaryOperator":
        op = node.get("opcode")
        _require(op in ("&&", "||", "&", "|", "==", "!=", "<", ">", "<=", ">=") and len(children) == 2,
                 "unsupported condition operator")
        left, right = children
        a, b = _original_condition_extent(left, lengths)
        c, d = _original_condition_extent(right, lengths)
        _require(a == first and d == last and b <= c
                 and [t.text for t in _tokens(original[b:c])] == [bytes([x]) for x in op.encode()],
                 "condition operator/source mismatch")
        for child, begin, end in ((left, a, b), (right, c, d)):
            child_spelling = tuple(t for t in spelling if begin <= t.start and t.end <= end)
            _certify_condition(child, child_spelling, params, declarations, original, derived, lengths)
        return
    if kind == "CallExpr":
        _require(len(children) == 2 and len(spelling) == 4 and texts[0] == b"__builtin_constant_p"
                 and texts[1] == b"(" and texts[3] == b")", "unreviewed condition call")
        target = _strip_expression_wrappers(children[0])
        ref = target.get("referencedDecl", {})
        callee = declarations.get(ref.get("id"), {})
        _require(ref.get("kind") == "FunctionDecl" and ref.get("name") == "__builtin_constant_p"
                 and callee.get("isImplicit") is True and _children_of_kind(callee, "BuiltinAttr"),
                 "constant predicate is not the compiler builtin")
        _require(target.get("kind") == "DeclRefExpr" and not target.get("inner")
                 and _extent(target) == (first, first + len(texts[0])), "constant predicate token mismatch")
        _certify_condition(children[1], spelling[2:3], params, declarations, original, derived, lengths)
        return
    raise CompatibilityFailure("unsupported conditional AST " + str(kind))


def _certify_conditional(owner, body, edits, declarations, original, derived, params, marker_names):
    """Require a source/AST bijection for every branch, statement and direct call.

    Pack-length zero is parser scaffolding, never a claim about native argument
    count. The owner stays excluded by linkage before any graph traversal.
    """
    tokens = [t for t in _tokens(original) if edits[0].body_start <= t.start < edits[0].body_end]
    tree, lengths = _conditional_body(tokens)
    _require(all(e.conditional and e.length_ranges == lengths for e in edits), "conditional edit group mismatch")
    for node in _walk(body):
        if node.get("kind") == "CallExpr" and node.get("inner"):
            target = _strip_expression_wrappers(node["inner"][0]).get("referencedDecl", {})
            _require(target.get("name") not in marker_names, "marker conditional call")
    observed = []

    def visit(expected, actual):
        _require(actual.get("kind") == expected.kind and _extent(actual) == (expected.start, expected.end),
                 "conditional statement structure/extent mismatch")
        children = actual.get("inner", [])
        if expected.kind == "CompoundStmt":
            _require(len(children) == len(expected.children), "conditional block inventory mismatch")
            for a, b in zip(expected.children, children):
                visit(a, b)
            return
        if expected.kind == "IfStmt":
            _require(len(children) == 1 + len(expected.children)
                     and not any(actual.get(key) for key in ("hasInit", "hasVar", "isConstexpr")),
                     "conditional branch inventory mismatch")
            _certify_condition(children[0], expected.expression, params, declarations, original, derived, lengths)
            for a, b in zip(expected.children, children[1:]):
                visit(a, b)
            return
        if expected.kind == "ReturnStmt":
            _require(len(children) == 1, "conditional return inventory mismatch")
            call = _strip_expression_wrappers(children[0])
        else:
            call = actual
        _require(call.get("kind") == "CallExpr" and _extent(call) == (expected.call_start, expected.call_end),
                 "conditional direct call extent mismatch")
        args = call.get("inner", [])
        _require(len(args) == len(expected.arguments) + 1, "conditional call argument inventory mismatch")
        target = _strip_expression_wrappers(args[0])
        ref = target.get("referencedDecl", {})
        callee = declarations.get(ref.get("id"), {})
        _require(ref.get("kind") == "FunctionDecl" and ref.get("name") == expected.callee
                 and callee.get("name") == expected.callee
                 and not callee.get("isImplicit") and callee.get("storageClass") == "extern"
                 and not _children_of_kind(callee, "CompoundStmt"), "conditional explicit external callee required")
        _require(target.get("kind") == "DeclRefExpr" and not target.get("inner")
                 and _extent(target) == (expected.call_start, expected.call_start + len(expected.callee)),
                 "conditional callee token mismatch")
        first, last = _extent(callee)
        _require(last < _extent(owner)[0] and original[first:last] == derived[first:last],
                 "conditional callee must be a prior unchanged prototype")
        fixed = _parameters(callee)
        _require(len(fixed) == len(expected.arguments), "conditional fixed parameter count mismatch")
        if expected.kind == "CallExpr":
            _require(not callee.get("variadic") and callee.get("type", {}).get("qualType") == "void (void)"
                     and len(_children_of_kind(callee, "ErrorAttr")) == 1,
                     "only an explicit void error helper is a conditional statement")
            attr = _children_of_kind(callee, "ErrorAttr")[0]
            _require(not attr.get("implicit"), "implicit diagnostic helper attribute")
            xs = _gnu_attribute_tokens(original, attr)
            _require(len(xs) == 4 and xs[0] in (b"error", b"__error__")
                     and xs[1] == b"(" and xs[2].startswith(b'"') and xs[3] == b")",
                     "diagnostic helper attribute source mismatch")
        else:
            _require(callee.get("type", {}).get("qualType", "").startswith("int (")
                     and bool(callee.get("variadic")) == bool(expected.pack), "conditional return signature mismatch")
            for node, spelling in zip(args[1:], expected.arguments):
                if IDENT.fullmatch(spelling[0].text):
                    _certify_operand(node, (spelling[0].text,), params, derived)
                else:
                    _certify_condition(node, spelling, params, declarations, original, derived, ())
            _require(all(arg.get("type") == parameter.get("type")
                         for arg, parameter in zip(args[1:], fixed)),
                     "conditional fixed argument types differ")
            if expected.pack:
                observed.append(expected.call_start)
    visit(tree, body)
    _require(observed == [e.call_start for e in edits], "conditional pack occurrence inventory mismatch")


def prepare_parser_input(original: bytes) -> tuple[bytes, tuple[ForwardingEdit, ...]]:
    """Propose offset-preserving edits; this step grants no coverage authority."""
    ts = _tokens(original)
    stack = []
    pairs = {}
    owners = {}
    for n, t in enumerate(ts):
        if t.text == b"{":
            stack.append(n)
        elif t.text == b"}":
            _require(bool(stack), "unbalanced brace")
            left = stack.pop()
            pairs[left] = n
        elif t.text == PACK:
            _require(bool(stack), "pack must be in one top-level body")
            owners.setdefault(stack[0], []).append(n)
    _require(not stack, "unbalanced brace")
    edits = []
    for left, occurrences in owners.items():
        right = pairs[left]
        if any(t.text == b"if" for t in ts[left + 1:right]):
            edits.extend(_prepare_conditional(ts[left:right + 1]))
            continue
        _require(len(occurrences) == 1, "multiple pack occurrences in body")
        body = ts[left + 1 : right]
        xs = [t.text for t in body]
        _require(
            len(xs) >= 9
            and xs[0] == b"return"
            and IDENT.fullmatch(xs[1])
            and (xs[2] == b"(")
            and (xs[-2:] == [b")", b";"]),
            "single direct return call required",
        )
        _require(xs[-6:-2] == [b",", PACK, b"(", b")"], "pack must be empty final operand")
        prefix = body[3:-6]
        args = []
        start = 0
        depth = 0
        for i, t in enumerate(prefix):
            if t.text == b"(":
                depth += 1
            elif t.text == b")":
                depth -= 1
                _require(depth >= 0, "unbalanced argument")
            elif t.text == b"," and depth == 0:
                args.append(tuple((x.text for x in prefix[start:i])))
                start = i + 1
        _require(depth == 0, "unbalanced argument")
        args.append(tuple((x.text for x in prefix[start:])))
        _require(all(args), "empty prefix operand")
        for arg in args:
            _argument_parameters(arg)
        edits.append(
            ForwardingEdit(
                ts[left].start,
                ts[right].end,
                body[1].start,
                body[-2].end,
                xs[1].decode(),
                tuple(args),
                body[-6].start,
                body[-3].end,
            )
        )
    _require(len(edits) == sum((t.text == PACK for t in ts)), "unmapped pack occurrence")
    result = bytearray(original)
    for edit in edits:
        for i in range(edit.start, edit.end):
            if result[i] not in (10, 13):
                result[i] = 32
    lengths = {r for edit in edits for r in edit.length_ranges}
    _require(len(lengths) == sum(t.text == PACK_LENGTH for t in ts), "unmapped pack length occurrence")
    for first, last in lengths:
        result[first:last] = bytes(48 if i == first else (original[i] if original[i] in (10, 13) else 32)
                                   for i in range(first, last))
    return (bytes(result), tuple(edits))


def _walk(node):
    yield node
    for child in node.get("inner", []):
        yield from _walk(child)


def _extent(node):
    r = node.get("range", {})
    a = r.get("begin", {})
    b = r.get("end", {})
    _require(
        type(a.get("offset")) is int
        and type(b.get("offset")) is int
        and (type(b.get("tokLen")) is int),
        "physical range absent",
    )
    return (a["offset"], b["offset"] + b["tokLen"])


def _strip_expression_wrappers(node):
    while node.get("kind") in ("ImplicitCastExpr", "ParenExpr"):
        _require(len(node.get("inner", [])) == 1, "ambiguous expression wrapper")
        node = node["inner"][0]
    return node


def _certify_operand(node, spelling, params, source):
    """Bind retained source tokens to a closed AST and exact parameter IDs."""
    a, b = _extent(node)
    _require(
        tuple((t.text for t in _tokens(source[a:b]))) == spelling, "operand source range mismatch"
    )

    def inspect(n, allow_object=False):
        n = _strip_expression_wrappers(n)
        kind = n.get("kind")
        children = n.get("inner", [])
        if kind == "DeclRefExpr":
            ref = n.get("referencedDecl", {})
            _require(
                not children
                and ref.get("kind") == "ParmVarDecl"
                and (params.get(ref.get("name")) == ref.get("id"))
                and (ref.get("id") is not None),
                "operand parameter binding mismatch",
            )
        elif kind == "IntegerLiteral":
            _require(
                not children and DECIMAL.fullmatch(str(n.get("value", "")).encode()) is not None,
                "nondecimal AST literal",
            )
        elif kind in ("UnaryOperator", "BinaryOperator"):
            _require(
                n.get("opcode")
                in (("+", "-") if kind == "UnaryOperator" else ("+", "-", "*", "/", "%"))
                and len(children) == (1 if kind == "UnaryOperator" else 2)
                and (not n.get("isPostfix", False)),
                "unsupported AST operator",
            )
            for child in children:
                inspect(child)
        elif kind == "CallExpr" and allow_object:
            _require(len(children) == 3, "object-size argument count")
            ref = _strip_expression_wrappers(children[0]).get("referencedDecl", {})
            _require(
                ref.get("kind") == "FunctionDecl"
                and ref.get("name", "").encode() == spelling[0]
                and (spelling[0] in OBJECT_SIZE),
                "object-size callee mismatch",
            )
            inspect(children[1])
            inspect(children[2])
            _require(
                _strip_expression_wrappers(children[1]).get("kind") == "DeclRefExpr"
                and _strip_expression_wrappers(children[2]).get("kind") == "IntegerLiteral"
                and (
                    str(_strip_expression_wrappers(children[2]).get("value")).encode()
                    == spelling[4]
                ),
                "object-size operand shape",
            )
        else:
            raise CompatibilityFailure("unsupported operand AST " + str(kind))

    inspect(node, bool(spelling and spelling[0] in OBJECT_SIZE))


def _certify_forwarding(original, derived, edits, ast, *, parse_returncode, marker_names):
    """No failed/partial AST can certify; caller must retain successful diagnostics.

    Only the two reviewed Clang 21.1.8 fortified signatures receive builtin
    authority. The diagnostic owner must verify the matching version profile.
    """
    _require(
        parse_returncode == 0 and ast.get("kind") == "TranslationUnitDecl",
        "complete successful parse required",
    )
    expected, expected_edits = prepare_parser_input(original)
    _require(derived == expected and edits == expected_edits, "transformation mismatch")
    if not edits:
        # Ordinary units need no forwarding index. Their complete AST still goes
        # through the existing graph checks; this skips only compatibility work.
        return []
    builtin_signatures = PRINTF_FIXED_PARAMETERS
    decls = {
        node["id"]: node
        for node in _walk(ast)
        if node.get("kind") == "FunctionDecl" and "id" in node
    }
    if any(edit.conditional for edit in edits):
        _require(len(decls) == sum(node.get("kind") == "FunctionDecl" for node in _walk(ast)),
                 "conditional function declaration identities ambiguous")
    excluded = []
    conditional_done = set()
    for edit in edits:
        body_key = (edit.body_start, edit.body_end)
        if edit.conditional and body_key in conditional_done:
            continue
        matches = [
            (f, b)
            for f in decls.values()
            for b in f.get("inner", [])
            if b.get("kind") == "CompoundStmt" and _extent(b) == (edit.body_start, edit.body_end)
        ]
        _require(len(matches) == 1, "ambiguous physical owner")
        owner, body = matches[0]
        _require(owner.get("name") not in marker_names, "modified marker implementation")
        attrs = {n["kind"] for n in owner.get("inner", []) if n.get("kind", "").endswith("Attr")}
        _require({"GNUInlineAttr", "AlwaysInlineAttr"} <= attrs, "owner lacks exclusion attributes")
        _require(
            owner.get("variadic") is True
            and owner.get("type", {}).get("qualType", "").startswith("int ("),
            "explicit int-returning variadic owner required",
        )
        begin, end = _extent(owner)
        _require(begin < edit.body_start and end == edit.body_end, "owner extent mismatch")
        header = [t.text for t in _tokens(original[begin : edit.body_start])]
        _require(
            not any(
                (
                    x in header
                    for x in (b"typeof", b"__typeof", b"__typeof__", b"__auto_type", b"auto")
                )
            ),
            "inferred/type-dependent declaration",
        )
        params = {
            n.get("name"): n.get("id")
            for n in owner.get("inner", [])
            if n.get("kind") == "ParmVarDecl"
        }
        if edit.conditional:
            _require(len(params) == len(_parameters(owner))
                     and all(type(name) is str and type(identity) is str for name, identity in params.items()),
                     "conditional owner parameter identities ambiguous")
            _require(all(not re.search(r"\b(?:volatile|_Atomic)\b", p.get("type", {}).get("qualType", ""))
                         for p in _parameters(owner)), "conditional volatile or atomic parameter")
            group = [e for e in edits if (e.body_start, e.body_end) == body_key]
            _certify_conditional(owner, body, group, decls, original, derived, params, marker_names)
            excluded.append({"name": owner["name"], "storage": owner.get("storageClass", ""),
                             "declarationId": owner["id"], "bodyRange": list(body_key)})
            conditional_done.add(body_key)
            continue
        _require(
            all((_argument_parameters(a) <= params.keys() for a in edit.arguments)),
            "operand is not owner parameter",
        )
        children = body.get("inner", [])
        _require(
            len(children) == 1 and children[0].get("kind") == "ReturnStmt", "body shape changed"
        )
        returned = children[0].get("inner", [])
        _require(len(returned) == 1, "return shape")
        call = _strip_expression_wrappers(returned[0])
        _require(
            call.get("kind") == "CallExpr" and _extent(call) == (edit.call_start, edit.call_end),
            "call ownership mismatch",
        )
        args = call.get("inner", [])
        _require(len(args) == len(edit.arguments) + 1, "argument count mismatch")
        for node, spelling in zip(args[1:], edit.arguments):
            _certify_operand(node, spelling, params, derived)
        ref = _strip_expression_wrappers(args[0]).get("referencedDecl", {})
        _require(
            ref.get("kind") == "FunctionDecl" and ref.get("name") == edit.callee,
            "not exact direct callee",
        )
        callee = decls.get(ref.get("id"))
        _require(
            callee is not None
            and callee.get("variadic") is True
            and callee.get("type", {}).get("qualType", "").startswith("int ("),
            "int variadic prototype absent",
        )
        fixed = [n for n in callee.get("inner", []) if n.get("kind") == "ParmVarDecl"]
        _require(len(edit.arguments) >= len(fixed), "fixed arguments missing")
        if callee.get("isImplicit") or edit.callee.startswith("__builtin_"):
            _require(
                edit.callee in builtin_signatures and len(fixed) == builtin_signatures[edit.callee],
                "unreviewed builtin signature",
            )
        _require(edit.callee not in marker_names, "marker forwarding call")
        excluded.append(
            {
                "name": owner["name"],
                "storage": owner.get("storageClass", ""),
                "declarationId": owner["id"],
                "bodyRange": [edit.body_start, edit.body_end],
            }
        )
    return excluded


def _parameters(function):
    return [node for node in function.get("inner", []) if node.get("kind") == "ParmVarDecl"]


def _children_of_kind(node, kind):
    return [child for child in node.get("inner", []) if child.get("kind") == kind]


def _is_plain_pointer(t):
    return isinstance(t, str) and t.strip().endswith("*") and ("(" not in t)


def _gnu_attribute_tokens(source, node):
    """Only an exact singleton GNU attribute list; reject other namespaces/lists."""
    a, b = _extent(node)
    ts = _tokens(source)
    starts = [i for i, t in enumerate(ts) if t.start == a]
    _require(len(starts) == 1, "attribute start absent")
    i = starts[0]
    ends = [j for j in range(i, len(ts)) if ts[j].end == b]
    _require(len(ends) == 1, "attribute end absent")
    j = ends[0]
    _require(
        i >= 3
        and [t.text for t in ts[i - 3 : i]] == [b"__attribute__", b"(", b"("]
        and ([t.text for t in ts[j + 1 : j + 3]] == [b")", b")"]),
        "not singleton GNU attribute",
    )
    return tuple((t.text for t in ts[i : j + 1]))


def _allocator_certificate(source, derived, functions, diagnostic):
    """Bind an allocator association to unchanged declarations and pointer types.

    The association is metadata, not a runtime deallocation call. GCC also uses
    it for inlining policy, so the allocator is excluded rather than treated as
    semantically equivalent after Clang ignores the association.
    """
    offset = diagnostic["offset"]
    matches = []
    for owner in functions:
        for attr in _children_of_kind(owner, "RestrictAttr"):
            try:
                a, b = _extent(attr)
            except CompatibilityFailure:
                continue
            if a == offset:
                matches.append((owner, attr, a, b))
    _require(len(matches) == 1, "malloc ownership ambiguous")
    owner, attr, a, b = matches[0]
    first, last = _extent(owner)
    _require(source[first:last] == derived[first:last], "malloc declaration changed")
    _require(
        not _children_of_kind(owner, "CompoundStmt")
        and (not owner.get("inline"))
        and (owner.get("storageClass") == "extern"),
        "malloc requires external declaration only",
    )
    xs = _gnu_attribute_tokens(source, attr)
    _require(
        len(xs) == 6
        and xs[0] in (b"malloc", b"__malloc__")
        and (xs[1] == b"(")
        and IDENT.fullmatch(xs[2])
        and (xs[3] == b",")
        and DECIMAL.fullmatch(xs[4])
        and (xs[5] == b")"),
        "unsupported malloc grammar",
    )
    index = int(xs[4])
    name = xs[2].decode()
    candidates = [f for f in functions if f.get("name") == name and (not f.get("isImplicit"))]
    _require(len(candidates) == 1, "ambiguous deallocator declaration")
    deallocator = candidates[0]
    dfirst, dlast = _extent(deallocator)
    _require(
        dlast < first
        and source[dfirst:dlast] == derived[dfirst:dlast]
        and (deallocator.get("storageClass") == "extern"),
        "deallocator must be prior unchanged external declaration",
    )
    _require(
        not _children_of_kind(deallocator, "CompoundStmt") and (not deallocator.get("variadic")),
        "unsupported deallocator shape",
    )
    ps = _parameters(deallocator)
    _require(1 <= index <= len(ps), "deallocator index invalid")
    result = owner.get("type", {}).get("qualType", "").split("(", 1)[0].strip()
    arg = ps[index - 1].get("type", {}).get("qualType", "").strip()
    _require(
        _is_plain_pointer(result) and _is_plain_pointer(arg) and (result == arg),
        "pointer association type not proved",
    )
    return {
        "kind": "malloc",
        "offset": offset,
        "name": owner["name"],
        "declarationId": owner["id"],
        "storage": owner.get("storageClass", ""),
        "declarationRange": [first, last],
        "attributeRange": [a, b],
        "deallocatorId": deallocator["id"],
        "deallocator": name,
        "pointerIndex": index,
    }


def _printf_signature(source, callee):
    authority = {name: (count, count + 1) for name, count in PRINTF_FIXED_PARAMETERS.items()}
    fs = _children_of_kind(callee, "FormatAttr")
    _require(len(fs) == 1, "format attribute ambiguous")
    if fs[0].get("implicit"):
        _require(
            callee.get("isImplicit")
            and _children_of_kind(callee, "BuiltinAttr")
            and (callee.get("name") in authority),
            "unreviewed implicit format signature",
        )
        return (authority[callee["name"]], {"authority": "LLVM 21.1.8 PrintfFormat index contract"})
    xs = _gnu_attribute_tokens(source, fs[0])
    _require(
        len(xs) == 8
        and xs[0] in (b"format", b"__format__")
        and (xs[1] == b"(")
        and (xs[2] in (b"printf", b"__printf__"))
        and (xs[3] == b",")
        and DECIMAL.fullmatch(xs[4])
        and (xs[5] == b",")
        and DECIMAL.fullmatch(xs[6])
        and (xs[7] == b")"),
        "unsupported explicit format syntax",
    )
    a, b = _extent(fs[0])
    fa, fb = _extent(callee)
    _require(fa <= a < b <= fb, "format attr not owned by callee")
    return ((int(xs[4]), int(xs[6])), {"authority": "explicit GNU format", "range": [a, b]})


def _format_certificate(source, derived, functions, edits, excluded, diagnostic):
    """Accept a warning only at the certified wrapper's last fixed format operand.

    A removed pack may contain zero arguments; this does not prove the original
    format safe. It proves only where the warning arose in an excluded owner.
    Ordinary calls without an original pack cannot use this certificate.
    """
    offset = diagnostic["offset"]
    byid = {f["id"]: f for f in functions}
    matches = [e for e in edits if e.call_start <= offset < e.call_end]
    _require(len(matches) == 1, "format warning outside unique edited call")
    edit = matches[0]
    _require(not edit.conditional, "conditional format diagnostic unsupported")
    owners = [x for x in excluded if x.get("bodyRange") == [edit.body_start, edit.body_end]]
    _require(len(owners) == 1, "edited owner certificate absent")
    owner = byid[owners[0]["declarationId"]]
    body = _children_of_kind(owner, "CompoundStmt")
    _require(len(body) == 1, "body absent")
    call = _strip_expression_wrappers(body[0]["inner"][0]["inner"][0])
    children = call["inner"]
    callee_ref = _strip_expression_wrappers(children[0])["referencedDecl"]
    callee = byid[callee_ref["id"]]
    sig, evidence = _printf_signature(source, callee)
    fixed = len(_parameters(callee))
    _require(
        callee.get("variadic") is True
        and sig == (fixed, fixed + 1)
        and (len(children) - 1 == fixed),
        "not last-fixed format without remaining varargs",
    )
    operand = _strip_expression_wrappers(children[-1])
    _require(operand.get("kind") == "DeclRefExpr", "nonparameter format operand")
    ref = operand.get("referencedDecl", {})
    ps = {p["id"]: p for p in _parameters(owner)}
    _require(
        ref.get("kind") == "ParmVarDecl"
        and ref.get("id") in ps
        and (ref.get("name") == ps[ref["id"]].get("name")),
        "format parameter binding mismatch",
    )
    a, b = _extent(operand)
    _require(
        offset == a
        and source[a:b] == ref["name"].encode()
        and (edit.arguments[-1] == (ref["name"].encode(),)),
        "not exact format token",
    )
    _require(
        b <= edit.start and (not source[b : edit.start].strip()),
        "format not directly before removed comma",
    )
    return {
        "kind": "format",
        "offset": offset,
        "name": owner["name"],
        "declarationId": owner["id"],
        "storage": owner.get("storageClass", ""),
        "callRange": [edit.call_start, edit.call_end],
        "formatTokenRange": [a, b],
        "formatParameterId": ref["id"],
        "calleeId": callee["id"],
        "formatPosition": sig[0],
        "firstCheckedPosition": sig[1],
        "signature": evidence,
    }


def _require_diagnostic_shape(diagnostic, certificate, source):
    """Keep the tested serialized diagnostic profile closed, including child notes."""
    _require(
        type(diagnostic["severity"]) is int
        and diagnostic["severity"] == 2
        and (type(diagnostic["fixits"]) is int)
        and (diagnostic["fixits"] == 0),
        "error/fixits not admitted",
    )
    kind = certificate["kind"]
    offset = certificate["offset"]
    _require(
        diagnostic["offset"] == offset and type(diagnostic["category"]) is int,
        "offset/category invalid",
    )
    _require(
        (
            (diagnostic["option"], diagnostic["category"], diagnostic["ranges"])
            == ("-Wignored-attributes", 3, [])
            if kind == "malloc"
            else (diagnostic["option"], diagnostic["category"], diagnostic["ranges"])
            == ("-Wformat-security", 35, [[offset, offset]])
        ),
        "unreviewed diagnostic shape",
    )
    semantic = 0
    for note in diagnostic["children"]:
        _require(
            set(note)
            == {"severity", "option", "category", "location", "ranges", "fixits", "children"},
            "note fields",
        )
        _require(
            type(note["severity"]) is int
            and note["severity"] == 1
            and (note["option"] == "")
            and (note["ranges"] == [])
            and (type(note["fixits"]) is int)
            and (note["fixits"] == 0)
            and (note["children"] == []),
            "nonleaf/fixit note",
        )
        loc = note["location"]
        _require(
            set(loc) == {"offset", "line", "column"}
            and all((type(x) is int and x >= 0 for x in loc.values()))
            and (loc["offset"] < len(source)),
            "note location",
        )
        _require(type(note["category"]) is int, "note category type")
        if note["category"] == 0:
            continue
        _require(
            kind == "format" and note["category"] == 3 and (loc["offset"] == offset),
            "unknown child note",
        )
        semantic += 1
    _require(semantic == (1 if kind == "format" else 0), "unexpected semantic notes")


def certify_diagnostics(original: bytes, derived: bytes, ast: dict, value: object):
    """Return occurrence decisions and explicit graph exclusions after parsing.

    The extraction owner supplies only a successful complete parse of derived
    bytes. Recompute its lexical transformation here, then bind all diagnostic
    offsets and declaration IDs before returning any exclusions to that owner.
    """
    _require(
        isinstance(value, dict)
        and set(value) == {"compilerVersion", "libraryVersion", "diagnostics"},
        "invalid structured diagnostic record",
    )
    require_matching_version(value["compilerVersion"], value["libraryVersion"])
    _require(isinstance(value["diagnostics"], list), "invalid diagnostic list")
    expected, edits = prepare_parser_input(original)
    _require(expected == derived, "parser compatibility bytes differ")
    excluded = _certify_forwarding(
        original, derived, edits, ast, parse_returncode=0, marker_names=set()
    )
    functions = [f for f in ast["inner"] if f.get("kind") == "FunctionDecl"]
    decisions = []
    seen = set()
    for diagnostic in value["diagnostics"]:
        _require(
            isinstance(diagnostic, dict)
            and set(diagnostic)
            == {"severity", "option", "offset", "ranges", "children", "category", "fixits"},
            "invalid diagnostic entry",
        )
        offset = diagnostic["offset"]
        _require(
            type(offset) is int and 0 <= offset < len(derived) and (offset not in seen),
            "invalid or duplicate diagnostic offset",
        )
        seen.add(offset)
        if diagnostic["option"] == "-Wunknown-attributes":
            _require(
                type(diagnostic["category"]) is int
                and diagnostic["category"] == 0
                and (type(diagnostic["fixits"]) is int)
                and (diagnostic["fixits"] == 0),
                "unreviewed access diagnostic profile",
            )
            old = {
                key: item for key, item in diagnostic.items() if key not in ("category", "fixits")
            }
            decisions.extend(admit_diagnostics(dict(value, diagnostics=[old]), derived))
            continue
        if diagnostic["option"] == "-Wignored-attributes":
            certificate = _allocator_certificate(original, derived, functions, diagnostic)
            excluded.append(certificate)
        elif diagnostic["option"] == "-Wformat-security":
            certificate = _format_certificate(
                original, derived, functions, edits, excluded, diagnostic
            )
        else:
            raise CompatibilityFailure("unclassified coverage diagnostic")
        _require_diagnostic_shape(diagnostic, certificate, derived)
        decisions.append(
            {
                "decision": "excluded-declaration-compatibility",
                "certificate": certificate,
                "diagnostic": diagnostic,
            }
        )
    return (decisions, excluded)
