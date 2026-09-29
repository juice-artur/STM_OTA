#pragma once

#include <stdint.h>

/* "ASTR" as it appears in memory on a little-endian target. */
#define ASTRA_MAGIC_VALUE 0x52545341UL

typedef struct
{
    uint32_t magic;
    uint32_t size;
    uint32_t crc;
    uint32_t version;
} AppHeader_t;

_Static_assert(sizeof(AppHeader_t) == 16U, "Application header must stay 16 bytes wide");

extern const AppHeader_t app_header;

const AppHeader_t *GetAppHeader();