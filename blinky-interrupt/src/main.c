
#include "main.h"

#define DELAY 123456UL

void reset_led(void) { GPIOA->BSRR = (1 << 21); } // Reset GPIOA to 0 (LED off)
void set_led(void) { GPIOA->BSRR = (1 << 5); } // Set GPIOA5 to 1 (LED on)
void toggle_led(void) { GPIOA->ODR ^= (1 << 5); } // Toggle GPIOA5

// flag to signal to hold the LED on, triggers when button is pressed
// Since it's value is modified in an IRQ, it must be volatile, so the compiler doesn't optimize it away
volatile uint8_t isHold = 0;

// IRQ for EXTI10-EXTI15, this program uses EXTI13
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR13) // check if interrupt was from EXTI13
	{
		// handle button press
		set_led(); // Turn LED On
		isHold = 1; // set isHold flag, to tell main loop to hold the LED on for 5x DELAY
		EXTI->PR = EXTI_PR_PR13; // clear interrupt flag in EXTI_PR by writing a 1 into PR13
	}
}

void init(void)
{
	// Set-up port A for LED (PA5)
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable clock on port A
	GPIOA->CRL &= ~(0b11 << 22); // Configure GPIOA5 to output push-pull (00) (by clearing CNF5, it's initial value is 01)
	GPIOA->CRL |= (0b11 << 20); // Configure GPIOA5 to output mode max speed 50 MHz (11)

	// Set-up port C for button (PC13), with pull-up
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Enable clock on port C

	GPIOC->CRH &= ~(0b11 << 22); // Clear CNF13
	GPIOC->CRH |= (0b10 << 22); // Set CNF13 to input with pull-up/pull down
	// GPIOC13 is already in input mode upon reset, no need to set MODE13
	GPIOC->ODR |= (1 << 13); // Select pull-up by setting ODR for GPIOC13 to 1

	// Configure EXTI13 for external interrupts from button
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable AFIO clock
	AFIO->EXTICR[3] |= (0b0010 << 4); // Configure AFIO_EXTICR4; Associate EXTI13 with PC13
	EXTI->IMR |= EXTI_IMR_MR13; // Set interrupt mask register EXTI_IMR
	EXTI->FTSR |= EXTI_FTSR_TR13; // Configure interrupt on falling-trigger (when the button goes from un-pressed->pressed)
	NVIC->ISER[1] = (1 << 8); // Enable EXTI13 in NVIC; EXTI13 is grouped in EXTI15_10_IRQn which corresponds to interrupt 40, so the enable bit is in ISER[1] at bit 8
}

// Blinks on-board LED LD2, connected through PA5
int main(void)
{
	init();
	uint32_t counter = 0;

    while (1)
    {
    	if (isHold) // Enters this when button is pressed (isHold is set in IRQ)
    	{
    		counter = 0;
    		while (counter++ < DELAY*5); // wait 5x DELAY
    		isHold = 0;
    		counter = 0;
    		reset_led(); // turn LED off
    	}
    	if (counter++ == DELAY)
    	{
    		toggle_led(); // toggle LED
    		counter = 0; // reset counter
    	}
    }
}
