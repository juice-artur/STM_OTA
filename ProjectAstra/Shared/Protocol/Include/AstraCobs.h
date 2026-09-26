#pragma once

#include <stddef.h>
#include <stdint.h>
#include "AstraCobsEnums.h"

AstraCobsStatus AstraCobsEncode(const uint8_t *src, size_t length, uint8_t *dst,
                                size_t dstCapacity);