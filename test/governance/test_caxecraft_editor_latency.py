from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[2]
EDITOR_SCREEN = (
    ROOT
    / "examples"
    / "caxecraft"
    / "src"
    / "caxecraft"
    / "app"
    / "CaxecraftEditorScreen.hx"
)
EDITOR_SESSION = (
    ROOT
    / "examples"
    / "caxecraft"
    / "src"
    / "caxecraft"
    / "editor"
    / "EditorSession.hx"
)


def section(source: str, start: str, end: str) -> str:
    """Return one source section between stable neighboring declarations."""

    start_index = source.index(start)
    end_index = source.index(end, start_index)
    return source[start_index:end_index]


class CaxecraftEditorLatencyContract(unittest.TestCase):
    def test_pointer_hover_uses_only_cached_presentation_state(self) -> None:
        source = EDITOR_SCREEN.read_text(encoding="utf-8")
        hover = section(source, "function updatePreview", "function activeRecipeFor")

        self.assertIn("presentationDraft", hover)
        self.assertNotIn("draftSnapshot()", hover)
        self.assertNotIn("current.preview(", hover)

    def test_commit_reuses_the_current_canonical_image(self) -> None:
        source = EDITOR_SESSION.read_text(encoding="utf-8")
        apply = section(source, "public function apply", "public function mutate")
        preview = section(source, "public function preview", "public function query")
        snapshot = section(source, "public function draftSnapshot", "public function lastPlayableSnapshot")
        canonical = section(source, "public function canonicalDraft", "public function selectionSnapshot")

        self.assertIn("applyToImage(draftImage, command)", apply)
        self.assertNotIn("captureScenario", apply)
        self.assertIn("stageCommands(draftImage", preview)
        self.assertNotIn("captureScenario", preview)
        self.assertIn("restoreScenario(draftImage.bytes)", snapshot)
        self.assertNotIn("captureScenario", snapshot)
        self.assertIn("draftImage.bytes.sub", canonical)
        self.assertNotIn("captureScenario", canonical)

    def test_single_voxel_refresh_uses_the_incremental_renderer_path(self) -> None:
        screen = EDITOR_SCREEN.read_text(encoding="utf-8")
        apply_tool = section(screen, "function applyToolAt", "function refreshProjection")
        refresh = section(screen, "function refreshProjection", "function syncWorldName")

        self.assertIn("terrainRefreshForCommand(value)", apply_tool)
        self.assertIn("terrainRefreshForBatch(commands)", apply_tool)
        self.assertIn("terrainPresentation.refreshVoxel", refresh)
        self.assertIn("terrainPresentation.refresh(draft.world", refresh)


if __name__ == "__main__":
    unittest.main()
