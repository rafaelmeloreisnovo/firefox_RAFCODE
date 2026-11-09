#include "raf_http_scan.h"
int raf_http_scan_l0(const unsigned char *bytes, unsigned int length,
                     unsigned int header_limit, unsigned int *end_offset) {
    unsigned int i;
    unsigned int state = 0u;
    if (!bytes || !end_offset || header_limit == 0u) return RAF_HTTP_BAD_ARGUMENT;
    for (i = 0u; i < length; ++i) {
        unsigned char ch;
        if (i >= header_limit) return RAF_HTTP_TOO_LONG;
        ch = bytes[i];
        if (ch == 0u) return RAF_HTTP_BAD_FRAMING;
        if (ch == 13u) {
            if (state == 1u || state == 3u) return RAF_HTTP_BAD_FRAMING;
            state = (state == 2u) ? 3u : 1u;
        } else if (ch == 10u) {
            if (state == 1u) state = 2u;
            else if (state == 3u) { *end_offset = i + 1u; return RAF_HTTP_FOUND; }
            else return RAF_HTTP_BAD_FRAMING;
        } else {
            if (state == 1u || state == 3u) return RAF_HTTP_BAD_FRAMING;
            state = 0u;
        }
    }
    return length >= header_limit ? RAF_HTTP_TOO_LONG : RAF_HTTP_NEED_MORE;
}
