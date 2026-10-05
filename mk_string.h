#ifndef MK_STRING_H_
#define MK_STRING_H_

#include "mk_common.h"
#include "mk_dynamic_array.h"

typedef struct {
    struct MkAllocator allocator;
    struct MkError error;
    char * string;
    size_t capacity;
    size_t length;
} String;

#if defined MK_STRING_IMPLEMENTATION || defined MK_IMPLEMENTATION

#include <string.h>
#include <assert.h>

String string_create(struct MkAllocator allocator) {
    String s = {0};
    s.allocator = allocator;
    s.error = mk_error_create(MK_ERROR_NONE, NULL);
    s.string = "";
    s.capacity = 0;
    s.length = 0;
    return s;
}

void mk_string_destroy(String * string) {
    mk_allocator_free(string->allocator, string->string);
    string->error = mk_error_create(MK_ERROR_NONE, NULL);
    string->string = "";
    string->capacity = 0;
    string->length = 0;
}

void string_append_char_array(String * string, const char * array, size_t length) {
    if (string->length == 0) {
        string->string = NULL;
    }
    mk_dynamic_array_append_many(
        &string->error,
        string->allocator,
        (void**)&string->string,
        &string->capacity,
        &string->length,
        array,
        length,
        sizeof(array[0])
    );
    mk_dynamic_array_append(
        &string->error,
        string->allocator,
        (void**)&string->string,
        &string->capacity,
        &string->length,
        "",
        sizeof(char)
    );
    if (string->error.type != MK_ERROR_NONE) {
        return;
    }
    assert(string->length > 0);
    string->length--;
}

void string_append_cstring(String * string, const char * cstring) {
    string_append_char_array(string, cstring, strlen(cstring));
}

void string_append_char(String * string, char c) {
    string_append_char_array(string, &c, 1);
}

#endif // IMPLEMENTATION
#endif // MK_STRING_H_