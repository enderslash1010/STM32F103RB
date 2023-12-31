
#include "i2c_hal.h"

#define DELAY 16000

/*
 * Note: When the STOP, START or PEC bit is set, the software must not perform any write access
 * to I2C_CR1 before this bit is cleared by hardware. Otherwise there is a risk of setting a
 * second STOP, START or PEC request (make sure STOP/START/PEC is cleared before doing any writes to I2C_CR1)
 */

/*
 * Note: Reading I2C_SR2 after reading I2C_SR1 clears the ADDR flag, even if the ADDR flag was
 * set after reading I2C_SR1. Consequently, I2C_SR2 must be read only when ADDR is found
 * set in I2C_SR1 or when the STOPF bit is cleared
 */

// Master sends START condition
// Assumes I2C is in slave mode, switches to master mode upon sending START
static I2C_ERROR_CODE i2c_start(I2C_TypeDef* I2C)
{
	uint16_t timeout = 0;

	I2C->CR1 |= I2C_CR1_START; // Sends START condition when the bus is free
	// Then the master waits for a read of the SR1 register followed by a write in the DR register with the Slave address
	while (!(I2C->SR1 & I2C_SR1_SB)) if (timeout++ >= DELAY) return I2C_START_ERROR; // Wait for start condition to be generated
	return I2C_OK;
}

// Master sends address of device it wants to communicate with
// Master enters transmitter (LSB=0) or receiver mode (LSB=1)
static I2C_ERROR_CODE i2c_send_addr(I2C_TypeDef* I2C, uint8_t addr, uint8_t LSB)
{
	uint16_t timeout = 0;

	uint8_t msg = (addr << 1) | (LSB & 0x1);
	I2C->DR = msg; // Write msg to DR, sends addr on SDA
	while (!(I2C->SR1 & I2C_SR1_ADDR)) if (timeout++ >= DELAY) return I2C_ADDR_ERROR; // Wait for addr to be sent by hardware
	if (I2C->SR2); // Read SR2 to clear ADDR (only when ADDR is found to be set)
	return I2C_OK;
}

// Sends len bytes of data to previously called device
// Assumes device address is already sent
static I2C_ERROR_CODE i2c_send_data(I2C_TypeDef* I2C, uint8_t* data, uint16_t len)
{
	uint16_t timeout = 0;

	for (int i = 0; i < len; i++) // Send all len # of data bytes
	{
		while (!(I2C->SR1 & I2C_SR1_TXE)); // Wait for the transmission buffer (TX) to be empty
		I2C->DR = data[i]; // Write data to DR, sends data on SDA
	}
	while (!(I2C->SR1 & I2C_SR1_TXE) && !(I2C->SR1 & I2C_SR1_BTF)) if (timeout++ >= DELAY) return I2C_DATA_ERROR; // Wait for TX to be empty, and byte transfer to finish (BTF=1)
	return I2C_OK;
}

// Master sends STOP condition
static void i2c_stop(I2C_TypeDef* I2C)
{
	I2C->CR1 |= I2C_CR1_STOP; // Send STOP condition
}

// Sends a data byte through the I2C peripheral specified
I2C_ERROR_CODE i2c_write(I2C_TypeDef* I2C, uint8_t addr, uint8_t data)
{
	while (I2C->SR2 & I2C_SR2_BUSY); // Wait for I2C bus to be not busy
	if (i2c_start(I2C) != I2C_OK)
	{
		i2c_stop(I2C);
		return I2C_START_ERROR;
	}
	if (i2c_send_addr(I2C, addr, 0) != I2C_OK)
	{
		i2c_stop(I2C);
		return I2C_ADDR_ERROR;
	}
	if (i2c_send_data(I2C, &data, 1) != I2C_OK)
	{
		i2c_stop(I2C);
		return I2C_DATA_ERROR;
	}
	i2c_stop(I2C);
	return I2C_OK;
}

void i2c_init(I2C_TypeDef* I2C, uint8_t isI2C1)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enable GPIOB Clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable AFIO Clock

	if (isI2C1)
	{
		// Configure GPIO for I2C1
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // Enable I2C1 Clock
		// Uses default AFIO mapping for I2C1 (PB6->SCL, PB7->SDA), no need for remapping
		I2C->CR1 &= ~(I2C_CR1_PE);
		GPIOB->CRL |= (0b11 << 24) | (0b11 << 28); // Set PB6 and PB7 to output mode
		GPIOB->ODR |= (1 << 6) | (1 << 7); // From errata, set ODR to 1
		while (!(((GPIOB->IDR & (1 << 6))) && (GPIOB->IDR & (1 << 7)))); // From errata, check IDR for PB6 and PB7 are high
		GPIOB->ODR &= ~(1 << 7); // From errata, set SCL ODR to 0
		while (GPIOB->IDR & (1 << 7)); // From errata, check if SCL is 0
		GPIOB->ODR &= ~(1 << 6); // From errata, set SDA to 0
		while (GPIOB->IDR & (1 << 6)); // From errata, check if SDA is 0
		GPIOB->ODR |= (1 << 7); // From errata, set SCL ODR to 1
		while (!(GPIOB->IDR & (1 << 7))); // From errata, check if SCL is 1
		GPIOB->ODR |= (1 << 6); // From errata, set SDA ODR to 1
		while (!(GPIOB->IDR & (1 << 6))); // From errata, check if SDA is 1

		GPIOB->CRL |= (0b11 << 26) | (0b11 << 30); // Set PB6 and PB7 to alternate function output open-drain
	}
	else // I2C2
	{
		// Configure GPIO for I2C2
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; // Enable I2C2 Clock
		// Uses default AFIO mapping for I2C2 (PB10->SCL, PB11->SDA), no need for remapping
		I2C->CR1 &= ~(I2C_CR1_PE); // Make sure I2C peripheral is disabled
		GPIOB->CRH |= (0b11 << 8) | (0b11 << 12); // Set PB10 and PB11 to output mode
		GPIOB->ODR |= (1 << 10) | (1 << 11); // From errata, set ODR to 1
		while (!(((GPIOB->IDR & (1 << 10))) && (GPIOB->IDR & (1 << 11)))); // From errata, check IDR for PB10 and PB11 are high
		GPIOB->ODR &= ~(1 << 10); // From errata, set SCL ODR to 0
		while (GPIOB->IDR & (1 << 10)); // From errata, check if SCL is 0
		GPIOB->ODR &= ~(1 << 11); // From errata, set SDA to 0
		while (GPIOB->IDR & (1 << 11)); // From errata, check if SDA is 0
		GPIOB->ODR |= (1 << 10); // From errata, set SCL ODR to 1
		while (!(GPIOB->IDR & (1 << 10))); // From errata, check if SCL is 1
		GPIOB->ODR |= (1 << 11); // From errata, set SDA ODR to 1
		while (!(GPIOB->IDR & (1 << 11))); // From errata, check if SDA is 1

		GPIOB->CRH |= (0b11 << 10) | (0b11 << 14); // Set PB10 and PB11 to alternate function output open-drain
	}

	// Reset I2C
	I2C->CR1 |= I2C_CR1_SWRST;
	I2C->CR1 &= ~(I2C_CR1_SWRST);

	I2C->OAR1 |= (1 << 14); // I2C_OAR1 bit 14 "Should always be kept at 1 by software"

	/* The I2C clock should be set to standard mode (100kHz), so the period of SCL should be 10us.
	 * T(SCL) = T(high) + T(low) -> 10us = 2*T(high), using a duty cycle of 1:1 (T(high)=T(low)) -> T(high) = 5us
	 * The datasheet provides a formula for CCR: T(high) = CCR * T(PCLK1). We have PCLK1 = 36MHz, so T(PCLK1) = 0.0278us
	 * Because we know T(high), we can solve for CCR: 5us = CCR * 0.0278us -> CCR = 180 (0xB4)
	 */
	I2C->CR2 |= 0x24; // Program the peripheral input clock in I2C_CR2 register to 36 MHz in order to generate correct timings
	I2C->CCR |= 0xB4; // Configure the clock control register
	I2C->TRISE = 0x25; // Configure the rise time register; The datasheet gives formula (maximum SCL rise time)/(T(PCLK1)) + 1 -> 1000ns/27.78ns + 1 = 37
	I2C->CR1 |= I2C_CR1_PE; // Program the I2C_CR1 register to enable the peripheral

	if (I2C->SR2 & I2C_SR2_BUSY) i2c_stop(I2C);
}
