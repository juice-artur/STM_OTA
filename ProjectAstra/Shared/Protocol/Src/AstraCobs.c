#include "AstraCobs.h"

size_t AstraCobsMaxEncodedSize(size_t length)
{
	return length + (length / ASTRA_COBS_MAX_BLOCK_PAYLOAD) + 2u;
}

AstraCobsStatus AstraCobsEncode(const uint8_t *src, size_t length, uint8_t *dst,
                                size_t dstCapacity, size_t *encodedSize)
{
	size_t readIndex = 0u;
	size_t writeIndex = 0u;

	if (dst == NULL || encodedSize == NULL || (src == NULL && length != 0u))
	{
		return ASTRA_COBS_STATUS_ERROR;
	}
	if (dstCapacity < AstraCobsMaxEncodedSize(length))
	{
		return ASTRA_COBS_STATUS_ERROR;
	}

	while (readIndex <= length)
	{
		size_t blockStart = readIndex;
		size_t run = 0u;

		while (readIndex < length && src[readIndex] != ASTRA_COBS_DELIMITER &&
		       run < ASTRA_COBS_MAX_BLOCK_PAYLOAD)
		{
			++readIndex;
			++run;
		}

		dst[writeIndex++] = (uint8_t)(run + 1u);
		for (size_t offset = 0u; offset < run; ++offset)
		{
			dst[writeIndex++] = src[blockStart + offset];
		}

		if (readIndex >= length)
		{
			break;
		}

		if (src[readIndex] == ASTRA_COBS_DELIMITER)
		{
			++readIndex;
			if (readIndex == length)
			{
				break;
			}
		}
	}

	dst[writeIndex++] = ASTRA_COBS_DELIMITER;
	*encodedSize = writeIndex;

	return ASTRA_COBS_STATUS_OK;
}

AstraCobsStatus AstraCobsDecode(const uint8_t *src, size_t length, uint8_t *dst,
                                size_t dstCapacity, size_t *decodedSize)
{
	size_t readIndex = 0u;
	size_t writeIndex = 0u;
	size_t run = 0u;
	uint8_t code = 0u;

	if (dst == NULL || decodedSize == NULL || (src == NULL && length != 0u))
	{
		return ASTRA_COBS_STATUS_ERROR;
	}

	while (readIndex < length && src[readIndex] != ASTRA_COBS_DELIMITER)
	{
		code = src[readIndex];
		++readIndex;
		run = (size_t)code - 1u;

		if (run > (length - readIndex) || run > (dstCapacity - writeIndex))
		{
			return ASTRA_COBS_STATUS_ERROR;
		}

		for (size_t offset = 0u; offset < run; ++offset)
		{
			dst[writeIndex + offset] = src[readIndex + offset];
		}

		readIndex += run;
		writeIndex += run;

		if (code < (ASTRA_COBS_MAX_BLOCK_PAYLOAD + 1u) && readIndex < length &&
		    src[readIndex] != ASTRA_COBS_DELIMITER)
		{
			if (writeIndex >= dstCapacity)
			{
				return ASTRA_COBS_STATUS_ERROR;
			}

			dst[writeIndex++] = ASTRA_COBS_DELIMITER;
		}
	}

	*decodedSize = writeIndex;

	return ASTRA_COBS_STATUS_OK;
}
