
#include "lcd.h"

static void send_i2c_nibble(LCD_t* lcd, uint8_t data, uint8_t rs, uint8_t rw)
{
	uint8_t msg = ((data << 4) & 0xF0) | (lcd->isBacklightOn << BL_Pos) | (rw << RW_Pos) | (rs << RS_Pos); // Compose the 8 bit data
	//i2c_write(lcd->I2C, lcd->addr, msg & ~(EN)); // Send data with enable bit reset
	i2c_write(lcd->I2C, lcd->addr, msg | EN); // Send data with enable bit set
	//delay_ms(1); // Wait for at least 450ns < 1ms
	i2c_write(lcd->I2C, lcd->addr, msg & ~(EN)); // Send data with enable bit reset
	//delay_ms(1);
}

static void send_i2c_byte(LCD_t* lcd, uint8_t data, uint8_t rs, uint8_t rw)
{
	/* First I2C transmission
	 * +-----+-----+-----+-----+----+----+-----+----+
	 * | DB7 | DB6 | DB5 | DB4 | BL | EN | R/W | RS |
	 * +-----+-----+-----+-----+----+----+-----+----+
	 * |   7 |   6 |   5 |   4 |  3 |  2 |   1 |  0 |
	 * +-----+-----+-----+-----+----+----+-----+----+
	 */

	/* Second I2C transmission
	 * +-----+-----+-----+-----+----+----+-----+----+
	 * | DB3 | DB2 | DB1 | DB0 | BL | EN | R/W | RS |
	 * +-----+-----+-----+-----+----+----+-----+----+
	 * |   7 |   6 |   5 |   4 |  3 |  2 |   1 |  0 |
	 * +-----+-----+-----+-----+----+----+-----+----+
	 */

	// Send most significant bits
	send_i2c_nibble(lcd, (data >> 4), rs, rw);

	// Send least significant bits
	send_i2c_nibble(lcd, data, rs, rw);
}

LCD_t lcd_init(I2C_TypeDef* I2C, uint8_t addr)
{
	LCD_t lcd;
	lcd.I2C = I2C;
	lcd.addr = addr;
	lcd.isBacklightOn = 0; // Keep backlight off until initialization is done

	// I2C Initialization
	i2c_init(I2C1, 1);

	// LCD Initialization
	delay_ms(100); // Wait more than 15ms after Vcc rises to 4.5V
	i2c_write(lcd.I2C, lcd.addr, 0); // Pull RS and R/W low to begin sending commands
	delay_ms(1000);

	send_i2c_nibble(&lcd, 0b0011, 0, 0); // Special function set
	delay_ms(5); // Wait for more than 4.1ms
	send_i2c_nibble(&lcd, 0b0011, 0, 0); // Special function set a 2nd time
	delay_ms(1); // Wait for more than 100us
	send_i2c_nibble(&lcd, 0b0011, 0, 0); // Special function set a 3rd time
	delay_ms(1); // Wait for more than 100us

	send_i2c_nibble(&lcd, 0b0010, 0, 0); // Initial function set to 4 bit
	delay_ms(1); // Wait for more than 100us

	// Now in 4-bit mode, can send 'regular' 4 bit commands
	lcd_function_set(&lcd, 0, 1, 0); // Regular function set
	lcd_display_control(&lcd, 0, 0, 0); // Display OFF (D=0, C=0, B=0)
	lcd_clear_display(&lcd); // Clear display
	lcd_entry_mode_set(&lcd, 1, 0); // Entry mode set (I/D=1 , S=0)

	lcd.isBacklightOn = 1; // Turn Backlight on, will update with the display on command sent next
	lcd_display_control(&lcd, 1, 1, 1); // Display On
	return lcd;
}

void lcd_clear_display(LCD_t* lcd)
{
	send_i2c_byte(lcd, 0b00000001, 0, 0);
	delay_ms(3);
}

void lcd_return_home(LCD_t* lcd)
{
	send_i2c_byte(lcd, 0b00000010, 0, 0);
	delay_ms(3);
}

void lcd_entry_mode_set(LCD_t* lcd, uint8_t ID, uint8_t S)
{
	send_i2c_byte(lcd, 0b00000100 | ((ID & 0x1) << 1) | ((S & 0x1) << 0), 0, 0);
	delay_ms(1);
}

void lcd_display_control(LCD_t* lcd, uint8_t D, uint8_t C, uint8_t B)
{
	send_i2c_byte(lcd, 0b00001000 | ((D & 0x1) << 2) | ((C & 0x1) << 1) | ((B & 0x1) << 0), 0, 0);
	delay_ms(1);
}

void lcd_cursor_display_shift(LCD_t* lcd, uint8_t SC, uint8_t RL)
{
	send_i2c_byte(lcd, 0b00010000 | ((SC & 0x1) << 3) | ((RL & 0x1) << 2), 0, 0);
	delay_ms(1);
}

void lcd_function_set(LCD_t* lcd, uint8_t DL, uint8_t N, uint8_t F)
{
	send_i2c_byte(lcd, 0b00100000 | ((DL & 0x1) << 4) | ((N & 0x1) << 3) | ((F & 0x1) << 2), 0, 0);
	delay_ms(1);
}

void lcd_set_CGRAM(LCD_t* lcd, uint8_t ACG)
{
	send_i2c_byte(lcd, 0b01000000 | (ACG & 0b111111), 0, 0);
	delay_ms(1);
}

void lcd_set_DDRAM(LCD_t* lcd, uint8_t ADD)
{
	send_i2c_byte(lcd, 0b10000000 | (ADD & 0b1111111), 0, 0);
	delay_ms(1);
}

void lcd_write(LCD_t* lcd, uint8_t data)
{
	send_i2c_byte(lcd, data, 1, 0);
	delay_ms(1);
}

void lcd_set_backlight(LCD_t* lcd, uint8_t isOn)
{
	lcd->isBacklightOn = isOn & 0x1;
	i2c_write(lcd->I2C, lcd->addr, (lcd->isBacklightOn << BL_Pos));
}

void lcd_write_string(LCD_t* lcd, char* str)
{
	char letter = *str;
	while (letter != 0x0) // Keep sending letters until the null terminator (0x0)
	{
		lcd_write(lcd, letter);
		letter = *(++str);
	}
}

void lcd_add_character(LCD_t* lcd, uint8_t location, const uint8_t charMap[])
{
	lcd_set_CGRAM(lcd, location << 3);
	for (int i = 0; i < 8; i++) lcd_write(lcd, charMap[i]);
	lcd_set_DDRAM(lcd, 0); // Go back to using DDRAM when writing to lcd
}
