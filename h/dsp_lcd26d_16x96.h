/*****************************************************************************
|*			
|*  Copyright		:	(c) 2002 CAS
|*  Filename		:	lcd.h
|*  Version			:	0.1
|*  Programmer(s)	:	Hong Yi-Phyo (HYP)
|*  Created			:	2003/04/14
|*  Description		:	CLP BASIC LCD global variable define    
|*				
****************************************************************************/

#ifdef _USE_LCD26d_16x96_

#ifndef _DSP_LCD26D_16x96_H
#define _DSP_LCD26D_16x96_H

#define LCD_CHAR_FONT_WIDTH		8

typedef union
{
    unsigned short aword;
    struct sprt
    {
        unsigned char l_byte;
	unsigned char h_byte;
    }twobyte;
}word2byteunion;

extern void LCD_Init(void);
extern void LCD_refresh(void);
extern void LCD_Check(unsigned char x, unsigned char y);
#if defined(USE_ARAB_FONT) || defined(HEBREW_FONT)
extern INT8U Dsp_Write_1CharLong(POINT p, INT16U c);
#endif
extern INT8U Dsp_Write_1Char(POINT,char);
#ifdef USE_VIETNAM_FONT
extern INT8U Dsp_Write_1Char16(POINT,INT16U);
#endif
extern INT8U Dsp_Write_2Char(POINT p, INT16U chw);
extern void LCD_BufferWriteTriangle(INT8U nDigitPos, INT8U onoff);
extern	void DSP_LCD7_Fill_Buffer(unsigned char x);
extern  INT8U   UseSaleModeEditFlag;
#endif	//_DSP_LCD26D_16x96_H
#endif	//_USE_LCD26d_16x96_

