/* test cases */

#include <stdio.h>
#include <string.h>
#include "json.h"

#define GETTEXT(X) X

int main(void) {
        struct {
                bool good;
                char *s;
        } t[]={
                /* simple examples */
                {true,"{}"},
                {true,"{\"hello\":\"there\"}"},
                {true,"[1]"},
                {true,"[1,4.3,9e10]"},
                {true,"{\"list\":[10,11,\"hi\",-3e-10]}"},
                /* real-world example */
                {true,"{\n\
                        \"glossary\": {\n\
                                \"title\": \"example glossary\",\n\
                                \"GlossDiv\": {\n\
                                        \"title\": \"S\",\n\
                                        \"GlossList\": {\n\
                                                \"GlossEntry\": {\n\
                                                        \"ID\": \"SGML\",\n\
                                                        \"SortAs\": \"SGML\",\n\
                                                        \"GlossTerm\": \"Standard Generalized Markup Language\",\n\
                                                        \"Acronym\": \"SGML\",\n\
                                                        \"Abbrev\": \"ISO 8879:1986\",\n\
                                                        \"GlossDef\": {\n\
                                                                \"para\": \"A meta-markup language.\",\n\
                                                                \"GlossSeeAlso\": [\"GML\", \"XML\"]\n\
                                                        },\n\
                                                        \"GlossSee\": \"markup\"\n\
                                                }\n\
                                        }\n\
                                }\n\
                        }\n\
                }"},
                {true,"[0,0.,1e1,1,2,-1,-2,0.0023,-0.0025,1e9,1.0023e9,-123.456e-78]"},
                /* error examples */
                {false,"{hello:3}"},
                {false,"[1,2,3,]"},
                {false,"what what?"},
        };
        int slen=sizeof(t)/sizeof(*t);
        int i;
        int goodc=0,badc=0;

        for(i=0;i<slen;i++) {
                char *s=t[i].s;
                bool good=t[i].good;
                const char *p;
                const char *pf;

                printf("--------------\n");
                p=json_parse(NULL,s);
                if (p && *p!='\0') {
                        printf("trailing data at %td\n",p-s);
                        p=NULL;
                }

                printf("%s -> ",s);
                if (good == !!p) {
                        goodc++; pf=GETTEXT("PASS");
                }
                else {
                        badc++; pf=GETTEXT("FAIL");
                }
                if (p) printf("%zu/%zu (%s)\n",p-s,strlen(s),pf);
                else printf("null (%s)\n",pf);
        }
        printf(GETTEXT("Results:\n"));
        printf(GETTEXT("Content test: check output by eye\n"));
        printf(GETTEXT("Structure parse test: good=%d bad=%d\n"),goodc,badc);
        printf("*** %s ***\n",(badc==0)?GETTEXT("PASS"):GETTEXT("FAIL"));
        return (badc==0)?0:1;
}
