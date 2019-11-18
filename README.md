# stack-json

Lightweight JSON interpreter in C.

This code is a single-pass interpreter over a JSON text
structure.  For every value found, it calls a user-supplied
function which can filter out any desired values.

Here is a small example:
```C
/* Pick off the value ["johnny"][5] */

#include <stdio.h>
#include "json.h"

const char *json="{\
        \"johnny\":[\n\
                \"broken\",\n\
                \"in pieces\",\n\
                \"behind shed\",\n\
                \"upside down\",\n\
                \"watching tv\",\n\
                \"alive\",\n\
                \"passed out\"]\
}";

static void johnny5(const json_valuecontext *root,const json_value *v,void *context) {
        (void)context; /* subdue unused warning */

        if (json_matches_path(root,"johnny","#5",NULL)) {
                json_printpath(root);
                printf(" is ");
                json_printvalue(v);
                printf("\n");
        }
}

int main(void) {
        json_callbacks cb={.got_value=johnny5};
        return json_parse(&cb,json)!=NULL;
}
```
