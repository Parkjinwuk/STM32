#include "main.h"
#include "button.h"

void set_rpm(int rpm);
void stepmotor_main_test(void);
void stepmotor_drive(int step);

extern void delay_us(unsigned long us);
extern int get_button(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN, uint8_t button_number);
// RPM(Revolutions Per Minutes) : 분당 회전 수
// 1분: 60sec : 1,000,000us(1초) * 60 = 60,000,000us
// 1,000,000us(1초)
// --> 1초(1000ms) --> 1ms(1000us) * 1000ms --> 1,000,000us
// 4096스텝 : 1바퀴(4096 스텝이동)
// --- 1바퀴도는데 필요한 총 스텝수 : 4096
// 4096/ 8(0.7) --> 512 sequence : 360도
// 1 sequence(8step) : 0.70312도
// 0.70312도 * 512sequence = 360도

// ------ set_rpm(13) 으로 지정시의 동작 상황 ------
// 60,000,000us(1분) / 4096 / rpm
// 1126us(1스텝 idle타임) * 4096 = 4,612,096us
//							   = 4612ms
//							   = 4.6초
// 60초 / 4.6(1회전시 소요시간 초) ==> 13회전
// 시계방향으로 1회전 <---> 반시계방향으로 1회전
void set_rpm(int rpm) // rpm 1~13
{
	delay_us(60000000/4096/rpm);
	// 최대 speed 기준(13) : delay_us(1126);
}

int idle = 1;
int forward = 0;
int backward = 0;
int fnd_floor = 0;
volatile int ext_int_no = -1;	// GPIO_Pin?�� 0번도 존재?��?���?????? 구분?���?????? ?��?��?�� -1�?????? 집어?��?��	photo interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case PHOTO0_Pin:
		ext_int_no=GPIO_Pin;
		fnd_floor = 1;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);	// LED1
		break;
	case PHOTO1_Pin:
		ext_int_no=GPIO_Pin;
		fnd_floor = 2;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);	// LED2
		break;
	case PHOTO2_Pin:
		ext_int_no=GPIO_Pin;
		fnd_floor = 3;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);	// LED3
		break;
	case PHOTO3_Pin:
		ext_int_no=GPIO_Pin;
		fnd_floor = 4;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);	// LED1
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);	// LED2
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);	// LED3
		break;
	}
}

void stepmotor_main_test(void)
{
#if 1
	//버튼 처리에 의한
	while(1)
	{
		if(idle)
		{
			if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
			{
				forward = 1;
				idle = 0;
				//backward = 0;
			}
		}
		if(forward)
		{
			stepmotor_drive(0);
			set_rpm(13);

			if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
			{
				backward = 1;
				forward = 0;
			}
			if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
			{
				idle = 1;
				forward = 0;
			}
		}
		if(backward)
		{
			stepmotor_drive(1);
			set_rpm(10);

			if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
			{
				forward = 1;
				backward = 0;
			}
			if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
			{
				idle = 1;
				backward = 0;
			}
		}
	}
#else
	while(1)
	{
		for(int i=0; i<512; i++)	// 시계방향 1회전
		{
			for(int j=0; j<8; j++)
			{
				stepmotor_drive(j);
				set_rpm(13);
			}
		}

		for(int i=0; i<512; i++)	// 반시계방향 1회전
		{
			for(int j=7; j>=0; j--)
			{
				stepmotor_drive(j);
				set_rpm(13);	// rpm값 만큼 wait
			}
		}
	}
#endif
}

#if 1
//
// direction : 1 == 시계방향 회전
//			   2 ---> 반시계
//			   0 : idle
void stepmotor_drive(int direction)
{
	static int step = 0;	// static을 쓰면 전역변수처럼 동작
	switch(step){
	case 0:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	}
	if(direction == 0)	// 정회전
	{
		step++;	// for(step = 0; step<8; step++)
		step %= 8;	// 다음 진행할 step 준비
		// if (step >= 8) step=0;
	}
	else if(direction == 1)	// 역회전
	{
		step--;
		if(step<0) step=7;
	}
}

#else
void stepmotor_drive(int step)
{
	switch(step){
	case 0:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 0);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOD, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOD, IN4_Pin, 1);
		break;
	}
}
#endif
