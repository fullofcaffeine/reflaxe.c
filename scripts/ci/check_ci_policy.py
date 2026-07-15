#!/usr/bin/env python3
"""Fail closed when required toolchain and native-smoke wiring drifts."""

from __future__ import annotations

import json
import sys
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parents[2]
WORKFLOW = ROOT / ".github/workflows/governance.yml"
PACKAGE = ROOT / "package.json"
PRE_COMMIT = ROOT / "scripts/hooks/pre-commit"

REQUIRED_NATIVE_FILES = (
    "runtime/hxrt/include/hxc_runtime.h",
    "runtime/hxrt/src/hxc_runtime.c",
    "runtime/hxrt/test/runtime_smoke.c",
    "runtime/hxrt/test/public_header_cpp.cpp",
    "test/c_ast/CASTGolden.hx",
    "test/c_ast/ExpressionGolden.hx",
    "test/c_ast/c_ast.hxml",
    "test/c_ast/expression.hxml",
    "test/c_ast/expected/declarators.c",
    "test/c_ast/expected/expressions.c",
    "test/c_ast/run.py",
    "test/native/pointlib/include/pointlib.h",
    "test/native/pointlib/src/pointlib.c",
    "test/native/pointlib/smoke.c",
    "test/native/cpp_shim/include/counter_shim.h",
    "test/native/cpp_shim/src/counter_shim.cpp",
    "test/native/cpp_shim/smoke.c",
    "scripts/ci/runtime_smoke.py",
)

REQUIRED_WORKFLOW_SNIPPETS = (
    "  pinned-toolchain:\n",
    "  native-smoke:\n",
    "  license-and-provenance:\n",
    "      fail-fast: false\n",
    "        toolchain:\n",
    "          - gcc\n",
    "          - clang\n",
    'python3 scripts/ci/runtime_smoke.py --toolchain "${{ matrix.toolchain }}"',
    "python3 scripts/ci/check_ci_policy.py",
)


def load_json(path: Path, errors: list[str]) -> dict[str, Any]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        errors.append(f"cannot read {path.relative_to(ROOT)}: {error}")
        return {}
    if not isinstance(value, dict):
        errors.append(f"{path.relative_to(ROOT)} must contain a JSON object")
        return {}
    return value


def read_text(path: Path, errors: list[str]) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        errors.append(f"cannot read {path.relative_to(ROOT)}: {error}")
        return ""


def validate() -> list[str]:
    errors: list[str] = []
    missing_files = [path for path in REQUIRED_NATIVE_FILES if not (ROOT / path).is_file()]
    if missing_files:
        errors.append("required native smoke files are missing: " + ", ".join(missing_files))

    package = load_json(PACKAGE, errors)
    scripts = package.get("scripts", {})
    if not isinstance(scripts, dict):
        errors.append("package.json scripts must be an object")
        scripts = {}
    if scripts.get("test:native") != "python3 scripts/ci/runtime_smoke.py":
        errors.append("package.json must retain the test:native entry point")
    if scripts.get("test:c-ast") != "python3 test/c_ast/run.py":
        errors.append("package.json must retain the test:c-ast entry point")
    if "npm run test:c-ast" not in str(scripts.get("test:toolchain", "")):
        errors.append("package.json test:toolchain must execute test:c-ast")
    if "npm run test:native" not in str(scripts.get("test", "")):
        errors.append("package.json test must execute test:native")
    if "python3 scripts/ci/check_ci_policy.py" not in str(
        scripts.get("test:governance", "")
    ):
        errors.append("package.json test:governance must execute the CI policy guard")

    workflow = read_text(WORKFLOW, errors)
    for snippet in REQUIRED_WORKFLOW_SNIPPETS:
        if snippet not in workflow:
            errors.append(f"governance workflow is missing required CI contract: {snippet.strip()}")

    pre_commit = read_text(PRE_COMMIT, errors)
    if "scripts/ci/runtime_smoke.py" not in pre_commit:
        errors.append("pre-commit must run the native smoke harness for relevant changes")
    if "scripts/ci/check_ci_policy.py" not in pre_commit:
        errors.append("pre-commit must validate required CI wiring")
    if "test/c_ast/run.py" not in pre_commit:
        errors.append("pre-commit must run the structural C AST golden test")

    runner = read_text(ROOT / "scripts/ci/runtime_smoke.py", errors)
    for required_flag in ("-std=c11", "-std=c++17", "-Werror", "-pedantic"):
        if required_flag not in runner:
            errors.append(f"native smoke runner lost strict flag {required_flag}")
    if "required toolchain" not in runner or "SKIP" not in runner:
        errors.append("native smoke runner must distinguish required failures from optional skips")
    if "structural-c-ast-golden-run" not in runner or "C_AST_GOLDENS" not in runner:
        errors.append("native smoke runner must compile and execute the declarator C AST golden")
    if "expression-precedence-golden-run" not in runner or "expressions.c" not in runner:
        errors.append("native smoke runner must compile and execute the expression C AST golden")

    return errors


def main() -> int:
    errors = validate()
    if errors:
        for error in errors:
            print(f"ci-policy: ERROR: {error}", file=sys.stderr)
        return 1
    print("ci-policy: OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
