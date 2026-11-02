"""Admit the reviewed fixed-buffer terrain calls from pinned software Raylib.

This is trusted local artifact staleness and semantic source admission. Ordinary
Linux/Darwin compiler, SDK, libc math/memory and monotonic-clock behavior remain
platform contracts; historical compiler and SDK bytes are not attested. The
reviewed rlsw raster tables are closed internal dispatch, not user callbacks.
This module cannot establish the generated game's transitive call coverage.

GetTime reads the monotonic clock into stack storage. In the software recipe,
rlSetTexture selects existing texture storage; rlBegin/rlEnd set draw state;
color, normal and texture-coordinate calls update fixed attributes (normal is
a no-op); rlVertex3f fills the fixed primitive buffer and dispatches the closed
eight-variant raster tables. Those paths write existing buffers. Texture/pool
creation and resize are outside this reviewed set, and are never admitted by
name similarity. The pinned source and recipe make that review invalid on drift.
"""
from __future__ import annotations

from dataclasses import dataclass
import hashlib
import json
from pathlib import Path
import shlex
from typing import Mapping

from scripts.raylib import provision

# Canonical JSON identity of the reviewed lock; formatting is not policy.
REVIEWED_LOCK_SHA256 = "1c127fc0a393497d9d278e79448fbfd3950bce35b15c619a162e62acf3aaeba9"
# Reviewed rcore recipe at locked dbc56a87; all six library units share this
# vector. Changes require reviewing the software draw closure again.
REVIEWED_DEFINES = (
    '-DEXTERNAL_CONFIG_FLAGS',
    '-DGRAPHICS_API_OPENGL_SOFTWARE',
    '-DPLATFORM_MEMORY',
    '-DRLGL_ENABLE_OPENGL_DEBUG_CONTEXT=0',
    '-DRLGL_SHOW_GL_DETAILS_INFO=0',
    '-DSUPPORT_AUTOMATION_EVENTS',
    '-DSUPPORT_CAMERA_SYSTEM',
    '-DSUPPORT_CLIPBOARD_IMAGE',
    '-DSUPPORT_COMPRESSION_API',
    '-DSUPPORT_FILEFORMAT_ASTC',
    '-DSUPPORT_FILEFORMAT_BDF',
    '-DSUPPORT_FILEFORMAT_BMP',
    '-DSUPPORT_FILEFORMAT_DDS',
    '-DSUPPORT_FILEFORMAT_FLAC',
    '-DSUPPORT_FILEFORMAT_FNT',
    '-DSUPPORT_FILEFORMAT_GIF',
    '-DSUPPORT_FILEFORMAT_GLTF',
    '-DSUPPORT_FILEFORMAT_HDR',
    '-DSUPPORT_FILEFORMAT_IQM',
    '-DSUPPORT_FILEFORMAT_JPG',
    '-DSUPPORT_FILEFORMAT_KTX',
    '-DSUPPORT_FILEFORMAT_M3D',
    '-DSUPPORT_FILEFORMAT_MOD',
    '-DSUPPORT_FILEFORMAT_MP3',
    '-DSUPPORT_FILEFORMAT_MTL',
    '-DSUPPORT_FILEFORMAT_OBJ',
    '-DSUPPORT_FILEFORMAT_OGG',
    '-DSUPPORT_FILEFORMAT_PIC',
    '-DSUPPORT_FILEFORMAT_PKM',
    '-DSUPPORT_FILEFORMAT_PNG',
    '-DSUPPORT_FILEFORMAT_PNM',
    '-DSUPPORT_FILEFORMAT_PSD',
    '-DSUPPORT_FILEFORMAT_PVR',
    '-DSUPPORT_FILEFORMAT_QOA',
    '-DSUPPORT_FILEFORMAT_QOI',
    '-DSUPPORT_FILEFORMAT_TGA',
    '-DSUPPORT_FILEFORMAT_TTF',
    '-DSUPPORT_FILEFORMAT_VOX',
    '-DSUPPORT_FILEFORMAT_WAV',
    '-DSUPPORT_FILEFORMAT_XM',
    '-DSUPPORT_GESTURES_SYSTEM',
    '-DSUPPORT_GPU_SKINNING',
    '-DSUPPORT_IMAGE_EXPORT',
    '-DSUPPORT_IMAGE_GENERATION',
    '-DSUPPORT_MESH_GENERATION',
    '-DSUPPORT_MODULE_RMODELS',
    '-DSUPPORT_MODULE_RSHAPES',
    '-DSUPPORT_MODULE_RTEXT',
    '-DSUPPORT_MODULE_RTEXTURES',
    '-DSUPPORT_MOUSE_GESTURES',
    '-DSUPPORT_QUADS_DRAW_MODE',
    '-DSUPPORT_RPRAND_GENERATOR',
    '-DSUPPORT_SCREEN_CAPTURE',
    '-DSUPPORT_SSH_KEYBOARD_RPI',
    '-DSUPPORT_TRACELOG',
    '-DSUPPORT_WINMM_HIGHRES_TIMER',
    '-DNDEBUG',
)
SOURCE_NAMES = ("raudio.c", "rcore.c", "rmodels.c", "rshapes.c", "rtext.c", "rtextures.c")
LEAVES = frozenset(("GetTime", "rlSetTexture", "rlBegin", "rlEnd", "rlColor4ub",
                    "rlNormal3f", "rlTexCoord2f", "rlVertex3f"))
ORDINARY_FLAGS = ("-fno-strict-aliasing", "-Werror=implicit-function-declaration",
                  "-Werror=pointer-arith", "-O3", "-std=gnu99")


class NativeContractFailure(ValueError):
    """The supplied native build does not match the reviewed terrain contract."""


@dataclass(frozen=True)
class NativeTerrainContract:
    """Immutable admitted names and a JSON identity snapshot for the game report."""
    leaves: frozenset[str]
    identity_json: str

    def identity_report(self) -> dict[str, object]:
        """Return a fresh JSON object so callers cannot mutate this contract."""
        return json.loads(self.identity_json)


def _digest(value: object) -> str:
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def _compiler(report: Mapping[str, object], platform: str) -> dict[str, str]:
    target = report.get("target")
    if not isinstance(target, dict) or not isinstance(target.get("compiler"), dict):
        raise NativeContractFailure("native receipt omitted compiler facts")
    compiler = target["compiler"]
    architecture = target.get("architecture")
    aliases = {"arm64": "aarch64", "aarch64": "aarch64", "x86_64": "x86_64", "amd64": "x86_64"}
    family, triple, version = (compiler.get(key) for key in ("family", "target", "versionLine"))
    if (not isinstance(architecture, str) or architecture not in aliases or not isinstance(triple, str) or not isinstance(version, str)
            or not version or not isinstance(family, str)):
        raise NativeContractFailure("native compiler identity is incomplete")
    if (family not in ({"clang", "apple-clang"} if platform == "macos" else {"clang", "gcc"})
            or aliases.get(triple.split("-")[0]) != aliases[architecture]
            or ("apple-darwin" not in triple if platform == "macos" else "linux" not in triple.split("-"))):
        raise NativeContractFailure("native compiler target or family is outside the platform contract")
    return {"family": family, "target": triple, "versionLine": version, "architecture": architecture}


def _recipe(entries: object, *, effective: Path, build: Path, platform: str,
            architecture: str) -> None:
    if not isinstance(entries, list) or len(entries) != len(SOURCE_NAMES):
        raise NativeContractFailure("native recipe must contain exactly six translation units")
    expected = {str(effective / "src" / name): name for name in SOURCE_NAMES}
    seen = set()
    common = None
    for entry in entries:
        if not isinstance(entry, dict) or set(entry) != {"directory", "command", "file", "output"}:
            raise NativeContractFailure("unsupported native recipe entry")
        source = entry["file"]
        if not isinstance(source, str) or source not in expected or source in seen:
            raise NativeContractFailure("native recipe has extra, missing or duplicate source")
        seen.add(source)
        name = expected[source]
        if (entry["directory"] != str(build / "raylib")
                or entry["output"] != f"raylib/CMakeFiles/raylib.dir/{name}.o"
                or not isinstance(entry["command"], str)):
            raise NativeContractFailure("native recipe source/output ownership differs")
        try:
            argv = shlex.split(entry["command"])
        except ValueError as error:
            raise NativeContractFailure("malformed native recipe command") from error
        if not argv or not Path(argv[0]).is_absolute():
            raise NativeContractFailure("native recipe needs the resolved compiler command")
        defines, flags = [], []
        include = []
        outputs = []
        sources = []
        arch = []
        sdk = []
        index = 1
        while index < len(argv):
            flag = argv[index]
            if flag in ("-o", "-c", "-arch", "-isysroot"):
                index += 1
                if index >= len(argv) or not argv[index] or argv[index].startswith("-"):
                    raise NativeContractFailure("missing native recipe operand")
                {"-o": outputs, "-c": sources, "-arch": arch, "-isysroot": sdk}[flag].append(argv[index])
            elif flag.startswith("-I"):
                include.append(flag[2:])
            elif flag.startswith("-D"):
                defines.append(flag)
            elif flag in ORDINARY_FLAGS:
                flags.append(flag)
            else:
                raise NativeContractFailure(f"unreviewed native recipe flag: {flag}")
            index += 1
        if (tuple(defines) != REVIEWED_DEFINES or tuple(flags) != ORDINARY_FLAGS
                or include != [str(effective / "src")]
                or sources != [source] or outputs != [f"CMakeFiles/raylib.dir/{name}.o"]):
            raise NativeContractFailure("native recipe defines, flags or owned paths differ")
        if platform == "macos":
            if arch != [architecture] or len(sdk) != 1 or not Path(sdk[0]).is_absolute() or not sdk[0].endswith(".sdk"):
                raise NativeContractFailure("native recipe architecture/SDK differs")
        elif arch or sdk:
            raise NativeContractFailure("Linux recipe has unreviewed target overrides")
        relevant = (argv[0], defines, flags, include, arch, sdk)
        if common is not None and relevant != common:
            raise NativeContractFailure("native recipe units disagree on compiler policy")
        common = relevant


def verify_native_contract(*, lock: Mapping[str, object], report: Mapping[str, object],
                           authority: str, platform_name: str, configuration: str,
                           source_root: Path, effective_source_root: Path, build_root: Path,
                           library_file: Path,
                           original_replacements: Mapping[str, str] | None = None) -> NativeTerrainContract:
    """Check trusted source/build receipts and the exact reviewed software recipe.

    Do not pass this result as proof of arbitrary foreign names or game coverage.
    No tools run, sources change, or library builds occur. Selected native tools
    are trusted as in ordinary builds; their receipt version/target are recorded,
    without claiming historical executable, environment or SDK byte identity.
    """
    if platform_name not in ("linux", "macos") or configuration != "memory-software":
        raise NativeContractFailure("terrain native contract requires Linux/Darwin memory-software")
    if _digest(dict(lock)) != REVIEWED_LOCK_SHA256:
        raise NativeContractFailure("Raylib lock changed; terrain contract requires review")
    compiler = _compiler(report, platform_name)
    try:
        provision.verify_source(source_root, lock)
        tree = provision.verify_source_build_receipt(
            lock=lock, report=report, authority=authority, platform_name=platform_name,
            configuration=configuration, effective_source_root=effective_source_root, library_file=library_file)
        recipe = build_root / "compile_commands.json"
        recipe_hash = provision.verify_compile_commands_receipt(
            report=report, compile_commands=recipe, original_replacements=original_replacements)
        entries = json.loads(recipe.read_text())
        _recipe(entries, effective=effective_source_root, build=build_root,
                platform=platform_name, architecture=compiler["architecture"])
        identity = {"schemaVersion": 1, "platform": platform_name, "configuration": configuration,
                    "sourceTreeSha256": tree.sha256, "librarySha256": provision.sha256_file(library_file),
                    "recipeSha256": recipe_hash, "lockSha256": REVIEWED_LOCK_SHA256,
                    "policySha256": provision.sha256_file(Path(__file__)), "compiler": compiler,
                    "leaves": sorted(LEAVES),
                    "trust": "ordinary native compiler/platform; no historical compiler or SDK byte attestation"}
    except (provision.ProvisionFailure, OSError, ValueError) as error:
        raise NativeContractFailure(str(error)) from error
    return NativeTerrainContract(LEAVES, json.dumps(identity, sort_keys=True))
