#include "utest.h"
#include "AstraCobs.h"

UTEST(AstraCobs, AstraCobsEncode)
{
	uint8_t src[] = {0x11, 0x22, 0x00, 0x33, 0x44, 0x00, 0x55};

	uint8_t dst[16];

	size_t encodedSize = 0u;

	AstraCobsStatus status =
	 AstraCobsEncode(src, sizeof(src), dst, sizeof(dst), &encodedSize);

	uint8_t expected[] = {0x03, 0x11, 0x22, 0x03, 0x33, 0x44, 0x02, 0x55, 0x00};

	EXPECT_EQ(ASTRA_COBS_STATUS_OK, status);
	EXPECT_EQ(sizeof(expected), encodedSize);

	for (size_t i = 0; i < sizeof(expected); ++i)
	{
		EXPECT_EQ(expected[i], dst[i]);
	}
}

UTEST(AstraCobs, AstraCobsEncodeFullZero)
{
	uint8_t src[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	uint8_t dst[16];

	size_t encodedSize = 0u;

	AstraCobsStatus status =
	 AstraCobsEncode(src, sizeof(src), dst, sizeof(dst), &encodedSize);

	uint8_t expected[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00};

	EXPECT_EQ(ASTRA_COBS_STATUS_OK, status);
	EXPECT_EQ(sizeof(expected), encodedSize);

	for (size_t i = 0; i < sizeof(expected); ++i)
	{
		EXPECT_EQ(expected[i], dst[i]);
	}
}

UTEST(AstraCobs, EncodeFullBlock)
{
	uint8_t src[254];

	uint8_t dst[257];

	size_t encodedSize = 0u;

	for (size_t i = 0u; i < sizeof(src); ++i)
	{
		src[i] = 0x01;
	}

	EXPECT_EQ((size_t)257u, AstraCobsMaxEncodedSize(sizeof(src)));

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsEncode(src, sizeof(src), dst, sizeof(dst), &encodedSize));

	EXPECT_EQ((size_t)256u, encodedSize);
	EXPECT_EQ((uint8_t)0xFF, dst[0]);
	EXPECT_EQ((uint8_t)0x01, dst[254]);
	EXPECT_EQ(ASTRA_COBS_DELIMITER, dst[255]);
}

UTEST(AstraCobs, EncodeSpillsIntoSecondBlock)
{
	uint8_t src[255];

	uint8_t dst[258];

	size_t encodedSize = 0u;

	for (size_t i = 0u; i < sizeof(src); ++i)
	{
		src[i] = 0x01;
	}

	EXPECT_EQ((size_t)258u, AstraCobsMaxEncodedSize(sizeof(src)));

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsEncode(src, sizeof(src), dst, sizeof(dst), &encodedSize));

	EXPECT_EQ((size_t)258u, encodedSize);
	EXPECT_EQ((uint8_t)0xFF, dst[0]);
	EXPECT_EQ((uint8_t)0x01, dst[254]);
	EXPECT_EQ((uint8_t)0x02, dst[255]);
	EXPECT_EQ((uint8_t)0x01, dst[256]);
	EXPECT_EQ(ASTRA_COBS_DELIMITER, dst[257]);
}

UTEST(AstraCobs, EncodeNullSourceIsEmptyPayload)
{
	uint8_t dst[2];

	size_t encodedSize = 0u;

	EXPECT_EQ((size_t)2u, AstraCobsMaxEncodedSize(0u));

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsEncode(NULL, 0u, dst, sizeof(dst), &encodedSize));

	EXPECT_EQ((size_t)2u, encodedSize);
	EXPECT_EQ((uint8_t)0x01, dst[0]);
	EXPECT_EQ(ASTRA_COBS_DELIMITER, dst[1]);

	uint8_t src[] = {0x00};

	uint8_t zeroDst[3];

	size_t zeroSize = 0u;

	EXPECT_EQ((size_t)3u, AstraCobsMaxEncodedSize(1u));

	EXPECT_EQ(ASTRA_COBS_STATUS_ERROR,
	          AstraCobsEncode(NULL, 1u, dst, sizeof(dst), &encodedSize));

	EXPECT_EQ(ASTRA_COBS_STATUS_OK, AstraCobsEncode(src, sizeof(src), zeroDst,
	                                                sizeof(zeroDst), &zeroSize));

	EXPECT_EQ(encodedSize, zeroSize);
	EXPECT_EQ(dst[0], zeroDst[0]);
	EXPECT_EQ(dst[1], zeroDst[1]);
}

UTEST(AstraCobs, EncodeRejectsInsufficientCapacity)
{
	uint8_t src[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	uint8_t dst[9];

	size_t encodedSize = 0u;

	EXPECT_EQ((size_t)9u, AstraCobsMaxEncodedSize(sizeof(src)));

	EXPECT_EQ(ASTRA_COBS_STATUS_ERROR,
	          AstraCobsEncode(src, sizeof(src), dst, 8u, &encodedSize));

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsEncode(src, sizeof(src), dst, sizeof(dst), &encodedSize));

	EXPECT_EQ((size_t)8u, encodedSize);

	EXPECT_EQ(ASTRA_COBS_STATUS_ERROR,
	          AstraCobsEncode(src, sizeof(src), NULL, sizeof(dst), &encodedSize));

	EXPECT_EQ(ASTRA_COBS_STATUS_ERROR,
	          AstraCobsEncode(src, sizeof(src), dst, sizeof(dst), NULL));
}

UTEST(AstraCobs, AstraCobsDecode)
{
	uint8_t src[] = {0x03, 0x11, 0x22, 0x03, 0x33, 0x44, 0x02, 0x55, 0x00};

	uint8_t dst[16];

	size_t decodedSize = 0u;

	AstraCobsStatus status =
	 AstraCobsDecode(src, sizeof(src), dst, sizeof(dst), &decodedSize);

	uint8_t expected[] = {0x11, 0x22, 0x00, 0x33, 0x44, 0x00, 0x55};

	EXPECT_EQ(ASTRA_COBS_STATUS_OK, status);
	EXPECT_EQ(sizeof(expected), decodedSize);

	for (size_t i = 0; i < sizeof(expected); ++i)
	{
		EXPECT_EQ(expected[i], dst[i]);
	}
}

UTEST(AstraCobs, DecodeEmptyPayload)
{
	uint8_t src[] = {0x01, 0x00};

	uint8_t dst[4] = {0xFF, 0xFF, 0xFF, 0xFF};

	size_t decodedSize = 123u;

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsDecode(src, sizeof(src), dst, sizeof(dst), &decodedSize));

	EXPECT_EQ((size_t)0u, decodedSize);
	EXPECT_EQ((uint8_t)0xFF, dst[0]);
}

UTEST(AstraCobs, DecodeRejectsInsufficientCapacity)
{
	uint8_t src[] = {0x03, 0x11, 0x22, 0x03, 0x33, 0x44, 0x02, 0x55, 0x00};

	uint8_t dst[7];

	size_t decodedSize = 0u;

	EXPECT_EQ(ASTRA_COBS_STATUS_ERROR,
	          AstraCobsDecode(src, sizeof(src), dst, 6u, &decodedSize));

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsDecode(src, sizeof(src), dst, sizeof(dst), &decodedSize));

	EXPECT_EQ((size_t)7u, decodedSize);
}

UTEST(AstraCobs, DecodeTruncatedBlock)
{
	uint8_t runsPastEnd[] = {0x05, 0x11, 0x22, 0x00};

	uint8_t cutMidStream[] = {0x03, 0x11, 0x22, 0x03, 0x33};

	uint8_t dst[8];

	size_t decodedSize = 0u;

	EXPECT_EQ(ASTRA_COBS_STATUS_ERROR,
	          AstraCobsDecode(runsPastEnd, sizeof(runsPastEnd), dst, sizeof(dst),
	                          &decodedSize));

	EXPECT_EQ(ASTRA_COBS_STATUS_ERROR,
	          AstraCobsDecode(cutMidStream, sizeof(cutMidStream), dst, sizeof(dst),
	                          &decodedSize));

	EXPECT_EQ(ASTRA_COBS_STATUS_ERROR,
	          AstraCobsDecode(NULL, 1u, dst, sizeof(dst), &decodedSize));
}

UTEST(AstraCobs, DecodeTruncatedFinalZeroRun)
{
	uint8_t src[] = {0x02, 0xAA, 0x00, 0x00};

	uint8_t dst[4] = {0xFF, 0xFF, 0xFF, 0xFF};

	size_t decodedSize = 123u;

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsDecode(src, sizeof(src), dst, sizeof(dst), &decodedSize));

	EXPECT_EQ((size_t)1u, decodedSize);
	EXPECT_EQ((uint8_t)0xAA, dst[0]);
	EXPECT_EQ((uint8_t)0xFF, dst[1]);

	uint8_t payload[] = {0xAA};

	uint8_t trailingZero[] = {0xAA, 0x00};

	uint8_t shortFrame[3];

	uint8_t longFrame[4];

	size_t shortSize = 0u;

	size_t longSize = 0u;

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsEncode(payload, sizeof(payload), shortFrame,
	                          sizeof(shortFrame), &shortSize));

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsEncode(trailingZero, sizeof(trailingZero), longFrame,
	                          sizeof(longFrame), &longSize));

	EXPECT_EQ(shortSize, longSize);
	EXPECT_EQ(shortFrame[0], longFrame[0]);
	EXPECT_EQ(shortFrame[1], longFrame[1]);
	EXPECT_EQ(shortFrame[2], longFrame[2]);
}

UTEST(AstraCobs, DecodeZeroAfterNonFullMaxBlock)
{
	uint8_t payload[256];

	uint8_t encoded[259];

	uint8_t decoded[256];

	size_t encodedSize = 0u;

	for (size_t i = 0u; i < 253u; ++i)
	{
		payload[i] = 0x01;
	}
	payload[253] = 0x00;
	payload[254] = 0x01;
	payload[255] = 0x02;

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsEncode(payload, sizeof(payload), encoded, sizeof(encoded),
	                          &encodedSize));

	EXPECT_EQ((size_t)258u, encodedSize);
	EXPECT_EQ((uint8_t)0xFE, encoded[0]);
	EXPECT_EQ((uint8_t)0x03, encoded[254]);

	size_t decodedSize = 0u;

	EXPECT_EQ(ASTRA_COBS_STATUS_OK,
	          AstraCobsDecode(encoded, encodedSize, decoded, sizeof(decoded),
	                          &decodedSize));

	EXPECT_EQ((size_t)256u, decodedSize);

	for (size_t i = 0u; i < sizeof(payload); ++i)
	{
		EXPECT_EQ(payload[i], decoded[i]);
	}
}

UTEST(AstraCobs, MaxEncodedSizeIsNeverShort)
{
	for (size_t length = 0u; length <= 6u; ++length)
	{
		uint8_t src[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		uint8_t dst[16];

		size_t encodedSize = 0u;

		EXPECT_GE(AstraCobsMaxEncodedSize(length), length + 1u);

		EXPECT_EQ(ASTRA_COBS_STATUS_OK,
		          AstraCobsEncode(src, length, dst, sizeof(dst), &encodedSize));

		EXPECT_LE(encodedSize, AstraCobsMaxEncodedSize(length));
	}
}
