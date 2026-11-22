#!/usr/bin/env bash
# Pair the diagnostic reader with the installed Clang 21 compiler.
# Install only its matching runtime library; never replace the compiler.
set -euo pipefail
: "${GITHUB_PATH:?This CI setup requires GITHUB_PATH}"
clang_root=/usr/lib/llvm-21
if [ ! -x "$clang_root/bin/clang" ] || [ ! -x "$clang_root/bin/clang++" ]; then
  echo "Coverage requires the installed canonical Clang 21 compiler pair" >&2
  exit 1
fi
if ! clang_package_version="$(dpkg-query -W -f='${Version}' clang-21)"; then
  echo "Coverage requires the installed clang-21 package" >&2
  exit 1
fi
compiler_sha256="$(sha256sum "$clang_root/bin/clang")"
sudo apt-get update
sudo env DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
  "libclang1-21=$clang_package_version"
if [ "$(dpkg-query -W -f='${Version}' libclang1-21)" != "$clang_package_version" ] || \
   [ "$(dpkg-query -W -f='${Version}' clang-21)" != "$clang_package_version" ] || \
   [ "$(sha256sum "$clang_root/bin/clang")" != "$compiler_sha256" ]; then
  echo "Coverage requires matching libclang1-21 without changing the installed compiler" >&2
  exit 1
fi
dpkg-query -W clang-21 libclang1-21
echo "$clang_root/bin" >> "$GITHUB_PATH"
