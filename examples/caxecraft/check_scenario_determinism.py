#!/usr/bin/env python3
"""Prove CAXEMAP codec determinism across locale and compiler reuse."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

CASE = Path(__file__).resolve().parent
sys.path.insert(0, str(CASE))

from check_scenario_model import EXPECTED_CODEC_TRACE  # noqa: E402
from run import (  # noqa: E402
    CaxecraftFailure,
    HaxeInstallation,
    HaxeServerConnection,
    alternate_locale,
    haxe_compilation_server,
    pinned_haxe_environment,
    pinned_haxe_installation,
    resolve_haxe_arguments,
    verify_pinned_haxe,
)

CODEC_HXML = "scenario-codec.hxml"


class ScenarioDeterminismFailure(RuntimeError):
    pass


def resolved_codec_arguments(locale: str) -> tuple[str, ...]:
    """Resolve the scoped HXML once for one explicit process locale."""

    return resolve_haxe_arguments((CODEC_HXML,), locale=locale)


def run_codec(
    label: str,
    installation: HaxeInstallation,
    arguments: tuple[str, ...],
    *,
    locale: str,
    connection: HaxeServerConnection | None = None,
) -> str:
    """Run the complete codec probe and require its exact successful trace."""

    command = [str(installation.compiler)]
    if connection is not None:
        command.extend(("--connect", connection.endpoint))
    command.extend(arguments)
    result = subprocess.run(
        command,
        cwd=CASE,
        env=pinned_haxe_environment(locale, installation),
        check=False,
        capture_output=True,
        text=True,
        encoding="utf-8",
        timeout=90,
    )
    if (
        result.returncode != 0
        or result.stdout != EXPECTED_CODEC_TRACE
        or result.stderr
    ):
        raise ScenarioDeterminismFailure(
            f"{label} changed the CAXEMAP codec result:\n"
            f"exit:            {result.returncode}\n"
            f"expected stdout: {EXPECTED_CODEC_TRACE!r}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )
    return result.stdout


def main() -> int:
    try:
        installation = pinned_haxe_installation()
        verify_pinned_haxe(installation)
        baseline_arguments = resolved_codec_arguments("C")
        locale = alternate_locale()
        if locale == "C":
            raise ScenarioDeterminismFailure(
                "no alternate locale is installed for the CAXEMAP reference lane"
            )
        locale_arguments = resolved_codec_arguments(locale)

        traces = [
            run_codec(
                "first cold C-locale Eval",
                installation,
                baseline_arguments,
                locale="C",
            ),
            run_codec(
                "repeated cold C-locale Eval",
                installation,
                baseline_arguments,
                locale="C",
            ),
            run_codec(
                f"cold {locale} Eval",
                installation,
                locale_arguments,
                locale=locale,
            ),
        ]

        with haxe_compilation_server() as connection:
            if connection.installation != installation:
                raise ScenarioDeterminismFailure(
                    "the warm server resolved a different pinned Haxe installation"
                )
            traces.extend(
                (
                    run_codec(
                        "first warm-server Eval",
                        installation,
                        baseline_arguments,
                        locale="C",
                        connection=connection,
                    ),
                    run_codec(
                        "repeated warm-server Eval",
                        installation,
                        baseline_arguments,
                        locale="C",
                        connection=connection,
                    ),
                )
            )

        if any(trace != traces[0] for trace in traces[1:]):
            raise ScenarioDeterminismFailure(
                "cold, locale, and warm-server codec traces did not converge"
            )
    except (
        CaxecraftFailure,
        OSError,
        ScenarioDeterminismFailure,
        subprocess.TimeoutExpired,
        UnicodeError,
    ) as error:
        print(f"caxemap-determinism: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "caxemap-determinism: OK: 3 cold Eval requests "
        f"(C and {locale}) plus 2 requests through one pinned Haxe server"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
