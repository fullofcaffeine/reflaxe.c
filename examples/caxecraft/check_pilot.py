#!/usr/bin/env python3
"""Check the target-neutral named Caxecraft game-pilot foundation."""

from __future__ import annotations

import re
import struct
import subprocess
import sys
import tempfile
import zlib
from pathlib import Path

CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
PILOT = CASE / "src/caxecraft/pilot"
sys.path.insert(0, str(CASE))

from run import (  # noqa: E402
    CaxecraftFailure,
    alternate_locale,
    pinned_haxe_environment,
    pinned_haxe_installation,
    resolve_haxe_arguments,
    verify_pinned_haxe,
)
import play as playable  # noqa: E402

EXPECTED_TRACE = (
    "caxecraft-pilot: 8 named scripts, 82 deterministic frames, 10 checkpoints; "
    "bounded quit and shared input interface\n"
)
FORBIDDEN_PILOT_TEXT = (
    re.compile(r"#if\b"),
    re.compile(r"\bDynamic\b"),
    re.compile(r"\bAny\b"),
    re.compile(r"\bReflect\b"),
    re.compile(r"\buntyped\b"),
    re.compile(r"\b__c__\b"),
    re.compile(r"\bArray\b"),
    re.compile(r"\bc\."),
    re.compile(r"\braylib\."),
)


class PilotFailure(RuntimeError):
    pass


def png_chunk(kind: bytes, payload: bytes) -> bytes:
    """Build one checksummed PNG chunk for the telemetry decoder fixtures."""
    checksum = zlib.crc32(kind + payload) & 0xFFFFFFFF
    return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", checksum)


def write_telemetry_fixture(path: Path, words: list[int], *, unknown_color: bool = False) -> None:
    """Write the smallest faithful 1x-scale framebuffer accepted by the native decoder."""
    width, height = 1280, 720
    stride = width * 4
    filtered = bytearray(height * (stride + 1))
    row_start = (height - 1) * (stride + 1) + 1
    for word_index, word in enumerate(words):
        for digit in range(8):
            nibble = (word >> ((7 - digit) * 4)) & 0xF
            sample_x = (word_index * 8 + digit) * 2 + 1
            color = playable.PILOT_TELEMETRY_COLORS[nibble]
            offset = row_start + sample_x * 4
            filtered[offset : offset + 4] = bytes(color)
    if unknown_color:
        filtered[row_start + 4 : row_start + 8] = bytes((1, 2, 3, 255))

    header = struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0)
    path.write_bytes(
        b"\x89PNG\r\n\x1a\n"
        + png_chunk(b"IHDR", header)
        + png_chunk(b"IDAT", zlib.compress(bytes(filtered), level=9))
        + png_chunk(b"IEND", b"")
    )


def expect_telemetry_failure(path: Path, words: list[int], expected: str, *, unknown_color: bool = False) -> None:
    """Prove malformed or unsupported telemetry fails with an actionable reason."""
    write_telemetry_fixture(path, words, unknown_color=unknown_color)
    try:
        playable.decode_pilot_telemetry(path, (1280, 720))
    except playable.PlayFailure as error:
        if expected not in str(error):
            raise PilotFailure(f"telemetry failure omitted {expected!r}: {error}") from error
        return
    raise PilotFailure(f"telemetry fixture unexpectedly admitted {expected}")


def check_native_telemetry_decoder() -> None:
    """Lock the versioned native report boundary without launching a graphical process."""
    words = [0] * playable.PILOT_TELEMETRY_WORDS
    words[0] = playable.PILOT_TELEMETRY_MAGIC
    words[1] = playable.PILOT_TELEMETRY_VERSION
    words[2] = playable.PILOT_TELEMETRY_WORDS
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-pilot-telemetry-") as temporary:
        path = Path(temporary) / "telemetry.png"
        write_telemetry_fixture(path, words)
        if playable.decode_pilot_telemetry(path, (1280, 720)) != words:
            raise PilotFailure("valid pilot telemetry did not round-trip exactly")

        malformed_magic = list(words)
        malformed_magic[0] ^= 1
        expect_telemetry_failure(path, malformed_magic, "magic drifted")

        unsupported_version = list(words)
        unsupported_version[1] += 1
        expect_telemetry_failure(path, unsupported_version, "version 2 is unsupported")

        malformed_length = list(words)
        malformed_length[2] -= 1
        expect_telemetry_failure(path, malformed_length, "declares 31 words")

        expect_telemetry_failure(path, words, "has unknown color", unknown_color=True)


def check_target_neutral_boundary() -> None:
    sources = sorted(PILOT.glob("*.hx"), key=lambda path: path.name.encode("utf-8"))
    if not sources:
        raise PilotFailure("pilot source inventory is empty")
    for source in sources:
        text = source.read_text(encoding="utf-8")
        for pattern in FORBIDDEN_PILOT_TEXT:
            if pattern.search(text):
                raise PilotFailure(
                    f"{source.relative_to(ROOT)} crosses the target-neutral pilot "
                    f"boundary: {pattern.pattern}"
                )


def run_probe(locale: str) -> str:
    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    arguments = resolve_haxe_arguments(("pilot.hxml",), locale=locale)
    result = subprocess.run(
        [str(installation.compiler), *arguments],
        cwd=CASE,
        env=pinned_haxe_environment(locale, installation),
        check=False,
        capture_output=True,
        text=True,
        encoding="utf-8",
        timeout=30,
    )
    if result.returncode != 0 or result.stdout != EXPECTED_TRACE or result.stderr:
        raise PilotFailure(
            f"{locale} pilot probe changed:\n"
            f"exit:            {result.returncode}\n"
            f"expected stdout: {EXPECTED_TRACE!r}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )
    return result.stdout


def main() -> int:
    try:
        check_target_neutral_boundary()
        check_native_telemetry_decoder()
        locale = alternate_locale()
        if locale == "C":
            raise PilotFailure("no alternate locale is installed for the pilot lane")
        baseline = run_probe("C")
        localized = run_probe(locale)
        if baseline != localized:
            raise PilotFailure("C and alternate-locale pilot traces did not converge")
    except (
        CaxecraftFailure,
        PilotFailure,
        OSError,
        subprocess.TimeoutExpired,
        UnicodeError,
    ) as error:
        print(f"caxecraft-pilot: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "caxecraft-pilot: OK: named semantic input, exact checkpoints, "
        f"bounded quit, and POSIX C/{locale} locale determinism"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
