#include "bootloaderJump.h"

#include <stdint.h>

#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"
#include "FlashLayout.h"

void JumpToApplication(void)
{
	uint32_t const appStack = *(volatile uint32_t const *)APP_VECTOR_TABLE_ADDR;
	uint32_t const appResetHandler =
	 *(volatile uint32_t const *)(APP_VECTOR_TABLE_ADDR + 4U);
	pFunction const appEntry = (pFunction)appResetHandler;

	HAL_RCC_DeInit();
	HAL_DeInit();

	__disable_irq();

	/* Stop SysTick */
	SysTick->CTRL = 0U;
	SysTick->LOAD = 0U;
	SysTick->VAL = 0U;

	for (uint32_t i = 0U; i < 8U; i++)
	{
		NVIC->ICER[i] = 0xFFFFFFFFU; // Disable interrupts
		NVIC->ICPR[i] = 0xFFFFFFFFU; // Clear pending interrupts
	}

	SCB->VTOR = APP_VECTOR_TABLE_ADDR;
	__DSB();
	__ISB();

	__set_MSP(appStack);
	__set_CONTROL(0U);
	__ISB();

	appEntry();

	/* The application reset handler must never return. */
	while (true)
	{
	}
}
