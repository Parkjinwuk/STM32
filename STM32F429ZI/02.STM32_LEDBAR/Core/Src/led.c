#include "led.h"	// < >

void led_main(void);
void led_all_on(void);
void led_all_off(void);
void led_on_up(void);
void led_on_down(void);
void led_keepon_up(void);
void led_keepon_down(void);
void led_flower_on(void);
void led_flower_off(void);

void led_main(void)
{
	while(1)
	{
		led_flower_on();
		led_flower_off();
		// [STM32] flower_on/off

		// [STM32] led_keepon_up/down
		// 1. source 2. 동작 동영상
		//phase#4
		// 0->1->2->3->4->5->6->7
		// 앞전에 ON 했던 LED는 그대로 유지
		//led_keepon_up();
		//led_all_off();
		// 7->6->5->4->3->2->1->0
		// 앞전에 ON 했던 LED는 그대로 유지
		//led_keepon_down();
		//led_all_off();

		// phase#3
		// 0->1->2->3->4->5->6->7
		// 해당 되는 bit의 LED만 ON
		//led_on_up();
		// 7->6->5->4->3->2->1->0
		//led_on_down();

		// phase#2
//		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_All);
//		HAL_Delay(500);

		// phase#1
//		led_all_on();
//		HAL_Delay(300);
//		led_all_off();
//		HAL_Delay(300);
	}
}

void led_all_on(void)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
//			GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1);
		HAL_GPIO_WritePin(GPIOD, 0xff, 1);
}

void led_all_off(void)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
//				GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOD, 0xff, 0);
}

void led_flower_on()
{
	led_all_off();

	for (int i=0; i<4; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x10 << i, 1);
		HAL_GPIO_WritePin(GPIOD, 0x08 >> i, 1);
		HAL_Delay(100*(i+1));
	}
}

void led_flower_off()
{
	for (int i=0; i<4; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 0);
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 0);
		HAL_Delay(100*(i+1));
	}
	HAL_Delay(50);
}

void led_keepon_up()
{
	for (int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		HAL_Delay(200);
	}
}

void led_keepon_down()
{
	for (int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		HAL_Delay(200);
	}
}

// 0->1->2->3->4->5->6->7
// 해당 되는 bit의 LED만 ON
void led_on_up()
{
	for (int i=0; i<8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		HAL_Delay(200);
	}
}

// 7->6->5->4->3->2->1->0
void led_on_down()
{
	for (int i=0; i<8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		HAL_Delay(200);
	}
}
