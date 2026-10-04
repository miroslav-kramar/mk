#ifndef MK_DYNAMIC_ARRAY_H_
#define MK_DYNAMIC_ARRAY_H_

#include "mk_common.h"

void mk_dynamic_array_create(
    struct MkError * error,
    void ** array,
    size_t * capacity,
    size_t * length
);

void mk_dynamic_array_reserve_capacity(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t target_capacity,
    size_t item_size
);

void mk_dynamic_array_adjust_capacity(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t item_size
);

void mk_dynamic_array_insert_many(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    const void * items,
    size_t items_length,
    size_t item_size
);

void mk_dynamic_array_insert(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    const void * item,
    size_t item_size
);

void mk_dynamic_array_append_many(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    const void * items,
    size_t items_length,
    size_t item_size
);

void mk_dynamic_array_append(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    const void * item,
    size_t item_size
);

void mk_dynamic_array_remove_many(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    size_t count,
    size_t item_size
);

void mk_dynamic_array_remove_range(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index_from,
    size_t index_to,
    size_t item_size
);

void mk_dynamic_array_remove(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    size_t item_size
);

void mk_dynamic_array_remove_unordered_many(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    size_t count,
    size_t item_size
);

void mk_dynamic_array_remove_unordered_range(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index_from,
    size_t index_to,
    size_t item_size
);

void mk_dynamic_array_remove_unordered(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    size_t item_size
);

#if defined MK_DA_IMPLEMENTATION || defined MK_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mk_dynamic_array_create(
    struct MkError * error,
    void ** array,
    size_t * capacity,
    size_t * length
) {
    *error = mk_error_create(MK_ERROR_NONE, NULL);
    *array = NULL;
    *capacity = 0;
    *length = 0;
}

void mk_dynamic_array_destroy(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length
) {
    mk_allocator_free(allocator, *array);
    *error = mk_error_create(MK_ERROR_NONE, NULL);
    *array = NULL;
    *capacity = 0;
    *length = 0;
}

void mk_dynamic_array_reserve_capacity(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t target_capacity,
    size_t item_size
) {
    (void) length;
    if (error->type != MK_ERROR_NONE) {
        return;
    }
    if (*capacity >= target_capacity) {
        return;
    }
    size_t new_capacity = 1;
    while (new_capacity < target_capacity) {
        new_capacity *= 2;
    }
    void * new_array = mk_allocator_realloc(allocator, *array, new_capacity * item_size);
    if (new_array == NULL) {
        error->type = MK_ERROR_OOM;
        return;
    }
    *capacity = new_capacity;
    *array = new_array;
    return;
}

void mk_dynamic_array_adjust_capacity(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t item_size
) {
    if (error->type != MK_ERROR_NONE) {
        return;
    }
    if (*length > *capacity / 4) {
        return;
    }
    size_t new_capacity = *capacity / 2;
    void * new_array = mk_allocator_realloc(allocator, *array, new_capacity * item_size);
    if (new_array == NULL) {
        error->type = MK_ERROR_OOM;
        return;
    }
    *array = new_array;
    *capacity = new_capacity;
}

void mk_dynamic_array_insert_many(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    const void * items,
    size_t items_length,
    size_t item_size
) {
    if (error->type != MK_ERROR_NONE) {
        return;
    }
    if (index > *length) {
        fprintf(stderr, "Index out of bounds!\n");
        abort();
    }
    mk_dynamic_array_reserve_capacity(
        error,
        allocator,
        array,
        capacity,
        length,
        *length + items_length,
        item_size
    );
    if (error->type != MK_ERROR_NONE) {
        return;
    }
    if (index < *length) {
        memmove(
            (unsigned char *)*array + (index + 1) * item_size,
            (unsigned char *)*array + index * item_size,
            (*length - index) * item_size
        );
    }
    memcpy(
        (unsigned char *)*array + index * item_size,
        items,
        items_length * item_size
    );
    *length += items_length;
}

void mk_dynamic_array_insert(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    const void * item,
    size_t item_size
) {
    mk_dynamic_array_insert_many(
        error,
        allocator,
        array,
        capacity,
        length,
        index,
        item,
        1,
        item_size
    );
}

void mk_dynamic_array_append_many(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    const void * items,
    size_t items_length,
    size_t item_size
) {
    mk_dynamic_array_insert_many(
        error,
        allocator,
        array,
        capacity,
        length,
        *length,
        items,
        items_length,
        item_size
    );
}

void mk_dynamic_array_append(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    const void * item,
    size_t item_size
) {
    mk_dynamic_array_append_many(
        error,
        allocator,
        array,
        capacity,
        length,
        item,
        1,
        item_size
    );
}

void mk_dynamic_array_remove_many(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    size_t count,
    size_t item_size
) {
    (void) allocator;
    (void) capacity;
    if (error->type != MK_ERROR_NONE) {
        return;
    }
    if (count == 0) {
        return;
    }
    if (index + count - 1 >= *length) {
        fprintf(stderr, "Range out of bounds!\n");
        abort();
    }
    memmove(
        (unsigned char *)*array + index * item_size,
        (unsigned char *)*array + (index + count) * item_size,
        (*length - (index + count - 1)) * item_size
    );
    *length -= count;
    mk_dynamic_array_adjust_capacity(
        error,
        allocator,
        array,
        capacity,
        length,
        item_size
    );
}

void mk_dynamic_array_remove_range(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index_from,
    size_t index_to,
    size_t item_size
) {
    if (error->type != MK_ERROR_NONE) {
        return;
    }
    if (index_from > index_to) {
        fprintf(stderr, "Invalid range!\n");
        abort();
    }
    mk_dynamic_array_remove_many(
        error,
        allocator,
        array,
        capacity,
        length,
        index_from,
        index_to - index_from + 1,
        item_size
    );
}

void mk_dynamic_array_remove(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    size_t item_size
) {
    mk_dynamic_array_remove_many(
        error,
        allocator,
        array,
        capacity,
        length,
        index,
        1,
        item_size
    );
}

void mk_dynamic_array_remove_unordered_many(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    size_t count,
    size_t item_size
) {
    (void) allocator;
    (void) capacity;
    if (error->type != MK_ERROR_NONE) {
        return;
    }
    if (index + count - 1 >= *length) {
        fprintf(stderr, "Range out of bounds!\n");
        abort();
    }
    if (count > *length - (index + count)) {
        memmove(
            (unsigned char *)*array + index * item_size,
            (unsigned char *)*array + (index + count) * item_size,
            (*length - (index + count)) * item_size
        );
    }
    else {
        memmove(
            (unsigned char *)*array + index * item_size,
            (unsigned char *)*array + (*length - count) * item_size,
            count * item_size
        );
    }
    *length -= count;
    mk_dynamic_array_adjust_capacity(
        error,
        allocator,
        array,
        capacity,
        length,
        item_size
    );
}

void mk_dynamic_array_remove_unordered_range(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index_from,
    size_t index_to,
    size_t item_size
) {
    if (error->type != MK_ERROR_NONE) {
        return;
    }
    if (index_from > index_to) {
        fprintf(stderr, "Invalid range!\n");
        abort();
    }
    mk_dynamic_array_remove_unordered_many(
        error,
        allocator,
        array,
        capacity,
        length,
        index_from,
        index_to - index_from + 1,
        item_size
    );
}

void mk_dynamic_array_remove_unordered(
    struct MkError * error,
    struct MkAllocator allocator,
    void ** array,
    size_t * capacity,
    size_t * length,
    size_t index,
    size_t item_size
) {
    mk_dynamic_array_remove_unordered_many(
        error,
        allocator,
        array,
        capacity,
        length,
        index,
        1,
        item_size
    );
}

#endif // IMPLEMENTATION
#endif // MK_DYNAMIC_ARRAY_H_