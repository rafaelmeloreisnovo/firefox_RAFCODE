/* Hosted test only. No standard library in the L0 scanner. */
#include "raf_http_scan.h"
#include <stdio.h>
static unsigned checks, fails;
static void expect(const unsigned char *b, unsigned int n, unsigned int limit,
                   int expected, unsigned int expected_offset) {
    unsigned int offset = 777u;
    int actual = raf_http_scan_l0(b,n,limit,&offset);
    ++checks;
    if (actual != expected || (actual == RAF_HTTP_FOUND && offset != expected_offset) ||
        (actual != RAF_HTTP_FOUND && offset != 777u)) {
        ++fails;
        fprintf(stderr,"case=%u expected=%d got=%d offset=%u\n",checks,expected,actual,offset);
    }
}
int main(void) {
    static const unsigned char complete[]="HTTP/1.1 200 OK\r\nX: v\r\n\r\nbody";
    static const unsigned char short_header[]="GET / HTTP/1.1\r\nHost: a\r\n";
    static const unsigned char bare_lf[]="A\n\n";
    static const unsigned char bad_cr[]="A\rZ";
    static const unsigned char nil[]={'A',0,'B',13,10,13,10};
    static const unsigned char empty[]="";
    unsigned char input[8];
    unsigned int i;
    expect(complete,sizeof complete-1u,256u,RAF_HTTP_FOUND,sizeof complete-1u-4u);
    /* First four bytes after the terminator are body, so exact end is size-1-4. */
    expect(complete,sizeof complete-1u,8u,RAF_HTTP_TOO_LONG,0u);
    expect(short_header,sizeof short_header-1u,256u,RAF_HTTP_NEED_MORE,0u);
    expect(bare_lf,sizeof bare_lf-1u,256u,RAF_HTTP_BAD_FRAMING,0u);
    expect(bad_cr,sizeof bad_cr-1u,256u,RAF_HTTP_BAD_FRAMING,0u);
    expect(nil,sizeof nil,256u,RAF_HTTP_BAD_FRAMING,0u);
    expect(empty,0u,256u,RAF_HTTP_NEED_MORE,0u);
    expect((const unsigned char *)0,0u,256u,RAF_HTTP_BAD_ARGUMENT,0u);
    expect(empty,0u,0u,RAF_HTTP_BAD_ARGUMENT,0u);
    for (i=0u;i<256u;i++) {
        input[0]=(unsigned char)i;
        expect(input,1u,4u,(i==0u||i==10u)?RAF_HTTP_BAD_FRAMING:RAF_HTTP_NEED_MORE,0u);
    }
    for (i=0u;i<4096u;i++) {
        input[0]='A'; input[1]=13; input[2]=10; input[3]=13;
        input[4]=(unsigned char)i;
        expect(input,5u,8u,(input[4]==10u)?RAF_HTTP_FOUND:RAF_HTTP_BAD_FRAMING,5u);
    }
    printf("checks=%u failures=%u\n",checks,fails);
    return fails?1:0;
}
