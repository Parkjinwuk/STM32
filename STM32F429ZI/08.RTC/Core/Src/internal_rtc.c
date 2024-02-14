#include "main.h"	// for GPIO & HAL
#include "i2c_lcd.h"
#include "button.h"

void get_rtc(void);
void set_rtc(char *date_time);
void set_time_button_ui(void);
unsigned char bin2dec(unsigned char byte);
unsigned char dec2bin(unsigned char byte);

extern RTC_HandleTypeDef hrtc;
extern uint8_t lcd_display_mode_flag;
extern RTC_TimeTypeDef mTime;	// time information

RTC_TimeTypeDef sTime = {0};	// time information
RTC_DateTypeDef sDate = {0};	// date information

// 예) 23년 save된 binary format
// 7654 3210
// 0010 0011
// 00000010
// STM32의 RTC로 부터 날짜 & 시각 정보를 읽어 오는 함수
unsigned char bin2dec(unsigned char byte)
{
	unsigned char high, low;
	low = byte & 0x0f;	// 하위 4bit(low nibble)
	high = (byte >> 4) * 10;	// 상위 4bit(high nibble)

	return high + low;
}
// decimal --> BCD ex) 23: 0001 0111
unsigned char dec2bin(unsigned char byte)
{
	unsigned char high, low;

	high = (byte / 10) << 4;
	low = byte % 10;

	return high + low;
}
void get_rtc(void)
{
	static RTC_TimeTypeDef oldTime;	// 이전 시각 정보를 가지고 있기 위함이다.
	char lcd_buff[40];

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

	if (oldTime.Seconds != sTime.Seconds)
	{
		// YYYY-MM-DD HH:mm:ss
		printf("%04d-%02d-%02d %02d:%02d:%02d\n",
				bin2dec(sDate.Year)+2000, bin2dec(sDate.Month), bin2dec(sDate.Date),
				bin2dec(sTime.Hours), bin2dec(sTime.Minutes), bin2dec(sTime.Seconds));

		if (lcd_display_mode_flag==0)
		{
			sprintf(lcd_buff, "DATE:%04d-%02d-%02d",
					bin2dec(sDate.Year)+2000, bin2dec(sDate.Month), bin2dec(sDate.Date));
			move_cursor(0, 0);
			lcd_string(lcd_buff);
			sprintf(lcd_buff, "TIME:%02d:%02d:%02d",
					bin2dec(sTime.Hours), bin2dec(sTime.Minutes), bin2dec(sTime.Seconds));
			move_cursor(1, 0);
			lcd_string(lcd_buff);
		}
	}
	oldTime.Seconds=sTime.Seconds;
}
// setrtc231016103800
//       678901234567 번방
void set_rtc(char *date_time)
{
	char yy[4], mm[4], dd[4];	// date
	char hh[4], min[4], ss[4];	// time

	strncpy(yy, date_time+6, 2);
	strncpy(mm, date_time+8, 2);
	strncpy(dd, date_time+10, 2);

	strncpy(hh, date_time+12, 2);
	strncpy(min, date_time+14, 2);
	strncpy(ss, date_time+16, 2);

	// ascii --> int --> bcd
	sDate.Year = dec2bin(atoi(yy));
	sDate.Month = dec2bin(atoi(mm));
	sDate.Date = dec2bin(atoi(dd));

	sTime.Hours = dec2bin(atoi(hh));
	sTime.Minutes = dec2bin(atoi(min));
	sTime.Seconds = dec2bin(atoi(ss));

	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

}

// button0 : 시간 정보 변경 버튼 00~23 (up counter)
// button1 : 분을 변경 하는 버튼 00~59 (up counter)
// button2 : 초를 변경 하는 버튼 00~59 (up counter)
// button3 : 변경 완료 현재까지 변경된 내용을 저장
//           HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

void set_time_button_ui(void)
{
	char lcd_buff[40];
	static int sel_hour=0, sel_min=0, sel_sec=0;



	if (lcd_display_mode_flag == 3)
	{
		sprintf(lcd_buff," SET:%02d:%02d:%02d",sel_hour,sel_min,sel_sec);
		move_cursor(0,0);
		lcd_string(lcd_buff);

		if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
		{
			// 시간정보 modify
			sel_hour++;
			sel_hour %= 24;
			sprintf(lcd_buff, "%02d", sel_hour);
			move_cursor(0, 5);
			lcd_string(lcd_buff);
		}
		if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
		{
			// 분 정보 modify
			sel_min++;
			sel_min %= 60;
			sprintf(lcd_buff, "%02d", sel_min);
			move_cursor(0, 8);
			lcd_string(lcd_buff);
		}
		if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS)
		{
			// 초 정보 modify
			sel_sec++;
			sel_sec %= 60;
			sprintf(lcd_buff, "%02d", sel_sec);
			move_cursor(0, 11);
			lcd_string(lcd_buff);
		}
		if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3) == BUTTON_PRESS)
		{
			// 완료 버튼
			sTime.Hours = dec2bin(sel_hour);
			sTime.Minutes = dec2bin(sel_min);
			sTime.Seconds = dec2bin(sel_sec);
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
			lcd_display_mode_flag == 0;
		}

	}
}


/*
void set_time_button_ui(void)
{
	char lcd_buff[40];

	if (lcd_display_mode_flag == 3)
	{

		if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
		{
			sprintf(lcd_buff, "TIME:%02d:%02d:%02d",
					bin2dec(mTime.Hours), bin2dec(mTime.Minutes), bin2dec(mTime.Seconds));
			move_cursor(0,0);
			lcd_string(lcd_buff);
			move_cursor(1,6);	// 시간 정보 field 로 커서를 이동

		}
		if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
		{
			// 분 정보 modify

		}
		if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS)
		{
			// 초 정보 modify

		}
		if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3) == BUTTON_PRESS)
		{
			// 완료 버튼
		//	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
			for(int i=0; i<5; i++)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
				HAL_Delay(500);
			}
		}
	}
}
*/
