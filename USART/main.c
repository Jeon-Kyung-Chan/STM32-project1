extern void pc_command_processing();
extern void bluetooth_command_processing();
extern volatile uint8_t rx_data;
extern volatile uint8_t rxbt_data;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  HAL_UART_Receive_IT(&huart2, &rx_data, 1);  //PC UART
  HAL_UART_Receive_IT(&huart1, &rxbt_data, 1);  //bluetooth TART
  while (1)
  {
	  pc_command_processing();
	  bluetooth_command_processing();
  }
}