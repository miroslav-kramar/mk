#include <stdio.h>

#define MK_IMPLEMENTATION
#include "mk_common.h"
#include "mk_tracking_allocator.h"
#include "mk_string.h"

String get_line(struct MkAllocator allocator, FILE * file) {
    String string = string_create(allocator);
    while (1) {
        int c = fgetc(file);
        if (c == EOF) {
            if (ferror(file)) {
                mk_allocator_free(allocator, string.string);
                return (String){
                    .error = mk_error_create(
                        MK_ERROR_IO,
                        "Error reading from file!"
                    )
                };
            }
            break;
        }
        if (c == '\n') {
            break;
        }
        string_append_char(&string, c);
        if (string.error.type != MK_ERROR_NONE) {
            mk_allocator_free(allocator, string.string);
            return (String){.error = string.error};
        }
    }
    return string;
}

int main() {
    struct MkAllocator std_allocator = mk_allocator_get_std_allocator();
    MkTrackingAllocator ta = mk_tracking_allocator_create(std_allocator);
    struct MkAllocator allocator = mk_tracking_allocator_get_allocator(&ta);

    String line = get_line(allocator, stdin);
    if (line.error.type != MK_ERROR_NONE) {
        mk_error_print(line.error, stdout);
        return 1;
    }

    printf("`%s`\n", line.string);

    mk_tracking_allocator_destroy(&ta);
    return 0;
}