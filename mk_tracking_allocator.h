#ifndef MK_TRACKING_ALLOCATOR_H_
#define MK_TRACKING_ALLOCATOR_H_

// -----------------------------------------------------------------------------
// PUBLIC HEADER
// -----------------------------------------------------------------------------

#include "mk_common.h"
#include "mk_dynamic_array.h"

// Types -----------------------------------------------------------------------

typedef struct {
    void * pointer;
} MkTrackingAllocatorRecord;

typedef struct {
    struct MkAllocator allocator;
    struct MkError error;
    MkTrackingAllocatorRecord * records;
    size_t capacity;
    size_t length;
} MkTrackingAllocator;

// Functions -------------------------------------------------------------------

MkTrackingAllocator mk_tracking_allocator_create(
    struct MkAllocator allocator
);

void mk_tracking_allocator_destroy(
    MkTrackingAllocator * tracking_allocator
);

void * mk_tracking_allocator_realloc(
    MkTrackingAllocator * tracking_allocator,
    void * pointer, size_t size
);

void * mk_tracking_allocator_alloc(
    MkTrackingAllocator * tracking_allocator,
    size_t size
);

void mk_tracking_allocator_free(
    MkTrackingAllocator * tracking_allocator,
    void * pointer
);

struct MkAllocator mk_tracking_allocator_get_allocator(
    MkTrackingAllocator * tracking_allocator
);

// -----------------------------------------------------------------------------
// IMPLEMENTATION
// -----------------------------------------------------------------------------

#if defined MK_TRACKING_ALLOCATOR_IMPLEMENTATION || defined MK_IMPLEMENTATION

#include <stdlib.h>
#include <stdbool.h>

MkTrackingAllocator mk_tracking_allocator_create(
    struct MkAllocator allocator
) {
    MkTrackingAllocator ta = {0};
    ta.allocator = allocator;
    ta.error = mk_error_create(MK_ERROR_NONE, NULL);
    ta.records = NULL;
    ta.capacity = 0;
    ta.length = 0;
    return ta;
}

void mk_tracking_allocator_destroy(
    MkTrackingAllocator * tracking_allocator
) {
    for (size_t i = 0; i < tracking_allocator->length; i++) {
        mk_allocator_free(
            tracking_allocator->allocator,
            tracking_allocator->records[i].pointer
        );
    }
    mk_allocator_free(
        tracking_allocator->allocator,
        tracking_allocator->records
    );
    tracking_allocator->error = mk_error_create(MK_ERROR_NONE, NULL);
    tracking_allocator->records = NULL;
    tracking_allocator->capacity = 0;
    tracking_allocator->length = 0;
}

void * mk_tracking_allocator_realloc(
    MkTrackingAllocator * tracking_allocator,
    void * pointer, size_t size
) {
    if (pointer == NULL) {
        if (size > 0) {
            void * new_pointer = mk_allocator_alloc(tracking_allocator->allocator, size);
            if (new_pointer == NULL) {
                tracking_allocator->error = mk_error_create(MK_ERROR_OOM, "Out of memory!");
                return NULL;
            }
            MkTrackingAllocatorRecord record = {0};
            record.pointer = new_pointer;
            mk_dynamic_array_append(
                &tracking_allocator->error,
                tracking_allocator->allocator,
                (void**)&tracking_allocator->records,
                &tracking_allocator->capacity,
                &tracking_allocator->length,
                &record,
                sizeof(record)
            );
            if (tracking_allocator->error.type != MK_ERROR_NONE) {
                mk_allocator_free(tracking_allocator->allocator, new_pointer);
                return NULL;
            }
            return new_pointer;
        }
        else {
            fprintf(stderr, "Invalid size!\n");
            abort();
        }
    }
    else {
        bool record_found = true;
        size_t record_index = 0;
        for (size_t i = 0; i < tracking_allocator->length; i++) {
            if (tracking_allocator->records->pointer == pointer) {
                record_found = true;
                record_index = i;
                break;
            }
        }
        if (!record_found) {
            fprintf(stderr, "Unknown pointer!\n");
            abort();
        }

        if (size > 0) {
            void * new_pointer = mk_allocator_realloc(
                tracking_allocator->allocator,
                pointer, size
            );
            if (new_pointer == NULL) {
                tracking_allocator->error = mk_error_create(MK_ERROR_OOM, "Out of memory!");
                return NULL;
            }
            tracking_allocator->records[record_index].pointer = new_pointer;
            return new_pointer;
        }
        else {
            mk_allocator_free(tracking_allocator->allocator, pointer);
            mk_dynamic_array_remove_unordered(
                &tracking_allocator->error,
                tracking_allocator->allocator,
                (void**)&tracking_allocator->records,
                &tracking_allocator->capacity,
                &tracking_allocator->length,
                record_index,
                sizeof(tracking_allocator->records[0])
            );
            return NULL;
        }
    }
}

void * mk_tracking_allocator_alloc(
    MkTrackingAllocator * tracking_allocator,
    size_t size
) {
    return mk_tracking_allocator_realloc(
        tracking_allocator,
        NULL,
        size
    );
}

void mk_tracking_allocator_free(
    MkTrackingAllocator * tracking_allocator,
    void * pointer
) {
    mk_tracking_allocator_realloc(
        tracking_allocator,
        pointer,
        0
    );
}

struct MkAllocator mk_tracking_allocator_get_allocator(
    MkTrackingAllocator * tracking_allocator
) {
    struct MkAllocator allocator = {0};
    allocator.context = tracking_allocator;
    allocator.alloc = (MkAllocatorAllocFunction)mk_tracking_allocator_alloc;
    allocator.realloc = (MkAllocatorReallocFunction)mk_tracking_allocator_realloc;
    allocator.free = (MkAllocatorFreeFunction)mk_tracking_allocator_free;
    return allocator;
}

#endif // IMPLEMENTATION
#endif // MK_TRACKING_ALLOCATOR_H_