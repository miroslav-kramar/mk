#include <stdio.h>
#include <stdlib.h>

#define MK_IMPLEMENTATION
#include "mk_common.h"
#include "mk_utils.h"

int main() {
    const char * const inputs[] = {
        "123",
        " 123",
        "123 ",
        " 123 ",
        "-123 ",
        "- 123 ",
        "+123 ",
        "+ 123 ",
        "123a",
        "a123",
        "a123a",
        "123 123",
        "123 a",
        "123 abc ",
        "abc 123",
        "abcd",
        "1.234",
        "-1.234",
        "01.234",
        "33.22",
        "0xFF",
        "-0xFF",
    };

    printf("--------------------\n");
    printf("SIGNED\n");
    printf("--------------------\n");
    for (size_t i = 0; i < mk_countof(inputs); i++) {
        struct MkStringToSignedResult r = mk_string_to_signed(inputs[i]);
        printf("input:  `%s`\n", inputs[i]);
        printf("result: %lld\n", r.result);
        mk_error_print(r.error, stdout);
        printf("\n");
    }

    printf("--------------------\n");
    printf("UNSIGNED\n");
    printf("--------------------\n");
    for (size_t i = 0; i < mk_countof(inputs); i++) {
        struct MkStringToUnsignedResult r = mk_string_to_unsigned(inputs[i]);
        printf("input:  `%s`\n", inputs[i]);
        printf("result: %llu\n", r.result);
        mk_error_print(r.error, stdout);
        printf("\n");
    }

    printf("--------------------\n");
    printf("FLOATING\n");
    printf("--------------------\n");
    for (size_t i = 0; i < mk_countof(inputs); i++) {
        struct MkStringToFloatingResult r = mk_string_to_floating(inputs[i]);
        printf("input:  `%s`\n", inputs[i]);
        printf("result: %f\n", r.result);
        mk_error_print(r.error, stdout);
        printf("\n");
    }

    return 0;
}