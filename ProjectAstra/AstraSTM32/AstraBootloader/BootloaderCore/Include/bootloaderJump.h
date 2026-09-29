#pragma once

#include <stdbool.h>

typedef enum
{
   APPLICATION_VALID,
	APPLICATION_ERR_MAGIC,
	APPLICATION_ERR_RESET_HANDLER,
	APPLICATION_ERR_STACK_POINTER,
} ApplicationStatus_t;


typedef void (*pFunction)(void);

void JumpToApplication(void);

ApplicationStatus_t IsApplicationValid(void);
