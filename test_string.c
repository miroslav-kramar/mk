#include <stdio.h>

#define MK_IMPLEMENTATION
#include "mk_common.h"
#include "mk_tracking_allocator.h"
#include "mk_string.h"

int main() {
    struct MkAllocator std_allocator = mk_allocator_get_std_allocator();
    MkTrackingAllocator ta = mk_tracking_allocator_create(std_allocator);
    struct MkAllocator allocator = mk_tracking_allocator_get_allocator(&ta);

    MkString a = mk_string_create_from_cstring(allocator, "Hello");
    MkString b = mk_string_create_from_cstring(allocator, " ");
    MkString c = mk_string_create_from_cstring(allocator, "World");
    MkString d = mk_string_create_from_cstring(allocator, "!");

    MkString result = mk_string_create(allocator);
    printf("capacity: %zu\n", result.capacity);

    mk_string_append(&result, a);
    printf("capacity: %zu\n", result.capacity);
    
    mk_string_append(&result, b);
    printf("capacity: %zu\n", result.capacity);
    
    mk_string_append(&result, c);
    printf("capacity: %zu\n", result.capacity);
    
    mk_string_append(&result, d);
    printf("capacity: %zu\n", result.capacity);

    mk_string_append(&result, result);
    printf("capacity: %zu\n", result.capacity);

    if (result.error.type != MK_ERROR_NONE) {
        mk_error_print(result.error, stdout);
        abort();
    }

    printf("`%s`\n", result.string);

    mk_tracking_allocator_destroy(&ta);
    return 0;
}