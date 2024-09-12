/*****************************************************************************
|*  Copyright		:	(c) 2003 CAS
|*  Filename		:	prt_app.c
|*  Version		:	0.1
|*  Modified		:	2004/08/20
|*  Modifier		:	JHC
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "globals.h"

#include "common.h"
#include "commbuf.h"
#include "prtdrv.h"
#include "timer.h"
#include "serial.h"
// #include "flash.h"
#include "sensor.h"
#include "ram.h"
#include "prt_render.h"
#include "bar_common.h"
#include "prt_font.h"
#include "dsp_common.h"
#include "prt_common.h"
#include "sensor.h"
#include "prt_interpreter.h"
#include "dsp_vfd7.h"

#include "adm.h"
#include "key.h"
#include "key_typedef.h"
#include "key_app.h"
#include "main.h"
#include "cal.h"
#include "debug.h"
#include "initial.h"
#include "plu_manager.h"
#include "caption_app.h"
#include "menu.h"
#include "cursor.h"
#include "flash_app.h"
#include "menu.h"
#include "prt_app.h"
#include "prt_cmd.h"
#include "mode_calibration.h"
#include "mode_commun.h"
#include "network_command.h"
#include "pgm_app.h"
#include "dsp_app.h"

//void test_receipt(void)
//{
//	char TestCmd[100];
//	
//	sprintf(TestCmd, "012345678901234567890123456789\nABCDEFG\n");
//	PutDataRxBuf(&CommBufPrt,TestCmd,strlen(TestCmd));
//
//	sprintf(TestCmd, "%c!",0x1b);
//	PutDataRxBuf(&CommBufPrt,TestCmd,strlen(TestCmd));
//}

/*
//void printer_modeset_callback(INT8U mode)
{
	CAPTION_STRUCT cap;
	POINT disp_p;
	char string_buf[32];
	INT8U len, i;

	disp_p = point(16, 2*6); 
	switch(mode) {
		case 0 : // Label Mode
			caption_split_by_code(0x9352, &cap);
			break;
		case 1 : // Receipt Mode
			caption_split_by_code(0x9353, &cap);
			break;
		default :// Invalid Value
			caption_split_by_code(0x9354, &cap);
			break;
			
	}
	len = strlen((char *)cap.form);
	memcpy((INT8U *)string_buf, (INT8U*)cap.form, len);
	for(i = len; i < 31; i++) {
		string_buf[i] = 0x20;
	}
	string_buf[31] = 0x00;
	
	DspStruct.DirectDraw = 1;
	Dsp_Write_String(disp_p, string_buf);
	DspStruct.DirectDraw = 0;
}
*/

// Deleted by CJK 20051219
//DELINT8U PrtTypeDefault[3][2] = {//GapMode,	Rewind
//DEL				{GAP,		1},
//DEL				{NO_GAP,	0},
//DEL				{NO_GAP,	0}
//DEL};

//DELvoid printer_configure_papertype(INT8U type)
//DEL{
//DEL////	// Set Prt Mode
//DEL////	PrtStruct.Mode = PrtTypeDefault[type][0];
//DEL////	Flash_swrite(PRT_FLASH_BASE+FLASH_OFFSET_Mode,(HUGEDATA INT8U *)&PrtStruct.Mode, 1);
//DEL////	Flash_flush_nvbuffer();//HYP 20040204 FLASH NVRAM 
//DEL////	PrtLoadModeSetting(PrtStruct.Mode);
//DEL
//DEL	// Set GapMode
//DEL	PrtDrvSetLabelGapMode((BOOLEAN)PrtTypeDefault[type][0]);
//DEL	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_LabelGapMode, (HUGEDATA INT8U *)&PrtDrvStruct.LabelGapMode, 1);
//DEL
//DEL	// Set RewindEnable
//DEL	PrtDrvStruct.RewindEnable = PrtTypeDefault[type][1];
//DEL	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_RewindEnable, (HUGEDATA INT8U *)&PrtDrvStruct.RewindEnable, 1);
//DEL
//DEL	Flash_flush_nvbuffer();//HYP 20040204 FLASH NVRAM
//DEL	// Set PeelEnable
//DEL	//PrtDrvStruct.PeelEnable = (BOOLEAN)set_val[0];
//DEL	//Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PeelEnable, (HUGEDATA INT8U *)&PrtDrvStruct.PeelEnable, 1);
//DEL
//DEL}

//extern INT8U MenuIndex;
//extern MENU_STR Menu[MAX_MENU_INDEX];
//void printer_methodtype_callback(INT8U type)
extern INT8U UseDspReverse;

void printer_methodtype_callback(long type)
{
	CAPTION_STRUCT cap;
	POINT disp_p;
	char string_buf[MAX_MENU_CHAR_BUF];
	INT8U len, i;
	//char backup_directdraw;
//	INT16U cap_code[2][3] = {{0x9752, 0x9753, 0x9754},	// Prog. Mode 
//				 {0x9352, 0x9353, 0x9354}};	// Cal Mode
	INT8U cap_id;
	INT16U w;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}

	disp_p = point(CallbackEditPosition_y*Startup.menu_ygap, CallbackEditPosition_x*Startup.menu_xgap); 
	w = Dsp_GetChars();
	switch(type) {
		case 0 : // 0.Label 
			caption_split_by_code(0x9352, &cap,0);
//			caption_split_by_code(cap_code[cap_id][0], &cap,0);
			break;
		case 1 : // 1.Ticket
			caption_split_by_code(0x9353, &cap,0);
//			caption_split_by_code(cap_code[cap_id][1], &cap,0);
#ifdef  USE_ONLY_LABEL                        
                        if (!(status_scale.restrict&FUNC_CLERK)) 
                        {
#ifdef HEBREW_FONT
						Convert_HebrewPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0); 
						hebrew_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
						disp_p.x = 202 - disp_p.x;
						Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
						Convert_ArabPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0, 0);
						arab_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
						disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
						Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
                		Dsp_Write_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
#endif
		                return;
                	}
#endif                        
			break;
		case 2 : // 1.Contineus label
			caption_split_by_code(0x9354, &cap,0);
//			caption_split_by_code(cap_code[cap_id][2], &cap,0);
			break;
		default :// Invalid Value
//			caption_split_by_code(0xF012, &cap,0);
			strcpy(cap.form,global_message[MSG_INVALID_VALUE]);
			break;
			
	}
	len = strlen((char *)cap.form);
	memcpy((INT8U *)string_buf, (INT8U*)cap.form, len);
	for(i = len; i < w; i++) {
		string_buf[i] = 0x20;
	}
	string_buf[w] = 0x00;
	
	//backup_directdraw = DspStruct.DirectDraw;
	//DspStruct.DirectDraw = 1;
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)string_buf, strlen(string_buf), 0, 0); 
	hebrew_codetemp[strlen(string_buf)] = 0;
	disp_p.x = 202 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)string_buf, strlen(string_buf), 0, 0, 0); 
	arab_codetemp[strlen(string_buf)] = 0;
	disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
    UseDspReverse = ON;
    DspStruct.Reverse = 1;
    if (!menuDispOn)
  #endif
	Dsp_Write_String(disp_p, string_buf);
#endif
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
    UseDspReverse = OFF;
    DspStruct.Reverse = 0;
#endif
	//DspStruct.DirectDraw = backup_directdraw;
}

//void printer_papertype_endcallback(INT8U index)
//{
//	INT8U i;
//	//printer_configure_papertype(type);
//	if(*(Menu[index].pval) < 3) {
//		for(i = 0; i < MenuIndex; i++) {
//			if(Menu[i].id == 4)
//				*(Menu[i].pval) = PrtTypeDefault[*(Menu[index].pval)][1];	// rewind motor
//			if(Menu[i].id == 5)
//				*(Menu[i].pval) = PrtTypeDefault[*(Menu[index].pval)][0];	// gap mode
//		}
//	}
//}

////void printer_papertype_callback(INT8U type)
////{
////	CAPTION_STRUCT cap;
////	POINT disp_p;
////	char string_buf[32];
////	INT8U len, i;
////	char backup_directdraw;
////
////
////	disp_p = point((Startup.menu_type+1)*Startup.menu_ygap, 4*Startup.menu_xgap); 
////	switch(type) {
////		case 0 : // Label Paper
////			caption_split_by_code(0x9365, &cap,0);
////			break;
////		case 1 : // Ticket Paper
////			caption_split_by_code(0x9366, &cap,0);
////			break;
////		case 2 : // Continuous Label
////			caption_split_by_code(0x9367, &cap,0);
////			break;
////		case 3 : // User Defined Paper
////			caption_split_by_code(0x9368, &cap,0);
////			break;
////		default :// Invalid Value
//////			caption_split_by_code(0xF012, &cap,0);
////			strcpy(cap.form,global_message[MSG_INVALID_VALUE]);
////			break;
////			
////	}
////	len = strlen((char *)cap.form);
////	memcpy((INT8U *)string_buf, (INT8U*)cap.form, len);
////	for(i = len; i < 31; i++) {
////		string_buf[i] = 0x20;
////	}
////	string_buf[31] = 0x00;
////	
////	backup_directdraw = DspStruct.DirectDraw;
////	DspStruct.DirectDraw = 1;
////	Dsp_Write_String(disp_p, string_buf);
////	DspStruct.DirectDraw = backup_directdraw;
////}


INT8U PrtPaperType;	// Flash 저장 필요

void printer_mode(void) // Modified by CJK 20050927
{
	INT32U set_val[1];
	INT8U result;
	CAPTION_STRUCT cap;
	INT16U cap_code[2][2] = {{0x1731, 0x9751}, 	// Prog. Mode
			 	 {0x8310, 0x9351}};	// Cal Mode
	INT8U cap_id;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}
#ifdef  USE_ONLY_LABEL
PRTMODE_INPUT:        
#endif
	set_val[0] = (INT32U)PrtPaperType;

	Menu_Init();

	//caption_split_by_code(0x1731, &cap,0);//"Printer Mode"	0x8310
	caption_split_by_code(cap_code[cap_id][0], &cap,0);//"Printer Mode"	0x8310
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(cap_code[cap_id][1], &cap,0);//"Mode (0:Label, 1:Receipt)  : [ ]"
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length, 
				cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, printer_methodtype_callback);
	Menu_SetCallbackMode(1, 1);	// below

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) 
        {
#ifdef  USE_ONLY_LABEL  //KKH for CL3500             
                if (!(status_scale.restrict&FUNC_CLERK)) 
		{
			if (set_val[0] == 1) 
			{
				BuzOn(2);
				goto PRTMODE_INPUT;
			}
		}
#endif                
		status_scale.cur_labelid = 0;	// Reload Label : Added by CJK 20050205
		printer_modeset((INT8U)set_val[0]);
		label_load(status_scale.cur_labelid);
	}
}

void printer_modeset(INT8U mode)	// sensor & motor	// mode = 0:Label, 1:Ticket, 2:Cont.Label, 9:current type
{
	INT8U temp;
	INT8U peelEnable;	// 0 : disable, 1: enable
	INT8U rewindEnable;	// 0 : disable, 1: enable
	INT8U gapMode;		// 0 : no gap, 1: gap
	INT16U width;		// label width or ticket width
	INT16U height;		// label height or ticket feed length
	INT16U endMargin;	// label gap length or ticket end margin

	PrtPaperType = get_global_bparam(GLOBAL_PAPER_TYPE);
	if (mode == 9) 
	{
	} 
	else 
	{
		if (PrtPaperType != mode)
		{
			PrtPaperType = mode;
			set_global_bparam(GLOBAL_PAPER_TYPE, PrtPaperType);
		}
	}
	if (PrtPaperType > 2) 
	{
		PrtPaperType = 0;
		set_global_bparam(GLOBAL_PAPER_TYPE, PrtPaperType);
	}

	width = get_global_wparam(GLOBAL_PRT_PAPER_WIDTH + PrtPaperType * 2);
	if (width > 56 * 8 || width < 10 * 8)
	{
		printer_modeset_default();
		width = get_global_wparam(GLOBAL_PRT_PAPER_WIDTH + PrtPaperType * 2);
	} 
	height = get_global_wparam(GLOBAL_PRT_PAPER_HEIGHT + PrtPaperType * 2);
	endMargin = get_global_wparam(GLOBAL_PRT_PAPER_ENDMARGIN + PrtPaperType * 2);
	temp = get_global_bparam(GLOBAL_PRT_SENSOR_MOTOR + PrtPaperType);
	peelEnable = temp & 0x01;
	rewindEnable = (temp >> 1) & 0x01;
	gapMode = (temp >> 2) & 0x01;


	PrtDrvStruct.PeelEnable = peelEnable; 
	PrtDrvStruct.RewindEnable = rewindEnable;
	PrtDrvSetLabelGapMode(gapMode);
	if(PrtFlashStruct.PeelEnable != PrtDrvStruct.PeelEnable)
	{
#if defined (USE_LOTTEMART_DEFAULT) || defined (USE_HYUNDAI_DEFAULT) || defined (USE_CHINA_DEFAULT)
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PeelEnable, (HUGEDATA INT8U *)&PrtDrvStruct.PeelEnable, 0);
#else
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PeelEnable, (HUGEDATA INT8U *)&PrtDrvStruct.PeelEnable, 1);
#endif
	}
	if(PrtFlashStruct.RewindEnable != PrtDrvStruct.RewindEnable)
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_RewindEnable, (HUGEDATA INT8U *)&PrtDrvStruct.RewindEnable, 1);
	if(PrtFlashStruct.LabelGapMode != PrtDrvStruct.LabelGapMode)
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_LabelGapMode, (HUGEDATA INT8U *)&PrtDrvStruct.LabelGapMode, 1);

	switch (PrtPaperType) 
	{
		case 0:	// label 
		case 2: // continuous label
			PrtStruct.Mode = LABEL;
			PrtStruct.LabelWidth = width;
			PrtStruct.LabelHeight = height;
			PrtDrvStruct.GapLength = endMargin;
			if(PrtFlashStruct.Mode != PrtStruct.Mode)
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_Mode,(HUGEDATA INT8U *)&PrtStruct.Mode, 1);
			if(PrtFlashStruct.LabelWidth != PrtStruct.LabelWidth)
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_LabelWidth, (HUGEDATA INT8U *)&PrtStruct.LabelWidth, 2);
			if(PrtFlashStruct.LabelHeight != PrtStruct.LabelHeight)
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_LabelHeight, (HUGEDATA INT8U *)&PrtStruct.LabelHeight, 2);
			if(PrtFlashStruct.GapLength != PrtDrvStruct.GapLength)
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapLength, (HUGEDATA INT8U *)&PrtDrvStruct.GapLength, 2);
			break;
		case 1: // ticket
			PrtStruct.Mode = RECEIPT;
			PrtStruct.ReceiptWidth = width;
			PrtStruct.ReceiptHeight = height;
			PrtDrvStruct.ReceiptFeedHeight = endMargin;
			if(PrtFlashStruct.Mode != PrtStruct.Mode)
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_Mode,(HUGEDATA INT8U *)&PrtStruct.Mode, 1);
			if(PrtFlashStruct.ReceiptWidth != PrtStruct.ReceiptWidth)
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_ReceiptWidth, (HUGEDATA INT8U *)&PrtStruct.ReceiptWidth, 2);
			if(PrtFlashStruct.ReceiptHeight != PrtStruct.ReceiptHeight)
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_ReceiptHeight, (HUGEDATA INT8U *)&PrtStruct.ReceiptHeight, 2);
			if(PrtFlashStruct.ReceiptFeedHeight != PrtDrvStruct.ReceiptFeedHeight)
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_ReceiptFeedHeight, (HUGEDATA INT8U *)&PrtDrvStruct.ReceiptFeedHeight, 2);
			break;
	}
	Flash_flush_nvbuffer();

	Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);

	PrtLoadModeSetting(PrtStruct.Mode);
	// CJK080226
	// DEL : cur_labelid가 초기화 되어있지 않는 경우 발생
	//       printer_modeset()에서 label_load()를 분리함. 
	//label_load(status_scale.cur_labelid);	

	//set_global_bparam(GLOBAL_PRT_TICKET_FONT_SIZE, 0x66 ); //defualt :ticket print font size
}

void printer_modeset_default(void)
{
	INT8U paperType;
	INT8U peelEnable;	// 0 : disable, 1: enable
	INT8U rewindEnable;	// 0 : disable, 1: enable
	INT8U gapMode;		// 0 : no gap, 1: gap
	INT16U width;		// label width or ticket width
	INT16U height;		// label height or ticket feed length
	INT16U endMargin;	// label gap length or ticket end margin

	for (paperType = 0; paperType < 3; paperType++)
	{
		switch (paperType) {
			case 0:	// label 
#if defined (USE_LOTTEMART_DEFAULT) || defined (USE_HYUNDAI_DEFAULT) || defined(USE_CHINA_DEFAULT)
				peelEnable = DISABLE;
#else
				peelEnable = ENABLE;
#endif
				rewindEnable = ENABLE;
				gapMode = GAP;
#ifdef USE_INDIA_TATA_DEFAULT
				width = 48 * 8;
#else
				width = 56 * 8;
#endif
#ifdef USE_INDIA_TATA_DEFAULT
				height = 30 * 8;
#else
				height = 40 * 8;
#endif
#ifdef USE_INDIA_TATA_DEFAULT
				endMargin = 3 * 8;
#else
				endMargin = 2 * 8;
#endif
				break;
			case 1: // ticket
				peelEnable = DISABLE; 
				rewindEnable = DISABLE;
				gapMode = NO_GAP;
				width = 56 * 8;
				height = 20 * 8;
				endMargin = 30 * 8;
				break;
			case 2: // continuous label
				peelEnable = DISABLE; 
				rewindEnable = ENABLE;
				gapMode = NO_GAP;
				width = 56 * 8;
				height = 40 * 8;
				endMargin = 30 * 8;
				break;
		}
		printer_save_sensor(paperType, peelEnable, rewindEnable, gapMode);
		printer_save_size(paperType, width, height, endMargin);
	}
}

void printer_save_sensor(INT8U paperType, INT8U peelEnable, INT8U rewindEnable, INT8U gapMode)
{
	INT8U temp;

	temp = get_global_bparam(GLOBAL_PRT_SENSOR_MOTOR + paperType);
	if (peelEnable)
	{
		temp |= (0x01 << 0);
	}
	else
	{
		temp &= ~(0x01 << 0);
	}

	if (rewindEnable)
	{
		temp |= (0x01 << 1);
	}
	else
	{
		temp &= ~(0x01 << 1);
	}

	if (gapMode)
	{
		temp |= (0x01 << 2);
	}
	else
	{
		temp &= ~(0x01 << 2);
	}
	set_global_bparam(GLOBAL_PRT_SENSOR_MOTOR + paperType, temp);
}

////////////////////////////////////////////////
//	INT16U width;		// label width or ticket width
//	INT16U height;		// label height or ticket feed length
//	INT16U endMargin;	// label gap length or ticket end margin
void printer_save_size(INT8U paperType, INT16U width, INT16U height, INT16U endMargin)
{
	set_global_wparam(GLOBAL_PRT_PAPER_WIDTH + paperType * 2, width);
	set_global_wparam(GLOBAL_PRT_PAPER_HEIGHT + paperType * 2, height);
	set_global_wparam(GLOBAL_PRT_PAPER_ENDMARGIN + paperType * 2, endMargin);
}

#define M_ID_LABSIZE_WIDTH  1
#define M_ID_LABSIZE_HEIGHT 2
#define M_ID_LABSIZE_GAPLEN 3
void printer_set_labsize(void)
{
	char string_buf[10];
	INT32U temp_value[3];
	
	INT8U id_width;
	INT8U id_height;
	INT8U id_gaplen;

	id_width = Menu_GetMenuId(M_ID_LABSIZE_WIDTH);
	if(id_width == 0xff) return;
	id_height = Menu_GetMenuId(M_ID_LABSIZE_HEIGHT);
	if(id_height == 0xff) return;
	id_gaplen = Menu_GetMenuId(M_ID_LABSIZE_GAPLEN);
	if(id_gaplen == 0xff) return;
	
	if(!Prt_CheckPeelAndHead(ON)) {//check peel and headup
		return;
	}
	if(PrtDrvStruct.LabelGapMode == GAP) {
		string_buf[0] = GS;
		string_buf[1] = 'A';
		PutDataRxBuf(&CommBufPrt, string_buf, 2);
//		while(CheckRxBuf(&CommBufPrt)) {
//			CASInterpreter(&CommBufPrt);
//		}
//		while(PrtDrvStruct.PrtState != PRT_STATE0I);
		PrintListWaitEndWithoutComm();
		temp_value[0] = PrtStruct.LabelWidth/8;
		if(temp_value[0] == 56) {
			temp_value[0] = 60;
		} else {
			temp_value[0] = temp_value[0] + 2;
		}
		temp_value[1] = PrtStruct.LabelHeight/8;
		if(PrtStruct.LabelHeight%8 >= 4) temp_value[1] = temp_value[1] + 1;
		temp_value[2] = PrtDrvStruct.GapLength/8;
		if(PrtDrvStruct.GapLength%8 >= 4) temp_value[2] = temp_value[2] + 1;
		
		*Menu[id_width].pval = temp_value[0];
		*Menu[id_height].pval = temp_value[1];
		*Menu[id_gaplen].pval = temp_value[2];
	} else {
		BuzOn(2);	
	}
}

void printer_labelset(void)
{
	INT16U backup_gap_length;
	short backup_label_width, backup_label_height;
	CAPTION_STRUCT cap;
	INT32U temp_value[3];
	INT16U cap_code[2][6] = {{0x9740, 0x9741, 0x9742, 0x9743, 0x9744, 0x9749},	// Prog. Mode  
				 {0x9340, 0x9341, 0x9342, 0x9343, 0x9344, 0x9345}};	// Cal Mode
	INT8U cap_id;
	INT8U result;
#if defined(USE_ARAB_CONVERT) || defined(HEBREW_FONT)
	INT32U tempP_value[3];
#endif
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}
	backup_label_width  = PrtFlashStruct.LabelWidth;
	backup_label_height = PrtFlashStruct.LabelHeight;
	backup_gap_length   = PrtFlashStruct.GapLength;

	temp_value[0] = PrtFlashStruct.LabelWidth/8;
	if(temp_value[0] == 56) {
		temp_value[0] = 60;
	} else {
		temp_value[0] = temp_value[0] + 2;
	}
	temp_value[1] = PrtFlashStruct.LabelHeight/8;
	temp_value[2] = PrtFlashStruct.GapLength/8;

	Menu_Init();

	if(PrtPaperType == 2) {		// Continuous Label
		caption_split_by_code(cap_code[cap_id][5], &cap,0);
	} else {			// Label
		caption_split_by_code(cap_code[cap_id][0], &cap,0);
	}
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(cap_code[cap_id][1], &cap,0); // Width
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_LABSIZE_WIDTH, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value[0], NULL, NULL, NULL);
	Menu_AddKeyFunction(M_ID_LABSIZE_WIDTH, KP_TEST, printer_set_labsize);

	caption_split_by_code(cap_code[cap_id][2], &cap,0); // Height
#ifdef CPU_PIC32MZ
	cap.input_max = MAX_FORM_LEN / 8;
#endif
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_LABSIZE_HEIGHT, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value[1], NULL, NULL, NULL);
	Menu_AddKeyFunction(M_ID_LABSIZE_HEIGHT, KP_TEST, printer_set_labsize);

	if(PrtPaperType == 2)		// Continuous Label
	{
		caption_split_by_code(cap_code[cap_id][4], &cap,0); // End Margin
	}
	else
	{
		caption_split_by_code(cap_code[cap_id][3], &cap,0); // Gap Length
	}
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_LABSIZE_GAPLEN, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value[2], NULL, NULL, NULL);
	Menu_AddKeyFunction(M_ID_LABSIZE_GAPLEN, KP_TEST, printer_set_labsize);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE)
	{
		if(temp_value[0] > 58) {
			PrtStruct.LabelWidth   = 56*8;
			PrtStruct.ReceiptWidth = 56*8;
		} else {
			PrtStruct.LabelWidth   = (temp_value[0]-2)*8;
			PrtStruct.ReceiptWidth = (temp_value[0]-2)*8;
		}
		PrtStruct.LabelHeight  = temp_value[1]*8;
		PrtDrvStruct.GapLength = temp_value[2]*8;

		set_global_wparam(GLOBAL_PRT_PAPER_WIDTH + 0 * 2, PrtStruct.LabelWidth);
		set_global_wparam(GLOBAL_PRT_PAPER_WIDTH + 1 * 2, PrtStruct.ReceiptWidth);
		set_global_wparam(GLOBAL_PRT_PAPER_WIDTH + 2 * 2, PrtStruct.LabelWidth);
		printer_save_size(PrtPaperType, PrtStruct.LabelWidth, PrtStruct.LabelHeight, PrtDrvStruct.GapLength);

		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_LabelWidth, (HUGEDATA INT8U *)&PrtStruct.LabelWidth, 2);
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_LabelHeight, (HUGEDATA INT8U *)&PrtStruct.LabelHeight, 2);
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapLength, (HUGEDATA INT8U *)&PrtDrvStruct.GapLength, 2);
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_ReceiptWidth, (HUGEDATA INT8U *)&PrtStruct.ReceiptWidth, 2);
		Flash_flush_nvbuffer();//HYP 20040204 FLASH NVRAM

		Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);

		PrtSetReceiptWidth(PrtFlashStruct.ReceiptWidth);
		PrtSetLabelSize(PrtFlashStruct.LabelWidth,PrtFlashStruct.LabelHeight);
		label_load(status_scale.cur_labelid);
		
	}
	else if(result == MENU_RET_ESC)
	{
		PrtStruct.LabelWidth   = backup_label_width;
		PrtStruct.LabelHeight  = backup_label_height;
		PrtDrvStruct.GapLength = backup_gap_length;
	}
}

void printer_receiptset(void) // Modified by CJK 20050927
{
	CAPTION_STRUCT cap;
	INT32U temp_value[3];
	INT8U result;
	INT16U cap_code[2][4] = {{0x9745, 0x9741, 0x9747, 0x9748}, 	// Prog. Mode
				 {0x9345, 0x9341, 0x9347, 0x9348}};	// Cal Mode
	INT8U cap_id;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}

	temp_value[0] = PrtStruct.ReceiptWidth/8;
	if(temp_value[0] == 56) {
		temp_value[0] = 60;
	} else {
		temp_value[0] = temp_value[0] + 2;
	}
	temp_value[1] = PrtStruct.ReceiptHeight/8;
	temp_value[2] = PrtDrvStruct.ReceiptFeedHeight/8;

	Menu_Init();
	caption_split_by_code(cap_code[cap_id][0], &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);
	
	caption_split_by_code(cap_code[cap_id][1], &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &temp_value[0], NULL, NULL, NULL);

	caption_split_by_code(cap_code[cap_id][2], &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &temp_value[1], NULL, NULL, NULL);

	caption_split_by_code(cap_code[cap_id][3], &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &temp_value[2], NULL, NULL, NULL);
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		BuzOn(1);
		if(temp_value[0] > 58) {
			PrtStruct.ReceiptWidth = 56*8;
			PrtStruct.LabelWidth   = 56*8;
		} else {			
			PrtStruct.ReceiptWidth = (temp_value[0]-2)*8;
			PrtStruct.LabelWidth   = (temp_value[0]-2)*8;			
		}
		PrtStruct.ReceiptHeight  = temp_value[1]*8;
		PrtDrvStruct.ReceiptFeedHeight = temp_value[2]*8;

		set_global_wparam(GLOBAL_PRT_PAPER_WIDTH + 0 * 2, PrtStruct.LabelWidth);
		set_global_wparam(GLOBAL_PRT_PAPER_WIDTH + 1 * 2, PrtStruct.ReceiptWidth);
		set_global_wparam(GLOBAL_PRT_PAPER_WIDTH + 2 * 2, PrtStruct.LabelWidth);
		printer_save_size(1, PrtStruct.ReceiptWidth, PrtStruct.ReceiptHeight, PrtDrvStruct.ReceiptFeedHeight);

		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_ReceiptWidth, (HUGEDATA INT8U *)&PrtStruct.ReceiptWidth, 2);
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_ReceiptHeight, (HUGEDATA INT8U *)&PrtStruct.ReceiptHeight, 2);
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_ReceiptFeedHeight, (HUGEDATA INT8U *)&PrtDrvStruct.ReceiptFeedHeight, 2);
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_LabelWidth, (HUGEDATA INT8U *)&PrtStruct.LabelWidth, 2);
		Flash_flush_nvbuffer();//HYP 20040204 FLASH NVRAM

		Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);

		PrtSetLabelWidth(PrtFlashStruct.LabelWidth);
		PrtSetReceiptWidth(PrtFlashStruct.ReceiptWidth);
		PrtSetReceiptHeight(PrtFlashStruct.ReceiptHeight);		//HYP 20040330	
		PrtSetReceiptFeedHeight(PrtDrvStruct.ReceiptFeedHeight);
	}
}

extern INT8U PrtFontType;
extern void StrInit4UserLabel(void);
void print_sensor_val(INT8U fontid)
{
	char string_buf[50];

	if(PrtStruct.Mode == LABEL) {
		LabCmd_ClearLabel();
		StrInit4UserLabel();	// Added by CJK 20060426
		RcptCmd_SetFontType(PrtFontType);//0=one byte font, 1=two byte font //SG060313
		RcptCmd_SetFontId(fontid);
		RcptCmd_IntercharSP(0);
		LabCmd_SetStrField(30, 80, 100, 418);
		LabCmd_CharSize(2, 3);
		LabCmd_StrStart();
#ifdef USE_ARAB_FONT
  #ifdef USE_ENG_BASED_ARAB_SUPPORT
        sprintf(string_buf, "Gap : %3d = (%3d+%3d)/2\n",SensorStruct.GapThreshold,SensorStruct.MaxGapValue,SensorStruct.MinGapValue);
  #else
		sprintf(string_buf, "2/(%3d+%3d) = %3d : Gap\n",SensorStruct.MaxGapValue,SensorStruct.MinGapValue,SensorStruct.GapThreshold);
  #endif
#else
		sprintf(string_buf, "Gap : %3d = (%3d+%3d)/2\n",SensorStruct.GapThreshold,SensorStruct.MaxGapValue,SensorStruct.MinGapValue);
#endif
		PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf));
		if (fontid==0) {
			LabCmd_StrEnd();
			LabCmd_SetStrField(30, 80+20, 100, 418);
			LabCmd_StrStart();
		}
#ifdef USE_ARAB_FONT
  #ifdef USE_ENG_BASED_ARAB_SUPPORT
		sprintf(string_buf, "Peel : %3d = (%3d+%3d)/2\n",SensorStruct.PeelThreshold,SensorStruct.MaxPeelValue,SensorStruct.MinPeelValue);
  #else
		sprintf(string_buf, "2/(%3d+%3d) = %3d :Peel\n",SensorStruct.MaxPeelValue,SensorStruct.MinPeelValue,SensorStruct.PeelThreshold);
  #endif
#else
		sprintf(string_buf, "Peel: %3d = (%3d+%3d)/2\n",SensorStruct.PeelThreshold,SensorStruct.MaxPeelValue,SensorStruct.MinPeelValue);
#endif
		PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf));

		LabCmd_StrEnd();
		RcptCmd_TypeNone();
		LabCmd_PrintStart(1);
		PrintListWaitEnd();
	} else {
		//RcptCmd_ChangeMode(TO_RECEIPT);
		RcptCmd_DefaultSet();

		RcptCmd_StrJust(LEFT);
		RcptCmd_CharLF(1);
		//RcptCmd_CharSize(1,2);
		RcptCmd_SetFontStyle(3, RPT_FONT_NORMAL, TKT_LAY_N, 0, LEFT);
		RcptCmd_TypeBold();
		RcptCmd_CharTab(1);
#ifdef USE_ARAB_FONT
  #ifdef USE_ENG_BASED_ARAB_SUPPORT
		sprintf(string_buf, "Gap : %3d = (%3d+%3d)/2\n",SensorStruct.GapThreshold,SensorStruct.MaxGapValue,SensorStruct.MinGapValue);
  #else
		sprintf(string_buf, "2/(%3d+%3d) = %3d : Gap\n",SensorStruct.MaxGapValue,SensorStruct.MinGapValue,SensorStruct.GapThreshold);
  #endif
#else
		sprintf(string_buf, "Gap  : %3d = (%3d+%3d)/2\n",SensorStruct.GapThreshold,SensorStruct.MaxGapValue,SensorStruct.MinGapValue);
#endif
		PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf));
		RcptCmd_CharTab(1);
#ifdef USE_ARAB_FONT
  #ifdef USE_ENG_BASED_ARAB_SUPPORT
        sprintf(string_buf, "Peel : %3d = (%3d+%3d)/2\n",SensorStruct.PeelThreshold,SensorStruct.MaxPeelValue,SensorStruct.MinPeelValue);
  #else
		sprintf(string_buf, "2/(%3d+%3d) = %3d :Peel\n",SensorStruct.MaxPeelValue,SensorStruct.MinPeelValue,SensorStruct.PeelThreshold);
  #endif
#else
        sprintf(string_buf, "Peel : %3d = (%3d+%3d)/2\n",SensorStruct.PeelThreshold,SensorStruct.MaxPeelValue,SensorStruct.MinPeelValue);
#endif
        PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf));
		RcptCmd_TypeNone();
		RcptCmd_Start();
		PrintListWaitEnd();
		RcptCmd_ReceiptFeed();
		PrintListWaitEnd();
		//RcptCmd_ChangeMode(TO_RESTORE);
	}
}

void printer_sensor_keytest(INT16U *t1,INT16U *t2,INT8U fontid)
{
	char string_buf[40];

	BuzOn(1);
	if(!Prt_CheckPeelAndHead(OFF)) {//check headup
		return;
	}
	string_buf[0] = GS;
	string_buf[1] = 'G';
	PutDataRxBuf(&CommBufPrt, string_buf, 2);
//	while(CheckRxBuf(&CommBufPrt)) {
//		CASInterpreter(&CommBufPrt);
//	}
//	while(PrtDrvStruct.PrtState != PRT_STATE0I);
	PrintListWaitEndWithoutComm();
	*t1 = SensorStruct.GapThreshold;
	*t2 = SensorStruct.PeelThreshold;
	print_sensor_val(fontid);
}

#define M_ID_SENS_GAP   1
#define M_ID_SENS_PEEL  2
#define M_ID_SENS_FIX  3
void printer_sensor_cal(INT32U num)
{
	INT8U id_gap;
	INT8U id_peel;
	INT16U gap, peel;

	id_gap = Menu_GetMenuId(M_ID_SENS_GAP);
	if(id_gap == 0xff) return;
	id_peel = Menu_GetMenuId(M_ID_SENS_PEEL);
	if(id_peel == 0xff) return;

	gap = *Menu[id_gap].pval;
	peel = *Menu[id_peel].pval;
	printer_sensor_keytest(&gap, &peel, 3);
	*Menu[id_gap].pval = gap;
	*Menu[id_peel].pval = peel;

#ifdef USE_CHECK_OUTOFPAPER_ON_LABEL
	// Out of paper threshold 설정 (Gap의 Max Sensor값+2 보다 클 경우 Out of paper로 판정 )
	// sensor test를 했을 때만 저장
	SensorStruct.GapOutOfPaperThresholdOnLabel = SensorStruct.MaxGapValue + 2;
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapOutOfPaperThresholdOnLabel, (HUGEDATA INT8U *)&SensorStruct.GapOutOfPaperThresholdOnLabel, 1);
	//
#endif
}

void printer_sensor(void) // Modified by CJK 20050927
{
	//char string_buf[40];
	//POINT disp_p;
	INT8U backup_gap_threshold, backup_peel_threshold;
	CAPTION_STRUCT cap;
	INT32U temp_value[2];
	//INT8U idx;
	//MENU_CURSOR_STR cursor;
	INT8U flagUseGap;//, start_idx;
	//INT8U input_x[2];
//	INT16U cap_code[2][5] = {{0x1733, 0x9701, 0x9702, 0x9704, 0x9705}, 	// Prog. Mode
//				 {0x8330, 0x9301, 0x9302, 0x9304, 0x9305}};	// Cal Mode
	INT8U cap_id;
	INT8U result;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}

	if(PrtStruct.Mode == LABEL && PrtDrvStruct.LabelGapMode	== GAP) {
		flagUseGap = ON;
	} else {
		flagUseGap = OFF;
	}
	backup_gap_threshold    = SensorStruct.GapThreshold;
	backup_peel_threshold   = SensorStruct.PeelThreshold;
	temp_value[0] = SensorStruct.GapThreshold;
	temp_value[1] = SensorStruct.PeelThreshold;
	
	
	Menu_Init();

//	caption_split_by_code(cap_code[cap_id][0], &cap,0);
	caption_split_by_code(0x1733, &cap,0);//caption 부족으로 인해 cal, program 모드 캡션 통합
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	if(flagUseGap == ON) {
		caption_split_by_code(0x9701, &cap, 0);//0x9301, &cap,0);
	} else {
		caption_split_by_code(0x9704, &cap, 0);//0x9304, &cap,0);
	}
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_SENS_GAP, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value[0], NULL, NULL, NULL);
	Menu_AddKeyFunction(M_ID_SENS_GAP, KP_TEST, printer_sensor_cal);
	Menu_SetProperties(M_ID_SENS_GAP, MENU_P_EXIT_MENU);

	caption_split_by_code(0x9702, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_SENS_PEEL, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value[1], NULL, NULL, NULL);

#if !defined(USE_SINGLE_LINE_GRAPHIC)
	if(Startup.menu_type != 0) {
		caption_split_by_code(0x9705, &cap,0);
		caption_adjust(&cap,NULL);
		Menu_InputCreate(M_ID_SENS_FIX, (HUGEDATA char *)cap.form, MENU_TYPE_FIX, cap.input_x, cap.input_dlength, cap.input_length,
					cap.input_max, cap.input_min, 0, NULL, NULL, NULL, NULL);
	}
#endif
	
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE || result == MENU_RET_EXIT) // TEST키에 의해 나올때, 저장함
	{
		if(flagUseGap) {
			SensorStruct.GapThreshold  = (INT8U)temp_value[0];
			Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapThreshold, (HUGEDATA INT8U *)&SensorStruct.GapThreshold, 1);
		} else {
			SensorStruct.GapThreshold = backup_gap_threshold;
		}
		SensorStruct.PeelThreshold = (INT8U)temp_value[1];
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PeelThreshold, (HUGEDATA INT8U *)&SensorStruct.PeelThreshold, 1);
		Flash_flush_nvbuffer();

		Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);
	}
	else if(result == MENU_RET_ESC)
	{
		SensorStruct.GapThreshold = backup_gap_threshold;
		SensorStruct.PeelThreshold = backup_peel_threshold;
	}
}

void printer_settings(void) // Modified by CJK 20050927
{
	INT32U set_val[4];
	INT8U result;
	CAPTION_STRUCT cap;
//	INT16U cap_code[2][5] = {{0x1734, 0x9761, 0x9762, 0x9763, 0x9764},	// Prog. Mode 
//				 {0x8340, 0x9361, 0x9362, 0x9363, 0x9364}};	// Cal Mode

	INT8U cap_id;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}
	
//DEL070206	set_val[0] = (INT32U)PrtDrvStruct.PeelEnable;
//DEL070206	set_val[1] = (INT32U)PrtDrvStruct.RewindEnable;
//DEL070206	set_val[2] = (INT32U)PrtDrvStruct.LabelGapMode;
	// CJK070206
	result = get_global_bparam(GLOBAL_PRT_SENSOR_MOTOR + PrtPaperType);
	set_val[0] = (INT32U)(result & 0x01);
#ifndef _CL3000_PRINTER_MECH
	set_val[1] = (INT32U)((result >> 1) & 0x01);
#endif
	set_val[2] = (INT32U)((result >> 2) & 0x01);
	set_val[3] = get_global_bparam(GLOBAL_PRT_HEADUP_SENSOR);
	///////////////

	Menu_Init();

	//caption_split_by_code(cap_code[cap_id][0], &cap,0);//"Printer Motor & Sensor"	0x8340	// Modified by CJK 20050922
	caption_split_by_code(0x1734, &cap,0);//caption 부족으로 cal mode, program mode caption 통합
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	//caption_split_by_code(cap_code[cap_id][1], &cap, 0);//0x9361, &cap,0);//"Peel Sensor (0:OFF, 1:ON)  : [ ]"
	caption_split_by_code(0x9361, &cap, 0);//caption 부족으로 cal mode, program mode caption 통합
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length, 
				cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);
#ifndef _CL3000_PRINTER_MECH
	//caption_split_by_code(cap_code[cap_id][2], &cap, 0);//0x9362, &cap,0);//"Rewind Motor (0:OFF, 1:ON) : [ ]"
	caption_split_by_code(0x9362, &cap, 0);//caption 부족으로 cal mode, program mode caption 통합
	caption_adjust(&cap,NULL);
	Menu_InputCreate(2, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length, 
				cap.input_max, cap.input_min, 0, &set_val[1], NULL, NULL, NULL);
#endif

	//caption_split_by_code(cap_code[cap_id][3], &cap, 0);//0x9363, &cap,0);//"Label Paper(0:No Gap,1:Gap): [ ]"
	caption_split_by_code(0x9363, &cap, 0);//caption 부족으로 통합 cal mode, program mode caption 통합
	caption_adjust(&cap,NULL);
	Menu_InputCreate(3, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length, 
				cap.input_max, cap.input_min, 0, &set_val[2], NULL, NULL, NULL);

	//caption_split_by_code(cap_code[cap_id][4], &cap, 0);
	caption_split_by_code(0x9364, &cap, 0);//caption 부족으로 통합 cal mode, program mode caption 통합
	caption_adjust(&cap,NULL);
	Menu_InputCreate(4, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length, 
				cap.input_max, cap.input_min, 0, &set_val[3], NULL, NULL, NULL);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		PrtDrvStruct.PeelEnable = (BOOLEAN)set_val[0];
#ifndef _CL3000_PRINTER_MECH
		PrtDrvStruct.RewindEnable = (BOOLEAN)set_val[1];
#endif
		PrtDrvSetLabelGapMode((BOOLEAN)set_val[2]);
		// CJK070206
		printer_save_sensor(PrtPaperType, PrtDrvStruct.PeelEnable, PrtDrvStruct.RewindEnable, PrtDrvStruct.LabelGapMode);
		///////////////
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PeelEnable, (HUGEDATA INT8U *)&PrtDrvStruct.PeelEnable, 1);
#ifndef _CL3000_PRINTER_MECH
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_RewindEnable, (HUGEDATA INT8U *)&PrtDrvStruct.RewindEnable, 1);
#endif
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_LabelGapMode, (HUGEDATA INT8U *)&PrtDrvStruct.LabelGapMode, 1);
		set_global_bparam(GLOBAL_PRT_HEADUP_SENSOR, (BOOLEAN)set_val[3]);
		Flash_flush_nvbuffer();//HYP 20040204 FLASH NVRAM

		Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);
	}
}

void printer_setdensity(INT16U density)
{
	PrtDrvStruct.StbTimeTrimValue = density * STB_TIME_DIV;
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_StbTimeTrimValue, (HUGEDATA INT8U *)&PrtDrvStruct.StbTimeTrimValue, 2);
	Flash_flush_nvbuffer();//HYP 20040204 FLASH NVRAM

	Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);
}

#define M_ID_DENSITY_VAL 1
#define M_ID_DENSITY_FIX 2
void printer_test_density(INT32U num)
{
	char string_buf[10];
	INT8U id_density;
	INT8U density;

	id_density = Menu_GetMenuId(M_ID_DENSITY_VAL);
	if(id_density == 0xff) return;

	density = *Menu[id_density].pval;

	if(!Prt_CheckPeelAndHead(ON)) //check peel and headup
	{
		return;
	}
	if(density > 20) {
		BuzOn(2);
	}
	else
	{
		PrtDrvStruct.StbTimeTrimValue = density * STB_TIME_DIV;

		string_buf[0] = _ESC;
		string_buf[1] = 'T';
		string_buf[2] = _ESC;
		string_buf[3] = '!';
		string_buf[4] = 0x01;
		PutDataRxBuf(&CommBufPrt, string_buf, 5);
	}
}

void printer_stb_time(void) // Modified by CJK 20050927
{
	INT16U backup_stb_time;
	INT32U temp_value;
	CAPTION_STRUCT cap;
	INT16U cap_code[2][3] = {{0x1735, 0x9711, 0x9712},	// Prog. Mode
				 {0x8350, 0x9311, 0x9312}};	// Cal Mode
	INT8U cap_id;
	INT8U result;

	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}

	backup_stb_time = PrtDrvStruct.StbTimeTrimValue;
	temp_value = PrtDrvStruct.StbTimeTrimValue/STB_TIME_DIV;


	Menu_Init();

	caption_split_by_code(cap_code[cap_id][0], &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(cap_code[cap_id][1], &cap, 0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_DENSITY_VAL, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value, NULL, NULL, NULL);
	Menu_AddKeyFunction(M_ID_DENSITY_VAL, KP_TEST, printer_test_density);

#if !defined(USE_SINGLE_LINE_GRAPHIC)
	caption_split_by_code(cap_code[cap_id][2], &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_DENSITY_FIX, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, NULL, NULL, NULL, NULL);
#endif

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE)
	{
		printer_setdensity((INT16U)temp_value);
	}
	else if(result == MENU_RET_ESC)
	{
		PrtDrvStruct.StbTimeTrimValue = backup_stb_time;
	}
}

#define M_ID_ADJUST_VAL 1
#define M_ID_ADJUST_FIX 2
void printer_adjust_test(INT32U num)
{
	char string_buf[10];
	INT8U id_len;
	INT32U length;
	INT16S pre_trim_value;

	id_len = Menu_GetMenuId(M_ID_ADJUST_VAL);
	if(id_len == 0xff) return;

	length = (INT16S)*Menu[id_len].pval;

	if(!Prt_CheckPeelAndHead(OFF)) {//check headup
		return;
	}
	pre_trim_value = SensorStruct.GapBufInPtrTrimValue;
	SensorStruct.GapBufInPtrTrimValue = 200 - (INT16S)length;
	SensorGapTrim(SensorStruct.GapBufInPtrTrimValue - pre_trim_value);
	PutCharRxBuf(&CommBufPrt, 0x0c);
}

void adjust_feed_length(void)
{
	char string_buf[50];
	short backup_trim_value, pre_trim_value;
	INT32U temp_value;
	CAPTION_STRUCT cap;
	INT16U cap_code[2][3] = {{0x1736, 0x9731, 0x9732},	// Prog. Mode 
				 {0x8360, 0x9331, 0x9332}};	// Cal Mode
	INT8U cap_id;
	INT8U result;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}
 
	backup_trim_value = SensorStruct.GapBufInPtrTrimValue;
	pre_trim_value = SensorStruct.GapBufInPtrTrimValue;

	temp_value = 200 - SensorStruct.GapBufInPtrTrimValue;

	Menu_Init();

	caption_split_by_code(cap_code[cap_id][0], &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(cap_code[cap_id][1], &cap, 0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_ADJUST_VAL, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value, NULL, NULL, NULL);
	Menu_AddKeyFunction(M_ID_ADJUST_VAL, KP_TEST, printer_adjust_test);

#if !defined(USE_SINGLE_LINE_GRAPHIC)
	caption_split_by_code(cap_code[cap_id][2], &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_ADJUST_FIX, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, 26, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, NULL, NULL, NULL, NULL);
#endif

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE)
	{
		pre_trim_value = SensorStruct.GapBufInPtrTrimValue;
		SensorStruct.GapBufInPtrTrimValue = 200 - (INT16S)temp_value;
		SensorGapTrim(SensorStruct.GapBufInPtrTrimValue - pre_trim_value);
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapBufInPtrTrimValue, (HUGEDATA INT8U *)&SensorStruct.GapBufInPtrTrimValue, 2);
		Flash_flush_nvbuffer();

		Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);
	}
	else if(result == MENU_RET_ESC)
	{
		pre_trim_value = SensorStruct.GapBufInPtrTrimValue;
		SensorStruct.GapBufInPtrTrimValue = backup_trim_value;
		SensorGapTrim(SensorStruct.GapBufInPtrTrimValue - pre_trim_value);
	}
}

void adjust_reverse_feed_length(void) // Modified by KKH 20110428
{
	CAPTION_STRUCT	cap;	
	char  uctemp, uctemp1;
	INT16S	result;
    INT32U  flash_addr;
    INT8U   strTemp1[64];
    INT8U   strTemp2[64];
    
	INT32U 	input_data;	//fix size
    	 
	input_data = Flash_wread(PRT_FLASH_BASE + FLASH_OFFSET_GapBufInPtrReverseTrimValue);
		    
	Menu_Init();
	caption_split_by_code(0x1738, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x9733, &cap,0);
	caption_adjust(&cap,NULL);
	if (caption_run_check(cap.code,cap.runflag)) {
		Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
					cap.input_max, cap.input_min, 0, &input_data, NULL, NULL, NULL);
	}

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		Flash_wwrite(PRT_FLASH_BASE + FLASH_OFFSET_GapBufInPtrReverseTrimValue, (INT16U)input_data);
		PrtDrvStruct.ReverseFeedLength = (INT16U)input_data;
//    	PrtDrvStruct.ReverseFeedLength = PrtFlashStruct.GapBufInPtrReverseTrimValue;

		Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);
	}
}

void printer_chess(void)
{
	char string_buf[20];
 
	BuzOn(1);
	if(!Prt_CheckPeelAndHead(ON)) {//check peel and headup
		BuzOn(2);
		return;
	}
	string_buf[0] = _ESC;
 
	string_buf[1] = 'T';
	string_buf[2] = _ESC;
 
	string_buf[3] = '!';
	string_buf[4] = 0x01;
	PutDataRxBuf(&CommBufPrt, string_buf, 5);
}

//#define CONTINEOUS_PRT_TEST
void printer_test(void)
{
	char string_buf[20];
	POINT disp_p;
	CAPTION_STRUCT cap;
	INT8U old_key;
#ifdef CONTINEOUS_PRT_TEST
	INT8U test=0;
	INT8U testDelayTime = 10; // 100ms단위 기존 기능의 기본값 1초
	INT32U label_cnt=0;
#endif
	INT16U cap_code[2][3] = {{0x1864, 0x97a2, 0x97a3}, 	// Prog. Mode
				 {0x8540, 0x9440, 0x9441}};	// Cal Mode
	INT8U cap_id;
	INT8U flagOut;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}

	old_key=KEY_GetMode();
	KEY_SetMode(1);
	Dsp_Fill_Buffer(0);
	DspLoadFont1(DSP_MENU_FONT_ID);
	menu_display_menucode(CurMenuCode, 1);
	menu_displaytitle(cap_code[cap_id][0], 0);//0x8540,0);
	menu_writepage(1, 1);

#if defined(USE_SINGLE_LINE_GRAPHIC)
	display_clear(DISPLAY_UNITPRICE);
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
  	disp_p = point(0,0);
	Dsp_Write_String(disp_p,(HUGEDATA char *)form_graphic);
  #endif
#endif

#if !defined(USE_SINGLE_LINE_GRAPHIC)
	caption_split_by_code(cap_code[cap_id][1], &cap, 0);//0x9440, &cap,0);
	disp_p = point((Startup.menu_type)*Startup.menu_ygap, 0);
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
	hebrew_codetemp[strlen(cap.form)] = 0;
	disp_p.x = 202 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)cap.form, strlen(cap.form), 0, 0, 0); 
	arab_codetemp[strlen(cap.form)] = 0;
	disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
	Dsp_Write_String(disp_p,(HUGEDATA char *)cap.form);
#endif
	caption_split_by_code(cap_code[cap_id][2], &cap, 0);//0x9441, &cap,0);
	disp_p = point((Startup.menu_type+1)*Startup.menu_ygap, 0);
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
	hebrew_codetemp[strlen(cap.form)] = 0;
	disp_p.x = 202 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)cap.form, strlen(cap.form), 0, 0, 0); 
	arab_codetemp[strlen(cap.form)] = 0;
	disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
	Dsp_Write_String(disp_p,(HUGEDATA char *)cap.form);
#endif
#endif
	Dsp_Diffuse();
	while(1) {
		while(!KEY_Read()) {
#ifdef CONTINEOUS_PRT_TEST
			if((PrtDrvStruct.PrtState == PRT_STATE0I) && (test!=0)) {
				if (PrtDrvStruct.OutOfPaperFlag) flagOut = ON;
				else flagOut = OFF;
				set_nvram_bparam(NVRAM_PRT_OUTOFPAPER, flagOut);
				//sprintf(string_buf,"temp=%3d",SensorStruct.TempFilVal);
				//disp_p = point(0,0);
				//Dsp_Write_String(disp_p,string_buf);
				sprintf(string_buf,"NO=%d",label_cnt);
				disp_p = point((Startup.menu_type+2)*Startup.menu_ygap,0);
#ifdef HEBREW_FONT
				Convert_HebrewPreview((INT8U *)string_buf, strlen(string_buf), 0, 0); 
				hebrew_codetemp[strlen(string_buf)] = 0;
				disp_p.x = 202 - disp_p.x;
				Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
				Convert_ArabPreview((INT8U *)string_buf, strlen(string_buf), 0, 0, 0); 
				arab_codetemp[strlen(string_buf)] = 0;
				disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
				Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
				Dsp_Write_String(disp_p,string_buf);
#endif
				//HYP 20040104
				if(!Prt_CheckOutOfPaper()) {
					test = 0;
					label_cnt = 0;
					BuzOn(2);
					break;
				}
				//HYP 20050104
				if(!Prt_CheckPeelAndHead(ON)) {//check peel and headup
					test = 0;
					label_cnt = 0;
					BuzOn(2);
					break;
				}
				label_cnt++;
				PrtSensorFlagInit();
				if(test == 1) {
					string_buf[0] = _ESC;
					string_buf[1] = 'T';
					string_buf[2] = _ESC;
					string_buf[3] = '!';
					string_buf[4] = 0x01;
					PutDataRxBuf(&CommBufPrt, string_buf, 5);
				} else {
					PrtTestHPattern();
					string_buf[0] = _ESC;
					string_buf[1] = '!';
					string_buf[2] = 0x01;
					PutDataRxBuf(&CommBufPrt, string_buf, 3);
				}
				Delay_100ms(testDelayTime);
			}
#endif
			CASInterpreter(&CommBufPrt);
		}
		//KeyDrv.CnvCode=KP_TEST;
		switch(KeyDrv.CnvCode)
		{
			case KP_TEST :
				BuzOn(1);
				if(!Prt_CheckPeelAndHead(ON)) {//check peel and headup
					//BuzOn(2);
					break;
				}
				//BuzOn(1);
				string_buf[0] = _ESC;
				string_buf[1] = 'T';
				string_buf[2] = _ESC;
				string_buf[3] = '!';
				string_buf[4] = 0x01;
				PutDataRxBuf(&CommBufPrt, string_buf, 5);
//HYP 20060308 for reverse feed test
//				string_buf[0] = BS;
//				string_buf[1] = ESC;
//				string_buf[2] = 'T';
//				string_buf[3] = ESC;
//				string_buf[4] = '!';
//				string_buf[5] = 0x01;
//				PutDataRxBuf(&CommBufPrt, string_buf, 6);
				break;
#ifdef CONTINEOUS_PRT_TEST
/* Disable for UL Test
			case KEY_NUM_7 ://CONTINEOUS_PRT_TEST
				BuzOn(1);
				PrtSensorFlagInit();
				test = 1;
				break;
*/
			case KEY_NUM_1 :
				BuzOn(1);
				test = 1;
				break;
			case KEY_NUM_5 ://CONTINEOUS_PRT_TEST 'H'
				BuzOn(1);
				test = 2;
				testDelayTime = 10; // 기존 기능 기본값 1초 간격
				sprintf(string_buf, "TEST");
				disp_p = point((Startup.menu_type+2)*Startup.menu_ygap,0);
				Dsp_Write_String(disp_p,string_buf);
				Dsp_Diffuse();
				break;
			case KEY_NUM_6 ://CONTINEOUS_PRT_TEST 'H'
				BuzOn(1);
				test = 2;
				testDelayTime = 50; // (5초 간격) 국내 형식인증 및 자체 테스트 용 연구소 파생 요청
				sprintf(string_buf, "TEST");
				disp_p = point((Startup.menu_type+2)*Startup.menu_ygap,0);
				Dsp_Write_String(disp_p,string_buf);
				Dsp_Diffuse();
				break;
			case KEY_NUM_7 ://CONTINEOUS_PRT_TEST 'H'
				BuzOn(1);
				test = 2;
				testDelayTime = 100; // (10초 간격) 국내 형식인증 및 자체 테스트 용 연구소 파생 요청
				sprintf(string_buf, "TEST");
				disp_p = point((Startup.menu_type+2)*Startup.menu_ygap,0);
				Dsp_Write_String(disp_p,string_buf);
				Dsp_Diffuse();
				break;
#endif
			case KP_CLEAR :
				BuzOn(1);
#ifdef CONTINEOUS_PRT_TEST
				label_cnt = 0;
				test = 0;
#endif
				break;
			case KP_FEED:
				BuzOn(1);
				if(!Prt_CheckPeelAndHead(OFF)) {//check headup
					//BuzOn(2);
					break;
				}
				PutCharRxBuf(&CommBufPrt, 0x0c);
				break;
			case KP_ESC :
// TEST_ESC:
	 			BuzOn(1);
				KEY_SetMode(old_key);
				return;
			default :
				BuzOn(2);
				break;
		}
	}
}

#ifdef USE_PB_BATTERY
extern float ConvertBatVolatage(INT16U ad);
INT16U BatVoltVal;
#endif

#if defined(USE_SINGLE_LINE_GRAPHIC)
void printer_sensor_test(void)
{
	char string_buf[50];
	POINT disp_p;
	CAPTION_STRUCT cap;
//	BOOLEAN prev_status_peel,prev_status_head;
//	INT8U prev_gap_val,prev_peel_val;
	INT8U peel_backup;
	INT8U x_point[2],x_length[2];//,i,w;
	INT8U backup_directdraw, page, flagDisplay;
//	INT16U cap_code[2][9] = {{0x1865, 0x9701, 0x9702, 0x97a4, 0x97a5, 0x97a6, 0x97a7, 0x97a8, 0x97a9}, 
//				 {0x8550, 0x9301, 0x9302, 0x9450, 0x9451, 0x9452, 0x9453, 0x9454, 0x9455}};//caption 부족으로 인해 caption 통합
	INT8U cap_id;
	INT8U max_page;
#ifdef USE_PB_BATTERY
	INT8U x_point3, x_length3;
	INT8U x_point4, x_length4;
	INT16U prev_bat_val = 0xffff;
#endif
	INT16U prev_timer;
	
	prev_timer = SysTimer100ms - 3;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}

	backup_directdraw = DspStruct.DirectDraw;
	DspStruct.DirectDraw = 0;

	page = 0;
#ifdef USE_PB_BATTERY
	max_page = 7;
#else
	max_page = 5;
#endif
	flagDisplay = 1;

	peel_backup=PrtDrvStruct.PeelEnable;
	PrtDrvStruct.PeelEnable=ENABLE;

	while(1) {
//		prev_status_peel = !PrtDrvStruct.HangingFormFlag;
//		prev_status_head = !PrtDrvStruct.HeadUpFlag;
//		prev_gap_val     = 0;
//		prev_peel_val    = 0;
#ifdef USE_PB_BATTERY
		prev_bat_val = 0xffff;
#endif

		if(flagDisplay) {
			flagDisplay = 0;
			Dsp_Fill_Buffer(0);
			DspLoadFont1(DSP_MENU_FONT_ID);

			menu_display_menucode(CurMenuCode, 1);
			//menu_displaytitle(cap_code[cap_id][0], 0);//0x8550,0);
			menu_displaytitle(0x1865, 0);//caption 부족으로 인하여 caption 통합
		   	menu_writepage(2, page+1);

			switch (page) 
			{
				case 0:
					display_clear(DISPLAY_UNITPRICE);
					caption_split_by_code(0x97a4, &cap, 0);//0x9450, &cap,0);
					caption_adjust(&cap,NULL);
					display_string(DISPLAY_UNITPRICE,(INT8U *)cap.form);
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
					disp_p = point(0,0);
					Dsp_Write_String(disp_p,(HUGEDATA char *)form_graphic);
#endif
					break;
				case 1:
					display_clear(DISPLAY_UNITPRICE);
					caption_split_by_code(0x97a5, &cap, 0);//0x9451, &cap,0);
					caption_adjust(&cap,NULL);
					display_string(DISPLAY_UNITPRICE,(INT8U *)cap.form);
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
					disp_p = point(0,0);
					Dsp_Write_String(disp_p,(HUGEDATA char *)form_graphic);
#endif
					break;
				case 2:
					display_clear(DISPLAY_UNITPRICE);
					caption_split_by_code(0x9701, &cap, 0);//0x9301, &cap,0);
					caption_adjust(&cap,NULL);
					display_string(DISPLAY_UNITPRICE,(INT8U *)cap.form);
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
					disp_p = point(0,0);
					Dsp_Write_String(disp_p,(HUGEDATA char *)form_graphic);
#endif
					break;
				case 3:
					display_clear(DISPLAY_UNITPRICE);
					caption_split_by_code(0x9702, &cap, 0);//0x9302, &cap,0);
					caption_adjust(&cap,NULL);
					display_string(DISPLAY_UNITPRICE,(INT8U *)cap.form);
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
					disp_p = point(0,0);
					Dsp_Write_String(disp_p,(HUGEDATA char *)form_graphic);
#endif
					break;
				case 4:
					display_clear(DISPLAY_UNITPRICE);
					//caption_split_by_code(cap_code[cap_id][2], &cap, 0);//0x9302, &cap,0);
					//caption_adjust(&cap,NULL);
					display_string(DISPLAY_UNITPRICE,(INT8U *)"TEMP");
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
					disp_p = point(0,0);
					Dsp_Write_String(disp_p,(HUGEDATA char *)"TEMP");
#endif
					break;
#ifdef USE_PB_BATTERY
				case 5:
					display_clear(DISPLAY_UNITPRICE);
					sprintf(cap.form, "BAT");
					display_string(DISPLAY_UNITPRICE,(INT8U *)cap.form);
					break;
				case 6:
					display_clear(DISPLAY_UNITPRICE);
					sprintf(cap.form, "AC ON");
					display_string(DISPLAY_UNITPRICE,(INT8U *)cap.form);
					break;
#endif
			}
	     	Dsp_Diffuse();
		}
		while(!KEY_Read()) {
			if ((INT16U)(SysTimer100ms - prev_timer) > 3) {
				prev_timer = SysTimer100ms;
				switch (page) {
					case 0:
						//if(prev_status_peel != PrtDrvStruct.HangingFormFlag){
						//	prev_status_peel = PrtDrvStruct.HangingFormFlag;
							display_clear(DISPLAY_PRICE);
							if(PrtDrvStruct.HangingFormFlag == ON) {
								caption_split_by_code(0x97a6, &cap, 0);//0x9452, &cap,0);//"  LOCKED"
							} else {
								caption_split_by_code(0x97a7, &cap, 0);//0x9453, &cap,0);//"UNLOCKED"
							}
							display_string(DISPLAY_PRICE,(INT8U *)cap.form);
						//}
						break;
					case 1:
						//if(prev_status_head != PrtDrvStruct.HeadUpFlag){
						//	prev_status_head = PrtDrvStruct.HeadUpFlag;
							display_clear(DISPLAY_PRICE);
							if(PrtDrvStruct.HeadUpFlag == ON) {
								caption_split_by_code(0x97a8, &cap, 0);//0x9454, &cap,0);
							} else {
								caption_split_by_code(0x97a9, &cap, 0);//0x9455, &cap,0);
							}
							display_string(DISPLAY_PRICE,(INT8U *)cap.form);
						//}
						break;
					case 2:
						//if(prev_gap_val != SensorStruct.GapFilVal) {
						//	prev_gap_val = SensorStruct.GapFilVal;
							display_clear(DISPLAY_PRICE);
							sprintf(string_buf, "%3d", SensorStruct.GapFilVal);
							display_string(DISPLAY_PRICE,(INT8U *)string_buf);
						//}
						break;
					case 3:
						//if(prev_peel_val != SensorStruct.PeelFilVal) {
						//	prev_peel_val = SensorStruct.PeelFilVal;
							display_clear(DISPLAY_PRICE);
							sprintf(string_buf, "%3d", SensorStruct.PeelFilVal);
							display_string(DISPLAY_PRICE,(INT8U *)string_buf);
						//}
						break;
					case 4:
						display_clear(DISPLAY_PRICE);
						sprintf(string_buf, "%3d", SensorStruct.TempFilVal);
						display_string(DISPLAY_PRICE,(INT8U *)string_buf);
						break;
#ifdef USE_PB_BATTERY
					case 5:
						//if(prev_bat_val != BatVoltVal) {
						//	prev_bat_val = BatVoltVal;
							display_clear(DISPLAY_PRICE);
							sprintf(string_buf, "%4.1fV", ConvertBatVolatage(BatVoltVal));
							display_string(DISPLAY_PRICE,(INT8U *)string_buf);
						//}
						break;
					case 6:
						display_clear(DISPLAY_PRICE);
						sprintf(string_buf, "%c", (AC_CHECK?'Y':'N'));
							display_string(DISPLAY_PRICE,(INT8U *)string_buf);
						break;
#endif
				}
				Dsp_Diffuse();
			}

			string_buf[0] = GS;
			string_buf[1] = 'V';
			PutDataRxBuf(&CommBufPrt, string_buf, 2);

//			while(CheckRxBuf(&CommBufPrt)) {
//				CASInterpreter(&CommBufPrt);
//			}
//			while(PrtDrvStruct.PrtState != PRT_STATE0I);
			PrintListWaitEndWithoutComm();
		}

		switch(KeyDrv.CnvCode) {
			case KP_ESC :
				BuzOn(1);
				PrtDrvStruct.HangingFormFlag = OFF;
				PrtDrvStruct.HeadUpFlag = OFF;
				DspStruct.DirectDraw = backup_directdraw;
				goto TEST_ESC;
			case KP_PAGE_UP :
			case KP_ARROW_UP :
				if(page <= 0) BuzOn(2);
				else {
					BuzOn(1);
					flagDisplay = 1;
					page--;
				}
				break;
			case KP_PAGE_DN :
			case KP_ARROW_DN :
				if(page >= max_page - 1) BuzOn(2);
				else {
					BuzOn(1);
					flagDisplay = 1;
					page++;
				}
				break;	
			//HYP 20050104
			case KP_FEED:
				BuzOn(1);
				//HYP 20050104
				if(!Prt_CheckPeelAndHead(OFF)) {//check headup
					//BuzOn(2);
					break;
				}
				PutCharRxBuf(&CommBufPrt, 0x0c);
				break;
			default :
				BuzOn(2);
				break;
		}
	}
TEST_ESC:
	PrtDrvStruct.PeelEnable=peel_backup;
}
#else //#if defined(USE_SINGLE_LINE_GRAPHIC)
void printer_sensor_test(void)
{
	char string_buf[50];
	POINT disp_p;
	CAPTION_STRUCT cap;
	BOOLEAN prev_status_peel,prev_status_head,peel_backup;
	INT8U prev_gap_val,prev_peel_val;
	INT8U x_point[2],x_length[2];//,i,w;
	INT16U cnt;
	INT8U backup_directdraw, page, flagDisplay;
	INT16U cap_code[2][9] = {{0x1865, 0x9701, 0x9702, 0x97a4, 0x97a5, 0x97a6, 0x97a7, 0x97a8, 0x97a9}, 
				 {0x8550, 0x9301, 0x9302, 0x9450, 0x9451, 0x9452, 0x9453, 0x9454, 0x9455}};
	INT8U cap_id;
#ifdef USE_PB_BATTERY
	INT8U x_point3, x_length3;
	INT8U x_point4, x_length4;
	INT16U prev_bat_val = 0xffff;
#endif
	INT16U prev_timer;
	
	prev_timer = SysTimer100ms - 3;
	
	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}

	backup_directdraw = DspStruct.DirectDraw;
	DspStruct.DirectDraw = 0;

	page = 0;
	cnt = 0;
	flagDisplay = 1;

	peel_backup=PrtDrvStruct.PeelEnable;
	PrtDrvStruct.PeelEnable=ENABLE;

	while(1) {
		prev_status_peel = !PrtDrvStruct.HangingFormFlag;
		prev_status_head = !PrtDrvStruct.HeadUpFlag;
		prev_gap_val     = 0;
		prev_peel_val    = 0;
#ifdef USE_PB_BATTERY
		prev_bat_val = 0xffff;
#endif

		if(flagDisplay) {
			flagDisplay = 0;
			Dsp_Fill_Buffer(0);
			DspLoadFont1(DSP_MENU_FONT_ID);

			menu_display_menucode(CurMenuCode, 1);
			menu_displaytitle(cap_code[cap_id][0], 0);//0x8550,0);
		   	menu_writepage(2, page+1);

			if(page == 0) {
				caption_split_by_code(cap_code[cap_id][3], &cap, 0);//0x9450, &cap,0);
				caption_adjust(&cap,NULL);
				x_point[0]  = cap.input_x;
				x_length[0] = cap.input_dlength;
				disp_p = point((Startup.menu_type+0)*Startup.menu_ygap, 0);
#ifdef HEBREW_FONT
				Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
				hebrew_codetemp[strlen(cap.form)] = 0;
				disp_p.x = 202 - disp_p.x;
				Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
				Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
#endif

				caption_split_by_code(cap_code[cap_id][4], &cap, 0);//0x9451, &cap,0);
				caption_adjust(&cap,NULL);
				x_point[1]  = cap.input_x;
				x_length[1] = cap.input_dlength;
				disp_p = point((Startup.menu_type+1)*Startup.menu_ygap, 0);
#ifdef HEBREW_FONT
				Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
				hebrew_codetemp[strlen(cap.form)] = 0;
				disp_p.x = 202 - disp_p.x;
				Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
				Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
#endif
			} else {
				caption_split_by_code(cap_code[cap_id][1], &cap, 0);//0x9301, &cap,0);
				caption_adjust(&cap,NULL);
				x_point[0]  = cap.input_x;
				x_length[0] = cap.input_dlength;
				disp_p = point((Startup.menu_type+0)*Startup.menu_ygap, 0);
#ifdef HEBREW_FONT
				Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
				hebrew_codetemp[strlen(cap.form)] = 0;
				disp_p.x = 202 - disp_p.x;
				Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
				Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
#endif

				caption_split_by_code(cap_code[cap_id][2], &cap, 0);//0x9302, &cap,0);
				caption_adjust(&cap,NULL);
				x_point[1]  = cap.input_x;
				x_length[1] = cap.input_dlength;
				disp_p = point((Startup.menu_type+1)*Startup.menu_ygap, 0);
#ifdef HEBREW_FONT
				Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
				hebrew_codetemp[strlen(cap.form)] = 0;
				disp_p.x = 202 - disp_p.x;
				Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
				Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
#endif

#ifdef USE_PB_BATTERY
				sprintf(cap.form, "BAT:[   ]");
				x_point3  = 5+11;
				x_length3 = 3;
				disp_p = point((Startup.menu_type+0)*Startup.menu_ygap, 11*Startup.menu_xgap);
				Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
				
				sprintf(cap.form, "AC ON :[ ]");
				x_point4  = 8+11;
				x_length4 = 1;
				disp_p = point((Startup.menu_type+1)*Startup.menu_ygap, 11*Startup.menu_xgap);
				Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
#endif
			}
	     		Dsp_Diffuse();
		}
		while(!KEY_Read()) {
			DspStruct.DirectDraw = 1;
			if(page == 0) {
				if(prev_status_peel != PrtDrvStruct.HangingFormFlag){
					prev_status_peel = PrtDrvStruct.HangingFormFlag;
					disp_p = point((Startup.menu_type+0)*Startup.menu_ygap, x_point[0]*Startup.menu_xgap);
					memset((INT8U*)string_buf, ' ', x_length[0]);	string_buf[x_length[0]] = 0;
#ifdef HEBREW_FONT
					Convert_HebrewPreview((INT8U *)string_buf, strlen(string_buf), 0, 0); 
					hebrew_codetemp[strlen(string_buf)] = 0;
					disp_p.x = 202 - disp_p.x;
					Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
					Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
#endif
					if(PrtDrvStruct.HangingFormFlag == ON) {
						caption_split_by_code(cap_code[cap_id][5], &cap, 0);//0x9452, &cap,0);//"  LOCKED"
					} else {
						caption_split_by_code(cap_code[cap_id][6], &cap, 0);//0x9453, &cap,0);//"UNLOCKED"
					}
#ifdef HEBREW_FONT
					Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
					hebrew_codetemp[strlen(cap.form)] = 0;
					//disp_p.x = 202 - disp_p.x;
					Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
					Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
#endif
				}
				if(prev_status_head != PrtDrvStruct.HeadUpFlag){
					prev_status_head = PrtDrvStruct.HeadUpFlag;
					disp_p = point((Startup.menu_type+1)*Startup.menu_ygap, x_point[1]*Startup.menu_xgap);
					memset((INT8U*)string_buf, ' ', x_length[1]);	string_buf[x_length[1]] = 0;
#ifdef HEBREW_FONT
					Convert_HebrewPreview((INT8U *)string_buf, strlen(string_buf), 0, 0); 
					hebrew_codetemp[strlen(string_buf)] = 0;
					disp_p.x = 202 - disp_p.x;
					Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
					Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
#endif
					if(PrtDrvStruct.HeadUpFlag == ON) {
						caption_split_by_code(cap_code[cap_id][7], &cap, 0);//0x9454, &cap,0);
					} else {
						caption_split_by_code(cap_code[cap_id][8], &cap, 0);//0x9455, &cap,0);
					}
#ifdef HEBREW_FONT
					Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
					hebrew_codetemp[strlen(cap.form)] = 0;
					//disp_p.x = 202 - disp_p.x;
					Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
					Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
#endif
				}
			} else {
				//if(!(cnt%10)) {
				//	cnt = 0;
				if ((INT16U)(SysTimer100ms - prev_timer) > 3) {
					prev_timer = SysTimer100ms;
					if(prev_gap_val != SensorStruct.GapFilVal) {
						prev_gap_val = SensorStruct.GapFilVal;
						disp_p = point((Startup.menu_type+0)*Startup.menu_ygap, x_point[0]*Startup.menu_xgap);
						memset((INT8U*)string_buf, ' ', x_length[0]);	string_buf[x_length[0]] = 0;
#ifdef HEBREW_FONT
						Convert_HebrewPreview((INT8U *)string_buf, strlen(string_buf), 0, 0); 
						hebrew_codetemp[strlen(string_buf)] = 0;
						disp_p.x = 202 - disp_p.x;
						Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
						Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
#endif
						sprintf(string_buf, "%3d", SensorStruct.GapFilVal);
#ifdef HEBREW_FONT
						Convert_HebrewPreview((INT8U *)string_buf, strlen(string_buf), 0, 0); 
						hebrew_codetemp[strlen(string_buf)] = 0;
						//disp_p.x = 202 - disp_p.x;
						Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
						Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
#endif
					}
					if(prev_peel_val != SensorStruct.PeelFilVal) {
						prev_peel_val = SensorStruct.PeelFilVal;
						disp_p = point((Startup.menu_type+1)*Startup.menu_ygap, x_point[1]*Startup.menu_xgap);
						memset((INT8U*)string_buf, ' ', x_length[1]);	string_buf[x_length[1]] = 0;
#ifdef HEBREW_FONT
						Convert_HebrewPreview((INT8U *)string_buf, strlen(string_buf), 0, 0); 
						hebrew_codetemp[strlen(string_buf)] = 0;
						disp_p.x = 202 - disp_p.x;
						Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
						Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
#endif
						sprintf(string_buf, "%3d", SensorStruct.PeelFilVal);
#ifdef HEBREW_FONT
						Convert_HebrewPreview((INT8U *)string_buf, strlen(string_buf), 0, 0); 
						hebrew_codetemp[strlen(string_buf)] = 0;
						//disp_p.x = 202 - disp_p.x;
						Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#else
						Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
#endif
					}
#ifdef USE_PB_BATTERY
					if(prev_bat_val != BatVoltVal) {
						prev_bat_val = BatVoltVal;
						disp_p = point((Startup.menu_type+0)*Startup.menu_ygap, x_point3*Startup.menu_xgap);
						memset((INT8U*)string_buf, ' ', x_length3);	string_buf[x_length3] = 0;
						Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
						sprintf(string_buf, "%3d]%4.1fV", BatVoltVal%1000, ConvertBatVolatage(BatVoltVal));
						Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
					}
					disp_p = point((Startup.menu_type+1)*Startup.menu_ygap, x_point4*Startup.menu_xgap);
					memset((INT8U*)string_buf, ' ', x_length4);	string_buf[x_length4] = 0;
					Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
					sprintf(string_buf, "%c", (AC_CHECK?'Y':'N'));
					Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
#endif
				}
			}
			DspStruct.DirectDraw = 0;

			//cnt++;

			string_buf[0] = GS;
			string_buf[1] = 'V';
			PutDataRxBuf(&CommBufPrt, string_buf, 2);

//			while(CheckRxBuf(&CommBufPrt)) {
//				CASInterpreter(&CommBufPrt);
//			}
//			while(PrtDrvStruct.PrtState != PRT_STATE0I);
			PrintListWaitEndWithoutComm();
		}

		switch(KeyDrv.CnvCode) {
			case KP_ESC :
				BuzOn(1);
				PrtDrvStruct.HangingFormFlag = OFF;
				PrtDrvStruct.HeadUpFlag = OFF;
				DspStruct.DirectDraw = backup_directdraw;
				goto TEST_ESC;
			case KP_PAGE_UP :
			case KP_ARROW_UP :
				if(page == 0) BuzOn(2);
				else {
					BuzOn(1);
					flagDisplay = 1;
				}
				page = 0;
				break;
			case KP_PAGE_DN :
			case KP_ARROW_DN :
				if(page == 1) BuzOn(2);
				else {
					BuzOn(1);
					flagDisplay = 1;
				}
				page = 1;
				break;	
			//HYP 20050104
			case KP_FEED:
				BuzOn(1);
				//HYP 20050104
				if(!Prt_CheckPeelAndHead(OFF)) {//check headup
					//BuzOn(2);
					break;
				}
				PutCharRxBuf(&CommBufPrt, 0x0c);
				break;
			default :
				BuzOn(2);
				break;
		}
	}
TEST_ESC:
	PrtDrvStruct.PeelEnable=peel_backup;
}
#endif //#if defined(USE_SINGLE_LINE_GRAPHIC)

void printer_preprint(INT16U a,INT16U b)
{
	PrtDrvStruct.PrePrintMode   = (INT8U)a;
	PrtDrvStruct.PrePrintLength = (INT8U)b*8;
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PrePrintMode, (HUGEDATA INT8U *)&PrtDrvStruct.PrePrintMode, 1);
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PrePrintLength, (HUGEDATA INT8U *)&PrtDrvStruct.PrePrintLength, 1);
	Flash_flush_nvbuffer();//HYP 20040204 FLASH NVRAM

	Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);

	PrtDrvSetPrePrintMode(PrtFlashStruct.PrePrintMode, PrtFlashStruct.PrePrintLength);
}

#define M_ID_PREPRT_MODE 1
#define M_ID_PREPRT_LEN 2
void printer_test_preprt(void)
{
	INT8U id_mode;
	INT8U id_len;
	INT32U mode;
	INT32U length;

	id_mode = Menu_GetMenuId(M_ID_PREPRT_MODE);
	if(id_mode == 0xff) return;
	id_len = Menu_GetMenuId(M_ID_PREPRT_LEN);
	if(id_len == 0xff) return;

	mode = *Menu[id_mode].pval;
	length = *Menu[id_len].pval;

	if(!Prt_CheckPeelAndHead(ON)) {//check peel and headup
		return;
	}
	PrtDrvSetPrePrintMode((INT8U)mode, (INT8U)length*8);
	PutCharRxBuf(&CommBufPrt, 0x0c);
}

void printer_preprint_setting(void)
{
	INT8U backup_preprint_length, backup_mode;
	CAPTION_STRUCT cap;
	INT32U temp_value[2];
	INT16U result;
	INT16U cap_code[2][3] = {{0x1737, 0x9771, 0x9772},	// Prog. Mode
				 {0x8370, 0x9371, 0x9372}};	// Cal Mode
	INT8U cap_id;

	if (status_run.run_mode==RUN_CALIBRATION) {
		cap_id = 1;
	} else {
		cap_id = 0;
	}

	backup_preprint_length  = PrtDrvStruct.PrePrintLength;
	backup_mode 		= PrtDrvStruct.PrePrintMode;

	temp_value[0] = (INT32U)PrtDrvStruct.PrePrintMode;
	temp_value[1] = (INT32U)PrtDrvStruct.PrePrintLength/8;

	Menu_Init();

	caption_split_by_code(cap_code[cap_id][0], &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(cap_code[cap_id][1], &cap, 0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_PREPRT_MODE, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value[0], NULL, NULL, NULL);
	Menu_AddKeyFunction(M_ID_PREPRT_MODE, KP_TEST, printer_test_preprt);

	caption_split_by_code(cap_code[cap_id][2], &cap, 0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_PREPRT_LEN, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &temp_value[1], NULL, NULL, NULL);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE)
	{
		printer_preprint((INT16U)temp_value[0],(INT16U)temp_value[1]);
	}
	else if(result == MENU_RET_ESC)
	{
		PrtDrvStruct.PrePrintMode   = backup_mode;
		PrtDrvStruct.PrePrintLength = backup_preprint_length;
	}
}

INT8U flagTempAllowSale;

void PrintListWaitEnd(void)
{
	flagTempAllowSale = ON;
	while(CheckRxBuf(&CommBufPrt)) {
		network_common_trans();		// Added by CJK 20051221
	}
	while(PrtDrvStruct.PrtState != PRT_STATE0I){
		network_common_trans();
	}
	flagTempAllowSale = OFF;
}


void PrintListWaitEndWithoutComm(void)
{
	flagTempAllowSale = ON;
	while(CheckRxBuf(&CommBufPrt)) {
		CASInterpreter(&CommBufPrt);
	}
	while(PrtDrvStruct.PrtState != PRT_STATE0I){
		network_common_trans();
	}
	flagTempAllowSale = OFF;
}

INT8U PrtAutoChangeMode(void)
{
	if (PrtStruct.Mode==RECEIPT) PrtChangeMode(LABEL);
	else PrtChangeMode(RECEIPT);
	return PrtStruct.Mode;
}

void PrtChangeMode(INT8U mode)
{
	char string_buf[128];
	if (mode==PrtStruct.Mode) return;
	if (mode==RECEIPT) {
		sprintf(string_buf, "%cO", GS);	// Print Mode
		PutDataRxBuf(&CommBufPrt,string_buf,2);
		string_buf[0] = RECEIPT;
		PutDataRxBuf(&CommBufPrt,string_buf,1);

		sprintf(string_buf, "%cM", GS);	// RewindMotor
		PutDataRxBuf(&CommBufPrt,string_buf,2);
		string_buf[0] = DISABLE;
		PutDataRxBuf(&CommBufPrt,string_buf,1);

		sprintf(string_buf, "%cJ", GS);	// Gap
		PutDataRxBuf(&CommBufPrt,string_buf,2);
		string_buf[0] = DISABLE;
		PutDataRxBuf(&CommBufPrt,string_buf,1);
	} else if (mode==LABEL) {
		sprintf(string_buf, "%cO", GS);
		PutDataRxBuf(&CommBufPrt,string_buf,2);
		string_buf[0] = LABEL;
		PutDataRxBuf(&CommBufPrt,string_buf,1);

		sprintf(string_buf, "%cM", GS);
		PutDataRxBuf(&CommBufPrt,string_buf,2);
		string_buf[0] = PrtFlashStruct.RewindEnable;
		PutDataRxBuf(&CommBufPrt,string_buf,1);

		sprintf(string_buf, "%cJ", GS);
		PutDataRxBuf(&CommBufPrt,string_buf,2);
		string_buf[0] = PrtFlashStruct.LabelGapMode;
		PutDataRxBuf(&CommBufPrt,string_buf,1);

		status_scale.cur_labelid = 0;	// Reload Label : Added by CJK 20050205
	}
	PrintListWaitEnd();
}


void prt_set_outofpaper(void)
{
	INT32U set_val;
	INT8U result;
	CAPTION_STRUCT cap;
	char string_buf[50];

	Menu_Init();
	set_val = SensorStruct.GapOutOfPaperThreshold;

	caption_split_by_code(0x9303, &cap,0);
	cap.form[32] = 0;
	sprintf(string_buf, "    %s", cap.form);
	Menu_HeadCreate(string_buf);

	caption_split_by_code(0x9303, &cap,0);// Question
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
			cap.input_max, cap.input_min, 0, &set_val, NULL, NULL, NULL);
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		SensorStruct.GapOutOfPaperThreshold = (INT8U)set_val;
		Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapOutOfPaperThreshold, (HUGEDATA INT8U *)&SensorStruct.GapOutOfPaperThreshold, 1);
		Flash_flush_nvbuffer();

		Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)&PrtFlashStruct, PRT_FLASH_SET_STRUCT_SIZE);
	}
}


void print_memory_LifeTime_Err(INT8U fontid)
{
	char string_buf[50];

	if(PrtStruct.Mode == LABEL) {
		LabCmd_ClearLabel();
		StrInit4UserLabel();	// Added by CJK 20060426
		RcptCmd_SetFontType(PrtFontType);//0=one byte font, 1=two byte font //SG060313
		RcptCmd_SetFontId(fontid);
		RcptCmd_IntercharSP(0);
		LabCmd_SetStrField(30, 80, 300, 418);
		LabCmd_CharSize(2, 2);
		LabCmd_StrStart();
		sprintf(string_buf, "%s\n","CAUTION!!!");
		PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf));
		if (fontid==0) {
			LabCmd_StrEnd();
			LabCmd_SetStrField(30, 80+20, 100, 418);
			LabCmd_StrStart();
		}
		sprintf(string_buf, "%s\n","Please Call Your CAS Dealer");
		PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf));

		LabCmd_StrEnd();
		RcptCmd_TypeNone();
		LabCmd_PrintStart(1);
		PrintListWaitEnd();
	} else {
		//RcptCmd_ChangeMode(TO_RECEIPT);
		RcptCmd_DefaultSet();

		RcptCmd_StrJust(LEFT);
		RcptCmd_CharLF(1);
		//RcptCmd_CharSize(1,2);
		RcptCmd_SetFontStyle(3, RPT_FONT_NORMAL, TKT_LAY_N, 0, LEFT);
		RcptCmd_TypeBold();
		RcptCmd_CharTab(1);
		sprintf(string_buf, "%s\n","CAUTION!!!");
		PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf));
		RcptCmd_CharTab(1);
		sprintf(string_buf, "%s\n","Please Call Your CAS Dealer");
		PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf));
		RcptCmd_TypeNone();
		RcptCmd_Start();
		PrintListWaitEnd();
		RcptCmd_ReceiptFeed();
		PrintListWaitEnd();
		//RcptCmd_ChangeMode(TO_RESTORE);
	}
}

#ifdef USE_INDIA_TATA_DEFAULT
void feedLength_default_for_indiaTATA(void)
{
	INT16S defaultValue_forIndiaTATA;

	defaultValue_forIndiaTATA = 210;	// 인도 현지에서 요청한 Feed Length 입력값(M1736) 210
	
	SensorStruct.GapBufInPtrTrimValue = 200 - defaultValue_forIndiaTATA;
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapBufInPtrTrimValue, (HUGEDATA INT8U *)&SensorStruct.GapBufInPtrTrimValue, 2);
}
#endif