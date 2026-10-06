#include <stdio.h>

#define MK_IMPLEMENTATION
#include "mk_common.h"
#include "mk_tracking_allocator.h"
#include "mk_string.h"

int main() {
    mk_allocator_enable_debug_output = true;

    struct MkAllocator std_allocator = mk_allocator_get_std_allocator();
    MkTrackingAllocator ta = mk_tracking_allocator_create(std_allocator);
    struct MkAllocator allocator = mk_tracking_allocator_get_allocator(&ta);

    MkString a = mk_string_create_from_cstring(allocator, "Hello");
    MkString b = mk_string_create_from_cstring(allocator, " ");
    MkString c = mk_string_create_from_cstring(allocator, "World");
    MkString d = mk_string_create_from_cstring(allocator, "!");

    
    MkString result = mk_string_create(allocator);
    mk_string_append(&result, a);
    mk_tracking_allocator_dump(&ta, stdout);

    mk_string_append(&result, b);
    mk_tracking_allocator_dump(&ta, stdout);

    mk_string_append(&result, c);
    mk_tracking_allocator_dump(&ta, stdout);

    mk_string_append(&result, d);
    mk_tracking_allocator_dump(&ta, stdout);

    mk_string_append(&result, result);
    mk_tracking_allocator_dump(&ta, stdout);
    
    if (result.error.type != MK_ERROR_NONE) {
        mk_error_print(result.error, stdout);
        abort();
    }

    printf("`%s`\n", result.string);
    mk_tracking_allocator_destroy(&ta);
    return 0;
}