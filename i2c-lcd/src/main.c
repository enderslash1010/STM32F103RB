
#include "main.h"

void clock_init(void)
{
	FLASH->ACR |= FLASH_ACR_LATENCY_1 | FLASH_ACR_PRFTBE; // Sets up flash latency for 48 MHz < SYSCLK < 72 MHz, and enables FLASH prefetch
	RCC->CFGR |= RCC_CFGR_PPRE1_2; // HCLK divided by 2 for APB1
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE; // HSE is not divided for PLL (this line doesn't do anything)

	RCC->CR |= RCC_CR_HSEON; // Switch on HSE oscillator
	while (!(RCC->CR & RCC_CR_HSERDY)); // Wait for HSE to be ready

	RCC->CFGR |= RCC_CFGR_PLLSRC; // Set HSE as source clock for PLL
	RCC->CFGR |= RCC_CFGR_PLLMULL9; // Multiply PLL by 9
	RCC->CR |= RCC_CR_PLLON; // Turn on PLL
	while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait for PLL to be ready

	RCC->CFGR |= RCC_CFGR_SW_PLL; // Set PLL as main clock source
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)); // Wait for clock source to be set
}

int main(void)
{
	clock_init();
	delay_init(72000000);

	const uint8_t heart[] = {
			0b00000,
			0b01010,
			0b11111,
			0b11111,
			0b01110,
			0b00100,
			0b00000,
	};

	LCD_t lcd = lcd_init(I2C1, 0b0111111);
	lcd_add_character(&lcd, 0, heart); // Add heart characer to the lcd's CGRAM slot 0

	lcd_write_string(&lcd, "Hello World!");

	lcd_set_DDRAM(&lcd, 0x40); // Go to 2nd line

	// Write heart character to lcd
	lcd_write(&lcd, 0);
	lcd_write(&lcd, 0);
	lcd_write(&lcd, 0);

    while (1);
}
