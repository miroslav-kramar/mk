#ifndef MK_UTILS_H_
#define MK_UTILS_H_

#include "mk_common.h"

struct MkStringToSignedResult {
    struct MkError error;
    long long result;
};

struct MkStringToUnsignedResult {
    struct MkError error;
    unsigned long long result;
};

struct MkStringToFloatingResult {
    struct MkError error;
    double result;
};

struct MkStringToSignedResult mk_string_to_signed(
    const char * string
);

struct MkStringToUnsignedResult mk_string_to_unsigned(
    const char * string
);

struct MkStringToFloatingResult mk_string_to_floating(
    const char * string
);

#if defined MK_UTILS_IMPLEMENTATION || defined MK_IMPLEMENTATION

#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

struct MkStringToSignedResult mk_string_to_signed(
    const char * string
) {
    struct MkStringToSignedResult out = {0};
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

struct MkStringToUnsignedResult mk_string_to_unsigned(
    const char * string
) {
    struct MkStringToUnsignedResult out = {0};
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

struct MkStringToFloatingResult mk_string_to_floating(
    const char * string
) {
    struct MkStringToFloatingResult out = {0};
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
