# Third-party notices and provenance

`reflaxe.c` repository-owned source is licensed under
[`GPL-3.0-only`](LICENSE). This document is an engineering provenance
inventory, not legal advice. An external tool listed here is not thereby part
of a `reflaxe.c` distribution.

The machine-readable source of truth for distribution-sensitive compiler and
runtime provenance, last verified on 2026-07-17, is
[`docs/specs/third-party-provenance.json`](docs/specs/third-party-provenance.json).
It distinguishes tools used to build or validate the project from source and
binaries actually redistributed with it.

## Current upstream inventory

| Component | Current use and provenance | Upstream license | Included here? |
| --- | --- | --- | --- |
| Reflaxe | Compiler framework dependency by Maybee “SomeRanDev” Rezbit. Version `4.0.0-beta`, commit `73a983112e039daad46b37912ab238df6bf0cf53`, Git tree `05ab4abc670b6e3ed5424e78c622b92979b5affb`; the selected 62-file compiler bundle is independently SHA-256 locked in `toolchain-lock.json`. | [MIT](vendor/reflaxe/LICENSE) | Compiler-only source is vendored under `vendor/reflaxe`; it is not linked into generated programs and is not an `hxrt` runtime feature. |
| Lix | Scoped Haxe toolchain and library resolver. npm package `17.0.2` is locked by exact registry integrity. | [MIT](https://github.com/lix-pm/lix/blob/v17.0.2/LICENSE) | No Lix source or binary; `npm ci` installs it as a development tool. |
| Gitleaks | Repository secret scanner `8.30.0`, source revision `6eaad039603a4de39fddd1cf5f727391efe9974e`; CI verifies the official Linux x64 release archive against SHA-256 `79a3ab579b53f71efd634f3aaf7e04a0fa0cf206b7ed434638d1547a2470a66e` before execution. | [MIT](https://github.com/gitleaks/gitleaks/blob/6eaad039603a4de39fddd1cf5f727391efe9974e/LICENSE) | No Gitleaks source or binary; it is an external development/CI tool. |
| Haxe Formatter | Repository source formatter `1.18.0`, source revision `93ba289893d515614298f4ce7cee8619c31b420c`. | [MIT](https://github.com/HaxeCheckstyle/haxe-formatter/blob/93ba289893d515614298f4ce7cee8619c31b420c/LICENSE) | No formatter source or binary; contributors and CI install it as an external development tool. |
| Haxe compiler | External build tool. The accepted language contract is Haxe `5.0.0-preview.1`, tag commit `2c1e544e0a2c7524ef4c8e103f1b0580362ea538`. | [GPL-2.0-or-later](https://github.com/HaxeFoundation/haxe/blob/2c1e544e0a2c7524ef4c8e103f1b0580362ea538/src/typing/typeload.ml) for compiler source | No compiler source or binary. |
| Haxe standard library | Source basis for future target `_std` work, Haxe `5.0.0-preview.1`, tag commit `2c1e544e0a2c7524ef4c8e103f1b0580362ea538`. Every copied or adapted file must have a per-file provenance entry. | [MIT](https://github.com/HaxeFoundation/haxe/blob/2c1e544e0a2c7524ef4c8e103f1b0580362ea538/extra/LICENSE.txt); retained text in [`LICENSES/HAXE-STDLIB-MIT.txt`](LICENSES/HAXE-STDLIB-MIT.txt) | The license notice is present; no derived `_std` source exists yet. |
| LLVM/Clang tooling | External syntax/ABI authority planned for bindgen and layout validation. The tool version remains to be selected; license evidence was observed at upstream revision `c4830afdbbc7b16a52a05d6a5b56e6c3fc14fb98`. | [Apache-2.0 WITH LLVM-exception](https://github.com/llvm/llvm-project/blob/c4830afdbbc7b16a52a05d6a5b56e6c3fc14fb98/LICENSE.TXT) | No LLVM/Clang source or binary. |
| Meson | Generated build-adapter validation tool `1.11.1`; CI installs the PyPI wheel only after checking SHA-256 `9b3a023657e393dbc5335b95c561337d49b7a458f5541e47ec44f2cc566e0d80`. | [Apache-2.0](https://github.com/mesonbuild/meson/blob/1.11.1/COPYING) | No Meson source or binary; it is an external development/CI tool. |
| raylib | Native framework dependency for RaylibHx/Caxecraft integration evidence. Release `6.0` (`6.0.0`), commit `dbc56a87da87d973a9c5baa4e7438a9d20121d28`; the immutable commit archive is SHA-256 `81b06ce7c19cf3b634b0271c23c361ba6ad8bf45fb8b036abbfeb4260ec1e126`, and the complete extracted source tree is independently content-locked by `raylib-provisioning-lock.json`. | [Zlib](https://github.com/raysan5/raylib/blob/dbc56a87da87d973a9c5baa4e7438a9d20121d28/LICENSE) | No raylib source or binary is included. CI provisions it as a separately supplied native application dependency; importing RaylibHx selects no `hxrt` feature. |
| Third-party runtime dependencies | None selected or included. `hxrt` is currently repository-authored source; future third-party runtime code must be declared before it is added. | Not applicable today | None. |

## Redistribution rules

- Code under `vendor/`, `third_party/`, `runtime/vendor/`, or
  `runtime/third_party/` must be owned by a manifest entry with an immutable
  source revision, license expression, local license evidence, and exact paths.
- Every file under `std/c/_std/` must appear in the derived-source ledger. An
  upstream-derived entry records its upstream path and revision, content hash,
  license, and retained notice; repository-authored overrides are classified
  separately.
- Adding a third-party runtime dependency requires a concrete
  `runtimeDependencies` entry. An empty array means none, not “not audited.”
- Release packages must carry `LICENSE`, this notice, the provenance manifest,
  and every local license file required by that manifest. CI compares those
  package files byte-for-byte with the reviewed repository copies.

The license treatment of generated C/headers, compiler templates, and emitted
or linked runtime slices is deliberately not inferred from the repository
license. Decision `haxe_c-od2.5` must be ratified before release packaging
makes promises about those artifacts.
