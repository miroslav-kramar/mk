#ifndef MK_COMMON_H_
#define MK_COMMON_H_

// -----------------------------------------------------------------------------
// PUBLIC HEADER
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdbool.h>

// Macros ----------------------------------------------------------------------

#define mk_countof(x) (sizeof(x)/sizeof(x[0]))

#define MK_X_MACRO_LIST_ERROR_TYPE \
    X(MK_ERROR_NONE) \
    X(MK_ERROR_OOM) \
    X(MK_ERROR_IO) \
    X(MK_ERROR_FORMAT) \
    X(MK_ERROR_RANGE)

// Types -----------------------------------------------------------------------

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
    const char * name;
    void * context;
    MkAllocatorAllocFunction alloc;
    MkAllocatorReallocFunction realloc;
    MkAllocatorFreeFunction free;
};

// Global Variables ------------------------------------------------------------

extern bool mk_allocator_enable_debug_output;
extern FILE * mk_allocator_debug_output_file;

// Functions -------------------------------------------------------------------

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

#define mk_allocator_alloc(allocator, size) mk_allocator_alloc_debug((allocator), (size), __FILE__, __LINE__)
void * mk_allocator_alloc_debug(
    struct MkAllocator allocator,
    size_t size,
    const char * file,
    int line
);

#define mk_allocator_realloc(allocator, pointer, size) mk_allocator_realloc_debug((allocator), (pointer), (size), __FILE__, __LINE__)
void * mk_allocator_realloc_debug(
    struct MkAllocator allocator,
    void * pointer, size_t size,
    const char * file,
    int line
);

#define mk_allocator_free(allocator, pointer) mk_allocator_free_debug((allocator), (pointer), __FILE__, __LINE__)
void mk_allocator_free_debug(
    struct MkAllocator allocator,
    void * pointer,
    const char * file,
    int line
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

// -----------------------------------------------------------------------------
// IMPLEMENTATION
// -----------------------------------------------------------------------------

#if defined MK_COMMON_IMPLEMENTATION || defined MK_IMPLEMENTATION

#include <stdlib.h>

bool mk_allocator_enable_debug_output = false;
FILE * mk_allocator_debug_output_file = NULL;

const char * mk_error_type_to_string(

    enum MkErrorType type
) {
    switch (type) {
        #define X(type) case type: {return #type;} break;
        MK_X_MACRO_LIST_ERROR_TYPE
        #undef X
        default: {return "";} break;
    }
}

struct MkError mk_error_create(
    enum MkErrorType type,
    const char * message
) {
    struct MkError error = {0};
    error.type = type;
    error.message = message;
    return error;
}

void mk_error_print(
    struct MkError error,
    FILE * file
) {
    if (error.message == NULL) {
        fprintf(file, "%s\n", mk_error_type_to_string(error.type));
        return;
    }
    fprintf(file, "%s: %s\n", mk_error_type_to_string(error.type), error.message);
}

void * mk_allocator_alloc_debug(
    struct MkAllocator allocator,
    size_t size,
    const char * file,
    int line
) {
    void * out = allocator.alloc(allocator.context, size);
    if (mk_allocator_enable_debug_output) {
        if (mk_allocator_debug_output_file == NULL) {
            mk_allocator_debug_output_file = stderr;
        }
        fprintf(
            mk_allocator_debug_output_file,
            "[%s][%s:%d] alloc: new pointer: %p, new size: %zu\n",
            allocator.name,
            file,
            line,
            out,
            size
        );
    }
    return out;
}

void * mk_allocator_realloc_debug(
    struct MkAllocator allocator,
    void * pointer,
    size_t size,
    const char * file,
    int line
) {
    void * out = allocator.realloc(allocator.context, pointer, size);
    if (mk_allocator_enable_debug_output) {
        if (mk_allocator_debug_output_file == NULL) {
            mk_allocator_debug_output_file = stderr;
        }
        fprintf(
            mk_allocator_debug_output_file,
            "[%s][%s:%d] realloc: old pointer: %p, new pointer: %p, new size: %zu\n",
            allocator.name,
            file,
            line,
            pointer,
            out,
            size
        );
    }
    return out;
}

void mk_allocator_free_debug(
    struct MkAllocator allocator,
    void * pointer,
    const char * file,
    int line
) {
    allocator.free(allocator.context, pointer);
    if (mk_allocator_enable_debug_output) {
        if (mk_allocator_debug_output_file == NULL) {
            mk_allocator_debug_output_file = stderr;
        }
        fprintf(
            mk_allocator_debug_output_file,
            "[%s][%s:%d] free: pointer: %p\n",
            allocator.name,
            file,
            line,
            pointer
        );
    }
}

void * mk_allocator_std_alloc(
    void * context,
    size_t size
) {
    (void) context;
    return malloc(size);
}

void * mk_allocator_std_realloc(
    void * context,
    void * pointer,
    size_t size
) {
    (void) context;
    return realloc(pointer, size);
}

void mk_allocator_std_free(
    void * context,
    void * pointer
) {
    (void) context;
    free(pointer);
}

struct MkAllocator mk_allocator_get_std_allocator(
    void
) {
    struct MkAllocator std;
    std.name = "mk_allocator_std";
    std.context = NULL;
    std.alloc = mk_allocator_std_alloc;
    std.realloc = mk_allocator_std_realloc;
    std.free = mk_allocator_std_free;
    return std;
}

#endif // IMPLEMENTATION
#endif // MK_COMMON_H_