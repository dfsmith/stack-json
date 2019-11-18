/* > json.h */
/* (C) Daniel F. Smith, 2019 */
/* SPDX-License-Identifier: LGPL-3.0-only */

/* Stack-based in-place JSON parser. */

/* The json_parse() function will scan the text given to it,
 * calling a function every time a value is found.  That function
 * can pick off interesting values.
 *
 * Because the parser runs on the JSON string in-place, most
 * strings are handled as a json_nchar, which is a pointer
 * and length.  These strings should be printed as:
 * printf("%.*s",str.n,str.s);
 */

#ifndef STACK_JSON_H
#define STACK_JSON_H

#include <stddef.h>
#include <stdbool.h>

/* Somewhere inside the JSON string in memory. */
typedef const char *json_in;

/* A pointer,length string type. */
typedef struct {
        json_in s; /* pointer to start of string */
        int n;     /* length of string */
} json_nchar;

/* A value that a JSON entity can have.  Note that json_type_object
 * and json_type_array are composite types, that are never returned.
 */
typedef struct {
        enum {
                json_type_null,
                json_type_bool,
                json_type_number,
                json_type_string,
                json_type_array,
                json_type_object,
        } type;
        union {
                /* set if... */
                bool truefalse;    /* ... json_type_bool */
                double number;     /* ... json_type_number */
                json_nchar string; /* ... json_type_string */
                json_in object;    /* ... json_type_object (pointer to start) */
                json_in array;     /* ... json_type_array (pointer to start) */
        };
} json_value;

/* A list of the names that contain the value. */
typedef struct json_valuecontext_s json_valuecontext;
struct json_valuecontext_s {
        /* double-linked list from NULL<->superelement<->root<->next...<->NULL */
        json_valuecontext *prev,*next;

        json_nchar name;  /* name of the JSON entity, or NULL if an array */
        int index;        /* index into an JSON array, if name.s==NULL */
        json_value value; /* the value of the entity */
};

/* A set of user-provided callback functions. If functions are NULL,
 * then some suitable printing functions will be used: see the default
 * values for these functions in the main file.
 */
typedef struct {
        /* user context */
        void *context;

        /* called when a value is found */
        void (*got_value)(
                const json_valuecontext *root, /* element chain */
                const json_value *value,       /* value at end of chain (convenience) */
                void *context);                /* user-supplied context */

        /* called when an error is found */
        void (*error)(
                const json_valuecontext *c, /* context of element */
                const char *etype,          /* type of element (object, array, number, etc) */
                json_in start,              /* start of element in error */
                json_in hint,               /* position in element of error */
                const char *msg,            /* description of error */
                void *context);             /* user-supplied context */
} json_callbacks;

/* -- utility functions -- */

/* Convert a json_nchar string to UTF-8.
 * Returns required length of dest or 0 on error.
 */
extern size_t json_string_to_utf8(char *dest,size_t destlen,const json_nchar *in);

/* Print the chain of path variables described by the context.
 * Returns the final (leaf) json_valuecontext.
 */
extern const json_valuecontext *json_printpath(const json_valuecontext *c);

/* Print a value. */
extern void json_printvalue(const json_value *v);

/* Returns true if the name matches the context. */
extern bool json_matches_name(const json_valuecontext *c,const char *name);

/* Returns true if the index matches the context. */
extern bool json_matches_index(const json_valuecontext *c,int index);

/* Returns true if the arguments match the context path.
 * Takes a list of element name strings to match against, finalized with NULL.
 * Use "#" to match an array, optionally "#nnn" to match index nnn.
 * Use "*" to match any element.
 * Use "**" to match all remaining elements.
 * Example: json_matches_path(c,"first","second","#1",NULL) will match "yes"
 *          in {"first":{"second":["no","yes","no"]}}
 * Note: typically used in got_value() callback.
 */
extern bool json_matches_path(const json_valuecontext *c,...);

/* -- main parser function -- */

/* Parse a JSON text object with optional callback functions.
 * Returns a pointer to the character after the JSON object/array/value, or
 * NULL on error.
 * If cb or the entries in cb are NULL, printing callbacks will be used.
 */
extern const char *json_parse(const json_callbacks *cb,const char *json_string);

#endif
