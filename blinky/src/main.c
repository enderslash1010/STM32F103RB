
#include "main.h"

#define DELAY 123456UL

void reset_led(void) { GPIOA->BSRR = (1 << 21); } // Reset GPIOA to 0 (LED off)
void set_led(void) { GPIOA->BSRR = (1 << 5); } // Set GPIOA5 to 1 (LED on)
void toggle_led(void) { GPIOA->ODR ^= (1 << 5); } // Toggle GPIOA5


void init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable clock on port A
	GPIOA->CRL &= ~(0b11 << 22); // Configure GPIOA5 to output push-pull (00) (by clearing CNF5, it's initial value is 01)
	GPIOA->CRL |= (0b11 << 20); // Configure GPIOA5 to output mode max speed 50 MHz (11)
}

// Blinks on-board LED LD2, connected through PA5
int main(void)
{
	init();
	uint32_t counter = 0;

    while (1)
    {
    	if (counter++ == DELAY)
    	{
    		toggle_led();
    		counter = 0;
    	}
    }
}
