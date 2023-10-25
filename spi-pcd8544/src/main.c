
#include "main.h"
#include "pcd8544.h"

#define RST_PIN 8
#define DC_PIN 9
#define BL_PIN 10
#define Vcc_PIN 11

// Initiates clock to 72 MHz
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

/*
 * PA8: RST
 * PA9: DC
 * PA10: BL
 * PA11: Vcc
 * PA4: NSS/CE
 * PA5: SCK
 * PA7: MOSI
 */
int main(void)
{
	clock_init();
	delay_init();
	PCD8544_t screen = pcd8544_init(GPIOA, RST_PIN, DC_PIN, BL_PIN, Vcc_PIN, 0x70, 0, 0b011);
	pcd8544_toggle_backlight(&screen);

	pcd8544_write_data(&screen, 0b00001100);
	pcd8544_write_data(&screen, 0b00011110);
	pcd8544_write_data(&screen, 0b00111100);
	pcd8544_write_data(&screen, 0b00011110);
	pcd8544_write_data(&screen, 0b00001100);
	pcd8544_write_data(&screen, 0);

	pcd8544_write_string(&screen, "Hello World!");

	pcd8544_write_data(&screen, 0b00001100);
	pcd8544_write_data(&screen, 0b00011110);
	pcd8544_write_data(&screen, 0b00111100);
	pcd8544_write_data(&screen, 0b00011110);
	pcd8544_write_data(&screen, 0b00001100);

    while (1);
}
