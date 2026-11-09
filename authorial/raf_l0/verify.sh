#!/usr/bin/env sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
CC=${CC:-clang}
OUT=$(mktemp -d "${TMPDIR:-/tmp}/raf-firefox-l0.XXXXXXXX")
test -d "$OUT"
trap 'rm -rf -- "$OUT"' EXIT HUP INT TERM
"$CC" -std=c11 -O2 -Wall -Wextra -Werror -pedantic -ffreestanding -fno-builtin -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -c "$ROOT/src/raf_l0.c" -o "$OUT/host.o"
if nm -u "$OUT/host.o" | grep -q '[A-Za-z_]' ; then echo "ERROR: external runtime symbol" >&2; exit 1; fi
for OPT in -O0 -O2 -Oz; do
  "$CC" -std=c11 "$OPT" -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined "$ROOT/src/raf_l0.c" "$ROOT/tests/test_l0.c" -o "$OUT/host-test"
  "$OUT/host-test"
  echo "PASS_HOST_SANITIZERS: $OPT"
done
for TARGET in armv7a-none-eabi aarch64-none-elf x86_64-unknown-none-elf; do
  "$CC" -target "$TARGET" -std=c11 -O2 -Wall -Wextra -Werror -pedantic -ffreestanding -fno-builtin -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -c "$ROOT/src/raf_l0.c" -o "$OUT/$TARGET.o"
  if nm -u "$OUT/$TARGET.o" | grep -q '[A-Za-z_]' ; then echo "ERROR: external runtime symbol for $TARGET" >&2; exit 1; fi
  readelf -h "$OUT/$TARGET.o" | grep -E 'Class:|Machine:'
  size "$OUT/$TARGET.o"
done
echo 'PASS_SCOPED: hosted UBSan/ASan (-O0/-O2/-Oz) and three freestanding ABI objects; device NOT_RUN'
