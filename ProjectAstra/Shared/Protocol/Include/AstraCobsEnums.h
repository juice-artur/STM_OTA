#pragma once

#define ASTRA_COBS_DELIMITER 0x00u
#define ASTRA_COBS_MAX_BLOCK_PAYLOAD 254u

typedef enum AstraCobsStatus
{
	ASTRA_COBS_STATUS_OK = 0,
	ASTRA_COBS_STATUS_ERROR = -1
} AstraCobsStatus;