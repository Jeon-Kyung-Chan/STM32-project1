#ifndef INC_I2C_LCD_H_
#define INC_I2C_LCD_H_

#define I2C_LCD_ADDRESS (0x27<<1)               //lcd의 주소
#define BACKLIGHT_ON 0x08
#define WRITE_IN_SIGNAL 0x04
#define WRITE_ENABLE 0x00
#define DATA_IN_SIGNAL 0x05
#define DATA_ENABLE 0x01

/* LCD command   */
#define DISPLAY_ON 0x0C
#define DISPLAY_OFF 0x08
#define CLEAR_DISPLAY 0x01  //Delay 2msec
#define RETURN_HOME 0x02

void lcd_command(uint8_t command);
void lcd_data(uint8_t data);
void i2c_lcd_init(void);
void lcd_string(uint8_t *str);
void move_cursor(uint8_t row, uint8_t column);

#endif /* INC_I2C_LCD_H_ */
