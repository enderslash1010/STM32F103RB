# STM32F103RB
Repo for bare-metal programming with STM32 (No use of the STM32 HAL), using the STM32F103RB (Nucleo) board

## Blinky
Classic blinky. Blinks the on-board LED (LD2) on and off.

## Blinky with Interrupt
Building upon blinky, the on-board LED is held on for a recognizable amount of time, around 2.5x blinks of time, when the on-board button (B1) is pressed. If the button is pressed while in this hold state, it does not add to the amount of time the LED is on. After exiting the hold state, the LED resumes blinking on and off.

## SPI and PCD8544
Controls a Nokia 5110 screen by sending commands to it using the SPI communication protocol. This particular program displays "Hello World!" with hearts on both sides of the text. The implemented library for the screen includes support for toggling the backlight, writing strings, and drawing an image (48x84 size bitmap).
