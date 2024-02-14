#include "led.h"	// < >
#include "button.h"

#define LED_ON_UP 0
#define LED_ON_DOWN 1

void led_main(void);
void led_all_on(void);
void led_all_off(void);
void led_on_up(void);
void led_on_down(void);
void led_keepon_up(void);
void led_keepon_down(void);
void led_flower_on(void);
void led_flower_off(void);
void button0_led_all_on_off_toggle(void);
void led_bar_up_down(void);

extern volatile int t1ms_counter;

void (*fi[8])() =
{
		led_all_on,
		led_all_off,
		led_flower_on,
		led_flower_off,
		led_keepon_up,
		led_keepon_down,
		led_on_up,
		led_on_down
};


void button0_toggle()
{
	static int button0_count = -1;

	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		button0_count++;
		button0_count %= 8;
	}
	if (button0_count >= 0)
	{
		fi[button0_count]();
	}

}

int func_index = 0;

void led_main(void)
{
	while(1)
	{
		button0_toggle();

		//fi[func_index]();

		//led_bar_up_down();

		/*
		if (t1ms_counter >= 200)
		{
			t1ms_counter = 0;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);	// LED1
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);	// LED2
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);	// LED3
		}
		*/

		/*
		 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);	// LED1
		 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);	// LED2
		 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);	// LED3
		 HAL_Delay(200);	// 200ms
		*/

		// PHASE #6
		//button0_led_all_on_off_toggle();
		// PHASE #5

		// flower on/off
		//led_flower_on();
		//led_flower_off();
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
		/*
		switch(func_index)
		{
			case LED_ON_UP:
				led_on_up();
				break;
			case LED_ON_DOWN:
				led_on_down();
				break;
			default:
				break;
		}
		*/
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

void led_bar_up_down(void)
{
	static int button_count = 0;

	if(get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << button_count, 1);
		button_count ++;
	}
	else if(get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << button_count, 0);
		button_count --;
	}
}

void button0_led_all_on_off_toggle(void)
{
	static int button0_count = 0;	// static 으로 선언을 하면 전역 변수처럼 동작을 한다.

	// 버튼을 한번 눌렀다 뗀 상태라면
	if(get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		button0_count++;
		button0_count %= 9;
		if (button0_count == 1)
		{
			led_all_on();
		}
		else if (button0_count == 2)
		{
			led_all_off();
		}
		else if (button0_count == 3)
		{
			led_flower_on();
		}
		else if (button0_count == 4)
		{
			led_flower_off();
		}
		else if (button0_count == 5)
		{
			led_keepon_up();
			led_all_off();
		}
		else if (button0_count == 6)
		{
			led_keepon_down();
			led_all_off();
		}
		else if (button0_count == 7)
		{
			led_on_up();
		}
		else if (button0_count == 8)
		{
			led_on_down();
		}
		else if (button0_count == 0)
		{
			led_all_off();
		}
	}
}

void led_all_on(void)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
//			GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1);
		HAL_GPIO_WritePin(GPIOD, 0xff, 1);
		if (t1ms_counter >= 200)
		{
			t1ms_counter = 0;
			func_index = (func_index + 1) % 8;
		}
}

void led_all_off(void)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
//				GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOD, 0xff, 0);
	if (t1ms_counter >= 200)
			{
				t1ms_counter = 0;
				func_index = (func_index + 1) % 8;
			}
}

void led_flower_on()
{
	/*
	led_all_off();

	for (int i=0; i<4; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x10 << i, 1);
		HAL_GPIO_WritePin(GPIOD, 0x08 >> i, 1);
	}
	*/
	static int i = 0;
	if (i == 0) HAL_GPIO_WritePin(GPIOD, 0xff, 0);

	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0x10 << i, 1);
		HAL_GPIO_WritePin(GPIOD, 0x08 >> i, 1);
		i++;
	}
	if(i>4)
	{
		i = 0;
		func_index = (func_index + 1) % 8;
		HAL_GPIO_WritePin(GPIOD, 0xff, 0);
	}
}

void led_flower_off()
{
	/*
	for (int i=0; i<4; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 0);
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 0);
	}
	*/
	static int i=0;
	if (i == 0) HAL_GPIO_WritePin(GPIOD, 0xff, 1);

	if (t1ms_counter >= 200)
		{
			t1ms_counter = 0;
			HAL_GPIO_WritePin(GPIOD, 0x01 << i, 0);
			HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 0);
			i++;
		}
	if(i>4)
	{
		i = 0;
		func_index = (func_index + 1) % 8;
		HAL_GPIO_WritePin(GPIOD, 0xff, 1);
	}
}

void led_keepon_up()
{
	/*
	for (int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
	}
	*/
	static int i = 0;
	if (i == 0) HAL_GPIO_WritePin(GPIOD, 0xff, 0);

	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		i++;
	}
	if(i>8)
	{
		i = 0;
		func_index = (func_index + 1) % 8;
		HAL_GPIO_WritePin(GPIOD, 0xff, 0);
	}
}

void led_keepon_down()
{
	/*
	for (int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
	}
	*/
	static int i = 0;
	if (i == 0) HAL_GPIO_WritePin(GPIOD, 0xff, 0);

	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		i++;
	}
	if(i>8)
	{
		i = 0;
		func_index = (func_index + 1) % 8;
		HAL_GPIO_WritePin(GPIOD, 0xff, 0);
	}
}

// 0->1->2->3->4->5->6->7
// 해당 되는 bit의 LED만 ON
void led_on_up()
{
#if 1
	static int i = 0;
	if(t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0xff, 0);
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		i++;
	}
	if (i>8)
	{
		i = 0;
		func_index = (func_index + 1) % 8;
	}
#else	// original
	for (int i=0; i<8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		HAL_Delay(200);
	}
#endif
}

// 7->6->5->4->3->2->1->0
void led_on_down()
{
#if 1
	static int i = 0;
	if(t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0xff, 0);
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		i++;
	}
		if (i>8)
		{
			i = 0;
			func_index = (func_index + 1) % 8;
		}
#else	// original
	for (int i=0; i<8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		HAL_Delay(200);
	}
#endif
}
