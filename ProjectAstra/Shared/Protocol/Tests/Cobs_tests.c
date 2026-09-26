#include "utest.h"

UTEST(AstraCobs, AstraCobsEncode)
{
	uint8_t src[] = {0x11, 0x22, 0x00, 0x33, 0x44, 0x00, 0x55};

	uint8_t dst[16];

	size_t encodedSize = AstraCobsEncode(src, sizeof(src), dst);

	uint8_t expected[] = {0x03, 0x11, 0x22, 0x03, 0x33, 0x44, 0x02, 0x55, 0x00};

	EXPECT_EQ(sizeof(expected), encodedSize);

	for (size_t i = 0; i < sizeof(expected); ++i)
	{
		EXPECT_EQ(expected[i], dst[i]);
	}
}