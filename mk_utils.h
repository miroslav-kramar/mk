#ifndef MK_UTILS_H_
#define MK_UTILS_H_

// -----------------------------------------------------------------------------
// PUBLIC HEADER
// -----------------------------------------------------------------------------

#include "mk_common.h"

// Types -----------------------------------------------------------------------

struct MkUtilsStringToSignedResult {
    struct MkError error;
    long long result;
};

struct MkUtilsStringToUnsignedResult {
    struct MkError error;
    unsigned long long result;
};

struct MkUtilsStringToFloatingResult {
    struct MkError error;
    double result;
};

// Functions -------------------------------------------------------------------

struct MkUtilsStringToSignedResult mk_utils_string_to_signed(
    const char * string
);

struct MkUtilsStringToUnsignedResult mk_utils_string_to_unsigned(
    const char * string
);

struct MkUtilsStringToFloatingResult mk_utils_string_to_floating(
    const char * string
);

// -----------------------------------------------------------------------------
// IMPLEMENTATION
// -----------------------------------------------------------------------------

#if defined MK_UTILS_IMPLEMENTATION || defined MK_IMPLEMENTATION

#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

struct MkUtilsStringToSignedResult mk_utils_string_to_signed(
    const char * string
) {
    struct MkUtilsStringToSignedResult out = {0};
    out.error = mk_error_create(MK_ERROR_NONE, NULL);
    out.result = 0;

    char * end = NULL;
    errno = 0;
    out.result = strtoll(string, &end, 0);
    if (errno == EINVAL) {
        goto LABEL_ERROR_FORMAT;
    }
    if (errno == ERANGE) {
        goto LABEL_ERROR_RANGE;
    }
    if (end == string) {
        goto LABEL_ERROR_FORMAT;
    }

    for (size_t i = 0; end[i] != '\0'; i++) {
        if (!isspace((unsigned char)end[i])) {
            goto LABEL_ERROR_FORMAT;
        }
    }

    return out;

    LABEL_ERROR_FORMAT:    
    out.error = mk_error_create(MK_ERROR_FORMAT, "Unsupported format!");
    return out;

    LABEL_ERROR_RANGE:
    out.error = mk_error_create(MK_ERROR_RANGE, "Value out of range!");
    return out;
}

struct MkUtilsStringToUnsignedResult mk_utils_string_to_unsigned(
    const char * string
) {
    struct MkUtilsStringToUnsignedResult out = {0};
    out.error = mk_error_create(MK_ERROR_NONE, NULL);
    out.result = 0;

    size_t first_non_whitespace_char_index = 0;
    for (
        first_non_whitespace_char_index = 0;
        string[first_non_whitespace_char_index] != '\0';
        first_non_whitespace_char_index++
    ) {
        if (!isspace((unsigned char)string[first_non_whitespace_char_index])) {
            break;
        }
    }
    if (string[first_non_whitespace_char_index] == '-') {
        goto LABEL_ERROR_RANGE;
    }

    char * end = NULL;
    errno = 0;
    out.result = strtoull(string, &end, 0);
    if (errno == EINVAL) {
        goto LABEL_ERROR_FORMAT;
    }
    if (errno == ERANGE) {
        goto LABEL_ERROR_RANGE;
    }
    if (end == string) {
        goto LABEL_ERROR_FORMAT;
    }

    for (size_t i = 0; end[i] != '\0'; i++) {
        if (!isspace((unsigned char)end[i])) {
            goto LABEL_ERROR_FORMAT;
        }
    }

    return out;

    LABEL_ERROR_FORMAT:    
    out.error = mk_error_create(MK_ERROR_FORMAT, "Unsupported format!");
    return out;

    LABEL_ERROR_RANGE:
    out.error = mk_error_create(MK_ERROR_RANGE, "Value out of range!");
    return out;
}

struct MkUtilsStringToFloatingResult mk_utils_string_to_floating(
    const char * string
) {
    struct MkUtilsStringToFloatingResult out = {0};
    out.error = mk_error_create(MK_ERROR_NONE, NULL);
    out.result = 0;

    char * end = NULL;
    errno = 0;
    out.result = strtod(string, &end);
    if (errno == EINVAL) {
        goto LABEL_ERROR_FORMAT;
    }
    if (errno == ERANGE) {
        goto LABEL_ERROR_RANGE;
    }
    if (end == string) {
        goto LABEL_ERROR_FORMAT;
    }

    for (size_t i = 0; end[i] != '\0'; i++) {
        if (!isspace((unsigned char)end[i])) {
            goto LABEL_ERROR_FORMAT;
        }
    }

    return out;

    LABEL_ERROR_FORMAT:    
    out.error = mk_error_create(MK_ERROR_FORMAT, "Unsupported format!");
    return out;

    LABEL_ERROR_RANGE:
    out.error = mk_error_create(MK_ERROR_RANGE, "Value out of range!");
    return out;
}

#endif // IMPLEMENTATION
#endif // MK_UTILS_H_
