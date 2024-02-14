#include "main.h"
#include "fnd.h"
#include "button.h"

extern volatile int fnd1ms_counter;  // fnd1ms
extern int fnd_floor;
void fnd4digit_main(void);
void fnd4digit_sec_clock(void);

uint32_t FND_font[10] =
{
  FND_a|FND_b|FND_c|FND_d|FND_e|FND_f,   // 0
  FND_b|FND_c,                           // 1
  FND_a|FND_b|FND_d|FND_e|FND_g,         // 2
  FND_a|FND_b|FND_c|FND_d|FND_g,         // 3
  FND_b|FND_c|FND_f|FND_g,   // 4
  FND_a|FND_c|FND_d|FND_f|FND_g,  // 5
  FND_a|FND_c|FND_d|FND_e|FND_f|FND_g,  // 6
  FND_a|FND_b|FND_c,      // 7
  FND_a|FND_b|FND_c|FND_d|FND_e|FND_f|FND_g,   // 8
  FND_a|FND_b|FND_c|FND_d|FND_f|FND_g   // 9
};

void fnd4digit_main(void)
{
	HAL_GPIO_WritePin(FND_DATA_PORT,FND_font[8], GPIO_PIN_SET);
	HAL_GPIO_WritePin(FND_DATA_PORT, FND_font[fnd_floor], GPIO_PIN_RESET);
/*
	while (0)
	{

        // common 캐소우드 CL5642AH30
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, 0);    // 1
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, 0);

		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, 1);

		HAL_Delay(1000);

        // common 캐소우드 CL5642AH30
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, 0);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, 1);    // 2
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, 0);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, 0);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, 0);


		HAL_Delay(1000);
	}
*/
}
