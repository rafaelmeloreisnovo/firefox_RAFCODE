# 001_PONTEIRO / FIREFOX_RAF_L0_V1

Source upstream: `mozilla-firefox/firefox`, fork `rafaelmeloreisnovo/firefox_RAFCODE`, baseline `main@d4a9923322691e221ec1afac57715016071aa506`.

Intent: place a tiny, fully isolated C11 freestanding raw-byte gate alongside, **not inside**, the Firefox browser. This preserves the original Mozilla source and its complex build chain.

Route: `authorial/raf_l0/include/raf_l0.h` -> `src/raf_l0.c` -> `tests/test_l0.c` -> `verify.sh` -> `.github/workflows/raf-l0-freestanding.yml` -> exact GitHub CI SHA -> external receipt.

Typed distinction: `ABSENT != EMPTY != SPACES_ONLY != ZEROS_ONLY != DATA`; absence is not numeric zero. No Portuguese/Unicode semantic inference for `molho`. Fingerprint is FNV-1a, *not* security integrity.

Authority: upstream Mozilla license/trademarks unchanged. New authorial code license approval pending. Draft review only; no merge, no full-browser build, no runtime integration, no physical E2E claims.

Falsifier: must compile and show `nm -u` empty on ARMv7, AArch64 and x86-64 and pass negative bounds/null tests under ASan/UBSan hosted. If any fails, hold. If Mozilla build fails for unrelated reasons, do not claim this scoped gate explains it.

R3 = <F_ok: source and isolated CI proposal, F_gap: Firefox integration/security/license/hardware, F_next: compare real ABI caller contract before implementing Gecko adapter>.
