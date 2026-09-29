#pragma once

#include <stdint.h>

/*
 * CRC-32/ISO-HDLC (a.k.a. CRC-32, zlib crc32).
 *
 *   width   32
 *   poly    0x04C11DB7, reflected to 0xEDB88320
 *   init    0xFFFFFFFF
 *   refin   true
 *   refout  true
 *   xorout  0xFFFFFFFF
 *   check   0xCBF43926 for the ASCII string "123456789"
 *
 * The build host must produce the same value: see scripts/astra_image.py, which
 * uses zlib.crc32.
 */
uint32_t AstraCrc32_Compute(const uint8_t *data, uint32_t length);
