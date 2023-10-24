
#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include "i2c_hal.h"
#include "delay.h"
#include "main.h"

#define BL_Pos 3
#define BL (1 << BL_Pos)

#define EN_Pos 2
#define EN (1 << EN_Pos)

#define RW_Pos 1
#define RW (1 << RW_Pos)

#define RS_Pos 0
#define RS (1 << RS_Pos)

typedef struct LCD_t
{
	I2C_TypeDef* I2C;
	uint8_t addr;
	uint8_t isBacklightOn;
} LCD_t;

// Initialization Function
LCD_t lcd_init(I2C_TypeDef* I2C, uint8_t addr);

// Software-Implemented Hardware Command Functions
void lcd_clear_display(LCD_t* lcd);
void lcd_return_home(LCD_t* lcd);
void lcd_entry_mode_set(LCD_t* lcd, uint8_t ID, uint8_t S);
void lcd_display_control(LCD_t* lcd, uint8_t D, uint8_t C, uint8_t B);
void lcd_cursor_display_shift(LCD_t* lcd, uint8_t SC, uint8_t RL);
void lcd_function_set(LCD_t* lcd, uint8_t DL, uint8_t N, uint8_t F);
void lcd_set_CGRAM(LCD_t* lcd, uint8_t ACG);
void lcd_set_DDRAM(LCD_t* lcd, uint8_t ADD);
void lcd_write(LCD_t* lcd, uint8_t data);

// Misc. Control Functions
void lcd_set_backlight(LCD_t* lcd, uint8_t isOn);
void lcd_write_string(LCD_t* lcd, char* str);
void lcd_add_character(LCD_t* lcd, uint8_t location, const uint8_t charMap[]);

#endif /* LCD_H_ */
