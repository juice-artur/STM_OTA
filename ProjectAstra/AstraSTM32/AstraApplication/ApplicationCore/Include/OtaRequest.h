#pragma once

#include "stm32g4xx_hal_flash.h"

void EnableOtaRequest()
{
    HAL_FLASH_Unlock();


}