#include "main.h"
#include "i2c_lcd.h"  

#include <string.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

void i2c_lcd_main(void)
{
	uint8_t value=0;

	while(1){
		move_cursor(0,0);
		lcd_string("Hello World!!!");
		move_cursor(1,0);
		lcd_data(value + '0');
		value++;
		if(value>9)value=0;
		HAL_Delay(500);
	}
}

void lcd_command(uint8_t command)
{
	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];
	high_nibble = command & 0xf0;											//상위4bit저장
	low_nibble = (command<<4) & 0xf0;										//하위4bit저장
	i2c_buffer[0] = high_nibble | WRITE_IN_SIGNAL | BACKLIGHT_ON; 			//en=1, rs=0, rw=0, backlight=1
	i2c_buffer[1] = high_nibble | WRITE_ENABLE | BACKLIGHT_ON; 				//en=0, rs=0, rw=0, backlight=1
	i2c_buffer[2] = low_nibble  | WRITE_IN_SIGNAL | BACKLIGHT_ON; 			//en=1, rs=0, rw=0, backlight=1
	i2c_buffer[3] = low_nibble  | WRITE_ENABLE | BACKLIGHT_ON; 				//en=0, rs=0, rw=0, backlight=1
	HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS,i2c_buffer, 4, 100);	//bit전송
}

//1 BYTE write
void lcd_data(uint8_t data)
{
	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];
	high_nibble = data & 0xf0;												//상위4bit저장
	low_nibble = (data<<4) & 0xf0;											//하위4bit저장
	i2c_buffer[0] = high_nibble | DATA_IN_SIGNAL | BACKLIGHT_ON; 			//en=1, rs=1, rw=0, backlight=1
	i2c_buffer[1] = high_nibble | DATA_ENABLE | BACKLIGHT_ON; 				//en=0, rs=1, rw=0, backlight=1
	i2c_buffer[2] = low_nibble  | DATA_IN_SIGNAL | BACKLIGHT_ON; 			//en=1, rs=1, rw=0, backlight=1
	i2c_buffer[3] = low_nibble  | DATA_ENABLE | BACKLIGHT_ON; 				//en=0, rs=1, rw=0, backlight=1
	HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS,i2c_buffer, 4, 100);	//bit전송
}

//lcd reset
void i2c_lcd_init(void)
{
	HAL_Delay(20);
	lcd_command(0x30);
	HAL_Delay(5);
	lcd_command(0x30);
	HAL_Delay(1);
	lcd_command(0x30);
	lcd_command(0x20);
	lcd_command(0x28);			//Function Set 4-bit mode
	lcd_command(DISPLAY_OFF);	//display off
	lcd_command(CLEAR_DISPLAY);	//clear display
	lcd_command(0x06);			//auto increment
	lcd_command(DISPLAY_ON);	//display on, cursor off
	HAL_Delay(2);
}

//null meeting string
void lcd_string(uint8_t *str)
{
	while(*str)lcd_data(*str++);
}

//move string
void move_cursor(uint8_t row, uint8_t column)
{
	lcd_command(0x80 | row<<6 | column);
	return;
}









