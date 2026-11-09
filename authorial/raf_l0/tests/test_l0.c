#include "../include/raf_l0.h"

static unsigned int failures;
#define CHECK(x) do { failures += (unsigned int)(!(x)); } while (0)

int main(void)
{
    raf_l0_report r;
    static const unsigned char spaces[4] = {32u,32u,32u,32u};
    static const unsigned char zeros[4] = {0u,0u,0u,0u};
    static const unsigned char word[5] = {'m','o','l','h','o'};
    static const unsigned char embedded[4] = {'m',0u,' ',255u};
    unsigned char series[257];
    unsigned int i, j, digest;

    CHECK(raf_l0_scan((const unsigned char *)0,0u,0u,&r)==RAF_L0_OK);
    CHECK(r.kind==RAF_L0_ABSENT && r.fnv1a32==0u);
    CHECK(raf_l0_scan(spaces,4u,0u,&r)==RAF_L0_OK);
    CHECK(r.kind==RAF_L0_EMPTY && r.fnv1a32==2166136261u);
    CHECK(raf_l0_scan(spaces,4u,4u,&r)==RAF_L0_OK);
    CHECK(r.kind==RAF_L0_SPACES_ONLY && r.ascii_spaces==4u && r.zero_bytes==0u);
    CHECK(raf_l0_scan(zeros,4u,4u,&r)==RAF_L0_OK);
    CHECK(r.kind==RAF_L0_ZEROS_ONLY && r.zero_bytes==4u && r.control_bytes==4u);
    CHECK(raf_l0_scan(word,5u,5u,&r)==RAF_L0_OK);
    CHECK(r.kind==RAF_L0_DATA && r.ascii_spaces==0u && r.nonascii_bytes==0u);
    CHECK(r.fnv1a32==0xf6a5a692u); /* Independent known-answer vector: molho bytes. */
    CHECK(raf_l0_scan(embedded,4u,4u,&r)==RAF_L0_OK);
    CHECK(r.kind==RAF_L0_DATA && r.zero_bytes==1u && r.ascii_spaces==1u && r.nonascii_bytes==1u);
    CHECK(r.fnv1a32==0xc5426c65u);

    CHECK(raf_l0_scan(word,3u,5u,&r)==RAF_L0_OUT_OF_BOUNDS);
    CHECK(r.kind==RAF_L0_INVALID && r.fnv1a32==0u);
    CHECK(raf_l0_scan((const unsigned char *)0,0u,1u,&r)==RAF_L0_BAD_ARGUMENT);
    CHECK(r.kind==RAF_L0_INVALID);
    CHECK(raf_l0_scan((const unsigned char *)0,1u,0u,&r)==RAF_L0_BAD_ARGUMENT);
    CHECK(r.kind==RAF_L0_INVALID);
    CHECK(raf_l0_scan(word,5u,5u,(raf_l0_report *)0)==RAF_L0_BAD_ARGUMENT);

    for(i=0u;i<257u;i++) series[i]=(unsigned char)(i & 255u);
    for(i=0u;i<=257u;i++) {
        digest=2166136261u;
        for(j=0u;j<i;j++) digest=(digest ^ (unsigned int)series[j]) * 16777619u;
        CHECK(raf_l0_scan(series,257u,i,&r)==RAF_L0_OK);
        CHECK(r.length==i && r.fnv1a32==digest);
        CHECK(r.kind==(i==0u ? RAF_L0_EMPTY : i==1u ? RAF_L0_ZEROS_ONLY : RAF_L0_DATA));
        CHECK(raf_l0_scan(series, i, i, &r)==RAF_L0_OK);
        CHECK(r.fnv1a32==digest);
        if(i<257u) {
            CHECK(raf_l0_scan(series,i,i+1u,&r)==RAF_L0_OUT_OF_BOUNDS);
            CHECK(r.kind==RAF_L0_INVALID);
        }
    }
    return failures > 255u ? 255 : (int)failures;
}
