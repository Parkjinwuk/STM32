#include "main.h"

void servo_motor_test_main(void);

extern TIM_HandleTypeDef htim2;
// 8400000HZ/1680 = 50000HZ
// T=1/f = 1/50000HZ = 0.00002sec(20us)
// 2ms(180도) : 0.00002 * 100개
// 1.5(90도) : 0.00002 * 75개
// 1ms(0도) : 0.00002 * 50개
void servo_motor_test_main(void)
{
	while(1)
	{
		// 180도 회전
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 125);	// 180도
		HAL_Delay(1000);

		// 0도
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 25);
		HAL_Delay(1000);

		// 90도
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 75);
		HAL_Delay(1000);
	}
}
