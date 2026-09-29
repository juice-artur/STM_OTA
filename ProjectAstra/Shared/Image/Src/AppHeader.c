#include "AppHeader.h"

__attribute__((section(".header"), used)) const AppHeader_t app_header = {
    .magic   = ASTRA_MAGIC_VALUE,
    .size    = 0U, 
    .crc     = 0U, 
    .version = 0U,
};

const AppHeader_t* GetAppHeader(void)
{
    return &app_header;
}