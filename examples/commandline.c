/* command line example */

#include "json.h"

int main(int argc,char *argv[]) {
        argc--;
        argv++;
        while(argc-->0) {
                json_parse(NULL,*(argv++));
        }
        return 0;
}
