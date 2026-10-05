#include <stdio.h>

#define MK_IMPLEMENTATION
#include "mk_common.h"
#include "mk_tracking_allocator.h"
#include "mk_string.h"

int main() {
    struct MkAllocator std_allocator = mk_allocator_get_std_allocator();
    MkTrackingAllocator ta = mk_tracking_allocator_create(std_allocator);
    struct MkAllocator allocator = mk_tracking_allocator_get_allocator(&ta);

    MkString a = mk_string_create

    mk_tracking_allocator_destroy(&ta);
    return 0;
}