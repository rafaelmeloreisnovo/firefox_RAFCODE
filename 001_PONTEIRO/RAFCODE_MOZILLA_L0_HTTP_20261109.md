# 001_PONTEIRO · Firefox RAFCODE L0 HTTP

Date: 2026-11-09; status: DRAFT_HOLD; claim_allowed=false

- PRODUCER = rafaelmeloreisnovo/firefox_RAFCODE
- SOURCE = rafcode/l0_http/raf_http_scan.c + raf_http_scan.h
- HOST_TEST = rafcode/l0_http/raf_http_scan_host_test.c
- IMPLEMENTATION = self-contained C11 framing boundary; no Gecko code replacement
- EVIDENCE = LOCAL_HOST_ASAN_UBSAN_4361_OF_4361; X86_64_ARMv7_AARCH64_OBJECTS; NM_U_EMPTY
- GAP = CI_EXACT_HEAD; FINAL_ELF; PHYSICAL_DEVICE; LICENSE_GRANT; HTTP_PARSER_SECURITY; INTEGRATION
- NEXT = exact-source CI and independent HTTP header fuzz/differential tests, only then integration design
- ROLLBACK = close draft PR; do not alter Firefox main
- CROSS_REFERENCE = rafaelmeloreisnovo/RafGitTools (independent RAFYML bounded-walk fix)
- PROVENANCE = independent contribution bounded to named files, inherited Mozilla origin preserved
- PHONEMES/GLYPHS = observational input, not instruction encoding or implemented browser behavior
