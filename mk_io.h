#ifndef MK_IO_H_
#define MK_IO_H_

// -----------------------------------------------------------------------------
// PUBLIC HEADER
// -----------------------------------------------------------------------------

#include "mk_common.h"

// Types -----------------------------------------------------------------------

struct MkIoFileLoadResult {
    struct MkError error;
    char * text;
    size_t length;
};

// Functions -------------------------------------------------------------------

struct MkIoFileLoadResult mk_io_file_load(
    struct MkAllocator allocator,
    const char * filename
);

// -----------------------------------------------------------------------------
// IMPLEMENTATION
// -----------------------------------------------------------------------------

#if defined MK_IO_IMPLEMENTATION || defined MK_IMPLEMENTATION  

struct MkIoFileLoadResult mk_io_file_load(
    struct MkAllocator allocator,
    const char * filename
) {
    struct MkIoFileLoadResult out = {0};
    out.error = mk_error_create(MK_ERROR_NONE, NULL);
    out.text = NULL;
    out.length = 0;

    FILE * file = fopen(filename, "rb");
    if (file == NULL) {
        out.error = mk_error_create(MK_ERROR_IO, "Could not open file!");
        return out;
    }

    fseek(file, 0, SEEK_END);
    out.length = ftell(file);
    out.text = mk_allocator_alloc(allocator, out.length + 1);
    if (out.text == NULL) {
        out.error = mk_error_create(MK_ERROR_OOM, "Out of memory!");
        return out;
    }

    fseek(file, 0, SEEK_SET);
    fread(out.text, sizeof(out.text[0]), out.length, file);
    fclose(file);
    out.text[out.length] = '\0';
    return out;
}

#endif // IMPLEMENTATION
#endif // MK_IO_H_