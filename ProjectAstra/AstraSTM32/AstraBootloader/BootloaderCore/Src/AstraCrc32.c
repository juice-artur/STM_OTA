#include "AstraCrc32.h"

#include <stddef.h>

#define ASTRA_CRC32_REFLECTED_POLY 0xEDB88320UL
#define ASTRA_CRC32_SEED 0xFFFFFFFFUL

uint32_t AstraCrc32_Compute(const uint8_t *data, uint32_t length)
{
	uint32_t crc = ASTRA_CRC32_SEED;

	if (data == NULL)
	{
		return 0U;
	}

	for (uint32_t i = 0U; i < length; i++)
	{
		crc ^= data[i];

		for (uint32_t bit = 0U; bit < 8U; bit++)
		{
			if ((crc & 1U) != 0U)
			{
				crc = (crc >> 1) ^ ASTRA_CRC32_REFLECTED_POLY;
			}
			else
			{
				crc >>= 1;
			}
		}
	}

	return crc ^ ASTRA_CRC32_SEED;
}
