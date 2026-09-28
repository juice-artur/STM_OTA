#pragma once

#include <stdbool.h>

typedef void (*pFunction)(void);

/* Hands control over to the application.  Never returns; only call it after
   IsApplicationValid() returned true. */
void JumpToApplication(void);
