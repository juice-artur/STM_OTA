#include "utest.h"
#include "AstraCobs.h"

UTEST(AstraCobs, AstraCobsEncode)
{
	uint8_t src[] = {0x11, 0x22, 0x00, 0x33, 0x44, 0x00, 0x55};

	uint8_t dst[16];

	AstraCobsStatus status = AstraCobsEncode(src, sizeof(src), dst, sizeof(dst));

	uint8_t expected[] = {0x03, 0x11, 0x22, 0x03, 0x33, 0x44, 0x02, 0x55, 0x00};

	EXPECT_EQ(ASTRA_COBS_STATUS_OK, status);

	for (size_t i = 0; i < sizeof(expected); ++i)
	{
		EXPECT_EQ(expected[i], dst[i]);
	}
}

UTEST(AstraCobs, AstraCobsEncodeFullZero)
{
	uint8_t src[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	uint8_t dst[16];

	AstraCobsStatus status = AstraCobsEncode(src, sizeof(src), dst, sizeof(dst));

	uint8_t expected[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00};

	EXPECT_EQ(ASTRA_COBS_STATUS_OK, status);

	for (size_t i = 0; i < sizeof(expected); ++i)
	{
		EXPECT_EQ(expected[i], dst[i]);
	}
}