/* test UTF-8 handline */

#include <stdio.h>
#include <string.h>
#include "json.h"

static void showutf(const char *src,size_t lenmod) {
        char utf8[24]; /* intentionally small buffer */
        size_t s,usize=sizeof(utf8);
        json_nchar test;

        /* set up the source as a json_nchar string */
        memset(utf8,0,sizeof(utf8));
        test.s=src;
        test.n=strlen(test.s) + lenmod;

        /* s is the number of bytes needed to store the result */
        s=json_string_to_utf8(utf8,sizeof(utf8),&test);

        printf("%d:\"%.*s\" -> %zd:\"%.*s\"\n",
                test.n,test.n,test.s,
                s,(int)sizeof(utf8),utf8);

        if (s==0) {
                printf("was invalid string\n");
                return;
        }

        if (s > usize) {
                /* buffer was too small: try again */
                char bigger[s];
                memset(bigger,0,s);
                size_t after=json_string_to_utf8(bigger,s,&test);
                printf("retry...\n");
                printf("%d:\"%.*s\" -> %zd:\"%.*s\"\n",
                        test.n,test.n,test.s,
                        after,(int)s,bigger);

        }
}

int main(void) {
        showutf("bonjour gar\\u00e7on",0);
        showutf("hello you\\b\\b\\bme!",0);
        showutf("into the \\u1d01ther",0);
        showutf("snowman \\u2603 star \\u2606",0);
        showutf("snowman \\u2603 star \\u2606",-2); /* invalid */
        showutf("invalid \\u23zz unicode",0); /* invalid */
        showutf("snowman line \\u2603\\u2603\\u2603\\u2603\\u2603\\u2603 ends",0);
        return 0;
}
