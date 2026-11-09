# RAFCODE L0 — proposed standalone byte gate (not integrated with Gecko)

This independent **C11 freestanding** proof of concept does not depend on libc,
`malloc`, syscalls, Android, JNI, Java, Rust, the Gecko build system, or external
libraries in its **runtime object**. A hosted compiler and diagnostic tools are
still required to build and verify it. It does **not** turn Firefox into a
freestanding web browser.

A caller supplies a pointer, byte capacity, requested length, and writable
report. The report categorizes raw bytes without interpreting Portuguese,
Unicode semantics, network payloads, or file origins.

| Input | Report kind | Rule |
|---|---|---|
| NULL + 0 capacity + 0 length | ABSENT | no backing pointer |
| Nonnull + 0 length | EMPTY | present buffer with no requested bytes |
| Four 0x20 bytes | SPACES_ONLY | ASCII spaces only |
| Four 0x00 bytes | ZEROS_ONLY | binary zero only |
| `molho` (five bytes) | DATA | raw bytes, semantic meaning unknown |
| Mixed UTF-8/raw bytes | DATA | no character decoding or translation |
| Requested length greater than capacity | error + INVALID | fail-closed |
| NULL with nonzero capacity/length | error + INVALID | fail-closed |

The report's FNV-1a32 fingerprint is **not** cryptographic; it must **not** be
used to authenticate downloads, code, licenses, or an APK. Bounds are checked
against the *caller's declared* capacity; this API cannot prove the actual
underlying allocation size. No heap, no global state, no IO, no language model.

`sh authorial/raf_l0/verify.sh` runs hosted tests under ASan and UBSan at three
optimization levels, cross-compiles standalone objects for ARMv7/AArch64/x86-64,
checks their ELF identity and rejects unresolved symbols. Cross-compilation is
**not** execution on physical hardware. Host test harness and the verification
shell are **hosted**, and may use system tools; the runtime C object does not.

## Non-regression and license boundary

- All existing upstream Mozilla source files and license documents remain
  unchanged. The fork inherits upstream licensing and trademark restrictions.
- This proposed RAFCODE module is new, separate source. The author’s choice of
  license for these *new files* has **not** been established; treat that as
  `TOKEN_VAZIO` and retain `DRAFT/HOLD` pending explicit legal/licensing review.
- This does not claim a new right to the Firefox brand or logo, and does not
  relicense copied Mozilla logic. No upstream code is copied into this module.
- Do not merge into Gecko, NSS, SpiderMonkey, networking, or UI without a
  separate interface contract, license review, full browser regression testing,
  and physical cross-ABI evidence.
- To roll back before merge, close the draft PR. After any authorized merge,
  revert only this standalone addition using a superseding receipt.

PIRC-2 levels: **N0** for standalone host compilation/tests and ELF symbol
inspection; **N3** for future suitability in Firefox subsystems; **N5** for
actual Firefox integration, Android performance, browser feature parity, and
hardware E2E. `claim_allowed=false` beyond scoped tests.
