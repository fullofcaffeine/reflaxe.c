"""Require real thumbnail detail without prescribing an authored atlas palette."""

from pathlib import Path
import sys
import unittest
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "examples/caxecraft"))
import play  # noqa: E402


def browser_pixels(thumbnail: str) -> bytes:
    """Draw a synthetic panel whose text and focus cannot stand in for its icon."""
    width, height = 1280, 720
    pixels = bytearray(bytes((12, 28, 36, 255)) * width * height)
    panel = b"".join(bytes((70 + band * 20, 80, 90, 255)) * 110 for band in range(8))
    for row in range(50, 670):
        start = (row * width + 200) * 4
        pixels[start : start + len(panel)] = panel
    start = (100 * width + 700) * 4
    pixels[start : start + 300 * 4] = bytes((255, 132, 47, 255)) * 300
    if thumbnail != "blank":
        for row in range(204, 238):
            for column in range(236, 270):
                shade = 32 + ((column - 236) // 4) * 16
                color = (shade, shade, shade, 255) if thumbnail == "texture" else (210, 105, 230, 255)
                start = (row * width + column) * 4
                pixels[start : start + 4] = bytes(color)
    return bytes(pixels)


class EditorThumbnailScreenshotTests(unittest.TestCase):
    def validate(self, thumbnail: str) -> tuple[int, int]:
        with patch.object(play, "decode_rgba_png", return_value=(1280, 720, browser_pixels(thumbnail))):
            return play.validate_editor_asset_browser_screenshot(Path("synthetic.png"), platform_name="linux")

    def test_textured_thumbnail_accepts_a_non_placeholder_palette(self) -> None:
        self.assertEqual(self.validate("texture"), (1280, 720))

    def test_blank_thumbnail_fails_despite_panel_colors_and_focus(self) -> None:
        with self.assertRaisesRegex(play.PlayFailure, "textured thumbnail"):
            self.validate("blank")

    def test_old_flat_placeholder_is_not_atlas_evidence(self) -> None:
        with self.assertRaisesRegex(play.PlayFailure, "textured thumbnail"):
            self.validate("placeholder")


if __name__ == "__main__":
    unittest.main()
