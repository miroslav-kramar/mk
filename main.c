#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MK_IMPLEMENTATION
#include "mk_common.h"
#include "mk_dynamic_array.h"

bool mk_io_get_line(
    struct MkError * error,
    struct MkAllocator allocator,
    char ** line,
    size_t * capacity,
    size_t * length,
    FILE * file
) {
    *length = 0;
    bool run = true;
    while (run) {
        int c = fgetc(file);
        if (c == EOF) {
            if (ferror(file)) {
                *error = mk_error_create(MK_ERROR_IO, "Could not read from file!");
            }
            if (*length > 0) {
                return true;
            }
            return false;
        }
        if (c == '\n') {
            run = false;
            c = '\0';
        }
        char to_append = (char) c;
        mk_dynamic_array_append(
            error,
            allocator,
            (void**)line,
            capacity,
            length,
            &to_append,
            sizeof(char)
        );
        if (error->type != MK_ERROR_NONE) {
            return false;
        }
        if (!run && *length > 0) {
            (*length)--;
        }
    }
    return true;
}

int main() {
    mk_allocator_enable_debug_output = true;

    struct MkAllocator allocator = mk_allocator_get_std_allocator();
    char * line = NULL;
    size_t capacity = 0;
    size_t length = 0;
    struct MkError error = mk_error_create(MK_ERROR_NONE, NULL);

    bool run = true;
    while (run) {
        printf(">>> ");
        run = mk_io_get_line(
            &error,
            allocator,
            &line,
            &capacity,
            &length,
            stdin
        );
        if (error.type != MK_ERROR_NONE) {
            mk_error_print(error, stdout);
            abort();
        }
        if (run) {
            printf("`%s`\n", line);
        }
    }
    printf("\nDone!\n");
    return 0;
}