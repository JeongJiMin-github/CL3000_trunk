/**
 * @file 	dsp_lcd7_26d_16x96.c
 * @version	0.2
 * @date 	2025/02/24
 * @brief 	LCD CL3000 Char LCD Version�� ������Ű�� ����̹�
 */

#include <stdio.h>
#include "globals.h"
#include "dsp_app.h"
#include "dsp_vfd7.h"
#include "prt_font.h"
#include "dsp_common.h"
#include "initial.h"

#ifdef _USE_LCD_7_26d_16x96_

#define MINUS_SEG             0x40
#define SPACE_SEG             0x08
#define DOT_SEG               0x01
#define COMMA_SEG             0x01
#define SPA_SEG               0x00
#define ALL_SEG               0xee
#define CURSOR_SEG			  0x08
#define OTHER_SEG             0x62
#define SLASH_SEG             0xC2

#define DIFFUSE_ALL		0
#define DIFFUSE_7SEG	1
#define DIFFUSE_12CHAR	2

#define LCD_IC1		1
#define LCD_IC2		2

#define LCD_SIGN_MAX            36 //S1~S26
#define LCD_PAGE_MAX		    2 // page 0,1
#define LCD_COLUMN_MAX		    96
#define LCD_SEG_AREA_SIZE		24
#define LCD_CHAR_AREA_SIZE		96

#define RW1087_CS1_GRAPHIC_SIZE		64
#define RW1087_CS2_SEG_START_ADDR	0x20

// ���� DISP IC(ST7522) INSTRUCTIONS(25�� 2�� ����) //
/** @brief Display on*/
#define ST7522_DISPLAY_ON	        0xAF
/** @brief Display off*/
#define ST7522_DISPLAY_OFF	        0xAE
/** @brief Set display page address*/
#define ST7522_PAGE_ADDR_SET	    0xB0
/** @brief Set MSB 4 bits of column address*/
#define ST7522_ADDR_SET_HIGH	    0x10
/** @brief Set LSB 4 bits of column address*/
#define ST7522_ADDR_SET_LOW		    0x00
/** @brief Determines the RAM display line for COM 0*/
#define ST7522_START_LINE_SET	    0x40
/** @brief Display RAM and Segment output correspondence*/
#define ST7522_ADC_SELECT		    0xA0
/** @brief Set LCD display reverse*/
#define ST7522_DISP_NORMAL		    0xA6
/** @brief Set LCD display reverse*/
#define ST7522_DISP_REVERSE		    0xA7
/** @brief Set display all point on*/
#define ST7522_DISP_ALL_ON		    0xA5
/** @brief Set display all point off*/
#define ST7522_DISP_ALL_OFF		    0xA4
/** @brief Select LCD duty*/
#define ST7522_DUTY_SELECT		    0xA8
/** @brief Select LCD bias voltage*/
#define ST7522_BIAS_SET		        0xA2
/** @brief Internal reset*/
#define ST7522_RESET		        0xE2
/** @brief Power control*/
#define ST7522_POWER_CONTROL	    0x28
/** @brief Set contrast by 64 level (V5 fine adjust)*/
#define ST7522_CONTRAST_SET		    0x81
/** @brief Internal OSC frequency adjust*/
#define ST7522_OSC_FREQUENCY_SET	0xF1
/** @brief V5 follower input voltage select(V5 coarse adjust)*/
#define ST7522_FOLLOWER_VOLT_SET	0xF8
/** @brief V5 follower amplified ratio*/
#define ST7522_AMPLIFIED_RATIO		0x20
/** @brief Booster input voltage select*/
#define ST7522_BOOSTER_VOLT_SET		0xF0
///////////////////////////////////////////////////////////////////////////

// �ű� DISP IC(RW1087) INSTRUCTIONS(25�� 2�� ����) //
/** @brief Set EXT Value*/
#define RW1087_MODE_SET				0X0C
/** @brief Display on*/
#define RW1087_DISPLAY_ON	        0x3F
/** @brief Display off*/
#define RW1087_DISPLAY_OFF	        0x3E
/** @brief Set display page address*/
#define RW1087_PAGE_ADDR_SET	    0xB8
/** @brief Set MSB 4 bits of column address*/
#define RW1087_ADDR_SET_HIGH	    0x40
/** @brief Set LSB 4 bits of column address*/
#define RW1087_ADDR_SET_LOW		    0x00
/** @brief Determines the RAM display line for COM 0*/
#define RW1087_START_LINE_SET	    0xC0
/** @brief Set LCD display Command*/
#define RW1087_DISP_COMMAND		    0x05
/** @brief Select LCD duty*/
#define RW1087_DUTY_SELECT		    0x06
/** @brief Internal OSC frequency adjust*/
#define RW1087_OSC_FREQUENCY_SET	0x07
/** @brief Set Power save mode*/
#define RW1087_POWER_MODE_SET		0x02
//////////////////////////////////////////////

/////// SCL //////////////////////
#define LCD_SCL_H   	PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1)		// DISP IC(ST7522, RW1087) SCLK "H"
#define LCD_SCL_L    PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1)		// DISP IC(ST7522, RW1087) SCLK "L"

/////// SDA //////////////////////
#define LCD_DATA_H	PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_3)		// DISP IC(ST7522, RW1087) SDA "H"
#define LCD_DATA_L  PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_3)	// DISP IC(ST7522, RW1087) SDA "L"

/////// A0(RS) //////////////////////
#define LCD_A0_H	PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_1)		// DISP IC(ST7522, RW1087) A0 "H" : "Data Mode"
#define LCD_A0_L	PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_1)	// DISP IC(ST7522, RW1087) A0 "L" : "Command Mode"

/////// CS(1, 2) //////////////////////
#define LCD_CS1_H   PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_2)		// IC1 : GLCD Control (0 ~ 63 Size)
#define LCD_CS1_L   PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_2)
#define LCD_CS2_H   PLIB_PORTS_PinSet( PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_12)	// IC2 : GLCD Control (64 ~ 96 Size) + 7-Seg (All)
#define LCD_CS2_L   PLIB_PORTS_PinClear( PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_12)    

INT8U DSP_Memory[DSP_MAX_PAGE][240]; // (96*2) + (24*2) = (graphic) + (7-seg)
POINT DSP_Memory_SegPos[DSP_MAX_DIGIT];
POINT DSP_Memory_DotPos[DSP_MAX_DIGIT];
POINT DSP_Memory_ComPos[DSP_MAX_DIGIT];
INT8U DSP_Memory_backup[240];
INT8U ContrastLevel;
extern INT8U Disp_control_ic_select;

POINT DSP_Memory_SegPos[DSP_MAX_DIGIT] = 
{
	// Tare
	{0,0},
	{2,0},
	{4,0},
	{7,0},
	{9,0},
	// Weight
	{11,0},
	{13,0},
	{15,0},
	{17,0},
	{19,0},
	{21,0},
	// Unit price
	{9,5},
	{11,5},
	{13,5},
	{15,5},
	{17,5},
	{19,5},
	{21,5},
	// Total price
	{31,2},
	{33,2},
	{35,2},
	{37,2},
	{39,2},
	{41,2},
	{43,2},
	{45,2},
};

POINT DSP_Memory_DotPos[DSP_MAX_DIGIT] =
{
	// Tare
	{2, 4},
	{4, 4},
	{7, 4},
	{9, 4},
	{0, 0},
	// Weight
	{13, 4},
	{15, 4},
	{17, 4},
	{19, 4},
	{21, 4},
	{0, 0},
	// Unit price
	{35, 1},
	{37, 1},
	{39, 1},
	{41, 1},
	{43, 1},
	{45, 1},
	{0, 0},
	// Total price
	{33, 6},
	{35, 6},
	{37, 6},
	{39, 6},
	{41, 6},
	{43, 6},
	{45, 6},
	{0, 0},
};

POINT DSP_Memory_ComPos[DSP_MAX_DIGIT] = 
{
	//Tare
	{3, 4},
	{5, 4},
	{8, 4},
	{10, 4},
	{0, 0},
	// Weight
	{14, 4},
	{16, 4},
	{18, 4},
	{20, 4},
	{22, 4},
	{0, 0},
	// Unit price
	{36, 1},
	{38, 1},
	{40, 1},
	{42, 1},
	{44, 1},
	{46, 1},
	{0, 0},
	// Total price
	{34, 6},
	{36, 6},
	{38, 6},
	{40, 6},
	{42, 6},
	{44, 6},
	{46, 6},
	{0, 0},
};

/* (0x20 ~ 0x7F) (ǥ������ ���ϴ� ���ڴ� 0x62�� �ϰ�ǥ��)*/
const INT8U AsciicodeSegTable[] = {
	/* space                                                *    ,    -    .    /  */
    0x00,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x40,0x62,0x01,0x40,0x01,0xC2,
/*   0    1    2    3    4    5    6    7    8    9                               */
    0xAF,0x06,0xCB,0x4F,0x66,0x6D,0xED,0x27,0xEF,0x6F,0x62,0x62,0x62,0x62,0x62,0x62,
	/*    A    B    C    D    E    F    G    H    I    J    K    L    M    N    O		*/
    0x62,0xE7,0xEC,0xA9,0xCE,0xE9,0xE1,0xAD,0xE6,0x04,0x8E,0xE0,0xA8,0xC5,0xC4,0xCC,
   /* P    Q    R    S    T    U    V    W    X    Y    Z                       _  */
	0xE3,0x67,0xC0,0x6D,0xE8,0xAE,0x8C,0x8D,0x49,0x6E,0xCB,0x62,0x62,0x62,0x62,0x08,
	/*    a    b    c    d    e    f    g    h    i    j    k    l    m    n    o		*/
    0x70,0xE7,0xEC,0xA9,0xCE,0xE9,0xE1,0xAD,0xE6,0x04,0x8E,0xE0,0xA8,0xC5,0xC4,0xCC,
	/*    p    q    r    s    t    u    v    w    x    y    z                          */
	0xE3,0x67,0xC0,0x6D,0xE8,0xAE,0x8C,0x8D,0x49,0x6E,0xCB,0x62,0x62,0x62,0x62,0x62
};

/* (0x80 ~ 0xFF) */
#if (FONT_CODEPAGE == CODEPAGE_1254)
/* (0x80 ~ 0xFF) */
const INT8U fontCodePage[] = {
    /* (0x80 ~ 0x8F) */
    0x62,0x62,0x00,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x6D,0x62,0x62,0x62,0x62,0x62,
    /* (0x90 ~ 0x9F) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x40,0x40,0x62,0x62,0x6D,0x62,0x62,0x62,0x62,0x6E,
    /* (0xA0 ~ 0xAF) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x44,0x40,0x62,0x01,
    /* (0xB0 ~ 0xBF) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x00,0x62,0x62,0x62,0x62,0x62,0x62,0x62,
    /* (0xC0 ~ 0xCF) */
    0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0x62,0xA9,0xE9,0xE9,0xE9,0xE9,0x04,0x04,0x04,0x04,
    /* (0xD0 ~ 0xDF) */
    0xAD,0xC4,0xCC,0xCC,0xCC,0xCC,0xCC,0x49,0xCC,0xAE,0xAE,0xAE,0xAE,0x04,0x6D,0x62,
    /* (0xE0 ~ 0xEF) */
    0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0x62,0xA9,0xE9,0xE9,0xE9,0xE9,0x04,0x04,0x04,0x04,
    /* (0xF0 ~ 0xFF) */
    0xAD,0xC4,0xCC,0xCC,0xCC,0xCC,0xCC,0x62,0xCC,0xAE,0xAE,0xAE,0xAE,0x04,0x6D,0x6E,
};
/* (0x80 ~ 0xFF) */
#elif (FONT_CODEPAGE == CODEPAGE_1256) //Display Font�� �����̱� ������ CDOEPAGE1252�� ����
const INT8U fontCodePage[] = {
    /* (0x80 ~ 0x8F) */
    0x70,0x70,0x01,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0xD6,0x70,0x70,0x70,0xBA,0x70,
    /* (0x90 ~ 0x9F) */
    0x70,0x70,0x70,0x70,0x70,0x70,0x10,0x10,0x70,0x70,0xD6,0x70,0x70,0x70,0xBA,0x76,
    /* (0xA0 ~ 0xAF) */
    0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x14,0x10,0x70,0x80,
    /* (0xB0 ~ 0xBF) */
    0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x01,0x70,0x70,0x70,0x70,0x70,0x70,0x70,
    /* (0xC0 ~ 0xCF) */
    0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x70,0xCA,0xDA,0xDA,0xDA,0xDA,0x04,0x04,0x04,0x04,
    /* (0xD0 ~ 0xDF) */
    0x3E,0x1C,0x1E,0x1E,0x1E,0x1E,0x1E,0x92,0x1E,0x6E,0x6E,0x6E,0x6E,0x76,0x70,0x70,
    /* (0xE0 ~ 0xEF) */
    0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x70,0xCA,0xDA,0xDA,0xDA,0xDA,0x04,0x04,0x04,0x04,
    /* (0xF0 ~ 0xFF) */
    0x1E,0x1C,0x1E,0x1E,0x1E,0x1E,0x1E,0x70,0x1E,0x6E,0x6E,0x6E,0x6E,0x76,0xF8,0x76
};
#elif ((FONT_CODEPAGE == CODEPAGE_1251) || (FONT_CODEPAGE == CODEPAGE_1251_MNG))
const INT8U fontCodePage[] = {
    /* (0x80 ~ 0x8F) */
    0x62,0x62,0x00,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,
    /* (0x90 ~ 0x9F) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x40,0x40,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,
    /* (0xA0 ~ 0xAF) */
    0x62,0x62,0x62,0x8E,0x62,0x62,0x62,0x62,0xE9,0x62,0x62,0x62,0x44,0x40,0x62,0x62,
    /* (0xB0 ~ 0xBF) */
    0x62,0x62,0x62,0x24,0xC0,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x6D,0x6D,0x62,
    /* (0xC0 ~ 0xCF) */
    0xE7,0x62,0x62,0x62,0xE7,0xE9,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0xE6,0xCC,0x62,
    /* (0xD0 ~ 0xDF) */
    0xE3,0xA9,0x62,0x6E,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0xEC,0x62,0x62,0x62,
    /* (0xE0 ~ 0xEF) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0xE6,0xCC,0x62,
    /* (0xF0 ~ 0xFF) */
    0xE3,0xA9,0x62,0x6E,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0xEC,0x62,0x62,0x62
};
#elif (FONT_CODEPAGE == CODEPAGE_1252_ARM)
const INT8U fontCodePage[] = {
    /* (0x80 ~ 0x8F) */
    0x70,0x70,0x01,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,
    /* (0x90 ~ 0x9F) */
    0x70,0x70,0x70,0x70,0x70,0x70,0x10,0x10,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,
    /* (0xA0 ~ 0xAF) */
    0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x4E,0x01,0x70,0x01,0x80,0x10,0x70,0x70,
    /* (0xB0 ~ 0xBF) */
    0x70,0x70,0x6E,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,
    /* (0xC0 ~ 0xCF) */
    0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x4A,0x70,0x70,0x70,0x70,0x7C,0x70,0x70,
    /* (0xD0 ~ 0xDF) */
    0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x6E,0x70,0x70,0x70,0x70,0x70,0x70,0x70,
    /* (0xE0 ~ 0xEF) */
    0x70,0x1C,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x6E,0x70,0x70,0x7C,0xD6,0x70,
    /* (0xF0 ~ 0xFF) */
    0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0xEE,0x1E,0x70,0x70,0x70,0x70
};
/* ���ο� CodePage�� �߰��Ϸ��� #elif (FONT_CODEPAGE == CODEPAGE_xxxx)�� ���� �߰��� �� */

#elif (FONT_CODEPAGE == CODEPAGE_1253)
const INT8U fontCodePage[] = {
    /* (0x80 ~ 0x8F) */
    0x62,0x62,0x01,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x6D,0x62,0x62,0x62,0xCB,0x62,
    /* (0x90 ~ 0x9F) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x40,0x40,0x62,0x62,0x6D,0x62,0x62,0x62,0xCB,0x6E,
    /* (0xA0 ~ 0xAF) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x44,0x40,0x62,0x01,
    /* (0xB0 ~ 0xBF) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x00,0x62,0x62,0x62,0x62,0x62,0x62,0x62,
    /* (0xC0 ~ 0xCF) */
    0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0x62,0xA9,0xE9,0xE9,0xE9,0xE9,0x04,0x66,0x04,0xC4,
    /* (0xD0 ~ 0xDF) */
    0xCE,0xC4,0xCC,0xCC,0xCC,0xCC,0xCC,0x49,0xCC,0xAE,0xAE,0xAE,0xAE,0x6E,0x62,0x62,
    /* (0xE0 ~ 0xEF) */
    0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0x62,0xA9,0xE9,0xE9,0xE9,0xE9,0x04,0x04,0x04,0x04,
    /* (0xF0 ~ 0xFF) */
    0xCC,0xC4,0xCC,0xCC,0xCC,0xCC,0xCC,0x62,0xCC,0xAE,0xAE,0xAE,0xAE,0x6E,0xE3,0x6E
};

#else // (FONT_CODEPAGE == CODEPAGE_1252, CODEPAGE_1251_GEO, CODEPAGE_1250_POL, CODEPAGE_1252_IND, CODEPAGE_1250, CODEPAGE_1251_KAZ)
const INT8U fontCodePage[] = {
    /* (0x80 ~ 0x8F) */
    0x62,0x62,0x01,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x6D,0x62,0x62,0x62,0xCB,0x62,
    /* (0x90 ~ 0x9F) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x40,0x40,0x62,0x62,0x6D,0x62,0x62,0x62,0xCB,0x6E,
    /* (0xA0 ~ 0xAF) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x44,0x40,0x62,0x01,
    /* (0xB0 ~ 0xBF) */
    0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x00,0x62,0x62,0x62,0x62,0x62,0x62,0x62,
    /* (0xC0 ~ 0xCF) */
    0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0x62,0xA9,0xE9,0xE9,0xE9,0xE9,0x04,0x04,0x04,0x04,
    /* (0xD0 ~ 0xDF) */
    0xCE,0xC4,0xCC,0xCC,0xCC,0xCC,0xCC,0x49,0xCC,0xAE,0xAE,0xAE,0xAE,0x6E,0x62,0x62,
    /* (0xE0 ~ 0xEF) */
    0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0x62,0xA9,0xE9,0xE9,0xE9,0xE9,0x04,0x04,0x04,0x04,
    /* (0xF0 ~ 0xFF) */
    0xCC,0xC4,0xCC,0xCC,0xCC,0xCC,0xCC,0x62,0xCC,0xAE,0xAE,0xAE,0xAE,0x6E,0xE3,0x6E
};
#endif

typedef struct{
    INT8U dispPos;
    INT8U bitPos;
}SIGN;

ROMDATA  SIGN signTable[LCD_SIGN_MAX] ={
	//   {Disp_buffer_position, bit MASK}
    // 0 ~ 4
	{23, 0x10}, //Z23     KG
	{23, 0x20}, //Z22     LB
	{30, 0x20}, //Z1      (�̻��)
	{33, 0x80}, //Z2      (�̻��)
	{35, 0x80}, //Z3      USB
	// 5 ~ 9
	{99, 0x00}, //    S6  (���Ұ�)
	{99, 0x00}, //    S7  (���Ұ�)
	{ 8, 0x80}, //Z36     <AUTO>
	{37, 0x80}, //Z4      Connect
	{32, 0x01}, //Z35     <SAVE>
	//10 ~ 14
	{99, 0x00}, //    S11 (���Ұ�)
	{ 8, 0x40}, //Z37     <PREPACK>
	{41, 0x80}, //Z6      CHECKIN
	{43, 0x80}, //Z7      <LN>
	{ 8, 0x20}, //Z38     <D/C>
	//15 ~ 19
	{45, 0x80}, //Z8      LANG
	{31, 0x01}, //Z31     <SHIFT>
	{99, 0x00}, //    S18 (���Ұ�)
	{47, 0x40}, //Z9      <TR>
	{ 7, 0x40}, //Z33     ZERO symbol
	//20 ~ 24
	{ 7, 0x80}, //Z32     TARE Tare symbol
	{ 7, 0x20}, //Z34     Stable symbol
	{30, 0x10}, //Z28     batt border
	{30, 0x04}, //Z29     batt hi
	{30, 0x08}, //Z30     batt low
	//25 ~ 29
	{ 6, 0x20}, //Z24     antenna symbol
	{ 6, 0x40}, //Z25     antenna low
	{ 6, 0x80}, //Z26     antenna mid
	{30, 0x01}, //Z27     antenna hi
	{39, 0x80}, //Z5      MASTER
	//30 ~ 34
	{99, 0x00}, //    S31 (���Ұ�) SLAVE �̻��
	{99, 0x00}, //    S32 (���Ұ�)
	{47, 0x01}, //Z19     PER_LB
	{23, 0x80}, //Z20     PER_G
	{23, 0x40}, //Z21     PER_KG
	//35
	{99, 0x00}, //    S36 (���Ұ�)
};


/**
 ********************************************************************************
 * @brief    St7522(���� DISP IC)�� command ���� �Լ�
 * @param    datcom : command\n
 * @return   none
 * @remark   
 ********************************************************************************
 */
static void st7522wr_send(INT8U Dat);
static void st7522_writecommand(INT8U datcom);
static void st7522_writedata(INT8U datcom);
static void st7522_clear(void);
static void st7522_seg_diffuse(void);
static void st7522_glcd_diffuse(void);
static void st7522_vfd7_init(void);
static void st7522_vfd7_reset(INT8U flag);
static INT8U rev_data(INT8U data);

/**
 ********************************************************************************
 * @brief    RW1087(�ű� DISP IC)�� command ���� �Լ�
 * @param    datcom : command\n
 * @return   none
 * @remark   
 ********************************************************************************
 */
static void rw1087wr_send(char Dat);
static void rw1087_writecommand(char datcom);
static void rw1087_writedata(char datcom);
static void rw1087_seg_diffuse(void);
static void rw1087_glcd_diffuse(void);
static void rw1087_vfd7_init(void);
static void rw1087_vfd7_reset(INT8U flag);
////* �׽�Ʈ�� �Լ� ���� ��� X *////
static void rw1087_full_display(char all);
static void rw1087_seg_test_display(void);
static void rw1087_delay_time(int time);
///////////////////////////////////

 /**
 ********************************************************************************
 * @brief    DISP IC�� ���� �Լ�
 * @param    datcom : command\n
 * @return   none
 * @remark   
 ********************************************************************************
 */
void delay(void);
void DSP_LCD7_diffuse(void);

//////////////////////////* Rw1087 ���� ���� �Լ� ���� *//////////////////////////
//////////////////////////////////////////////////////////////////////////////////
static void __attribute__((optimize("-O0"))) rw1087wr_send(char Dat)
{
	INT8U i;
	
	for (i = 0; i < 8 ; i++)
	{
		LCD_SCL_L;
		delay();
		
		if (Dat & 0x80)
		{
			LCD_DATA_H;
		}
		else
		{
			LCD_DATA_L;
		}
		delay();
		
		LCD_SCL_H;
		delay();

		

		Dat = Dat << 1;
	}
}

static void __attribute__((optimize("-O0"))) rw1087_writedata(char datcom)
{
	LCD_A0_H;
	delay();
	
	rw1087wr_send(datcom);
}

static void __attribute__((optimize("-O0"))) rw1087_writecommand(char datcom)
{
	LCD_A0_L;
	delay();
	
	rw1087wr_send(datcom);
}

/* Segment �������� ���� */
static void rw1087_seg_diffuse(void)
{
	int i ,current_page;
	char seg_offset, column_address;
	INT8U cmd;

	/* CS2 Control */
	LCD_CS1_H;
	LCD_CS2_L; 

	// 7-Segment Control
	for (current_page = 0; current_page < LCD_PAGE_MAX; current_page++) // Page 0, 1 Diffuse
	{
		seg_offset = current_page * LCD_SEG_AREA_SIZE;

		for (i = seg_offset; i < (seg_offset + LCD_SEG_AREA_SIZE); i++)
		{
			rw1087_writecommand(RW1087_ADDR_SET_HIGH); // High column set
			column_address = RW1087_CS2_SEG_START_ADDR + (i - seg_offset);
			rw1087_writecommand(column_address);
			rw1087_writecommand(RW1087_START_LINE_SET);
			cmd = RW1087_PAGE_ADDR_SET | current_page;
			rw1087_writecommand(cmd);
			rw1087_writedata(DSP_Memory[DspStruct.Page][i]);
		}	
	}
	LCD_CS1_H;
	LCD_CS2_H;
}

/* Grpic LCD �������� ���� */
static void rw1087_glcd_diffuse(void)
{
	INT8U i, current_page, cmd;

	for (current_page = 0; current_page < LCD_PAGE_MAX; current_page++) // Page 0, 1 Diffuse
	{
		/* CS1 Control */
		LCD_CS1_L;
		LCD_CS2_H;

		rw1087_writecommand(RW1087_ADDR_SET_HIGH); 	// High column set
		rw1087_writecommand(RW1087_ADDR_SET_LOW); 	// Low column set

		rw1087_writecommand(RW1087_START_LINE_SET);
		cmd = (RW1087_PAGE_ADDR_SET | current_page);
		rw1087_writecommand(cmd);

		for (i = 0; i < (RW1087_CS1_GRAPHIC_SIZE); i++)	// CS1 : 0�������� 63������ Graphic Lcd ����(64size)
		{
			rw1087_writedata(DspBuf[DspStruct.Page][(i * 2) + current_page]);
		}

		/* CS2 Control */
		LCD_CS1_H;
		LCD_CS2_L;

		rw1087_writecommand(RW1087_ADDR_SET_HIGH); 	// High column set
		rw1087_writecommand(RW1087_ADDR_SET_LOW); 	// Low column set
		
		rw1087_writecommand(RW1087_START_LINE_SET);
		cmd = (RW1087_PAGE_ADDR_SET | current_page);
		rw1087_writecommand(cmd);

		for (i = RW1087_CS1_GRAPHIC_SIZE; i < (LCD_CHAR_AREA_SIZE); i++)	// CS2 : 64�������� 95���� Graphic Lcd ����(32size)
		{
			rw1087_writedata(DspBuf[DspStruct.Page][(i * 2) + current_page]);
		}
	}
	LCD_CS1_H;
	LCD_CS2_H;
}

/* Initialization */
static void rw1087_vfd7_init(void)
{
	int i;
	
	LCD_CS1_H;
	LCD_CS2_H;
	LCD_SCL_H;
	delay();
	
	// IC1, IC2 Initial
	LCD_CS1_L;
	LCD_CS2_L;
	
	rw1087_writecommand(RW1087_MODE_SET | 0x01); 		// EXT = 1 Mode Set
	/* Start Double Command */
	rw1087_writecommand(RW1087_DISP_COMMAND); 			// Display Command
	rw1087_writecommand(0x00); 							// SHLA, REV, ALLON, FRSEL (Default ���)
	rw1087_writecommand(RW1087_DUTY_SELECT); 			// Duty Select
	rw1087_writecommand(0X05);							// USE 16Duty
	rw1087_writecommand(RW1087_OSC_FREQUENCY_SET); 		// Frequency Select
	rw1087_writecommand(0X0C); 							// USE 147HZ
	/* End Double Command */
	rw1087_writecommand(RW1087_POWER_MODE_SET | 0x00); 	// Normal Mode Power Set
	rw1087_writecommand(RW1087_MODE_SET | 0x00); 		// EXT = 0 Mode Set
	rw1087_writecommand(RW1087_START_LINE_SET); 		// Start Lin Set
	
	// 200ms
	for (i = 0; i < 350000; i++)
	{
		delay();
	}
	rw1087_writecommand(RW1087_DISPLAY_ON); 			// display on

	LCD_CS1_H;
	LCD_CS2_H;

	DSP_LCD7_diffuse();
}

/* DISP Reset */
static void rw1087_vfd7_reset(INT8U flag)
{
	int i;
	
	LCD_CS1_H;
	LCD_CS2_H;
	LCD_SCL_H;
	delay();
	
	// IC1, IC2 Initial
	LCD_CS1_L;
	LCD_CS2_L;
	
	rw1087_writecommand(RW1087_MODE_SET | 0x01); 		// EXT = 1 Mode Set
	/* Start Double Command */
	rw1087_writecommand(RW1087_DISP_COMMAND); 			// Display Command
	rw1087_writecommand(0x00); 							// SHLA, REV, ALLON, FRSEL (Default ���)
	rw1087_writecommand(RW1087_DUTY_SELECT); 			// Duty Select
	rw1087_writecommand(0X05);							// USE 16Duty
	rw1087_writecommand(RW1087_OSC_FREQUENCY_SET); 		// Frequency Select
	rw1087_writecommand(0X0C); 							// USE 147HZ
	/* End Double Command */
	rw1087_writecommand(RW1087_POWER_MODE_SET | 0x00); 	// Normal Mode Power Set
	rw1087_writecommand(RW1087_MODE_SET | 0x00); 		// EXT = 0 Mode Set
	rw1087_writecommand(RW1087_START_LINE_SET); 		// Start Lin Set
	
	// 200ms
	switch(flag)
	{
		case 1: // 0ms
			break;
		case 2: // 25ms
			for (i = 0; i < 43750; i++)
			{
				delay();
			}
			break;
		case 3: // 200ms  
			for (i = 0; i < 350000; i++)
			{
				delay();
			}
			break;
		default:
			break;
	}
	rw1087_writecommand(RW1087_DISPLAY_ON); 			// display on
	LCD_CS1_H;
	LCD_CS2_H;

	DSP_LCD7_diffuse();
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////* ST7522 ���� ���� �Լ� ���� *//////////////////////////
//////////////////////////////////////////////////////////////////////////////////

static void __attribute__((optimize("-O0"))) st7522wr_send(INT8U Dat)
{
	INT8U i;
	
	for (i = 0; i < 8 ; i++)
	{
		LCD_SCL_L;
		delay();
		
		if (Dat & 0x80)
		{
			LCD_DATA_H;
		}
		else
		{
			LCD_DATA_L;
		}
		delay();
		
		LCD_SCL_H;
		delay();
		
		Dat = Dat << 1;
	}
}

static void __attribute__((optimize("-O0"))) st7522_writedata(INT8U datcom)
{
	LCD_A0_H;
	delay();
	
	st7522wr_send(datcom);
}

static void __attribute__((optimize("-O0"))) st7522_writecommand(INT8U datcom)
{
	LCD_A0_L;
	delay();
	
	st7522wr_send(datcom);
}

static void __attribute__((optimize("-O0"))) st7522_clear(void)
{
	int i, j;
	INT8U cmd;
	
	
	for (j = 0; j < LCD_PAGE_MAX; j++)
	{
		cmd = ST7522_PAGE_ADDR_SET | j;
		st7522_writecommand(cmd);
		st7522_writecommand(ST7522_ADDR_SET_LOW); 	// Low column 0 set
		st7522_writecommand(ST7522_ADDR_SET_HIGH); 	// High column 0 set

		for (i = 0; i < LCD_CHAR_AREA_SIZE; i++)
		{
			st7522_writedata(0x00); // 0���� �ʱ�ȭ
		}
	}
}

// pin map�� ���ۿ� �ݴ�� �Ǿ��־� ������ �������ִ� �Լ�(ST7522 ����)
static INT8U rev_data(INT8U data)
{
	INT8U fData, rData;
	int i;
	
	rData = 0;
	
	for (i = 0; i < 8; i++)
	{
		fData = (data >> i) & 0x01;
		rData |= fData << (7 - i);
	}
	return rData;
}

/* ST7522 ���� �ֵ� ���� �Լ�(�ű� DISP IC(RW1087)���� �ֵ� ���� ����� ����) */
void VFD7_display_bright(INT8U bright)//1~9 level
{
	ContrastLevel = bright + 0x10;
	VFD7_Reset(3);
}

/* Segment �������� ���� */
static void st7522_seg_diffuse(void)
{
	int i, j, addr;
	INT8U cmd;

	/* CS2 Control */
	LCD_CS1_H;
	LCD_CS2_L;

	delay();
	
	for (j = 0; j < LCD_PAGE_MAX; j++) // Page 0, 1 Diffuse
	{
		cmd = ST7522_PAGE_ADDR_SET | j;
		st7522_writecommand(cmd); // page 0
		st7522_writecommand(ST7522_ADDR_SET_LOW); // Low column set
		st7522_writecommand(ST7522_ADDR_SET_HIGH); // High column set
		addr = j * LCD_SEG_AREA_SIZE; // ���� ������� ���׸�Ʈ ���� 1 page size = 24
		for (i = addr; i < (addr + LCD_SEG_AREA_SIZE); i++)
		{
			st7522_writedata(DSP_Memory[DspStruct.Page][i]);
		}
	}
	LCD_CS1_H;
	LCD_CS2_H;
}

/* Grpic LCD �������� ���� */
static void st7522_glcd_diffuse(void)
{
	int i, addr;
	INT8U cmd, data;

	/* CS1 Control */
	LCD_CS1_L;
	LCD_CS2_H;

	delay();
	
	// Graphic LCD
	// pin map�� �������� �ֱ� ������ page1�� �ִ� �����͸� 
	cmd = ST7522_PAGE_ADDR_SET | 0x01;
	st7522_writecommand(cmd); // page 0
	st7522_writecommand(ST7522_ADDR_SET_LOW); // Low column set
	st7522_writecommand(ST7522_ADDR_SET_HIGH); // High column set  // seg8
	addr = 0;

	for (i = 0; i < (LCD_CHAR_AREA_SIZE); i++)
	{
		data = rev_data(DspBuf[DspStruct.Page][addr + (i * 2)]);
		st7522_writedata(data);
	}

	cmd = ST7522_PAGE_ADDR_SET;
	st7522_writecommand(cmd); // page 0
	st7522_writecommand(ST7522_ADDR_SET_LOW); // Low column set
	st7522_writecommand(ST7522_ADDR_SET_HIGH); // High column set  // seg8
	addr = 0;

	for (i = 0; i < (LCD_CHAR_AREA_SIZE); i++)
	{
		data = rev_data(DspBuf[DspStruct.Page][addr + (i * 2) + 1]);
		st7522_writedata(data);
	}
	LCD_CS1_H;
	LCD_CS2_H;
}

/* Initialization (��ü���� �� �ڵ�� contrast�� �����ϰ� ����) */ 
static void st7522_vfd7_init(void)
{
	int i;
	INT8U cc;
	
	cc = get_global_bparam(GLOBAL_DISPLAY_BRIGHT);
	if ((cc < 1) || (cc > 9))
	{
		cc = 5;
		set_global_bparam(GLOBAL_DISPLAY_BRIGHT, cc);
	}
	ContrastLevel = cc + 0x10;
	
	LCD_CS1_H;
	LCD_CS2_H;
	LCD_SCL_H;
	delay();
	
	// IC1, IC2 Initial
	LCD_CS1_L;
	LCD_CS2_L;
	
	st7522_writecommand(ST7522_RESET); 					// Reset
	st7522_writecommand(0xF8); 							// Follower input voltage set
	st7522_writecommand(0x02); 							// V5 input voltage=2/6*VSS
	st7522_writecommand(ST7522_ADC_SELECT | 0x00); 		// 0: Normal, 1: Reverse
	st7522_writecommand(0xA4);
	st7522_writecommand(ST7522_DUTY_SELECT | 0x00); 	// 0: 1/17 duty, 1: 1/33 duty
	st7522_writecommand(ST7522_BIAS_SET | 0x01); 		// 0: 1/6 bias, 1: 1/5 bias
	st7522_writecommand(ST7522_AMPLIFIED_RATIO | 0x04); // V5 amplified ratio = 4
	st7522_writecommand(ST7522_CONTRAST_SET); 			// Contrast set command
	st7522_writecommand(ContrastLevel); 				// Contrast set, Default 35H // setting deault 0x15
	st7522_writecommand(ST7522_POWER_CONTROL | 0x01); 	// power control
	
	// 200ms
	for (i = 0; i < 350000; i++)
	{
		delay();
	}
	st7522_writecommand(ST7522_DISPLAY_ON); // display on
	LCD_CS1_H;
	LCD_CS2_H;

	DSP_LCD7_diffuse();
}

/* Reset */
static void st7522_vfd7_reset(INT8U flag)
{
	int i;
	
	LCD_CS1_H;
	LCD_CS2_H;
	LCD_SCL_H;
	delay();
	
	// IC1, IC2 Initial
	LCD_CS1_L;
	LCD_CS2_L;
	
	st7522_writecommand(ST7522_RESET); 					// Reset
	st7522_writecommand(0xF8); 							// Follower input voltage set
	st7522_writecommand(0x02); 							// V5 input voltage=2/6*VSS
	st7522_writecommand(ST7522_ADC_SELECT | 0x00); 		// 0: Normal, 1: Reverse
	st7522_writecommand(0xA4);
	st7522_writecommand(ST7522_DUTY_SELECT | 0x00); 	// 0: 1/17 duty, 1: 1/33 duty
	st7522_writecommand(ST7522_BIAS_SET | 0x01); 		// 0: 1/6 bias, 1: 1/5 bias
	st7522_writecommand(ST7522_AMPLIFIED_RATIO | 0x04); // V5 amplified ratio = 4
	st7522_writecommand(ST7522_CONTRAST_SET); 			// Contrast set command
	st7522_writecommand(ContrastLevel); 				// Contrast set, Default 0x15
	st7522_writecommand(ST7522_POWER_CONTROL | 0x01); 	// power control
	
	// 200ms
	switch(flag)
	{
		case 1: // 0ms
			break;
		case 2: // 25ms
			for (i = 0; i < 43750; i++)
			{
				delay();
			}
			break;
		case 3: // 200ms
			for (i = 0; i < 350000; i++)
			{
				delay();
			}
			break;
		default:
			break;
	}
	st7522_writecommand(ST7522_DISPLAY_ON); 			// display on
	LCD_CS1_H;
	LCD_CS2_H;
	
	DSP_LCD7_diffuse();
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////* Command ���� �� �ʱ�ȭ �Լ� ȣ��� ���� *//////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// LCD ��ü Diffuse
void DSP_LCD7_diffuse(void)
{
	if (Disp_control_ic_select)
	{
		rw1087_seg_diffuse();
		rw1087_glcd_diffuse();
	}
	else
	{
		st7522_seg_diffuse();
		st7522_glcd_diffuse();
	}
}

// �׷��� LCD�� ���
void DSP_LCD7_char_diffuse(void)
{
	if (Disp_control_ic_select)
	{
		rw1087_glcd_diffuse();
	}
	else
	{
		st7522_glcd_diffuse();
	}
}

// LCD Initialization
void VFD7_Init(void)
{
	if (Disp_control_ic_select)
	{
		rw1087_vfd7_init();
	}
	else
	{
		st7522_vfd7_init();
	}
	
}

// LCD Reset
void VFD7_Reset(INT8U flag)
{
	if(Disp_control_ic_select)
	{
		/* ���� RW1087 DISP IC Reset ���� �Լ� ���� ������ ESD Test�� Reset ������ ���� ���� */	
		rw1087_vfd7_reset(flag);
	}
	else
	{
		/* Init �Լ��� ��������� ����ڿ��� �Է¹��� �ֵ� ���� ����� ���� �ʿ� */
		st7522_vfd7_reset(flag);
	}
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////* Rw1087 �׽�Ʈ�� �Լ� ���� *//////////////////////////
//////////////////////////////////////////////////////////////////////////////////
static void rw1087_full_display(char all)
{
	unsigned char i, j, k;
	
	for (i = 0; i < LCD_PAGE_MAX; i++)
	{
	// i = 1;
		LCD_CS1_L;
		LCD_CS2_H;

		rw1087_writecommand(RW1087_ADDR_SET_HIGH);
		rw1087_writecommand(RW1087_ADDR_SET_LOW);

		rw1087_writecommand(RW1087_START_LINE_SET);
		rw1087_writecommand(RW1087_PAGE_ADDR_SET + i);
		
		for(j = 0; j < 64; j++)
		{
			/* 
			*  DISP ALL ON  -> all : 0xff
			*  DISP ALL OFF -> all : 0x00
			*/
			rw1087_writedata(all);
		}

		LCD_CS1_H;
		LCD_CS2_L;

		rw1087_writecommand(RW1087_ADDR_SET_HIGH);
		rw1087_writecommand(RW1087_ADDR_SET_LOW);

		rw1087_writecommand(RW1087_START_LINE_SET);
		rw1087_writecommand(RW1087_PAGE_ADDR_SET + i);
		
		for(k = 0; k < 64; k++)
		{
			/* 
			*  DISP ALL ON  -> all : 0xff
			*  DISP ALL OFF -> all : 0x00
			*/
			rw1087_writedata(all);
		}

		rw1087_writecommand(RW1087_DISPLAY_ON);
	}
	LCD_CS1_H;
	LCD_CS2_H;
	delay();
}

static void rw1087_seg_test_display(void)
{
	unsigned char i, j, k;
	
	for (i = 0; i < LCD_PAGE_MAX; i++)
	{
	// i = 1;
		LCD_CS1_H;
		LCD_CS2_L;

		rw1087_writecommand(RW1087_ADDR_SET_HIGH);
		rw1087_writecommand(RW1087_ADDR_SET_LOW | 0x20);

		rw1087_writecommand(RW1087_START_LINE_SET);
		rw1087_writecommand(RW1087_PAGE_ADDR_SET + i);
	
		if(i == 0)
			rw1087_writedata(0x0A);
		else
			rw1087_writedata(0x00);
	}
}

static void rw1087_delay_time(int time)
{
	INT8U i, j;

	for(i = 0; i < time; i++)
	{
		for(j = 0; j < 125; j++);
	}
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////* ���� �Լ� *//////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void __attribute__((optimize("-O0"))) delay(void)
{
	// Write mode minimum delay => 5V = 1.67 usec. 3V = 3.34 usec
	INT8U i = 15; // 25.05 usec
	
	while(i--);
}

extern HUGEDATA INT8U *DspFontAddrHpf;

// Seg ���� ���ۿ� ������ �־��ִ� �Լ�
void VFD7_display_char(INT8U nDigitPos, INT8U cData)
{
	INT8U i;
	INT8U fData, oData, hData, lData;
	INT8U digitPos;
	INT8U mask;
	POINT dPos;
	
//	DspStruct.Page = 0;
	
	if (nDigitPos < DSP_MAX_DIGIT)
	{
		// Offset ���κ� ����
		// nDigitPos 0���� ���´�.
		digitPos = nDigitPos;
		
		// ������ �з�
		if (cData >= 0x20 && cData <= 0x7F)
		{
			fData = AsciicodeSegTable[cData - 0x20];
		}
		else if (cData >= 0x80 && cData <= 0xFF)
		{
			fData = fontCodePage[cData - 0x80];
		}
		else if (cData == CURSOR_CHAR)
		{
			fData = CURSOR_SEG;
		}
		else if (cData == '\0')
		{
			fData = '\0';
		}
		else
		{
			fData = OTHER_SEG;
		}
		
		if (cData == '.')
		{
			if (digitPos != 4 && digitPos != 10 && digitPos != 17 && digitPos != 25) // dot,comma�� ���� ����
			{
				dPos = DSP_Memory_DotPos[digitPos];
				fData = 0x01 << dPos.y;
				DSP_Memory[DspStruct.Page][dPos.x] |= fData;
			}
		}
		else if (cData == ',')
		{
			if (digitPos != 4 && digitPos != 10 && digitPos != 17 && digitPos != 25) // dot,comma�� ���� ����
			{
				dPos = DSP_Memory_ComPos[digitPos];
				fData = 0x01 << dPos.y;
				DSP_Memory[DspStruct.Page][dPos.x] |= fData;
			}
		}
		else
		{
			// digitPos�� �޸𸮻� �ּҷ� ��ȯ
			dPos = DSP_Memory_SegPos[digitPos];
			mask = 0x0F << dPos.y;
			hData = fData >> 4; // ������ 2byte�� ������
			lData = fData & 0x0F;

			// Segment ���� ���
			oData = DSP_Memory[DspStruct.Page][dPos.x] & ~(mask); // �ٲ��ִ� �κ� �̿� ������ ��ȣ
			DSP_Memory[DspStruct.Page][dPos.x] = (hData << dPos.y) | oData;
			oData = DSP_Memory[DspStruct.Page][dPos.x + 1] & ~(mask);
			DSP_Memory[DspStruct.Page][dPos.x+1] = (lData << dPos.y) | oData;
			if (dPos.y > 4) // page address�� �Ѿ�� ���
			{
				mask = 0xFF << (dPos.y - 4);
				oData = DSP_Memory[DspStruct.Page][dPos.x + LCD_SEG_AREA_SIZE] & mask;
				DSP_Memory[DspStruct.Page][dPos.x + LCD_SEG_AREA_SIZE] = (hData >> 3) | oData;
				oData = DSP_Memory[DspStruct.Page][dPos.x + LCD_SEG_AREA_SIZE + 1] & mask;
//				cData = lData >> 3;
				DSP_Memory[DspStruct.Page][dPos.x + LCD_SEG_AREA_SIZE + 1] = (lData >> 3) | oData;
			}
			
			if (digitPos != 4 && digitPos != 10 && digitPos != 17 && digitPos != 25) // dot,comma�� ���� ����
			{
				// dot clear
				dPos = DSP_Memory_DotPos[digitPos];
				fData = 0x01 << dPos.y;
				DSP_Memory[DspStruct.Page][dPos.x] &= ~fData;
				// comma clear
				dPos = DSP_Memory_ComPos[digitPos];
				fData = 0x01 << dPos.y;
				DSP_Memory[DspStruct.Page][dPos.x] &= ~fData;
			}
		}
	}
	else return;
}

void VFD7_display_char_graphic(INT16U x, char cData)
{
	INT8U i;
	INT8U charPos;
	INT16U font_addr;
	
	if (x < DSP_MAX_CHAR * LCD_CHAR_FONT_WIDTH)
	{
		charPos = x * 2;
		
		// 16x8 font
		font_addr = ((INT8U)cData)&0x00ff;
		font_addr *= 2;
		font_addr *= 8;
		font_addr += 64;
		for (i = 0; i < LCD_CHAR_FONT_WIDTH * 2; i++)
		{
			DspBuf[DspStruct.Page][charPos] = DspFontAddrHpf[font_addr + i];
			charPos++;
		}
	}
}

void VFD7_display_triangle(INT8U nDigitPos, INT8U onoff)
{
	INT8U pos;
	INT8U seg;
	INT8U subSignFlag = 0;
	pos = signTable[nDigitPos].dispPos;
	seg = signTable[nDigitPos].bitPos;
	
#ifdef USE_PB_BATTERY
	if(nDigitPos > SIGN_BAT3) return;
#else
	if(nDigitPos >= LCD_SIGN_MAX) return;
#endif
	
	if (onoff) DSP_Memory[DspStruct.Page][pos] |= seg;
	else DSP_Memory[DspStruct.Page][pos] &= ~seg;
	
#ifdef USE_PB_BATTERY
	else if(nDigitPos == SIGN_BAT1){		
		if(onoff) DSP_Memory[DspStruct.Page][0] |= 0x02;    // TEST ONLY: BAT1 Signal
		else  DSP_Memory[DspStruct.Page][0] &= 0xFD;   //			
	}
	else if(nDigitPos == SIGN_BAT2){		
		if(onoff) DSP_Memory[DspStruct.Page][1] |= 0x02;    // TEST ONLY: BAT2 Signal
		else  DSP_Memory[DspStruct.Page][1] &= 0xFD;   //			
	}
	else if(nDigitPos == SIGN_BAT3){		
		if(onoff) DSP_Memory[DspStruct.Page][2] |= 0x02;    // TEST ONLY: BAT3 Signal
		else  DSP_Memory[DspStruct.Page][2] &= 0xFD;   //			
	}
#endif
	
	// SubSign ǥ��(Ʈ���̾ޱ� �Ʒ� ����)
	if ((nDigitPos == 3) || (nDigitPos == 4) || (nDigitPos == 8) || (nDigitPos == 29) || (nDigitPos == 12) || (nDigitPos == 13) || (nDigitPos == 15)) // Z2~Z8
	{
		pos += 1;
		subSignFlag = 1;
	}
	else if ((nDigitPos == 2) || (nDigitPos == 18)) // Z1, Z9
	{
		seg = 0x80;
		subSignFlag = 1;
	}
	
	if	(subSignFlag = 1)
	{
		if (onoff) DSP_Memory[DspStruct.Page][pos] |= seg;
		else DSP_Memory[DspStruct.Page][pos] &= ~seg;
	}
}

void VFD7_display_triangle_all(INT8U onoff)
{
	INT8U i;
	
	for (i = 0; i < LCD_SIGN_MAX; i++)
	{
		VFD7_display_triangle(i, onoff);
	}
}

static inline void __display_string_reverse(char *dataString, char size, INT8U mode)
{
	INT16S start, end;

	start = display_parameter.mode_pos[mode];
	end = start + display_parameter.mode_len[mode] - 1;

	while (size && start <= end) {
		char ch = dataString[--size];

		if (ch == '.' || ch == ',') {
			if (size - 1 >= 0) {
				VFD7_display_char(end, dataString[size - 1]);
				size--;
			}
		}
		VFD7_display_char(end, ch);
		end--;
	}

	while(start <= end)
	{
		VFD7_display_char(end, '\0');
		end--;
	}
}

static inline void __display_string(char *dataString, INT8U nDigitPos, char size, INT8U type_7seg)
{
	INT8U i,j;

	i=nDigitPos;
	j=0; 

	if(i == 0)
    {
        i = nDigitPos;
    }

//    if ((nDigitPos+size)>=DSP_MAX_DIGIT) return;
	while ((*(dataString) != '\0') && (j < size))
	{
		VFD7_display_char(i, *dataString);
		
		if (type_7seg)
		{
			if (*(dataString + 1) != '\0')
			{
				if (*(dataString + 1) == '.' || *(dataString + 1) == ',')
				{
					VFD7_display_char(i, *(dataString + 1));
					dataString++;
				}
			}
		}
		
		dataString++;
		i++;
		j++;
	}   
	//clear remain area
	while(j<size)
	{
		VFD7_display_char(i, '\0');
		i++;
		j++;
	}
	//	DSP_LCD7_diffuse();//comm speed slow	
}

void VFD7_display_string(HUGEDATA char *dataString, INT8U nDigitPos, char size, INT8U mode, BOOLEAN reverse) {
	//DSP_str_common((HUGEDATA char *)dataString, nDigitPos, size);
	INT8U type_7seg;
	
	if (nDigitPos < (INT8U)DSP_MAX_DIGIT) type_7seg = 1;
	else type_7seg = 0;
	
	if (reverse && mode != DISPLAY_UNDEFINED) {
		__display_string_reverse(dataString, size, mode);
	} else {
		__display_string(dataString, nDigitPos, size, type_7seg);
	}
}

void VFD7_Diffuse(void)
{
	DSP_LCD7_diffuse();	
}

void VFD7_Fill_Buffer(unsigned char x)
{
	memset(DSP_Memory[DspStruct.Page], x, LCD_SEG_AREA_SIZE * LCD_PAGE_MAX);
}

void VFD7_Fill_Buffer_CHAR(unsigned char x)
{
	memset(DspBuf[DspStruct.Page], x, LCD_CHAR_AREA_SIZE * LCD_PAGE_MAX);
}

void VFD7_save_restore(INT8U x)
{
	if (x) {	// backup
		memcpy(DSP_Memory_backup, DSP_Memory[DspStruct.Page], sizeof(DSP_Memory[DspStruct.Page]));
	} else {	// restore
		memcpy(DSP_Memory[DspStruct.Page], DSP_Memory_backup, sizeof(DSP_Memory[DspStruct.Page]));
	}
}

#endif