#ifndef RAF_L0_H
#define RAF_L0_H

/* Independent proposed authorial source: license selection pending owner review. */
#if defined(__cplusplus)
extern "C" {
#endif

enum {
    RAF_L0_OK = 0u,
    RAF_L0_BAD_ARGUMENT = 1u,
    RAF_L0_OUT_OF_BOUNDS = 2u
};

enum {
    RAF_L0_INVALID = 0u,
    RAF_L0_ABSENT = 1u,
    RAF_L0_EMPTY = 2u,
    RAF_L0_SPACES_ONLY = 3u,
    RAF_L0_ZEROS_ONLY = 4u,
    RAF_L0_DATA = 5u
};

typedef struct raf_l0_report {
    unsigned int kind;
    unsigned int length;
    unsigned int ascii_spaces;
    unsigned int zero_bytes;
    unsigned int nonascii_bytes;
    unsigned int control_bytes;
    unsigned int fnv1a32;
} raf_l0_report;

/* Caller owns and validates the actual memory backing 'capacity'. */
unsigned int raf_l0_scan(const unsigned char *data,
                         unsigned int capacity,
                         unsigned int length,
                         raf_l0_report *out);

#if defined(__cplusplus)
}
#endif
#endif
