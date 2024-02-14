#include "main.h"

int dotmatrix_main_test(void);
int dotmatrix_main_test(void);

GPIO_TypeDef *col_port[] =
{
		COL1_GPIO_Port, COL2_GPIO_Port, COL3_GPIO_Port, COL4_GPIO_Port,
		COL5_GPIO_Port, COL6_GPIO_Port, COL7_GPIO_Port, COL8_GPIO_Port
};

GPIO_TypeDef *row_port[] =
{
		ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port, ROW4_GPIO_Port,
		ROW5_GPIO_Port, ROW6_GPIO_Port, ROW7_GPIO_Port, ROW8_GPIO_Port
};

uint16_t col_pin[] =
{
		COL1_Pin, COL2_Pin, COL3_Pin, COL4_Pin,
		COL5_Pin, COL6_Pin, COL7_Pin, COL8_Pin
};

uint16_t row_pin[] =
{
		ROW1_Pin, ROW2_Pin, ROW3_Pin, ROW4_Pin,
		ROW5_Pin, ROW6_Pin, ROW7_Pin, ROW8_Pin
};

unsigned char all_on[] = // all on 문자 정의
{
#if 1
		0b01100110,
		0b10011001,
		0b10000001,
		0b10000001,
		0b10000001,
		0b01000010,
		0b00100100,
		0b00011000
#endif

#if 0
		0b00011000,
		0b00100100,
		0b01100110,
		0b10011001,
		0b10011001,
		0b01100110,
		0b00100100,
		0b00011000
#endif

#if 0
		0b00111100,
		0b01000010,
		0b10000001,
		0b10011001,
		0b01100110,
		0b10000001,
		0b10000001,
		0b01111110
#endif

#if 0
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111
#endif
};

const uint8_t blank[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t number_data[20][10] =
{
#if 0
	{
		0b01110000,	//0
		0b10001000,
		0b10011000,
		0b10101000,
		0b11001000,
		0b10001000,
		0b01110000,
	    0b00000000
	},
	{
		0b01000000,	//1
		0b11000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b11100000,
	    6   // ??0b00000110
	},
	{
		0b01110000,	//2
		0b10001000,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b11111000,
	    6
	},
	{
		0b11111000,	//3
	    0b00010000,
		0b00100000,
		0b00010000,
		0b00001000,
		0b10001000,
		0b01110000,
	    6
	},
	{
		0b00010000,	//4
		0b00110000,
		0b01010000,
		0b10010000,
		0b11111000,
		0b00010000,
		0b00010000,
	    6
	},
	{
		0b11111000,	//5
		0b10000000,
		0b11110000,
		0b00001000,
		0b00001000,
		0b10001000,
		0b01110000,
	    6
	},
	{
		0b00110000,	//6
		0b01000000,
		0b10000000,
		0b11110000,
		0b10001000,
		0b10001000,
		0b01110000,
	    6
	},
	{
		0b11111000,	//7
		0b10001000,
		0b00001000,
		0b00010000,
		0b00100000,
		0b00100000,
		0b00100000,
	    6
	},
	{
		0b01110000,	//8
		0b10001000,
		0b10001000,
		0b01110000,
		0b10001000,
		0b10001000,
		0b01110000,
	    6
	},
	{
		0b01110000,	//9
		0b10001000,
		0b10001000,
		0b01111000,
		0b00001000,
		0b00010000,
		0b01100000,
	    6
	},
	{
		0b00000000,    // hart
		0b01100110,
		0b11111111,
		0b11111111,
		0b11111111,
		0b01111110,
		0b00111100,
		0b00011000,
		6
	},
	{
		0b00111000,	// P
		0b0100100,
		0b0100100,
		0b0111000,
		0b0100000,
		0b0100000,
		0b0100000,
		0b0000000,
		6
	},
	{
		0b00000000,	// J
		0b00011111,
		0b00000100,
		0b00000100,
		0b00000100,
		0b00100100,
		0b00011000,
		0b00000000,
		6
	},
	{
		0b00000000,	// W
		0b00101010,
		0b00101010,
		0b00101010,
		0b00101010,
		0b00101010,
		0b00011100,
		0b00000000
	},
#endif
	{
		0b00001100,
		0b00011100,
		0b00111100,
		0b00001100,
		0b00001100,
		0b00001100,
		0b00111110,
		0b00000000
	},
	{
		0b00011000,
		0b00111100,
		0b01111110,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00000000
	}
};



unsigned char display_data[8];	// 최종 8x8 출력할 데이터
unsigned char scroll_buffer[50][8] = {0};	// 스크롤할 데이터가 들어있는 버퍼
int number_of_character = 3;	// 출력할 문자 갯수

// 초기화 작업
// 1. display_data에 number_data[0]에 있는 내용 복사
// 2. number_data를 scroll_buffer에 복사
// 3. dotmaxrix의 led를 off
void init_dotmatrix(void)
{
	for (int i=0; i<8; i++)
	{
		display_data[i] = number_data[i];

	}
	for (int i=1; i < number_of_character+1; i++)
	{
		for (int j=0; j<8; j++)	// scroll_buffer[0] = blank
		{
			scroll_buffer[i][j] = number_data[i-1][j];
		}
	}
	for (int i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(col_port[i], col_pin[i], 1);	// led all off
	}
}


void write_column_data(int column)
{
	for (int i=0; i<8; i++)
	{
		if (i == column)
			HAL_GPIO_WritePin(col_port[i], col_pin[i], 0); // on
		else
			HAL_GPIO_WritePin(col_port[i], col_pin[i], 1); // off
	}
}

//0b00111100
void write_row_data(unsigned char data)
{
	unsigned char d = data;

	for (int i=0; i<8; i++)
	{
		if ( d & (1 << i)) // 1인 경우
			HAL_GPIO_WritePin(row_port[i], row_pin[i], 1);
		else
			HAL_GPIO_WritePin(row_port[i], row_pin[i], 0);
	}
}

#if 1
// scroll 문자 출력 프로그램
int dotmatrix_main_test(void)
{
	static int count = 0;	// 컬럼 count
	static int index = 0;	// scroll_buffer의 2차원 index값
	static uint32_t past_time = 0;	// 이전 tick값 저장

	init_dotmatrix();

	while(1)
	{
		uint32_t now = HAL_GetTick();	// 1ms
		// 1. 처음시작시 past_time = 0; now: 500 --> past_time = 500
		if (now - past_time >= 100)	// 100ms scroll
		{
			past_time = now;
			for (int i=0; i<8; i++)
			{
				display_data[i] = (scroll_buffer[index][i] >> count) | (scroll_buffer[index+1][i] << (8 - count));
			}
			if (++count == 8)	// 8칼럼을 다 처리 했으면 다음 scroll_buffer로 이동
			{
				count = 0;
				index++;
				if (index == number_of_character+1) index = 0;
				// 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
			}
		}
		for(int i=0; i<8; i++)
		{
			// common anode 방식
			// column 에는 low, row 에는 high 를 출력해야 해당 LED 가 켜진다.
			write_column_data(i);
			write_row_data(display_data[i]);
			HAL_Delay(1);
		}
	}

	return 0;
}
#else
// 고정문자 출력 demo program
int dotmatrix_main_test(void)
{
	while(1)
	{
		for(int i=0; i<8; i++)
		{
			// common anode 방식
			// column 에는 low, row 에는 high 를 출력해야 해당 LED 가 켜진다.
			write_column_data(i);
			write_row_data(all_on[i]);
			HAL_Delay(1);
		}
	}

	return 0;
}
#endif
