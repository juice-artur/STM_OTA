#include "AstraCobs.h"

static size_t AstraCobsMaxEncodedSize(size_t length)
{
	return length + (length / ASTRA_COBS_MAX_BLOCK_PAYLOAD) + 2u;
}

AstraCobsStatus AstraCobsEncode(const uint8_t *src, size_t length, uint8_t *dst,
                                size_t dstCapacity)
{
	size_t readIndex = 0u;
	size_t writeIndex = 0u;

	if (dst == NULL || (src == NULL && length != 0u))
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

	return ASTRA_COBS_STATUS_OK;
}
