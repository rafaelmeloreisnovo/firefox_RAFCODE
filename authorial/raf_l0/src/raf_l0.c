#include "../include/raf_l0.h"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(unsigned int) == 4, "Requires 32-bit unsigned int");
_Static_assert((unsigned char)256u == 0u, "Requires 8-bit bytes");
#endif

unsigned int raf_l0_scan(const unsigned char *data,
                         unsigned int capacity,
                         unsigned int length,
                         raf_l0_report *out)
{
    unsigned int i;
    unsigned int digest = 2166136261u;
    unsigned int spaces = 0u, zeros = 0u, nonascii = 0u, controls = 0u;
    if (out == (raf_l0_report *)0) return RAF_L0_BAD_ARGUMENT;

    out->kind = RAF_L0_INVALID;
    out->length = 0u;
    out->ascii_spaces = 0u;
    out->zero_bytes = 0u;
    out->nonascii_bytes = 0u;
    out->control_bytes = 0u;
    out->fnv1a32 = 0u;

    if (data == (const unsigned char *)0) {
        if (length != 0u || capacity != 0u) return RAF_L0_BAD_ARGUMENT;
        out->kind = RAF_L0_ABSENT;
        return RAF_L0_OK;
    }
    if (length > capacity) return RAF_L0_OUT_OF_BOUNDS;

    for (i = 0u; i < length; ++i) {
        unsigned int byte = (unsigned int)data[i];
        zeros += (unsigned int)(byte == 0u);
        spaces += (unsigned int)(byte == 32u);
        nonascii += (unsigned int)(byte >= 128u);
        controls += (unsigned int)(byte < 32u || byte == 127u);
        digest = (digest ^ byte) * 16777619u;
    }
    out->length = length;
    out->ascii_spaces = spaces;
    out->zero_bytes = zeros;
    out->nonascii_bytes = nonascii;
    out->control_bytes = controls;
    out->fnv1a32 = digest;
    out->kind = length == 0u ? RAF_L0_EMPTY :
                spaces == length ? RAF_L0_SPACES_ONLY :
                zeros == length ? RAF_L0_ZEROS_ONLY : RAF_L0_DATA;
    return RAF_L0_OK;
}
