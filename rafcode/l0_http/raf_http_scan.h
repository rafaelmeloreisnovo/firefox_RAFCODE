#ifndef RAF_HTTP_SCAN_H
#define RAF_HTTP_SCAN_H
#if defined(__CHAR_BIT__) && __CHAR_BIT__ != 8
#error "8-bit bytes required"
#endif
_Static_assert(sizeof(unsigned int) == 4, "32-bit unsigned int required");
/* Separately authored, review-only candidate. License grant pending. */
enum raf_http_scan_status {
    RAF_HTTP_BAD_ARGUMENT = -1,
    RAF_HTTP_BAD_FRAMING = -2,
    RAF_HTTP_TOO_LONG = -3,
    RAF_HTTP_NEED_MORE = 0,
    RAF_HTTP_FOUND = 1
};
int raf_http_scan_l0(const unsigned char *bytes, unsigned int length,
                     unsigned int header_limit, unsigned int *end_offset);
#endif
