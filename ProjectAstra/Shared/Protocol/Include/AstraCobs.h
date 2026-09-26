#pragma once

#include <stddef.h>
#include <stdint.h>
#include "AstraCobsEnums.h"

/*
 * Buffer size that is always sufficient for AstraCobsEncode of a payload of
 * `length` bytes. The bound carries one spare byte on purpose, so callers must
 * allocate this value rather than the exact encoded size, which is not
 * predictable without scanning the payload.
 */
size_t AstraCobsMaxEncodedSize(size_t length);

/*
 * Encodes `length` bytes from `src` into `dst` as classic COBS terminated by a
 * single ASTRA_COBS_DELIMITER.
 *
 * `src` may be NULL only when `length` is 0, which encodes as the two byte
 * frame {0x01, 0x00}. `dstCapacity` must be at least
 * AstraCobsMaxEncodedSize(length). On ASTRA_COBS_STATUS_OK `*encodedSize`
 * receives the frame length, always at least 1. On ASTRA_COBS_STATUS_ERROR the
 * out parameters are left untouched and `dst` is not written.
 */
AstraCobsStatus AstraCobsEncode(const uint8_t *src, size_t length, uint8_t *dst,
                                size_t dstCapacity, size_t *encodedSize);

/*
 * Decodes a classic COBS frame from the first `length` bytes of `src`.
 *
 * `src` may be NULL only when `length` is 0, which decodes to an empty
 * payload. On ASTRA_COBS_STATUS_OK `*decodedSize` receives the payload length,
 * which may legitimately be 0, so success must be judged by the status and not
 * by the size. On ASTRA_COBS_STATUS_ERROR the out parameters are left untouched
 * and `dst` may already hold bytes written by earlier blocks.
 */
AstraCobsStatus AstraCobsDecode(const uint8_t *src, size_t length, uint8_t *dst,
                                size_t dstCapacity, size_t *decodedSize);
