#include "main.h"

#define COMMAND_MAX 50      //수신된 데이터 저장 최대 50
#define COMMAND_LENGTH 30   //통신시 보내는 글자의 수 최대 30

// bluetooth
volatile uint8_t bt_buffer[COMMAND_MAX][COMMAND_LENGTH];    // BT로 부터 수신된 char를 저장하는 공간. \n를 만날 때 까지 저장
volatile int bt_write_index=0;                              //수신된 글자 저장
volatile int command_bt_index=0;                            //수신된 데이터 저장
volatile int read_bt_index=0;                               //수신된 데이터 읽기
uint8_t command_bt_count=0;                                 //수신된 데이터 숫자 기록

// PC
volatile uint8_t command_buffer[COMMAND_MAX][COMMAND_LENGTH];   // PC로 부터 수신된 char를 저장하는 공간. \n를 만날 때 까지 저장
volatile int command_index=0;                                   //수신된 데이터 저장
volatile int read_index=0;                                      //수신된 데이터 읽기
volatile int write_index=0;                                     //수신된 글자 저장
uint8_t command_count=0;                                        //수신된 데이터 숫자 기록

//pc로부터 온 명령어를 저장하는 2차원 array(circular queue)
volatile uint8_t rx_data;                   //PC로 부터 1byte의 INT가 들어오면 저장하는 변수
volatile uint8_t rxbt_data;                 //Bluetooth로 부터 1byte의 INT가 들어오면 저장하는 변수

extern UART_HandleTypeDef huart1;           //Bluetooth 통신
extern UART_HandleTypeDef huart2;           //PC 통신 

//USART로 부터 1byte가 수신되면 H/W가 call을 해준다.
// USART1 / USART2번으로 부터 1byte가 수신(stop bit)하면 rx interrupt가 발생
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)                                                //PC 통신일 때 
	{
		if(write_index < COMMAND_LENGTH)                                //수신된 글자 30byte 이내
		{
			if(rx_data == '\n'|| rx_data == '\r')                       //수신된 데이터를 다 보내주었다면
			{
				write_index=0;                                          //다음 command를 저장하기 위해서 변수를 0으로 초기화
				command_index++;                                        //다음 저장할 command를 준비
				command_index %= COMMAND_MAX;                           //환형 큐로 명령어가 모두 채워지면 맨 앞으로 저장됨.크기 50
				command_count++;                                        //현재 들어온 command 갯수 증가
			}
			else
				command_buffer[command_index][write_index++] = rx_data; //수신된 데이터 저장
		}
		else                                                            //수신된 글자가 30byte 넘을 때
			printf("buffer overflow!!\n");                              //overflow 출력
		HAL_UART_Receive_IT(&huart2, &rx_data, 1);                      //PC UART 다음 인터럽트가 뜨도록 call해줌
	}

	if(huart == &huart1)                                                    //Bluetooth 통신일 때
	{
		if(bt_write_index < COMMAND_LENGTH)                                 //수신된 글자30byte 이내
		{
			if(rxbt_data == '\n'|| rxbt_data == '\r')                       //수신된 데이터를 다 보내주었다면
			{
				bt_write_index=0;                                           //다음 command를 저장하기 위해서 변수를 0으로 초기화
				command_bt_index++;                                         //다음 저장할 command를 준비
				command_bt_index %= COMMAND_MAX;                            //환형 큐로 명령어가 모두 채워지면 맨 앞으로 저장됨.크기 50
				command_bt_count++;                                         //현재 들어온 command 갯수 증가
			}
			else
				bt_buffer[command_bt_index][bt_write_index++] = rxbt_data;  //수신된 데이터 저장
		}
		else                                                                //수신된 글자가 30byte 넘을 때
			printf("BT buffer overflow!!\n");                               //overflow 출력
		HAL_UART_Receive_IT(&huart1, &rxbt_data, 1);                        //Bluetooth UART 다음 인터럽트가 뜨도록 call해줌
	}
}

//USART2
void pc_command_processing()
{
	if(command_count)                                           //command_buffer에 완전한 command가 들어 왔으면
	{    
		command_count--;                                        //수신된 데이터 읽음
		printf("%s\n",command_buffer[read_index]);              //수신된 문자를 터미널로 전송
		memset(command_buffer[read_index],0,COMMAND_LENGTH);    //command_buffer[read_index]를 0으로 set
		read_index++;                                           //다음 수신될 데이터로 이동
		read_index %= COMMAND_MAX;                              //환형 큐로 명령어가 모두 채워지면 맨 앞으로 저장됨. 크기 50
	}
}

//USART1
void bluetooth_command_processing()
{
	if(command_bt_count)                                        //bt_buffer에 완전한 command가 들어 왔으면
	{
        command_bt_count--;                                     //수신된 데이터 읽음
		printf("%s\n",bt_buffer[read_bt_index]);                //수신된 문자를 터미널로 전송
        memset(bt_buffer[read_bt_index],0,COMMAND_LENGTH);      //bt_buffer[read_bt_index]를 0으로 set
		read_bt_index++;                                        //다음 수신될 데이터로 이동
		read_bt_index %= COMMAND_MAX;                           //환형 큐로 명령어가 모두 채워지면 맨 앞으로 저장됨. 크기 50
	}
}