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
EDITOR_AUTOMATION = (
    ROOT
    / "examples"
    / "caxecraft"
    / "src"
    / "caxecraft"
    / "editor"
    / "EditorAutomationProtocol.hx"
)
EDITOR_WORLD_GRID = (
    ROOT
    / "examples"
    / "caxecraft"
    / "src"
    / "caxecraft"
    / "editor"
    / "EditorWorldGrid.hx"
)
EDITOR_SNAPSHOT = (
    ROOT
    / "examples"
    / "caxecraft"
    / "src"
    / "caxecraft"
    / "editor"
    / "EditorScenarioSnapshot.hx"
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
        self.assertIn("InspectPresentation", refresh)
        self.assertNotIn("draftSnapshot()", refresh)
        self.assertIn("terrainPresentation.refreshVoxel", refresh)
        self.assertIn("terrainPresentation.refresh(draft.world", refresh)

    def test_single_voxel_refresh_retains_unrelated_editor_projections(self) -> None:
        source = EDITOR_SCREEN.read_text(encoding="utf-8")
        refresh = section(source, "function refreshProjection", "function syncWorldName")
        voxel = section(refresh, "case RefreshTerrainVoxel(point, paletteCode):", "case RefreshAllTerrain:")

        self.assertIn("retainedPresentation = true", voxel)
        self.assertNotIn("InspectPresentationDetails", voxel)
        self.assertIn("patchPlanVoxel", refresh)
        self.assertIn("if (!retainedPresentation)", refresh)

    def test_flow_panels_reuse_one_isolated_draft_per_revision(self) -> None:
        source = EDITOR_SCREEN.read_text(encoding="utf-8")
        draft = section(source, "function currentDraftScenario", "/** Draw every admitted environment")
        rule = section(source, "function currentFlowRule", "/** True while the next visible object")

        self.assertIn("flowAuthoringRevision == current.revision()", draft)
        self.assertIn("current.query(InspectDraft)", draft)
        self.assertIn("flowAuthoringDraft = draft", draft)
        self.assertNotIn("draftSnapshot()", draft)
        self.assertIn("copyFlowRule(rule)", rule)
        self.assertNotIn("InspectDraft", rule)

    def test_all_visual_commits_defer_parser_metadata_until_validation(self) -> None:
        source = EDITOR_SESSION.read_text(encoding="utf-8")
        capture = section(source, "function captureReduction", "/**\n\t\tRestore the state")
        validation = section(source, "function validateImage", "/** Convert the public validator")

        self.assertIn("return captureReducerOwnedEdit(scenario, worldGridEditable)", capture)
        self.assertNotIn("captureScenario", capture)
        self.assertIn("case DeferredScenarioParse", validation)
        self.assertIn("restoreScenario(image.bytes)", validation)

    def test_single_voxel_edit_rewrites_only_its_trusted_chunk(self) -> None:
        grid = EDITOR_WORLD_GRID.read_text(encoding="utf-8")
        snapshot = EDITOR_SNAPSHOT.read_text(encoding="utf-8")
        session = EDITOR_SESSION.read_text(encoding="utf-8")
        paint = section(grid, "function paint", "/** Decode and rewrite")
        patch = section(grid, "private function patchVoxel", "/** Append one positive run")

        self.assertIn("trustedEditableLayout ? patchVoxel", paint)
        self.assertIn("world.chunks.copy()", patch)
        self.assertNotIn("decode(", patch)
        self.assertNotIn("rewriteChunks", patch)
        self.assertIn("worldGridEditable", snapshot)
        self.assertIn("before.worldGridEditable", session)

    def test_spatial_queries_reuse_the_copy_owned_presentation(self) -> None:
        source = EDITOR_AUTOMATION.read_text(encoding="utf-8")
        surface = section(source, "private function encodeSurface", "private function encodeColumn")
        column = section(source, "private function encodeColumn", "private function encodeMutation")

        for query in (surface, column):
            self.assertIn("InspectPresentation", query)
            self.assertNotIn("InspectDraft", query)
            self.assertNotIn("projectWorld", query)


if __name__ == "__main__":
    unittest.main()
