#include "uart.h"
#include <string.h>		// strncmp, strcpy, ...

void pc_command_processing();
void bt_command_processing();

extern void led_main(void);
extern void led_all_on(void);
extern void led_all_off(void);
extern void led_on_up(void);
extern void led_on_down(void);
extern void led_keepon_up(void);
extern void led_keepon_down(void);
extern void led_flower_on(void);
extern void led_flower_off(void);

extern uint8_t rx_data;	// uart3 rx data
extern uint8_t bt_rx_data;	// uart6 rx data

extern UART_HandleTypeDef huart3;	// PC
extern UART_HandleTypeDef huart6;	// BT
// HW와 SW의 만나는 약속 장소 : call back function
// move from HAL_UART_RxCpltCallback of stm32f4xx_hal_uart to here
// UART로 1 byte가 수신되면 H/W가 call을 해준다.
// UART RX INT가 발생이 되면 이곳으로 자동적으로 들어 온다.
// 1. 숙제#1 : 1차원 array 가지고는 overflow 가 발생 되므로
// 2차원 array circular queue 를 구성하여 overflow 가 발생 되지 않도록 구현 해야 한다.
// comport mastart
// ledallon\n
// ledallof\n
// led_toggle\n

#define COMMAND_LENGTH 40
#define COMMAND_SU 40

volatile unsigned char rx_buff[COMMAND_LENGTH];	// UART3으로부터 수신된 char를 저장하는 공간 (\n을 만날때 까지)
volatile int rx_index = 0;	// rx_buff의 save 위치
volatile int rx_index2 = 1;	// 현재 들어온 입력 위치
volatile int newline_detect_flag = 0;	// new line을 만났을 때의 indicator 예) ledallon\n

volatile unsigned char bt_rx_buff[COMMAND_LENGTH];	// UART3으로부터 수신된 char를 저장하는 공간 (\n을 만날때 까지)
volatile int bt_rx_index = 0;	// bt_rx_buff의 save 위치
volatile int bt_rx_index2 = 1;	// 현재 들어온 입력 위치
volatile int bt_newline_detect_flag = 0;	// new line을 만났을 때의 indicator 예) ledallon\n

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart3)	// comport master와 연결된 uart
	{
		if (rx_index < COMMAND_LENGTH)	// 현재까지 들어온 byte가 40byte룰 넘지 않으면
		{
			if (rx_data == '\n' || rx_data == '\r')
			{
				rx_buff[rx_index] = 0;	// '\0'
				newline_detect_flag = 1;	// new line을 만났다는 flag를 set한다.
				rx_index = 0;	// 다음 message 저장을 위해서 rx_index 값을 0으로 한다.ㅣ
			}
			else
			{
				rx_buff[rx_index++] = rx_data;
			}
		}
		else
		{
			rx_index = 0;
			printf("Message Overflow !!!\n");
		}
		// 주의: 반드시 HAL_UART_Receive_IT를 call 해줘야 다음 INT가 발생이 된다.
		HAL_UART_Receive_IT(&huart3, &rx_data, 1);
	}

	if (huart == &huart6)	// comport master와 연결된 uart
		{
			if (bt_rx_index < COMMAND_LENGTH)	// 현재까지 들어온 byte가 40byte룰 넘지 않으면
			{
				if (bt_rx_data == '\n' || bt_rx_data == '\r')
				{
					bt_rx_buff[bt_rx_index] = 0;	// '\0'
					bt_newline_detect_flag = 1;	// new line을 만났다는 flag를 set한다.
					bt_rx_index = 0;	// 다음 message 저장을 위해서 bt_rx_index 값을 0으로 한다.ㅣ
				}
				else
				{
					bt_rx_buff[bt_rx_index++] = bt_rx_data;
				}
			}
			else
			{
				bt_rx_index = 0;
				printf("BT Message Overflow !!!\n");
			}
			// 주의: 반드시 HAL_UART_Receive_IT를 call 해줘야 다음 INT가 발생이 된다.
			HAL_UART_Receive_IT(&huart6, &bt_rx_data, 1);
		}
}



void pc_command_processing(void)
{
	if (newline_detect_flag)	// comport master로 부터 완전한 문장이 들어오면 (\n을 만나면)
	{
		 newline_detect_flag = 0;
		 printf("%s\n", rx_buff);

		 if (!strncmp(rx_buff, "led_all_on", strlen("led_all_on")))		// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == NULL)
		 {
			 led_all_on();
			 return;
		 }
		 if (!strncmp(rx_buff, "led_all_off", strlen("led_all_off")))
		 {
			 led_all_off();
			 return;
		 }
		 if (!strncmp(rx_buff, "led_on_up", strlen("led_on_up")))
		 {
			 led_on_up();
			 return;
		 }
		 if (!strncmp(rx_buff, "led_on_down", strlen("led_on_down")))
		 {
			 led_on_down();
			 return;
		 }
		 if (!strncmp(rx_buff, "led_keepon_up", strlen("led_keepon_up")))
		 {
			 led_keepon_up();
			 return;
		 }
		 if (!strncmp(rx_buff, "led_keepon_down", strlen("led_keepon_down")))
		 {
			 led_keepon_down();
			 return;
		 }
		 if (!strncmp(rx_buff, "led_flower_on", strlen("led_flower_on")))
		 {
			 led_flower_on();
			 return;
		 }
		 if (!strncmp(rx_buff, "led_flower_off", strlen("led_flower_off")))
		 {
			 led_flower_off();
			 return;
		 }
	}
}

void bt_command_processing(void)
{
	if (bt_newline_detect_flag)	// comport master로 부터 완전한 문장이 들어오면 (\n을 만나면)
	{
		 bt_newline_detect_flag = 0;
		 printf("%s\n", bt_rx_buff);

		 if (!strncmp(bt_rx_buff, "led_all_on", strlen("led_all_on")))		// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == NULL)
		 {
			 led_all_on();
			 return;
		 }
		 if (!strncmp(bt_rx_buff, "led_all_off", strlen("led_all_off")))
		 {
			 led_all_off();
			 return;
		 }
		 if (!strncmp(bt_rx_buff, "led_on_up", strlen("led_on_up")))
		 {
			 led_on_up();
			 return;
		 }
		 if (!strncmp(bt_rx_buff, "led_on_down", strlen("led_on_down")))
		 {
			 led_on_down();
			 return;
		 }
		 if (!strncmp(bt_rx_buff, "led_keepon_up", strlen("led_keepon_up")))
		 {
			 led_keepon_up();
			 return;
		 }
		 if (!strncmp(bt_rx_buff, "led_keepon_down", strlen("led_keepon_down")))
		 {
			 led_keepon_down();
			 return;
		 }
		 if (!strncmp(bt_rx_buff, "led_flower_on", strlen("led_flower_on")))
		 {
			 led_flower_on();
			 return;
		 }
		 if (!strncmp(bt_rx_buff, "led_flower_off", strlen("led_flower_off")))
		 {
			 led_flower_off();
			 return;
		 }
	}
}
