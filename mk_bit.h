#ifndef MK_BIT_H_
#define MK_BIT_H_

#include "mk_common.h"

unsigned char mk_bit_get(
    unsigned char byte,
    size_t bit
);

unsigned char mk_bit_set(
    unsigned char byte,
    size_t bit
);

unsigned char mk_bit_clear(
    unsigned char byte,
    size_t bit
);

void mk_bit_array_set(
    void * array,
    size_t size_bytes,
    size_t bit_index
);

void mk_bit_array_clear(
    void * array,
    size_t size_bytes,
    size_t bit_index
);

unsigned char mk_bit_array_get(
    void * array,
    size_t size_bytes,
    size_t bit_index
);

#if defined MK_BIT_IMPLEMENTATION || defined MK_IMPLEMENTATION

#include <limits.h>

unsigned char mk_bit_get(
    unsigned char byte,
    size_t bit
) {
    MK_ASSERT(bit_index < CHAR_BIT, "Bit out of range!");
    return byte >> bit & 1;
}

unsigned char mk_bit_set(
    unsigned char byte,
    size_t bit
) {
    MK_ASSERT(bit_index < CHAR_BIT, "Bit out of range!");
    return byte | (1 << bit);
}

unsigned char mk_bit_clear(
    unsigned char byte,
    size_t bit
) {
    MK_ASSERT(bit_index < CHAR_BIT, "Bit out of range!");
    return byte & ~(1 << bit);
}

void mk_bit_array_set(
    void * array,
    size_t size_bytes,
    size_t bit_index
) {
    MK_ASSERT(bit_index < size_bytes * CHAR_BIT, "Bit out of range!");
    ((unsigned char *)array)[bit_index / CHAR_BIT] |= 1 << (bit_index % CHAR_BIT);
}

void mk_bit_array_clear(
    void * array,
    size_t size_bytes,
    size_t bit_index
) {
    MK_ASSERT(bit_index < size_bytes * CHAR_BIT, "Bit out of range!");
    ((unsigned char *)array)[bit_index / CHAR_BIT] &= ~(1 << (bit_index % CHAR_BIT));
}

unsigned char mk_bit_array_get(
    void * array,
    size_t size_bytes,
    size_t bit_index
) {
    MK_ASSERT(bit_index < size_bytes * CHAR_BIT, "Bit out of range!");
    return (((unsigned char *)array)[bit_index / CHAR_BIT] >> (bit_index % CHAR_BIT)) & 1;
}

#endif // IMPLEMENTATION
#endif // MK_BIT_H_