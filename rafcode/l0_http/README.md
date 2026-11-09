# RAFCODE L0 — HTTP/1 framing boundary (review candidate)

Status: **SOURCE_COMMITTED / LOCAL_SCOPED_PASS / CI_NOT_RUN / DEVICE_NOT_RUN / NOT_INTEGRATED / claim_allowed=false**.

This is **not Gecko**, Firefox network stack, an HTTP/1 parser, TLS, DNS, HTTP/2, HTTP/3 or a browser. No inherited Firefox source has been modified. This contribution belongs to `rafcode/l0_http/`; it does not grant authorship of Firefox.

## Contract

- `raf_http_scan_l0(bytes, length, header_limit, end_offset)`: scan a **contiguous, caller-owned readable byte array** for the first `CR LF CR LF` boundary.
- Return `FOUND(1)` with an exclusive end offset; `NEED_MORE(0)`; `BAD_ARGUMENT(-1)`; `BAD_FRAMING(-2)`; or `TOO_LONG(-3)`. On any non-FOUND result, leave `*end_offset` untouched.
- `NULL` bytes with length 0 means **missing** and returns BAD_ARGUMENT; non-NULL empty input means **present but incomplete** and returns NEED_MORE; numeric zero is not an absence marker.
- Scan at most `header_limit` bytes. Reject NUL, bare LF, malformed CR transitions. Stop on first boundary; bytes after boundary are not inspected.
- **Does not** validate HTTP field names/values, field folding, status lines, Content-Length, Transfer-Encoding, request smuggling, host authority, Unicode, full payload framing, socket behavior or network security. Upstream protocol-compliance integration requires separate parser, differential tests and adversarial review.
- No runtime heap, libc, syscalls, JNI, JDK, NDK, Android SDK, Gradle or external imports in the C core. The **compiler, ABI, runner and host OS remain build/execution dependencies**. C has ordinary function/variable identifiers; absence of names is not a correctness, security, or overhead invariant.

## Evidence (local, 2026-11-09)

Host Clang 17: `-std=c11 -O1 -g -fsanitize=address,undefined -Wall -Wextra -Werror`, 4,361 checks, 0 failures.
Clang freestanding objects: `armv7a-none-eabi` ELF32 ARM, `aarch64-none-elf` ELF64 AArch64, `x86_64-unknown-none-elf` ELF64 x86-64. `nm -u` empty. Object-only; no target executable/device test.
Source SHA-256: `d6dee71cece97c6d003e3609b0bc2fd82e1a6d9c02fd8f24411877213c4eede0`. Test SHA-256: `abe60ca52eb434b684d4abecd51729a05708ed61aae0fd28bbf27f01956e8caf`. These were measured on local test files; GitHub blob comparison is a separate receipt.
ARMv7 object may contain a local ARM EHABI `.ARM.exidx` relocation; this is **not** an external dynamic import and **not** evidence of zero unwind metadata. Check final ELF and stack/segment/relocation profile before any promotion.

## Reproduction

```sh
cd rafcode/l0_http
clang -std=c11 -O1 -g -fsanitize=address,undefined -Wall -Wextra -Werror raf_http_scan.c raf_http_scan_host_test.c -o /tmp/raf_http_scan_test
/tmp/raf_http_scan_test
clang -target armv7a-none-eabi -std=c11 -Oz -ffreestanding -fno-builtin -fno-stack-protector -Wall -Wextra -Werror -c raf_http_scan.c -o /tmp/raf_http_armv7.o
nm -u /tmp/raf_http_armv7.o
readelf -h /tmp/raf_http_armv7.o
```

## Rights and rollback

The repository is an inherited Mozilla Firefox fork. Its root LICENSE points to Firefox's licensing terms and reserves Mozilla trademarks. **This separately authored candidate has no new standalone license grant assigned here; RIGHTS_DECISION=TOKEN_VAZIO**. Do not infer that the entire fork is authorial or that this candidate can be shipped as Firefox without file-level rights review. No original third-party licenses were changed.

Rollback: close this draft pull request; `main` and all Gecko consumers remain untouched. Any successor must preserve the parent SHA and readback receipt. CI and physical execution are still required. For navigation see `001_PONTEIRO/RAFCODE_MOZILLA_L0_HTTP_20261109.md`.
