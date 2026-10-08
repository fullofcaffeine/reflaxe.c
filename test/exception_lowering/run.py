#!/usr/bin/env python3
"""Prove closed try/catch/rethrow lowering with Eval and strict native C."""

from __future__ import annotations

import json
import os
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
if str(ROOT) not in sys.path:
	sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402


class ExceptionLoweringFailure(RuntimeError):
	"""One focused exception contract did not hold."""


def development_tool(name: str) -> str:
	if name == "haxe" and os.environ.get("HXC_TEST_HAXE"):
		return os.environ["HXC_TEST_HAXE"]
	local = ROOT / "node_modules" / ".bin" / name
	return str(local) if local.is_file() else name


def run(
	command: list[str], *, cwd: Path, timeout: int = 120, server: bool = False
) -> subprocess.CompletedProcess[str]:
	environment = os.environ.copy()
	if server:
		environment.pop("HAXE_NO_SERVER", None)
	else:
		environment["HAXE_NO_SERVER"] = "1"
	return run_bounded_process(
		command,
		cwd=cwd,
		env=environment,
		check=False,
		capture_output=True,
		text=True,
		timeout=timeout,
	)


def require_success(result: subprocess.CompletedProcess[str], label: str) -> None:
	if result.returncode != 0:
		raise ExceptionLoweringFailure(
			f"{label} failed with {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
		)


def reported_hxcir(result: subprocess.CompletedProcess[str], label: str) -> str:
	"""Read the semantic report emitted by the compiler's existing inspection path."""
	prefix = "HXC_STATIC_INITIALIZATION="
	lines = [line for line in result.stdout.splitlines() if line.startswith(prefix)]
	if len(lines) != 1:
		raise ExceptionLoweringFailure(f"{label} omitted its single HxcIR report")
	payload = json.loads(lines[0][len(prefix) :])
	hxcir = payload.get("hxcir") if isinstance(payload, dict) else None
	if not isinstance(hxcir, str):
		raise ExceptionLoweringFailure(f"{label} emitted a malformed HxcIR report")
	return hxcir


def prove_eval_oracle() -> None:
	result = run([development_tool("haxe"), "oracle.hxml"], cwd=HERE, timeout=30)
	require_success(result, "Haxe Eval exception oracle")
	if result.stderr or result.stdout.strip() != "body|inner-int:7|outer-int:8|after":
		raise ExceptionLoweringFailure(
			f"Eval exception order drifted\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
		)


def prove_cpp_header(output: Path) -> None:
	"""Prove that the private frame types remain consumable from C++17."""
	compilers = [compiler for compiler in (shutil.which("clang++"), shutil.which("g++")) if compiler]
	if not compilers:
		raise ExceptionLoweringFailure("exception header proof requires Clang++ or G++")
	for compiler in compilers:
		result = run(
			[
				compiler,
				"-std=c++17",
				"-Wall",
				"-Wextra",
				"-Werror",
				"-pedantic",
				"-I",
				str(ROOT / "runtime/hxrt/include"),
				"-c",
				str(ROOT / "runtime/hxrt/test/exception_header_cpp.cpp"),
				"-o",
				str(output / f"exception-header-{Path(compiler).name}.o"),
			],
			cwd=ROOT,
			timeout=30,
		)
		require_success(result, f"{Path(compiler).name} exception header compile")


def prove_generated_c(output: Path, connect: str) -> None:
	compile_result = run(
		[
			development_tool("haxe"),
			"--connect",
			connect,
			"generated.hxml",
			"--custom-target",
			f"c={output}",
		],
		cwd=HERE,
		server=True,
	)
	require_success(compile_result, "runtime-free exception generation")
	if "exception-strategy=closed-result" not in reported_hxcir(compile_result, "runtime-free exception generation"):
		raise ExceptionLoweringFailure("closed exception lowering did not report its selected strategy")

	plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
	if plan.get("selectedFeatures") != [] or plan.get("artifacts") != []:
		raise ExceptionLoweringFailure("closed exception region selected hxrt despite hxc_runtime=none")

	module = (output / "src" / "modules" / "Main.c").read_text(encoding="utf-8")
	for marker in (
		"hxc_Main_nestedCatchAndRethrow",
		"hxc_l_tmp_catch_number",
		"goto hxc_tmp_catch_1",
		"goto hxc_tmp_catch_0",
	):
		if marker not in module:
			raise ExceptionLoweringFailure(f"generated C omitted exception marker {marker!r}")
	if "catch_message" in module or "hxrt" in module.lower():
		raise ExceptionLoweringFailure("incompatible unused catch introduced payload storage or hxrt")

	sources = [output / "src" / "hxc" / "main.c", output / "src" / "modules" / "Main.c"]
	compilers = [compiler for compiler in (shutil.which("clang"), shutil.which("gcc")) if compiler]
	if not compilers:
		raise ExceptionLoweringFailure("strict native exception proof requires Clang or GCC")
	for compiler in compilers:
		for optimization in ("-O0", "-O2"):
			executable = output / f"exception-{Path(compiler).name}-{optimization[1:]}"
			result = run(
				[
					compiler,
					"-std=c11",
					optimization,
					"-Wall",
					"-Wextra",
					"-Werror",
					"-pedantic",
					"-I",
					str(output / "include"),
					*[str(source) for source in sources],
					"-o",
					str(executable),
				],
				cwd=ROOT,
				timeout=30,
			)
			require_success(result, f"{Path(compiler).name} {optimization} exception compile")
			require_success(run([str(executable)], cwd=ROOT, timeout=10), f"{Path(compiler).name} {optimization} exception execution")


def prove_cleanup_before_catch(output: Path, connect: str) -> None:
	compile_result = run(
		[
			development_tool("haxe"),
			"--connect",
			connect,
			"cleanup.hxml",
			"--custom-target",
			f"c={output}",
		],
		cwd=HERE,
		server=True,
	)
	require_success(compile_result, "managed cleanup exception generation")

	plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
	selected = [entry.get("id") for entry in plan.get("selectedFeatures", [])]
	if "array" not in selected or "exception" in selected:
		raise ExceptionLoweringFailure(
			f"closed cleanup region selected the wrong runtime features: {selected!r}"
		)

	module = (output / "src" / "modules" / "Main.c").read_text(encoding="utf-8")
	release = module.find("hxc_array_ref_release")
	catch_jump = module.find("goto hxc_tmp_catch_0", release)
	if release == -1 or catch_jump == -1 or release > catch_jump:
		raise ExceptionLoweringFailure("managed Array release did not precede the catch continuation")
	if module.count("hxc_array_ref_release") != 2:
		raise ExceptionLoweringFailure("normal and exceptional Array cleanup paths drifted")

	compiler = shutil.which("clang") or shutil.which("gcc")
	if compiler is None:
		raise ExceptionLoweringFailure("managed cleanup proof requires Clang or GCC")
	sources = sorted((output / "src").rglob("*.c")) + sorted((output / "runtime" / "src").glob("*.c"))
	executable = output / "exception-cleanup"
	result = run(
		[
			compiler,
			"-std=c11",
			"-O2",
			"-Wall",
			"-Wextra",
			"-Werror",
			"-pedantic",
			"-I",
			str(output / "include"),
			"-I",
			str(output / "runtime" / "include"),
			*[str(source) for source in sources],
			"-o",
			str(executable),
		],
		cwd=ROOT,
		timeout=30,
	)
	require_success(result, "managed cleanup strict native compile")
	require_success(run([str(executable)], cwd=ROOT, timeout=10), "managed cleanup native execution")


def prove_runtime_cross_call(output: Path, connect: str) -> None:
	"""Prove one contained Dynamic transfer across a generated call boundary."""
	compile_result = run(
		[
			development_tool("haxe"),
			"--connect",
			connect,
			"runtime.hxml",
			"--custom-target",
			f"c={output}",
		],
		cwd=HERE,
		server=True,
	)
	require_success(compile_result, "contained runtime exception generation")
	if "exception-strategy=contained-runtime" not in reported_hxcir(compile_result, "contained runtime exception generation"):
		raise ExceptionLoweringFailure("contained exception lowering did not report its selected strategy")

	plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
	selected = [entry.get("id") for entry in plan.get("selectedFeatures", [])]
	for required in ("array", "dynamic", "exception", "gc"):
		if required not in selected:
			raise ExceptionLoweringFailure(
				f"contained exception omitted runtime feature {required!r}: {selected!r}"
			)

	module = (output / "src" / "modules" / "Main.c").read_text(encoding="utf-8")
	for marker in (
		"HXC_EXCEPTION_SETJMP",
		"hxc_exception_frame_push",
		"hxc_exception_frame_take_payload",
		"hxc_exception_frame_pop",
		"hxc_exception_cleanup_push",
		"hxc_exception_cleanup_run",
		"hxc_exception_cleanup_discard",
		"hxc_exception_raise",
		"hxc_exception_root_slot_update",
		"hxc_gc_root_frame_pop_cleanup",
		"hxc_array_ref_release_slot",
		"hxc_string_release_slot",
	):
		if marker not in module:
			raise ExceptionLoweringFailure(
				f"generated contained exception omitted marker {marker!r}"
			)

	compiler = shutil.which("clang") or shutil.which("gcc")
	if compiler is None:
		raise ExceptionLoweringFailure("contained exception proof requires Clang or GCC")
	sources = sorted((output / "src").rglob("*.c")) + sorted(
		(output / "runtime" / "src").glob("*.c")
	)
	executable = output / "exception-runtime"
	result = run(
		[
			compiler,
			"-std=c11",
			"-O2",
			"-Wall",
			"-Wextra",
			"-Werror",
			"-pedantic",
			"-I",
			str(output / "include"),
			"-I",
			str(output / "runtime" / "include"),
			*[str(source) for source in sources],
			"-o",
			str(executable),
		],
		cwd=ROOT,
		timeout=30,
	)
	require_success(result, "contained exception strict native compile")
	require_success(
		run([str(executable)], cwd=ROOT, timeout=10),
		"contained exception native execution",
	)


def prove_runtime_policy_rejection(output: Path, connect: str) -> None:
	"""Prove that a required contained frame cannot bypass runtime=none."""
	result = run(
		[
			development_tool("haxe"),
			"--connect",
			connect,
			"runtime-none.hxml",
			"--custom-target",
			f"c={output}",
		],
		cwd=HERE,
		server=True,
	)
	if result.returncode == 0:
		raise ExceptionLoweringFailure("contained exception unexpectedly bypassed hxc_runtime=none")
	combined = result.stdout + result.stderr
	if "runtime policy `none`" not in combined or "runtime.exception.general-exception-region" not in combined:
		raise ExceptionLoweringFailure(
			"runtime policy rejection omitted the exception selection reason\n" + combined
		)


def available_port() -> int:
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
		candidate.bind(("127.0.0.1", 0))
		return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
	deadline = time.monotonic() + 10
	while time.monotonic() < deadline:
		if server.poll() is not None:
			stdout, stderr = server.communicate()
			raise ExceptionLoweringFailure(
				f"Haxe server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
			)
		try:
			with socket.create_connection(("127.0.0.1", port), timeout=0.2):
				return
		except OSError:
			time.sleep(0.05)
	raise ExceptionLoweringFailure("Haxe server did not accept connections")


def main() -> int:
	server: subprocess.Popen[str] | None = None
	try:
		prove_eval_oracle()
		with tempfile.TemporaryDirectory(prefix="hxc-exception-header-") as temporary:
			prove_cpp_header(Path(temporary))
		endpoint = os.environ.get("HXC_TEST_HAXE_CONNECT")
		if endpoint is None:
			port = available_port()
			endpoint = str(port)
			environment = os.environ.copy()
			environment.pop("HAXE_NO_SERVER", None)
			server = subprocess.Popen(
				[development_tool("haxe"), "--wait", endpoint],
				cwd=ROOT,
				env=environment,
				stdout=subprocess.PIPE,
				stderr=subprocess.PIPE,
				text=True,
			)
			wait_for_server(server, port)
		with tempfile.TemporaryDirectory(prefix="hxc-exception-lowering-") as temporary:
			root = Path(temporary)
			prove_generated_c(root / "generated", endpoint)
			prove_cleanup_before_catch(root / "cleanup", endpoint)
			prove_runtime_cross_call(root / "runtime", endpoint)
			prove_runtime_policy_rejection(root / "runtime-none", endpoint)
	except (ExceptionLoweringFailure, subprocess.TimeoutExpired, OSError, json.JSONDecodeError) as error:
		print(f"exception-lowering: ERROR: {error}", file=sys.stderr)
		return 1
	finally:
		if server is not None:
			server.terminate()
			try:
				server.wait(timeout=5)
			except subprocess.TimeoutExpired:
				server.kill()
				server.wait(timeout=5)
	print("exception-lowering: OK")
	return 0


if __name__ == "__main__":
	raise SystemExit(main())
