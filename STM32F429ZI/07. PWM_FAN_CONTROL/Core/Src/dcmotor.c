#include "button.h"
#include "dcmotor.h"

extern TIM_HandleTypeDef htim4;
extern volatile int t1ms_counter;

void dcmotor_pwm_control(void);

// BUTTON0 : start/stop : LED1 (toggle)
// BUTTON1 : SPEED-UP : LED2 (toggle)
// BUTTON2 " SPEED-DOWN : LED3 (toggle)
uint8_t pwm_start_flag = 0;	// dcmotor start/stop indicator flag
uint16_t CCR_Value = 0;	// PWM control
uint16_t forward_back_dcmotor = 0;	// 초기 세팅은 정방향 forward

void dcmotor_pwm_control(void)
{
	// start/stop : button0
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);	// LD1

		if (pwm_start_flag == 0)
		{
			pwm_start_flag = 1;
			if (forward_back_dcmotor == 0)
			{
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
				CCR_Value = __HAL_TIM_GET_COMPARE(&htim4,TIM_CHANNEL_1);
				for(int i = 0; i < CCR_Value/10-2; i++)
					HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
				CCR_Value = __HAL_TIM_GET_COMPARE(&htim4,TIM_CHANNEL_1);
				for(int i = 0; i < CCR_Value/10-2; i++)
					HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
			}
			HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
		}
		else
		{
			pwm_start_flag = 0;
			HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
			HAL_GPIO_WritePin(GPIOD, 0xff, 0);
		}

	}
	// speed-up : button1
	if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
	{

		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);	// LED2
		CCR_Value = __HAL_TIM_GET_COMPARE(&htim4,TIM_CHANNEL_1);
		CCR_Value += 10;
		if (CCR_Value > 100) CCR_Value = 100;
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,CCR_Value);
		for (int i = 0; i < CCR_Value/10-2; i++)
		{
			HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		}

	}

	// speed-down : button1
	if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS)
	{

		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);	// LED14
		CCR_Value = __HAL_TIM_GET_COMPARE(&htim4,TIM_CHANNEL_1);
		CCR_Value -= 10;
		if (CCR_Value < 30) CCR_Value = 30;
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,CCR_Value);
		HAL_GPIO_WritePin(GPIOD, 0xff, 0);
		for (int i = 0; i < CCR_Value/10-2; i++)
		{
			HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		}

	}

	// forward_back : button3
	if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3) == BUTTON_PRESS)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
		if (forward_back_dcmotor == 0)	// forward -> backward
		{
			forward_back_dcmotor = 1;
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
		}
		else							// backward -> forward
		{
			forward_back_dcmotor = 0;
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
		}
	}
	if (pwm_start_flag == 1)
	{
		if (forward_back_dcmotor == 0)		//forward
		{
			if (t1ms_counter >= 50)
			{
				t1ms_counter = 0;
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
			}
		}
		else
		{
			if (t1ms_counter >= 100)		//backward
			{
				t1ms_counter = 0;
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
			}
		}
	}
}
