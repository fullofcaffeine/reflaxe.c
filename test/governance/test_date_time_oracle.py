"""Keep Date oracle exceptions narrow while admitting host-owned DST choices."""

from __future__ import annotations

import importlib.util
import unittest
from pathlib import Path
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("date_time_oracle_subject", ROOT / "test/date_time/run.py")
assert SPEC is not None and SPEC.loader is not None
RUNNER = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(RUNNER)

SPRING = (
    "spring-before|time=1.710057599e+12|utc=2024-3-10T7:59:59|"
    "local=2024-03-10 01:59:59|offset="
)
FALL = (
    "fall-first|time=1.7306154e+12|utc=2024-11-3T6:30:0|"
    "local=2024-11-03 01:30:00|offset="
)
DAYLIGHT = "fall-overlap|time=1.7306154e+12|local=2024-11-03 01:30:00|offset=300"
STANDARD = "fall-overlap|time=1.730619e+12|local=2024-11-03 01:30:00|offset=360"
COMMON = "identity|alias=true|equal-value=false\n"


def output(spring: str, fall: str, overlap: str) -> str:
    return f"{SPRING}{spring}\n{FALL}{fall}\n{overlap}\n{COMMON}"


class DateOracleTests(unittest.TestCase):
    def compare(self, native: str, evaluated: str) -> None:
        with mock.patch.object(RUNNER, "run_eval", return_value=evaluated):
            RUNNER.compare_eval_and_native(native, RUNNER.CENTRAL_TZ)

    def test_recorded_macos_and_linux_eval_offsets(self) -> None:
        macos = output("300", "300", STANDARD)
        linux = output("300", "360", DAYLIGHT.removesuffix("300") + "360")
        for evaluated in (macos, linux):
            for overlap in (DAYLIGHT, STANDARD):
                with self.subTest(evaluated=evaluated, overlap=overlap):
                    self.compare(output("360", "300", overlap), evaluated)

    def test_native_offsets_cannot_copy_eval_bugs(self) -> None:
        evaluated = output("300", "360", STANDARD)
        for native in (
            output("300", "300", STANDARD),
            output("360", "360", STANDARD),
            output("360", "300", DAYLIGHT.removesuffix("300") + "360"),
            output("360", "300", STANDARD.removesuffix("360") + "300"),
        ):
            with self.subTest(native=native), self.assertRaises(RUNNER.DateTimeFailure):
                self.compare(native, evaluated)

    def test_missing_duplicate_or_wrong_overlap_is_rejected(self) -> None:
        evaluated = output("300", "300", STANDARD)
        native = output("360", "300", STANDARD)
        for replacement in ("", STANDARD + "\n" + STANDARD + "\n", STANDARD.replace("01:30", "02:30") + "\n"):
            with self.subTest(replacement=replacement), self.assertRaises(RUNNER.DateTimeFailure):
                self.compare(native.replace(STANDARD + "\n", replacement), evaluated)

    def test_other_output_stays_exact(self) -> None:
        evaluated = output("300", "300", STANDARD)
        native = output("360", "300", STANDARD).replace("alias=true", "alias=false")
        with self.assertRaises(RUNNER.DateTimeFailure):
            self.compare(native, evaluated)

    def test_other_timezones_have_no_exception(self) -> None:
        with mock.patch.object(RUNNER, "run_eval", return_value="expected\n"):
            RUNNER.compare_eval_and_native("expected\n", "UTC0")
            with self.assertRaises(RUNNER.DateTimeFailure):
                RUNNER.compare_eval_and_native("different\n", "UTC0")


if __name__ == "__main__":
    unittest.main()
