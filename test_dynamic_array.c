#include <stdio.h>

#define MK_IMPLEMENTATION
#include "mk_common.h"
#include "mk_dynamic_array.h"

int main() {
    struct MkAllocator allocator = mk_allocator_get_std_allocator();
    int * array = NULL;
    size_t capacity = 0;
    size_t length = 0;
    struct MkError error = mk_error_create(MK_ERROR_NONE, NULL);

    int data[] = {1,2,3,4,5};
    mk_dynamic_array_append_many(
        &error,
        allocator,
        (void**)&array,
        &capacity,
        &length,
        data,
        mk_countof(data),
        sizeof(array[0])
    );

    for (size_t i = 0; i <= length; i++) {
        mk_dynamic_array_insert_many(
            &error,
            allocator,
            (void**)&array,
            &capacity,
            &length,
            i,
            array,
            length,
            sizeof(array[0])
        );
        for (size_t i = 0; i < length; i++) {
            printf("%d, ", array[i]);
        }
        printf("\n");
        mk_dynamic_array_destroy(
            &error,
            allocator,
            (void**)&array,
            &capacity,
            &length
        );
        mk_dynamic_array_append_many(
            &error,
            allocator,
            (void**)&array,
            &capacity,
            &length,
            data,
            mk_countof(data),
            sizeof(array[0])
        );
    }

    mk_allocator_free(allocator, array);
    return 0;
}