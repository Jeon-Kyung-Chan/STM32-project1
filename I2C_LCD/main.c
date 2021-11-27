extern void i2c_lcd_init(void);
extern void i2c_lcd_main(void);

I2C_HandleTypeDef hi2c1;
static void MX_I2C1_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();
    i2c_lcd_init();
    while (1)
    {
        i2c_lcd_main();
    }
}