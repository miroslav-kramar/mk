#ifndef MK_COMMON_H_
#define MK_COMMON_H_

#include <stdio.h>

#define mk_countof(x) (sizeof(x)/sizeof(x[0]))

#define MK_X_MACRO_LIST_ERROR_TYPE \
    X(MK_ERROR_NONE) \
    X(MK_ERROR_OOM) \
    X(MK_ERROR_IO)

enum MkErrorType {
    #define X(type) type,
    MK_X_MACRO_LIST_ERROR_TYPE
    #undef X
};

struct MkError {
    enum MkErrorType type;
    const char * message;
};

typedef void * (*MkAllocatorAllocFunction)(void * context, size_t size);
typedef void * (*MkAllocatorReallocFunction)(void * context, void * pointer, size_t size);
typedef void (*MkAllocatorFreeFunction)(void * context, void * pointer);

struct MkAllocator {
    void * context;
    MkAllocatorAllocFunction alloc;
    MkAllocatorReallocFunction realloc;
    MkAllocatorFreeFunction free;
};

const char * mk_error_type_to_string(
    enum MkErrorType type
);

struct MkError mk_error_create(
    enum MkErrorType type,
    const char * message
);

void mk_error_print(
    struct MkError error,
    FILE * file
);

void * mk_allocator_alloc(
    struct MkAllocator allocator,
    size_t size
);

void * mk_allocator_realloc(
    struct MkAllocator allocator,
    void * pointer, size_t size
);

void mk_allocator_free(
    struct MkAllocator allocator,
    void * pointer
);

void * mk_allocator_std_alloc(
    void * context, size_t size
);

void * mk_allocator_std_realloc(
    void * context,
    void * pointer,
    size_t size
);

void mk_allocator_std_free(
    void * context,
    void * pointer
);

struct MkAllocator mk_allocator_get_std_allocator(
    void
);

#if defined (MK_COMMON_IMPLEMENTATION) || defined (MK_IMPLEMENTATION)

#include <stdlib.h>

const char * mk_error_type_to_string(enum MkErrorType type) {
    switch (type) {
        #define X(type) case type: {return #type;} break;
        MK_X_MACRO_LIST_ERROR_TYPE
        #undef X
        default: {return "";} break;
    }
}

struct MkError mk_error_create(enum MkErrorType type, const char * message) {
    struct MkError error = {0};
    error.type = type;
    error.message = message;
    return error;
}

void mk_error_print(struct MkError error, FILE * file) {
    if (error.message == NULL) {
        fprintf(file, "%s\n", mk_error_type_to_string(error.type));
        return;
    }
    fprintf(file, "%s: %s\n", mk_error_type_to_string(error.type), error.message);
}

void * mk_allocator_alloc(struct MkAllocator allocator, size_t size) {
    return allocator.alloc(allocator.context, size);
}

void * mk_allocator_realloc(struct MkAllocator allocator, void * pointer, size_t size) {
    return allocator.realloc(allocator.context, pointer, size);
}

void mk_allocator_free(struct MkAllocator allocator, void * pointer) {
    allocator.free(allocator.context, pointer);
}

void * mk_allocator_std_alloc(void * context, size_t size) {
    (void) context;
    return malloc(size);
}

void * mk_allocator_std_realloc(void * context, void * pointer, size_t size) {
    (void) context;
    return realloc(pointer, size);
}

void mk_allocator_std_free(void * context, void * pointer) {
    (void) context;
    free(pointer);
}

struct MkAllocator mk_allocator_get_std_allocator(void) {
    struct MkAllocator std;
    std.context = NULL;
    std.alloc = mk_allocator_std_alloc;
    std.realloc = mk_allocator_std_realloc;
    std.free = mk_allocator_std_free;
    return std;
}

#endif // IMPLEMENTATION
#endif // MK_COMMON_H_