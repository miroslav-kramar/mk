#include <stdio.h>

#define MK_IMPLEMENTATION
#include "mk_common.h"
#include "mk_dynamic_array.h"
#include "mk_tracking_allocator.h"

struct String {
    struct MkAllocator allocator;
    struct MkError error;
    char * string;
    size_t capacity;
    size_t length;
};

struct String string_create(struct MkAllocator allocator) {
    struct String s = {0};
    mk_dynamic_array_create(
        &s.error,
        (void**)&s.string,
        &s.capacity,
        &s.length
    );
    return (struct String){.allocator = allocator};
}

void string_append_char_array(struct String * string, const char * array, size_t length) {
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
    if (string->length > 0) {
        string->length--;
    }
}

void string_append_cstring(struct String * string, const char * cstring) {
    string_append_char_array(string, cstring, strlen(cstring));
}

void string_append_char(struct String * string, char c) {
    string_append_char_array(string, &c, 1);
}

struct String get_line(struct MkAllocator allocator, FILE * file) {
    struct String string = string_create(allocator);
    while (1) {
        int c = fgetc(file);
        if (c == EOF) {
            if (ferror(file)) {
                mk_allocator_free(allocator, string.string);
                return (struct String){.error = mk_error_create(MK_ERROR_IO, "Error reading from file!")};
            }
            break;
        }
        if (c == '\n') {
            break;
        }
        string_append_char(&string, c);
        if (string.error.type != MK_ERROR_NONE) {
            mk_allocator_free(allocator, string.string);
            return (struct String){.error = string.error};
        }
    }
    return string;
}

int main() {
    struct MkAllocator std_allocator = mk_allocator_get_std_allocator();
    MkTrackingAllocator ta = mk_tracking_allocator_create(std_allocator);
    struct MkAllocator allocator = mk_tracking_allocator_get_allocator(&ta);

    struct String line = get_line(allocator, stdin);
    if (line.error.type != MK_ERROR_NONE) {
        printf("ERROR!\n");
        return 1;
    }

    printf("`%s`\n", line.string);

    mk_tracking_allocator_destroy(&ta);
    return 0;
}