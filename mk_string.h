#ifndef MK_STRING_H_
#define MK_STRING_H_

// -----------------------------------------------------------------------------
// PUBLIC HEADER
// -----------------------------------------------------------------------------

#include "mk_common.h"
#include "mk_dynamic_array.h"

// Types -----------------------------------------------------------------------

typedef struct {
    struct MkAllocator allocator;
    struct MkError error;
    char * string;
    size_t capacity;
    size_t length;
} MkString;

// Functions -------------------------------------------------------------------

MkString mk_string_create(
    struct MkAllocator allocator
);

MkString mk_string_create_from_cstring(
    struct MkAllocator allocator,
    const char * cstring
);

void mk_string_destroy(
    MkString * string
);

void mk_string_append_char_array(
    MkString * string,
    const char * array,
    size_t length
);

void mk_string_append_cstring(
    MkString * string,
    const char * cstring
);

void mk_string_append_char(
    MkString * string,
    char c
);

void mk_string_append(
    MkString * string,
    MkString to_append
);

// -----------------------------------------------------------------------------
// IMPLEMENTATION
// -----------------------------------------------------------------------------

#if defined MK_STRING_IMPLEMENTATION || defined MK_IMPLEMENTATION

#include <string.h>
#include <assert.h>

MkString mk_string_create(
    struct MkAllocator allocator
) {
    MkString s = {0};
    s.allocator = allocator;
    s.error = mk_error_create(MK_ERROR_NONE, NULL);
    s.string = "";
    s.capacity = 0;
    s.length = 0;
    return s;
}

MkString mk_string_create_from_cstring(
    struct MkAllocator allocator,
    const char * cstring
) {
    MkString s = mk_string_create(allocator);
    mk_string_append_cstring(&s, cstring);
    return s;
}

void mk_string_destroy(
    MkString * string
) {
    mk_allocator_free(string->allocator, string->string);
    string->error = mk_error_create(MK_ERROR_NONE, NULL);
    string->string = "";
    string->capacity = 0;
    string->length = 0;
}

void mk_string_append_char_array(
    MkString * string,
    const char * array,
    size_t length
) {
    if (string->capacity == 0) {
        string->string = NULL;
    }
    mk_dynamic_array_reserve_capacity(
        &string->error,
        string->allocator,
        (void**)&string->string,
        &string->capacity,
        &string->length,
        16,
        sizeof(string->string[0])
    );
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
        sizeof(string->string[0])
    );
    if (string->error.type != MK_ERROR_NONE) {
        return;
    }
    assert(string->length > 0);
    string->length--;
}

void mk_string_append_cstring(
    MkString * string,
    const char * cstring
) {
    mk_string_append_char_array(string, cstring, strlen(cstring));
}

void mk_string_append_char(
    MkString * string,
    char c
) {
    mk_string_append_char_array(string, &c, 1);
}

void mk_string_append(
    MkString * string,
    MkString to_append
) {
    mk_string_append_char_array(string, to_append.string, to_append.length);
}

#endif // IMPLEMENTATION
#endif // MK_STRING_H_