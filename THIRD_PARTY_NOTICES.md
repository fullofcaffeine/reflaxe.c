# Third-party notices and provenance

`reflaxe.c` repository-owned source is licensed under
[`GPL-3.0-only`](LICENSE). This document is an engineering provenance
inventory, not legal advice. An external tool listed here is not thereby part
of a `reflaxe.c` distribution.

The machine-readable source of truth for distribution-sensitive compiler and
runtime provenance, last verified on 2026-07-15, is
[`docs/specs/third-party-provenance.json`](docs/specs/third-party-provenance.json).
It distinguishes tools used to build or validate the project from source and
binaries actually redistributed with it.

## Current upstream inventory

| Component | Current use and provenance | Upstream license | Included here? |
| --- | --- | --- | --- |
| Reflaxe | Compiler framework dependency by Maybee “SomeRanDev” Rezbit. Version `4.0.0-beta`, commit `73a983112e039daad46b37912ab238df6bf0cf53`, Git tree `05ab4abc670b6e3ed5424e78c622b92979b5affb`; the selected 62-file compiler bundle is independently SHA-256 locked in `toolchain-lock.json`. | [MIT](vendor/reflaxe/LICENSE) | Compiler-only source is vendored under `vendor/reflaxe`; it is not linked into generated programs and is not an `hxrt` runtime feature. |
| Lix | Scoped Haxe toolchain and library resolver. npm package `17.0.2` is locked by exact registry integrity. | [MIT](https://github.com/lix-pm/lix/blob/v17.0.2/LICENSE) | No Lix source or binary; `npm ci` installs it as a development tool. |
| Haxe compiler | External build tool. The accepted language contract is Haxe `4.3.7`, tag commit `e0b355c6be312c1b17382603f018cf52522ec651`. | [GPL-2.0-or-later](https://github.com/HaxeFoundation/haxe/blob/e0b355c6be312c1b17382603f018cf52522ec651/src/typing/typeload.ml) for compiler source | No compiler source or binary. |
| Haxe standard library | Source basis for future target `_std` work, Haxe `4.3.7`, tag commit `e0b355c6be312c1b17382603f018cf52522ec651`. Every copied or adapted file must have a per-file provenance entry. | [MIT](https://github.com/HaxeFoundation/haxe/blob/e0b355c6be312c1b17382603f018cf52522ec651/extra/LICENSE.txt); retained text in [`LICENSES/HAXE-STDLIB-MIT.txt`](LICENSES/HAXE-STDLIB-MIT.txt) | The license notice is present; no derived `_std` source exists yet. |
| LLVM/Clang tooling | External syntax/ABI authority planned for bindgen and layout validation. The tool version remains to be selected; license evidence was observed at upstream revision `c4830afdbbc7b16a52a05d6a5b56e6c3fc14fb98`. | [Apache-2.0 WITH LLVM-exception](https://github.com/llvm/llvm-project/blob/c4830afdbbc7b16a52a05d6a5b56e6c3fc14fb98/LICENSE.TXT) | No LLVM/Clang source or binary. |
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
