
#include "pcd8544.h"

// 5x7 font
uint8_t DEFAULT_FONT[][5] = {
		{0x0, 0x0, 0x0, 0x0, 0x0}, // Space
		{0x0, 0x0, 0x4F, 0x0, 0x0}, // !
		{0x0, 0x7, 0x0, 0x7, 0x0}, // "
		{0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
		{0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
		{0x23, 0x13, 0x8, 0x64, 0x62}, // %
		{0x36, 0x49, 0x55, 0x22, 0x50}, // &
		{0x0, 0x5, 0x3, 0x0, 0x0}, // '
		{0x0, 0x1C, 0x22, 0x41, 0x0}, // (
		{0x0, 0x41, 0x22, 0x1C, 0x0}, // )
		{0x14, 0x8, 0x3E, 0x8, 0x14}, // *
		{0x8, 0x8, 0x3E, 0x8, 0x8}, // +
		{0x0, 0x50, 0x30, 0x0, 0x0}, // ,
		{0x8, 0x8, 0x8, 0x8, 0x8}, // -
		{0x0, 0x60, 0x60, 0x0, 0x0}, // .
		{0x20, 0x10, 0x8, 0x4, 0x2}, // /

		{0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
		{0x0, 0x42, 0x7F, 0x40, 0x0}, // 1
		{0x42, 0x61, 0x51, 0x49, 0x46}, // 2
		{0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
		{0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
		{0x27, 0x45, 0x45, 0x45, 0x39}, // 5
		{0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
		{0x3, 0x1, 0x71, 0x9, 0x7}, // 7
		{0x36, 0x49, 0x49, 0x49, 0x36}, // 8
		{0x6, 0x49, 0x49, 0x29, 0x1E}, // 9
		{0x0, 0x36, 0x36, 0x0, 0x0}, // :
		{0x0, 0x56, 0x36, 0x0, 0x0}, // ;
		{0x8, 0x14, 0x22, 0x41, 0x0}, // <
		{0x14, 0x14, 0x14, 0x14, 0x14}, // =
		{0x0, 0x41, 0x22, 0x14, 0x8}, // >
		{0x2, 0x1, 0x51, 0x9, 0x6}, // ?

		{0x32, 0x49, 0x79, 0x41, 0x3E}, // @
		{0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
		{0x7F, 0x49, 0x49, 0x49, 0x36}, // B
		{0x3E, 0x41, 0x41, 0x41, 0x22}, // C
		{0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
		{0x7F, 0x49, 0x49, 0x49, 0x41}, // E
		{0x7F, 0x9, 0x9, 0x9, 0x1}, // F
		{0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
		{0x7F, 0x8, 0x8, 0x8, 0x7F}, // H
		{0x0, 0x41, 0x7F, 0x41, 0x0}, // I
		{0x20, 0x40, 0x41, 0x3F, 0x1}, // J
		{0x7F, 0x8, 0x14, 0x22, 0x41}, // K
		{0x7F, 0x40, 0x40, 0x40, 0x40}, // L
		{0x7F, 0x2, 0xC, 0x2, 0x7F}, // M
		{0x7F, 0x4, 0x8, 0x20, 0x7F}, // N
		{0x3E, 0x41, 0x41, 0x41, 0x3E}, // O

		{0x7F, 0x9, 0x9, 0x9, 0x6}, // P
		{0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
		{0x7F, 0x9, 0x19, 0x29, 0x46}, // R
		{0x46, 0x49, 0x49, 0x49, 0x31}, // S
		{0x1, 0x1, 0x7F, 0x1, 0x1}, // T
		{0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
		{0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
		{0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
		{0x63, 0x14, 0x8, 0x14, 0x63}, // X
		{0x7, 0x8, 0x70, 0x8, 0x7}, // Y
		{0x61, 0x51, 0x49, 0x45, 0x43}, // Z
		{0x7F, 0x41, 0x41, 0x0, 0x0}, // [
		{0x15, 0x16, 0x7C, 0x16, 0x15}, // ¥
		{0x0, 0x41, 0x41, 0x7F, 0x0}, // ]
		{0x4, 0x2, 0x1, 0x2, 0x4}, // ^
		{0x40, 0x40, 0x40, 0x40, 0x40}, // _

		{0x0, 0x1, 0x2, 0x4, 0x0}, // `
		{0x20, 0x54, 0x54, 0x54, 0x78}, // a
		{0x7F, 0x48, 0x44, 0x44, 0x38}, // b
		{0x38, 0x44, 0x44, 0x44, 0x20}, // c
		{0x38, 0x44, 0x44, 0x48, 0x7F}, // d
		{0x38, 0x54, 0x54, 0x54, 0x18}, // e
		{0x8, 0x7E, 0x9, 0x1, 0x2}, // f
		{0xC, 0x52, 0x52, 0x52, 0x3E}, // g
		{0x7F, 0x8, 0x4, 0x4, 0x78}, // h
		{0x0, 0x44, 0x7D, 0x40, 0x0}, // i
		{0x20, 0x40, 0x44, 0x3D, 0x0}, // j
		{0x7F, 0x10, 0x28, 0x44, 0x0}, // k
		{0x0, 0x41, 0x7F, 0x40, 0x0}, // l
		{0x7C, 0x4, 0x18, 0x4, 0x78}, // m
		{0x7C, 0x8, 0x4, 0x4, 0x78}, // n
		{0x38, 0x44, 0x44, 0x44, 0x38}, // o

		{0x7C, 0x14, 0x14, 0x14, 0x8}, // p
		{0x8, 0x14, 0x14, 0x18, 0x7C}, // q
		{0x7C, 0x8, 0x4, 0x4, 0x8}, // r
		{0x48, 0x54, 0x54, 0x54, 0x20}, // s
		{0x4, 0x3F, 0x44, 0x40, 0x20}, // t
		{0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
		{0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
		{0x3C, 0x40, 0x38, 0x40, 0x3C}, // w
		{0x44, 0x28, 0x10, 0x28, 0x44}, // x
		{0xC, 0x50, 0x50, 0x50, 0x3C}, // y
		{0x44, 0x64, 0x54, 0x4C, 0x44}, // z
		{0x0, 0x8, 0x36, 0x41, 0x0}, // {
		{0x0, 0x0, 0x7F, 0x0, 0x0}, // |
		{0x0, 0x41, 0x36, 0x8, 0x0}, // }
		{0x8, 0x8, 0x2A, 0x1C, 0x8}, // ->
		{0x8, 0x1C, 0x2A, 0x8, 0x8}, // <-

		{0x7, 0x5, 0x7, 0x0, 0x0} // Degree Sign (0x80)
};

inline static void config_gpio(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t mode, uint8_t cnf)
{
	if (pin < 8) // CRL
	{
		GPIOx->CRL &= ~((0b11 << (pin * 4)) | (0b11 << ((pin * 4) + 2))); // Clear MODE(pin) and CNF(pin)
		GPIOx->CRL |= (((mode & 0b11) << (pin * 4)) | ((cnf & 0b11) << ((pin * 4) + 2))); // Set MODE(pin) and CNF(pin)
	}
	else // CRH
	{
		GPIOx->CRH &= ~((0b11 << ((pin - 8) * 4)) | (0b11 << (((pin - 8) * 4) + 2))); // Clear MODE(pin) and CNF(pin)
		GPIOx->CRH |= (((mode & 0b11) << ((pin - 8) * 4)) | ((cnf & 0b11) << (((pin - 8) * 4) + 2))); // Set MODE(pin) and CNF(pin)
	}
}

PCD8544_t pcd8544_init(GPIO_TypeDef* GPIOx, uint8_t RST_Pin, uint8_t DC_Pin, uint8_t BL_Pin, uint8_t Vcc_Pin, uint8_t Vop, uint8_t TC, uint8_t BS)
{
	spi1_init(); // Initiate SPI

	// Enable GPIOx Clock
	switch ((int)GPIOx)
	{
		case (int)GPIOA:
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
			break;
		case (int)GPIOB:
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
			break;
		case (int)GPIOC:
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			break;
		case (int)GPIOD:
			RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
			break;
		case (int)GPIOE:
			RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
			break;
	}

	PCD8544_t screen;
	screen.GPIOx = GPIOx;
	screen.RST_Pin = RST_Pin;
	screen.DC_Pin = DC_Pin;
	screen.BL_Pin = BL_Pin;

	config_gpio(GPIOx, RST_Pin, 0b11, 0b00); // Configure RST: Mode 0b11, CNF 0b00
	config_gpio(GPIOx, DC_Pin, 0b11, 0b00); // Configure DC: Mode 0b11, CNF 0b00
	config_gpio(GPIOx, BL_Pin, 0b11, 0b00); // Configure BL: Mode 0b11, CNF 0b00
	config_gpio(GPIOx, Vcc_Pin, 0b11, 0b00); // Configure Vcc: Mode 0b11, CNF 0b00

	// Set RST to high
	GPIOx->ODR |= (1 << RST_Pin);

	GPIOx->ODR |= (1 << Vcc_Pin); // Set Vcc to high
	delay_ms(10);
	GPIOx->ODR &= ~(1 << RST_Pin); // Pull RST low within 30ms of power on
	delay_ms(10);
	GPIOx->ODR |= (1 << RST_Pin); // Pull RST back high to start initialization
	delay_ms(10);

	// Initialization
	pcd8544_function_set(&screen, 0, 0, 1);
	pcd8544_set_Vop(&screen, Vop);
	pcd8544_set_temperature_control(&screen, TC);
	pcd8544_set_bias(&screen, BS);
	pcd8544_function_set(&screen, 0, 0, 0);
	pcd8544_set_display_control(&screen, 1, 0);

	return screen;
}

static void send_spi(PCD8544_t* screen, uint8_t data, uint8_t DC)
{
	// Set DC
	if (DC & 0x1) screen->GPIOx->ODR |= (1 << 9);
	else screen->GPIOx->ODR &= ~(1 << 9);

	spi_write(SPI1, data, screen->GPIOx);
}

// H=0 or H=1

void pcd8544_function_set(PCD8544_t* screen, uint8_t PD, uint8_t V, uint8_t H)
{
	uint8_t instruction = 0b00100000 | ((PD & 0x1) << 2) | ((V & 0x1) << 1) | (H & 0x1);
	send_spi(screen, instruction, 0);
}

void pcd8544_write_data(PCD8544_t* screen, uint8_t data)
{
	send_spi(screen, data, 1);
}

// H=0

void pcd8544_set_display_control(PCD8544_t* screen, uint8_t D, uint8_t E)
{
	uint8_t instruction = 0b00001000 | ((D & 0x1) << 2) | (E & 0x1);
	send_spi(screen, instruction, 0);
}

void pcd8544_set_X_RAM(PCD8544_t* screen, uint8_t X)
{
	uint8_t instruction = 0b10000000 | ((X & 0x7F) % 84); // X: [0, 83]
	send_spi(screen, instruction, 0);
}

void pcd8544_set_Y_RAM(PCD8544_t* screen, uint8_t Y)
{
	uint8_t instruction = 0b01000000 | ((Y & 0b111) % 6); // Y: [0, 5]
	send_spi(screen, instruction, 0);
}

// H=1

void pcd8544_set_temperature_control(PCD8544_t* screen, uint8_t TC)
{
	uint8_t instruction = 0b00000100 | (TC & 0b11);
	send_spi(screen, instruction, 0);
}

void pcd8544_set_bias(PCD8544_t* screen, uint8_t BS)
{
	uint8_t instruction = 0b00010000 | (BS & 0b111);
	send_spi(screen, instruction, 0);
}

void pcd8544_set_Vop(PCD8544_t* screen, uint8_t Vop)
{
	uint8_t instruction = 0b10000000 | (Vop & 0x7F);
	send_spi(screen, instruction, 0);
}

// Misc. Control Functions

void pcd8544_toggle_backlight(PCD8544_t* screen)
{
	screen->GPIOx->ODR ^= (1 << screen->BL_Pin);
}

void pcd8544_write_string(PCD8544_t* screen, const char* str)
{
	char letter = *str;
	while (letter != 0x0)
	{
		unsigned int letterUInt = letter - 0x20; // Translate char at index i to index in font array; handles degree character with cast to unsigned int
		for (int j = 0; j < 5; j++)
		{
			pcd8544_write_data(screen, DEFAULT_FONT[letterUInt][j]); // write each column of letter to screen, according to font
		}
		pcd8544_write_data(screen, 0); // Put blank column after each letter, to space out each letter

		letter = *(++str);
	}

}

void pcd8544_set_cursor(PCD8544_t* screen, uint8_t X, uint8_t Y)
{
	pcd8544_set_X_RAM(screen, X);
	pcd8544_set_Y_RAM(screen, Y);
}

void pcd8544_write_bitmap(PCD8544_t* screen, const uint8_t img[504])
{
	pcd8544_set_cursor(screen, 0, 0); // Sets cursor at top left corner (0, 0)
	for (int i = 0; i < 504; i++) pcd8544_write_data(screen, img[i]); // Sends pixel data to screen, which auto-increments to next RAM address
	pcd8544_set_cursor(screen, 0, 0); // Sets cursor at top left corner (0, 0); Sometimes the screen contrast gets weird after sending the image, and this fixes it
}
