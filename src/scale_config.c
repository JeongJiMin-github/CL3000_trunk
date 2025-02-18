/*****************************************************************************
|*  Copyright		:	(c) 2004 CAS
|*  Filename		:	scale_config.c
|*  Version		:	0.9
|*  Programmer(s)	:	getwing
|*  Created		:	2004/2/13
|*  Description		:	Scale configure      				
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "globals.h"

#include "commbuf.h"
#include "prtdrv.h"
#include "timer.h"
#include "serial.h"
#include "flash.h"
#include "sensor.h"
#include "ram.h"
#include "prt_render.h"
#include "bar_common.h"
#include "prt_font.h"
#include "dsp_common.h"
#include "bar_common.h"
#include "prt_interpreter.h"
#include "initial_glb.h"
#include "rtc.h"

#include "adm.h"
#include "main.h"
#include "initial.h"
#include "key.h"
#include "key_typedef.h"
#include "key_function.h"
#include "mode_main.h"
#include "caption_app.h"
#include "common.h"
#include "prt_common.h"
#include "menu.h"
#include "flash_app.h"
#include "dsp_app.h"
#include "dsp_vfd7.h"
#include "key_app.h"

#include "math.h" 
#include "mode_program.h"
#include "cursor.h"
#include "department.h"
#include "menu_code_text1.h"
#include "barcode_file.h"
#include "initial.h"
#include "pgm_app.h"
#include "mode_sale.h"
#include "shopname.h"
#include "prt_app.h"
#include "traceability.h"
#include "Scale_config.h"
#include "origin.h"
#include "license.h"
#include "mode_calibration.h"
#include "global_set.h"
#include "plu_device.h"
#include "initial_russia.h"
#include "debug.h"
#include "clerk_app.h"

//extern INT16S plucall_department_position,plucall_department_pos_y;
extern INT16S store_callback_position,store_callback_pos_y;
extern void salemode_reset(void);
//extern INT32U plu_get_display_info(INT16U id,CAPTION_STRUCT *cap,INT8U x);
extern INT8U  OnOff(INT8U tmp);
extern INT16U get_new_labeladdress(INT16U id,INT8U chk);
extern INT16U get_new_bitmapaddress(INT16U id,INT8U chk);
extern INT32U SearchParamSet(INT16U code, PARAMETER_SETTING_TYPE *pset);
extern void Flash_write_data(INT32U startAddr,INT16U memSize,HUGEDATA INT8U *value,INT8U v_leng,INT8U bitFlag,INT8U startBit,INT8U countBit,INT16U fcode);
extern INT32U commun_get_block_type_address(INT8U data,INT32U *max_size);
extern void SetScaleType(INT8U scaleType, INT8U display, INT8U alwaysChange);

// Menu 1897(Scanner Init) 관련
extern void barcodefile_default_KR_trace(void);
extern void scanner_default_KR_trace(void);
extern void barcodefile_default_us(void);

int store_label_bmp_data (char *file_name, LABELHEADER *l_header);

void read_datfile(void);
void update_flashmemory (INT32U addr, INT32U size);

INT8U callback_check_dept_no(INT32U num)
{
	INT32U addr;
	INT16U dept_code;

	addr =  DFLASH_BASE + FLASH_DEPARTMENT_AREA;
		addr = addr + DEPARTMENT_STRUCT_SIZE * (num-1);
	dept_code = Flash_wread(addr);
	if(dept_code != (INT16U)num) {
		BuzOn(2);
		return FALSE;
	}
	return TRUE;
}

void	scale_set_name(void)
{
	INT8U name_buffer[22];
	INT32U set_val[3], old_dept, start_addr;
	INT8U result;
	CAPTION_STRUCT cap;
	DEPARTMENT_STRUCT dept;
#ifdef USE_CHN_CART_SCALE
	INT32U market_no;
#endif

	set_val[0]=(INT32U)get_net_wparam(NETWORK_DEPARTMENT_ID);
	set_val[1]=(INT32U)get_net_wparam(NETWORK_STORE_ID);
	set_val[2]=(INT32U)get_net_wparam(NETWORK_DIVISION_ID);
#ifdef USE_CHN_CART_SCALE
	market_no = (INT32U)get_net_wparam(FLASH_MARKET_ID);
#endif
	old_dept = set_val[0];
// 현재 사용안함
	memset(name_buffer,0x0,22);	
	get_net_sparam(NETWORK_SCALE_NAME,(INT8U *)name_buffer,20);
///////////////

	Menu_Init();

	caption_split_by_code(0x1830, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	if (!Startup.disable_dept) {
		plu_get_display_info(PTYPE_DEPARTMENT_NO,&cap,0);
		Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL,plucall_department);
		Menu_AddCheckFunction(1, callback_check_dept_no);
	}

	if (Startup.country == COUNTRY_KR) {
		caption_split_by_code(0x141E, &cap,0);	// "Division Code:"
		caption_adjust(&cap,NULL);
		if (set_val[2] < cap.input_min || set_val[2] > cap.input_max)
			set_val[2] = cap.input_min;
		Menu_InputCreate(2, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[2], NULL,NULL,NULL);
	}

	caption_split_by_code(0x141A, &cap,0);	// StoreID
#ifdef USE_CHN_CART_SCALE
	cap.input_length = 3;
	cap.input_min = 1;
	cap.input_max = 999;
#endif
	caption_adjust(&cap,NULL);
	if (set_val[1] < cap.input_min || set_val[1] > cap.input_max)
		set_val[1] = cap.input_min;
	Menu_InputCreate(3, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[1], NULL,NULL,store_call_back);

	//caption_split_by_code(0x141A, &cap,0);	// StoreID
#ifdef USE_CHN_CART_SCALE
	sprintf(cap.form, "懇끝긍뵀:");
	cap.input_length = 4;
	cap.input_min = 1;
	cap.input_max = 9999;
	caption_adjust(&cap,NULL);
	if (market_no < cap.input_min || market_no > cap.input_max)
		market_no = cap.input_min;
	Menu_InputCreate(4, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &market_no, NULL,NULL,NULL);
#endif

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		set_net_wparam(NETWORK_DEPARTMENT_ID,(INT16U)set_val[0]);
		set_net_wparam(NETWORK_STORE_ID     ,(INT16U)set_val[1]);
		set_net_wparam(NETWORK_DIVISION_ID  ,(INT16U)set_val[2]);
#ifdef USE_CHN_CART_SCALE
		set_net_wparam(FLASH_MARKET_ID, (INT16U)market_no);
#endif
		if (old_dept!=set_val[0]) {
			status_scale.departmentid=set_val[0];
			start_addr = (set_val[0]-1)*DEPARTMENT_STRUCT_SIZE;
			start_addr+=  DFLASH_BASE + FLASH_DEPARTMENT_AREA;
			Flash_sread(start_addr,(HUGEDATA INT8U *)&dept,DEPARTMENT_STRUCT_SIZE);
			status_scale.cur_speedkeyid=1;
			if ((dept._speedkey>=1) && (dept._speedkey<=5))
				status_scale.cur_speedkeyid=dept._speedkey;
		}
	}
}

void callback_clerk_sale_mode(long num)
{
 	CAPTION_STRUCT 		cap;
	POINT 			disp_p;
	char	buffer[MAX_MENU_CHAR_BUF];
	INT16U 	w;

	disp_p = point(CallbackEditPosition_y*Startup.menu_ygap, CallbackEditPosition_x*Startup.menu_xgap);
	w = Dsp_GetChars();
	w -= CallbackEditPosition_x;
	memset((INT8U *)buffer,0x20, w);
	buffer[w] = 0;
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)buffer, strlen(buffer), 0, 0); 
	hebrew_codetemp[strlen(buffer)] = 0;
	disp_p.x = 202 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)buffer, strlen(buffer), 0, 0, 0); 
	arab_codetemp[strlen(buffer)] = 0;
	disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
	Dsp_Write_String(disp_p,(HUGEDATA char *)buffer);
#endif

	caption_split_by_code(0x20D1, &cap,0);
	if (!(status_scale.restrict&FUNC_CLERK)) {
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0); 
		hebrew_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0, 0);
		arab_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
		Dsp_Write_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
#endif
		return;
	}
	if ((num < cap.input_min) || (num > cap.input_max)) {
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)&global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0); 
		hebrew_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0, 0);
		arab_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
		Dsp_Write_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
#endif
	} else {
		caption_split_by_code(0x20DA-1+num, &cap,0);
		DspStruct.Reverse = 1;
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)&cap.form[3], strlen(&cap.form[3]), 0, 0); 
		hebrew_codetemp[strlen(&cap.form[3])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)&cap.form[3], strlen(&cap.form[3]), 0, 0, 0); 
		arab_codetemp[strlen(&cap.form[3])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
		Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form[0]);
//		Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form[3]);
#endif
		DspStruct.Reverse = 0;
	}
}

void callback_operation_mode(long num)
{
 	CAPTION_STRUCT 		cap;
	POINT 			disp_p;
	char	buffer[MAX_MENU_CHAR_BUF];
	INT16U 	w;

	disp_p = point(CallbackEditPosition_y*Startup.menu_ygap, CallbackEditPosition_x*Startup.menu_xgap);
	w = Dsp_GetChars();
	w -= CallbackEditPosition_x;
	memset((INT8U *)buffer,0x20, w);
	buffer[w] = 0;
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)buffer, strlen(buffer), 0, 0); 
	hebrew_codetemp[strlen(buffer)] = 0;
	disp_p.x = 202 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)buffer, strlen(buffer), 0, 0, 0); 
	arab_codetemp[strlen(buffer)] = 0;
	disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
	Dsp_Write_String(disp_p,(HUGEDATA char *)buffer);
#endif

	caption_split_by_code(0x20D2, &cap,0);
	if ((num < cap.input_min) || (num > cap.input_max)) {
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0); 
		hebrew_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0, 0);
		arab_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else		
		Dsp_Write_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
#endif
	} else {
		caption_split_by_code(0x20DD-1+num, &cap,0);
		DspStruct.Reverse = 1;
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)&cap.form[3], strlen(&cap.form[3]), 0, 0); 
		hebrew_codetemp[strlen(&cap.form[3])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)&cap.form[3], strlen(&cap.form[3]), 0, 0, 0);
		arab_codetemp[strlen(&cap.form[3])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
		Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form[0]);
//		Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form[3]);
#endif
		DspStruct.Reverse = 0;
	}
}

void callback_sale_mode(long num)
{
 	CAPTION_STRUCT 		cap;
	POINT 			disp_p;
	char	buffer[MAX_MENU_CHAR_BUF];
	INT16U 	w;
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
    if (menuDispOn) return;
  #endif
	disp_p = point(CallbackEditPosition_y*Startup.menu_ygap, CallbackEditPosition_x*Startup.menu_xgap);
	w = Dsp_GetChars();
	w -= CallbackEditPosition_x;
	memset((INT8U *)buffer,0x20, w);
	buffer[w] = 0;
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)buffer, strlen(buffer), 0, 0); 
	hebrew_codetemp[strlen(buffer)] = 0;
	disp_p.x = 202 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)buffer, strlen(buffer), 0, 0, 0); 
	arab_codetemp[strlen(buffer)] = 0;
	disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
	Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)buffer);
  #else
	Dsp_Write_String(disp_p,(HUGEDATA char *)buffer);
  #endif
#endif

	caption_split_by_code(0x3F20, &cap,0);
	if (!(status_scale.restrict&FUNC_CLERK)) {
		if ((num == 1) || (num == 4)) {
#ifdef HEBREW_FONT
			Convert_HebrewPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0); 
			hebrew_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
			Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
			Convert_ArabPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0, 0);
			arab_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
			Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
            Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
  #else
			Dsp_Write_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
  #endif
#endif
			return;
		}
	}
	if ((num < cap.input_min) || (num > cap.input_max)) {
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0); 
		hebrew_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0, 0);
		arab_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
        Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
  #else
		Dsp_Write_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
  #endif
#endif
	} else {
		caption_split_by_code((0x3F20+num), &cap,0);	  
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)&cap.form[3], strlen(&cap.form[3]), 0, 0); 
		hebrew_codetemp[strlen(&cap.form[3])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)&cap.form[3], strlen(&cap.form[3]), 0, 0, 0);
		arab_codetemp[strlen(&cap.form[3])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
        Dsp_Write_Reverse_String(disp_p,(HUGEDATA char*)&cap.form[0]);
  #else
		Dsp_Write_String(disp_p,(HUGEDATA char*)&cap.form[0]);
  #endif
//		Dsp_Write_String(disp_p,(HUGEDATA char*)&cap.form[3]);
#endif
	}
}

void scale_set_salemode(void)
{
	CAPTION_STRUCT	cap;
	INT16U	result;
	INT8U   scaleType;
	INT32U  sale_mode;
	POINT	disp_p;

	//SG060925. if self-service mode, 
	scaleType = get_global_bparam(GLOBAL_AREA_SCALETYPE);
	if(scaleType == SCALE_TYPE_SELF){
		BuzOn(3);
		return;
	}
KI_IN:
	sale_mode = get_global_bparam(GLOBAL_SALERUN_MODE);


    	Menu_Init();

	caption_split_by_code(0x1810, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x3F20, &cap, 0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length, 
				cap.input_max, cap.input_min, 0, &sale_mode, NULL, NULL, callback_sale_mode);
	Menu_SetCallbackMode(1, 1);

	result = Menu_InputDisplay();

	if(result == MENU_RET_SAVE)	//Modified by JJANG 20081201  
	{
		if (!(status_scale.restrict&FUNC_CLERK)) 
		{
			if ((sale_mode == 1) || (sale_mode == 4)) 
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
				BuzOn(2);
				goto KI_IN;
			}
		}
		set_global_bparam(GLOBAL_SALERUN_MODE, sale_mode);
		salemode_reset();
	}
}


void scale_set_operationmode(void)
{
	CAPTION_STRUCT	cap;
	INT16U	result;
	INT8U   scaleType;
	INT32U  oper_mode;
	INT32U  sale_mode;

	//SG060925. if self-service mode, 
	scaleType = get_global_bparam(GLOBAL_AREA_SCALETYPE);
	if(scaleType == SCALE_TYPE_SELF){
		BuzOn(3);
		return;
	} 
	oper_mode = get_global_bparam(GLOBAL_OPERATION_MODE);
	sale_mode = get_global_bparam(GLOBAL_AREA_SALEMODE);

    	Menu_Init();
#if !defined(USE_SINGLE_LINE_GRAPHIC)
	MenuNumLinePerPage = 2;
	MenuDefaultCallbackMode = 2;	// 2:bottom
#endif

	caption_split_by_code(0x1820, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);
#ifndef USE_ONLY_LABEL
	caption_split_by_code(0x20D1, &cap, 0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length, 
				cap.input_max, cap.input_min, 0, &sale_mode, NULL, NULL, callback_clerk_sale_mode);
	//Menu_SetCallbackMode(1, 1);
#endif
	caption_split_by_code(0x20D2, &cap, 0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(2, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length, 
				cap.input_max, cap.input_min, 0, &oper_mode, NULL, NULL, callback_operation_mode);
	//Menu_SetCallbackMode(2, 1);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		set_global_bparam(GLOBAL_AREA_SALEMODE, sale_mode);
		set_global_bparam(GLOBAL_OPERATION_MODE, oper_mode);
		Operation.operationMode = oper_mode;
	}
}

void salemode_reset(void)
{
	INT8U cc,prtset,prt_mode;
	cc=get_global_bparam(GLOBAL_SALERUN_MODE);
	if (cc==0) {
		cc=2;
		set_global_bparam(GLOBAL_SALERUN_MODE,cc);
	}
	prtset=0;
	switch (cc) {
		case 1: // REG:Ticket & Floating
			Operation.operationMode=MODE_NORMAL;
                        Operation.operationClerk=OPER_FLOATING;
			set_global_bparam(GLOBAL_AREA_SALEMODE, OPER_FLOATING);
			// Printer Mode
			//PrtStruct.Mode = (INT8U)1;
			prt_mode = 1;
			prtset=1;
			// Prepack
			prepack_onoff(2);
			break;
		case 2: // REG:Label
			Operation.operationMode=MODE_NORMAL;
                        Operation.operationClerk=OPER_NORMAL;
			set_global_bparam(GLOBAL_AREA_SALEMODE, OPER_NORMAL);
			// Printer Mode
			//PrtStruct.Mode = (INT8U)0;
			prt_mode = 0;
			prtset=1;
			// Prepack
			prepack_onoff(2);
			break;
		case 3: // REG:Prepack
			Operation.operationMode=MODE_NORMAL;
                        Operation.operationClerk=OPER_NORMAL;
			set_global_bparam(GLOBAL_AREA_SALEMODE, OPER_NORMAL);
			// Printer Mode
			// PrtStruct.Mode = (INT8U)0;
			prt_mode = 0;
			prtset=1;
			// Prepack
			prepack_onoff(1);
			break;
		case 4: // REG:Label & floating		// Modified by CJK 20050903
			Operation.operationMode=MODE_NORMAL;
                        Operation.operationClerk=OPER_FLOATING;
			set_global_bparam(GLOBAL_AREA_SALEMODE, OPER_FLOATING);
			prt_mode = 0;
			prtset   = 1;
			prepack_onoff(2);
			break;
		case 5: // Manager
			Operation.operationMode=MODE_MANAGER;
			// set_global_bparam(GLOBAL_AREA_SALEMODE, 2);
			// Printer Mode
			// Prepack
			prepack_onoff(2);
			break;
		case 6: // Repack & Label
			Operation.operationMode=MODE_REPACK;
			Operation.operationClerk=OPER_NORMAL;
			set_global_bparam(GLOBAL_AREA_SALEMODE, OPER_NORMAL);
			// set_global_bparam(GLOBAL_AREA_SALEMODE, 2);
			// Printer Mode
			prt_mode = 0;
			prtset=1;
			// Prepack
			prepack_onoff(2);
			break;
		case 7:
			Operation.operationMode=MODE_SELF;
			prt_mode = 0;
			prtset=1;
			break;
	}
        sale_display_update(UPDATE_MODE);
	set_global_bparam(GLOBAL_OPERATION_MODE,Operation.operationMode);
	if (prtset) {
#ifdef USE_BELARUS_DEFAULT
		if(prt_mode==0)	prt_mode=2;
#endif
		printer_modeset(prt_mode);
		label_load(status_scale.cur_labelid);
	}
}

void scale_parameter_display(void)
{
	CAPTION_STRUCT 	cap;
	INT32U set_val[4];
	INT8U  	info,
		nickname,
		align,
		scroll, //se-hyung 080124
		temp,	//se-hyung 080124
		uctemp;
	INT16U  result;

	uctemp  = get_global_bparam(GLOBAL_DISPLAY_INFO_FLAG);
	align	= get_global_bparam(GLOBAL_DISPLAY_ALIGN);
	temp	= get_global_bparam(GLOBAL_SALE_SETUP_FLAG7);
	
	info 	 = (uctemp>>0) & 0x01;
	nickname = (uctemp>>3) & 0x01;
	scroll	 = (temp>>2) & 0x01;	//se-hyung 080124
	set_val[0] = info;
	set_val[1] = scroll; //se-hyung 080124
	set_val[2] = nickname;
	set_val[3] = align;

	Menu_Init();
	caption_split_by_code(0x1871, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

#if !defined(USE_SINGLE_LINE_GRAPHIC)
	caption_split_by_code(0x21b1, &cap,0);
	caption_adjust(&cap,NULL);
	if (caption_run_check(cap.code,cap.runflag)) {
		Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);
	}
#ifndef USE_ONLY_LABEL
	caption_split_by_code(0x21b2, &cap,0);
	caption_adjust(&cap,NULL);
	if (caption_run_check(cap.code,cap.runflag)) {
		Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[2], NULL, NULL, NULL);
	}
#endif	
	caption_split_by_code(0x21b3, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[3], NULL, NULL, NULL);

#endif //#if !defined(USE_SINGLE_LINE_GRAPHIC)

	caption_split_by_code(0x21b4, &cap,0);
	caption_adjust(&cap,NULL);
		Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[1], NULL, NULL, NULL);

	
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		info    = (INT8U)set_val[0];
		scroll	= (INT8U)set_val[1];	//se-hyung 080124
		nickname=(INT8U)set_val[2];
		align   =(INT8U)set_val[3];
		if(info) {
			uctemp |= 0x01;// 0x40
		} else {
			uctemp &= (~0x01);
		}
		if(nickname) {
			uctemp |= 0x08;// 0x40
		} else {
			uctemp &= (~0x08);
		}
		if (scroll)		    //se-hyung 080124
		{
			temp |= 0x04;
		}
		else
		{
			temp &= (~0x04);
		}
		set_global_bparam(GLOBAL_DISPLAY_INFO_FLAG, uctemp);
		set_global_bparam(GLOBAL_DISPLAY_ALIGN, align);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG7, temp);   //se-hyung 080124
	}
}

void scale_parameter_scalesetup(void)
{	 
	CAPTION_STRUCT 	cap;
	INT8U	price; 
	INT8U	type;
	INT8U	mode;
	INT8U	sale;
	INT8U	nonfood;
	INT8U	weight;
	INT8U	tax;
	INT8U	shift;
	INT8U	dblkey;
	INT8U   dblkeytime;
	INT8U	disc;
	INT8U	startAuto;
	INT8U	quickPlu;
	INT8U	memberCode;
	INT8U	dbldate;
	INT8U	extPrintOnPrepack;
	INT8U	uctemp;
	INT8U	uctemp1;
	INT8U	uctemp2;
	INT8U	uctemp3;
	INT8U	uctemp4;
	INT8U	uctemp5;
	INT8U	uctemp6;
	INT8U	uctemp7;
	INT8U	uctemp8;
	INT8U	uctemp9;
#ifdef USE_INDIA_FUNCTION
	INT8U   uctemp10;
	INT8U   uctemp11;
#endif
#ifdef USE_CHECK_CHANGE_REASON
	INT32U	askReason;
	INT8U	uctemp12;
#endif
	INT8U	uctemp13;
#ifdef USE_TRACE_STANDALONE
	INT8U	saveTrace;
	INT8U	sendTrace;
	INT8U 	prev_saveTrace;
	INT16U	i;
#endif
	INT16U  result;
	INT32U 	set_val[18];
#ifdef USE_INDIA_FUNCTION
	INT32U  area_scale;
	INT32U  item_code13;
	INT32U  inventory;
#endif
#ifdef USE_INDIA_TAX
	INT32U	prtGSTSummary;
	INT8U	uctemp14;
#endif
		
	uctemp  = get_global_bparam(GLOBAL_SALE_SAVE_FLAG);
	uctemp1 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG1);
	uctemp2 = get_global_bparam(GLOBAL_PRINT_OPER_FLAG);
	uctemp3 = get_global_bparam(GLOBAL_DISPLAY_INFO_FLAG);
	uctemp4 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG2);
	uctemp5 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG6);  //SG060419
	uctemp6 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG5);  //SG070206
	uctemp7 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG7);	// CJK080320
//#ifdef USE_TRACE_STANDALONE
	uctemp8 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG10);
//#endif
	uctemp9 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG11);
#ifdef USE_INDIA_FUNCTION
	uctemp10 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG12);
	uctemp11 = get_global_bparam(GLOBAL_PRICESIGN_1000);
#endif
#ifdef USE_CHECK_CHANGE_REASON
	uctemp12 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG12);
#endif
	uctemp13 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG15);
#ifdef USE_INDIA_TAX
	uctemp14 = get_global_bparam(GLOBAL_SALE_SETUP_FLAG17);
#endif
	price 	= (uctemp >>0) & 0x01;
	type 	= (uctemp >>1) & 0x01;
	mode	= (uctemp1>>0) & 0x01;
	sale	= (uctemp1>>5) & 0x01;
	nonfood	= (uctemp2>>6) & 0x01;
	weight	= (uctemp2>>7) & 0x01;
	tax 	= (uctemp3>>1) & 0x01;
	shift	= OnOff(uctemp4&0x04);
	//dblkey 	= OnOff(uctemp4&0x10);
	dblkey	= (uctemp8>>5) & 0x03;
	disc	= (uctemp5>>0) & 0x01;
	startAuto= (uctemp5>>2) & 0x01;
	quickPlu= (uctemp6>>3) & 0x01;
	dbldate = (uctemp7>>3) & 0x01;
#ifdef USE_TRACE_STANDALONE
	saveTrace= (uctemp8>>0) & 0x07;
	sendTrace= (uctemp8>>3) & 0x03;
#endif
#ifdef USE_SCALE_POS_PROTOCOL
	memberCode= (uctemp6>>4) & 0x01;
#endif
	dblkeytime	= (uctemp9) & 0x0f;
#ifdef USE_CHECK_CHANGE_REASON
	askReason = (INT32U)((uctemp12>>4) & 0x01);
#endif
	extPrintOnPrepack = ((uctemp13 >> 4) & 0x01);
	set_val[0] = price;
	set_val[1] = type;
	set_val[2] = mode;
	set_val[3] = sale;
	set_val[4] = nonfood;
	set_val[5] = weight;
	set_val[6] = tax;
	set_val[7] = shift;
	set_val[8] = dblkey;
	set_val[9] = disc;
	set_val[10]= startAuto;
	set_val[11]= quickPlu;
	set_val[12]= memberCode;
	set_val[13]= dbldate;
#ifdef USE_TRACE_STANDALONE
	set_val[14]= saveTrace;
	set_val[15]= sendTrace;
	prev_saveTrace = saveTrace;
#endif
	set_val[16]= dblkeytime;
	set_val[17]= extPrintOnPrepack;
#ifdef USE_INDIA_FUNCTION
	area_scale = (INT32U)((uctemp10>>6) & 0x03);
	item_code13 = (INT32U)((uctemp11>>2) & 0x01);
	inventory = (INT32U)((uctemp11>>1) & 0x01);
#endif
#ifdef USE_INDIA_TAX
	prtGSTSummary = (INT32U)((uctemp14>>7) & 0x01);
#endif

    	// ]

	Menu_Init();
	caption_split_by_code(0x1873, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

#ifdef USE_CHECK_CHANGE_REASON
	caption_split_by_code(0x1BD2, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &askReason, NULL, NULL, NULL);
#endif

	caption_split_by_code(0x3891, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);

	caption_split_by_code(0x3892, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[1], NULL, NULL, NULL);

	caption_split_by_code(0x3893, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[2], NULL, NULL, NULL);
	caption_split_by_code(0x3894, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[3], NULL, NULL, NULL);
	caption_split_by_code(0x3895, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[4], NULL, NULL, NULL);
	caption_split_by_code(0x3896, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[5], NULL, NULL, NULL);

#ifdef USE_TAX_RATE    
	caption_split_by_code(0x3897, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[6], NULL, NULL, NULL);
#endif    
#ifndef USE_ONLY_LABEL
	caption_split_by_code(0x3898, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[7], NULL, NULL, NULL);
#endif	
	caption_split_by_code(0x3899, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[8], NULL, NULL, NULL);

	caption_split_by_code(0x3890, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[16], NULL, NULL, NULL);

	caption_split_by_code(0x389E, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[13], NULL, NULL, NULL);
	//SG060419
	caption_split_by_code(0x389A, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[9], NULL, NULL, NULL);

	//SG060928
	caption_split_by_code(0x389B, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[10], NULL, NULL, NULL);
	//CJK070130
	caption_split_by_code(0x389C, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[11], NULL, NULL, NULL);
	//SG070206. Member Code Setting
#ifdef USE_SCALE_POS_PROTOCOL
	caption_split_by_code(0x389D, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, ( char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[12], NULL, NULL, NULL);
#endif

#ifdef USE_TRACE_STANDALONE
	caption_split_by_code(0x389F, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[14], NULL, NULL, NULL);

	caption_split_by_code(0x38A0, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[15], NULL, NULL, NULL);
#endif

#ifdef USE_INDIA_FUNCTION
	caption_split_by_code(0x1C6A, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(10, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &area_scale, NULL, NULL, NULL);
	
	caption_split_by_code(0x1C6B, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(11, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &item_code13, NULL, NULL, NULL);
	
	caption_split_by_code(0x1C6C, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(12, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &inventory, NULL, NULL, NULL);
#endif
	caption_split_by_code(0x38A3, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[17], NULL, NULL, NULL);
#ifdef USE_INDIA_TAX
	caption_split_by_code(0x38A7, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &prtGSTSummary, NULL, NULL, NULL);
#endif

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		price 	= (INT8U)set_val[0];
		type  	= (INT8U)set_val[1];
		mode  	= (INT8U)set_val[2];
		sale  	= (INT8U)set_val[3];
		nonfood = (INT8U)set_val[4];
		weight	= (INT8U)set_val[5];
		tax     = (INT8U)set_val[6];
		shift	= (INT8U)set_val[7];
		dblkey 	= (INT8U)set_val[8];
		disc 	= (INT8U)set_val[9];
		startAuto=(INT8U)set_val[10];
		quickPlu= (INT8U)set_val[11];
#ifdef USE_SCALE_POS_PROTOCOL
		memberCode= (INT8U)set_val[12];
#endif
		dbldate = (INT8U)set_val[13];
#ifdef USE_TRACE_STANDALONE
		saveTrace = (INT8U)set_val[14];
		sendTrace = (INT8U)set_val[15];
#endif
		dblkeytime = (INT8U)set_val[16];
		extPrintOnPrepack = (INT8U)set_val[17];
	
		
		if(price) {
			uctemp |= 0x01;// 0
		} else {
			uctemp &= (~0x01);
		}
		if(type) {
			uctemp |= 0x02;// 1
		} else {
			uctemp &= (~0x02);
		}
		if(mode) {
			uctemp1 |= 0x01;// 0
		} else {
			uctemp1 &= (~0x01);
		}
		if(sale) {
			uctemp1 |= 0x20;// 6  0010 0000 = 
		} else {
			uctemp1 &= (~0x20);
		}  
		if(nonfood) {
			uctemp2 |= (0x01<<6);// 0
		} else {
			uctemp2 &= (~(0x01<<6));
		}
		if(weight) {
			uctemp2 |= (0x01<<7);// 6  0010 0000 = 
		} else {
			uctemp2 &= (~(0x01<<7));
		}

		if(tax) {
			uctemp3 |= 0x02;// 0x40
		} else {
			uctemp3 &= (~0x02);
		}

		if(shift) {
			uctemp4 |= 0x04;// 2bit	-> 3
		} else {
			uctemp4 &= (~0x04);
		}
		//if(dblkey) {
		//	uctemp4 |= 0x10;// 4bit	-> 
		//} else {
		//	uctemp4 &= (~0x10);
		//}
		uctemp8 &= (~0x60);
		uctemp8 |= ((dblkey<<5)&0x60);
		if(disc) {
			uctemp5 |= 0x01; // 0 
		} else {
			uctemp5 &= (~0x01);
		}
		if(startAuto) {
			uctemp5 |= 0x04;   
		} else {
			uctemp5 &= (~0x04);
		}
		if(quickPlu) {
			uctemp6 |= 0x08;   
		} else {
			uctemp6 &= (~0x08);
		}
		if(memberCode) {
			uctemp6 |= 0x10;   
		} else {
			uctemp6 &= (~0x10);
		}
		if(dbldate) {
			uctemp7 |= 0x08;
		} else {
			uctemp7 &= (~0x08);
		}

#ifdef USE_TRACE_STANDALONE
		uctemp8 &= (~0x07);
		uctemp8 |= ((saveTrace<<0)&0x07);

		uctemp8 &= (~0x18);
		uctemp8 |= ((sendTrace<<3)&0x18);
#endif
		uctemp9 &= (~0x0f);
		uctemp9 |= ((dblkeytime)&0x0f);
		
#ifdef USE_INDIA_FUNCTION
		uctemp10 &= (~(0x03<<6));
		uctemp10 |= ((area_scale&0x03)<<6); //UseAreaScaleMode
		
		uctemp11 &= (~(0x01<<2));
		uctemp11 |= ((item_code13&0x01)<<2); //UseItemCode13Digit
		
		uctemp11 &= (~(0x01<<1));
		uctemp11 |= ((inventory&0x01)<<1);

		if (item_code13)
		{
			plu_inhibit_bycode(PTYPE_EXT_BARCODE, 0); //Ext. Barcode
		}
#endif
#ifdef USE_CHECK_CHANGE_REASON
		uctemp12 &= (~(0x01 << 4));
		uctemp12 |= ((askReason & 0x01) << 4);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG12, uctemp12);
#endif
		if(extPrintOnPrepack){
			uctemp13 |= 0x10;
		}else {
			uctemp13 &= (~0x10);
		}
#ifdef USE_INDIA_TAX
		uctemp14 &= (~(0x01 << 7));
		uctemp14 |= ((prtGSTSummary & 0x01) << 7);
#endif
		set_global_bparam(GLOBAL_SALE_SAVE_FLAG  , uctemp);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG1, uctemp1);
		set_global_bparam(GLOBAL_PRINT_OPER_FLAG,  uctemp2);
		set_global_bparam(GLOBAL_DISPLAY_INFO_FLAG,uctemp3);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG2, uctemp4);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG6, uctemp5);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG5, uctemp6);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG7, uctemp7);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG10, uctemp8);
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG11, uctemp9);
#ifdef USE_INDIA_FUNCTION
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG12, uctemp10);
		set_global_bparam(GLOBAL_PRICESIGN_1000, uctemp11);
#endif
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG15, uctemp13);
#ifdef USE_INDIA_TAX
		set_global_bparam(GLOBAL_SALE_SETUP_FLAG17, uctemp14);
#endif
#ifdef USE_TRACE_STANDALONE
		GlbOper.TraceabilityCallType = saveTrace;
		if (saveTrace != prev_saveTrace)
		{
			TNT_InitTraceNoTable();
		}
#endif

	} 
}
	  
// <Func> JHC, 040408, -scale_parameter_logoff()
// [
// $Return	: void
// $Arguments	: void
// $Remark	:
// ]
void scale_parameter_logoff(void)
{
	CAPTION_STRUCT 	cap;
	INT8U	logoff,	uctemp;
	INT16U  result, uctemp1;
	INT32U set_val[4];
	
	uctemp  = get_global_bparam(GLOBAL_LOG_FLAG);
	uctemp1  = get_global_wparam(GLOBAL_TIME_CLERKLOGOUT);
	logoff 	 = (uctemp>>0) & 0x01;
	set_val[0] = logoff;
	set_val[1] = uctemp1;

	Menu_Init();
	caption_split_by_code(0x1874, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x38A1, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);
	caption_split_by_code(0x38A2, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[1], NULL, NULL, NULL);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		logoff =(INT8U)set_val[0];
		uctemp1=(INT16U)set_val[1];
		if(logoff) {
			uctemp |= 0x01;// 0x40
		} else {
			uctemp &= (~0x01);
		}
		set_global_bparam(GLOBAL_LOG_FLAG, uctemp);
		set_global_wparam(GLOBAL_TIME_CLERKLOGOUT, uctemp1);
	}
}

#ifdef USE_LOT_NUMBER_FUNCTION
void hungary_lotnumber_default(void)
{
	INT8U lot_str[20];
	INT16U str_size;
	INT32U addr_start;

	addr_start = DFLASH_BASE + FLASH_LOT_NUMBER;

	str_size = sizeof(lot_str);
	memset(lot_str, 0x00, str_size);
	Flash_swrite(addr_start, (HUGEDATA INT8U *)lot_str, str_size);			
}
#endif

#ifdef USE_USB_DATA_ROM_UPDATE
//INIT DATAROM INFO
static struct DATAROM_INFO Datarom_section[] = {
	{	.type = 6,	},
	{	.type = 20,	},
	{	.type = 21,	},
	{	.type = 8,	},
	{	.type = 4,	},
	{	.type = 12,	},
	{	.type = 13,	},
};

static INT8U Datarom_country;

SYS_FS_HANDLE dataRomHandle;
SYS_FS_FSTAT dataRomStat;

#ifdef CL3000_PROJECT
#ifdef USE_PIC32MZ_EFG
#define DATA_ROM_ROOT_DIRECTORY_PATH			"/mnt/myDrive1/cl3000n"
#define DATA_ROM_UPDATE_DIRECTORY_PATH			"/mnt/myDrive1/cl3000n/rom"
#define DATA_ROM_LABEL_UPDATE_DIRECTORY_PATH	"/mnt/myDrive1/cl3000n/rom/label"
#define DATA_ROM_ROOT_DIRECTORY_REL_PATH		"cl3000n"
#define DATA_ROM_UPDATE_DIRECTORY_REL_PATH		"cl3000n/rom"
#define VERSION_INFO_START_POS                  15 // CL3000N_YYMMDD_XX_V3_~.hex
#else
#define DATA_ROM_ROOT_DIRECTORY_PATH			"/mnt/myDrive1/cl3000"
#define DATA_ROM_UPDATE_DIRECTORY_PATH			"/mnt/myDrive1/cl3000/rom"
#define DATA_ROM_LABEL_UPDATE_DIRECTORY_PATH	"/mnt/myDrive1/cl3000/rom/label"
#define DATA_ROM_ROOT_DIRECTORY_REL_PATH		"cl3000"
#define DATA_ROM_UPDATE_DIRECTORY_REL_PATH		"cl3000/rom"
#define VERSION_INFO_START_POS                  14 // CL3000_YYMMDD_XX_V2_~.hex
#endif
#define DATA_UPDATE_FILE_PREFIX					"CL30-*.dat"
#else
#ifdef USE_PIC32MZ_EFG
#define DATA_ROM_ROOT_DIRECTORY_PATH			"/mnt/myDrive1/cl5200jn"
#define DATA_ROM_UPDATE_DIRECTORY_PATH			"/mnt/myDrive1/cl5200jn/rom"
#define DATA_ROM_LABEL_UPDATE_DIRECTORY_PATH	"/mnt/myDrive1/cl5200jn/rom/label"
#define DATA_ROM_ROOT_DIRECTORY_REL_PATH		"cl5200jn"
#define DATA_ROM_UPDATE_DIRECTORY_REL_PATH		"cl5200jn/rom"
#else
#define DATA_ROM_ROOT_DIRECTORY_PATH			"/mnt/myDrive1/cl5200j"
#define DATA_ROM_UPDATE_DIRECTORY_PATH			"/mnt/myDrive1/cl5200j/rom"
#define DATA_ROM_LABEL_UPDATE_DIRECTORY_PATH	"/mnt/myDrive1/cl5200j/rom/label"
#define DATA_ROM_ROOT_DIRECTORY_REL_PATH		"cl3000"
#define DATA_ROM_UPDATE_DIRECTORY_REL_PATH		"cl5200j/rom"
#endif
#define DATA_UPDATE_FILE_PREFIX					"CL52J-*.dat"
#endif
#define DATA_ROM_UPDATE_FILE_CL5200J_PREFIX				"CL52J-*.rom"
#define DATA_ROM_UPDATE_FILE_CL3000_PREFIX				"CL30-*.rom"
#define DATA_ROM_UPDATE_INI_FILE_PREFIX         "romconfig.ini"
#ifdef USE_PIC32MZ_EFG
  #ifdef CL3000_BP
	#define FW_UPDATE_FILE_PREFIX_UPPER		"CL3000N_*.hex"
	#define FW_UPDATE_FILE_PREFIX_LOWER		"cl3000n_*.hex"
  #else
	#define FW_UPDATE_FILE_PREFIX_UPPER		"CL5200JN_*.hex"
	#define FW_UPDATE_FILE_PREFIX_LOWER		"cl5200jn_*.hex"
  #endif
#else
  #ifdef CL3000_BP
	#define FW_UPDATE_FILE_PREFIX_UPPER		"CL3000_*.hex"
	#define FW_UPDATE_FILE_PREFIX_LOWER		"cl3000_*.hex"
  #else
	#define FW_UPDATE_FILE_PREFIX_UPPER		"CL5200j_*.hex"
	#define FW_UPDATE_FILE_PREFIX_LOWER		"cl5200j_*.hex"
  #endif
#endif

#define INI_KEY_NAME_INDEX     "index"
#define INI_KEY_NAME_DOWNLOAD  "download"
#define INI_KEY_NAME_SCALE_TYPE  "scaletype"

#define MAX_FLASH_UPDATE_MASK_NUM	5

// mask_range 추가 시 주소 순서대로 등록, 배열 Max Number 증가
// CL5200J_USBUpdate_MaskRange.docx 파일에 mask영역 등록하여 관리 TXT 문서로 관리.
// 메모리 주소관련 문서 참조하여 추가
INT32U update_mask_range[MAX_FLASH_UPDATE_MASK_NUM][2] = {
	//{start_adress, mask_size}
	{8, 73},		// ROM INIT ~ FW download setting
	{90, 312},		// Power On Counter ~ ADC Area(Basic)
	{412, 6},		// Ethernet MAC
	{614796, 80},	// License Key
	{4194255, 49}	// EXT_MEMORY (USB_DATA_ROM_UPDATE, FTL_ERR_MANAGEMENT_AREA)
};

//END INIT DATAROM INFO

int GetCountry(char *header_ver)
{
	char *pos;
	char fname[3];

	pos = strstr(header_ver, "-MA-0"); //CL52J-MA-03EUR-..., CL55-MA-03EUR-..., CLP-MA-03EUR-...
	if(pos == NULL)
		return -1;
	safe_strncpy (fname, &pos[4], sizeof(fname));
	fname[2] = 0; 
	return atoi(fname);
}
extern INT8U UsbdeviceIsConnected;
int get_datarom_name (char *name, int size, const char * path)
{
	SYS_FS_HANDLE dirHandle;
	SYS_FS_FSTAT stat;

	char longFileName[FAT_FS_MAX_LFN];
	char fname[FAT_FS_MAX_LFN];
	char temp[FAT_FS_MAX_LFN];
	
	memset(name, 0, size);
	
	dirHandle = SYS_FS_DirOpen(DATA_ROM_UPDATE_DIRECTORY_PATH);

	if (dirHandle == SYS_FS_HANDLE_INVALID)
	{
		return -1;
	}
	
	stat.lfname = longFileName;
	stat.lfsize = FAT_FS_MAX_LFN;
	
	if(SYS_FS_DirSearch(dirHandle,path,SYS_FS_ATTR_ARC,&stat) == SYS_FS_RES_FAILURE)
	{
		if(SYS_FS_DirClose(dirHandle) == SYS_FS_RES_FAILURE)
		{
			return -1;
		}
		return -1;
	}

	if(SYS_FS_DirClose(dirHandle) == SYS_FS_RES_FAILURE)
	{
		return -1;
	}
	else
	{
		if (stat.lfname[0] != 0)
		{
			strcpy(temp, stat.lfname);
		}
		else if (stat.fname[0] != 0)
		{
			strcpy(temp, stat.fname);
		}
		strcpy(name,temp);
	}
	
	if(name[0])
		return 0;
	else
		return -1;
}

INT8S get_download_inf (ROM_DOWNLOAD_INF *downloadInf)
{
	SYS_FS_HANDLE dataRomInfHandle;
	char full_path[FAT_FS_MAX_LFN];
	INT32U pos;
	INT32U i;
	ROM_DOWNLOAD_INI fileInf;
	INT8U ch;
	INT8U temp[256];
	INT32U index;
	INT8U nameStr[12];
	
	INT8U iniFileStatus;
	iniFileStatus = INI_CHECK_STATUS;
	pos = 0;
	i = 0;
	sprintf(full_path, "%s/", DATA_ROM_UPDATE_DIRECTORY_PATH);
	strcat(full_path, DATA_ROM_UPDATE_INI_FILE_PREFIX);
	
	dataRomInfHandle = SYS_FS_FileOpen(full_path,(SYS_FS_FILE_OPEN_READ));
	if (dataRomInfHandle == SYS_FS_HANDLE_INVALID)
	{
		return -1;
	}
	memset(&fileInf,0x00,sizeof(ROM_DOWNLOAD_INI));
	
	if (dataRomInfHandle == SYS_FS_HANDLE_INVALID)
	{
		return -1;
	}
		
	while(1)
	{
		if(SYS_FS_FileEOF(dataRomInfHandle)) break;
		SYS_FS_FileSeek(dataRomInfHandle,pos, SYS_FS_SEEK_SET);
		SYS_FS_FileRead(dataRomInfHandle, &ch, 1);
		pos++;
		
		switch(iniFileStatus)
		{
			case INI_CHECK_STATUS:
				if(ch == '[')
					iniFileStatus = INI_SECTION;
				else
				{
					iniFileStatus = INI_KEY;
					temp[i++] = ch;
				}
				break;	
			case INI_SECTION:
				i = 0;
				memset(temp,0x00,sizeof(temp));
				iniFileStatus = INI_GET_SECTION_NAME;
			case INI_GET_SECTION_NAME:
				if(ch == ']')
				{
					iniFileStatus = INI_SAVE_SECTION_NAME;
				}
				else
				{
					temp[i++] = ch;
				}
				break;
			case INI_SAVE_SECTION_NAME:
				strcpy((char*)fileInf.section, (char*)temp);
				i = 0;
				memset(temp,0x00,sizeof(temp));
				iniFileStatus = INI_CHECK_CRLF;
				break;
			case INI_CHECK_CRLF:
				if(ch == 0x0A) 
				{
					iniFileStatus = INI_CHECK_STATUS;
				}
				break;
			case INI_KEY: //index
				if(ch == '=')
				{
					if(!strcmp((char*)temp,INI_KEY_NAME_INDEX))
						iniFileStatus = INI_INDEX_VALUE;
					if(!strcmp((char*)temp,INI_KEY_NAME_DOWNLOAD))
						iniFileStatus = INI_DOWNLOAD_VALUE;
					if(!strcmp((char*)temp,INI_KEY_NAME_SCALE_TYPE))
						iniFileStatus = INI_SCALE_TYPE_VALUE;
					i = 0;
					memset(temp,0x00,sizeof(temp));
				}
				else
				{
					temp[i++] = ch;
				}
				break;
			case INI_INDEX_VALUE:
				if(ch == 0x0D)
				{
					fileInf.index = str2long_n(temp,strlen((char*)temp));
					i = 0;
					memset(temp,0x00,sizeof(temp));
				}
				else if(ch == 0x0A)
				{
					iniFileStatus = INI_CHECK_STATUS;
				}
				else
				{
					temp[i++] = ch;
				}
				break;
			case INI_DOWNLOAD_VALUE:
				if(ch == 0x0D)
				{
					fileInf.download = temp[0];
					i = 0;
					memset(temp,0x00,sizeof(temp));
					iniFileStatus = NEXT_SECTION;
				}
				else if(ch == 0x0A)
				{
					break;
				}
				else
				{
					temp[i++] = ch;
				}
				break;
			case INI_SCALE_TYPE_VALUE:
				if(ch == 0x0D)
				{
					downloadInf->scaleType = str2long_n(temp,strlen((char*)temp));
					i = 0;
					memset(temp,0x00,sizeof(temp));
				}
				else if(ch == 0x0A)
				{
					iniFileStatus = INI_CHECK_STATUS;
				}
				else
				{
					temp[i++] = ch;
				}
				break;
			case NEXT_SECTION:
				downloadInf->download[fileInf.index] = fileInf.download;
				iniFileStatus = INI_CHECK_STATUS;
				break;
		}
	}
	if (dataRomInfHandle != SYS_FS_HANDLE_INVALID)
	{
		SYS_FS_FileClose(dataRomInfHandle);
	}
	return 0;
}

int open_datarom (void)
{
	char datarom_name[FAT_FS_MAX_LFN];
	char full_path[FAT_FS_MAX_LFN];
	
	if (get_datarom_name(datarom_name, FAT_FS_MAX_LFN, DATA_ROM_UPDATE_FILE_CL3000_PREFIX) < 0)
	{
		if (get_datarom_name(datarom_name, FAT_FS_MAX_LFN, DATA_ROM_UPDATE_FILE_CL5200J_PREFIX) < 0)
		{
			return -1;
		}
	}
	
	sprintf(full_path, "%s/", DATA_ROM_UPDATE_DIRECTORY_PATH);
	strcat(full_path, datarom_name);
	
	dataRomHandle = SYS_FS_FileOpen(full_path,(SYS_FS_FILE_OPEN_READ));
	if (dataRomHandle == SYS_FS_HANDLE_INVALID)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int close_datarom (void)
{
	if (dataRomHandle != SYS_FS_HANDLE_INVALID)
	{
		SYS_FS_FileClose(dataRomHandle);
	}
	return 0;
}

int get_firmware_name(char* name, const char *prefix)
{    
    char longFileName[FAT_FS_MAX_LFN];
    char temp[FAT_FS_MAX_LFN];
    SYS_FS_HANDLE dirHandle;
    SYS_FS_FSTAT stat;
    
    memset(name, 0, FAT_FS_MAX_LFN);
    
    dirHandle = SYS_FS_DirOpen(DATA_ROM_ROOT_DIRECTORY_PATH);

    if (dirHandle == SYS_FS_HANDLE_INVALID)
    {
        return -1;
    }
    
    stat.lfname = longFileName;
    stat.lfsize = FAT_FS_MAX_LFN;
    
    if(SYS_FS_DirSearch(dirHandle,prefix,SYS_FS_ATTR_ARC,&stat) == SYS_FS_RES_FAILURE)
    {
        if(SYS_FS_DirClose(dirHandle) == SYS_FS_RES_FAILURE)
        {
            return -1;
        }
        return -1;
    }

    if(SYS_FS_DirClose(dirHandle) == SYS_FS_RES_FAILURE)
    {
        return -1;
    }
    else
    {
        if (stat.lfname[0] != 0)
        {
            strcpy(temp, stat.lfname);
        }
        else if (stat.fname[0] != 0)
        {
            strcpy(temp, stat.fname);
        }
        strcpy(name,temp);
    }
    
    if(name[0])
        return 0;
    else
        return -1;
}

#ifdef USE_DSP_USB_FW_UPDATE_VERSION
int get_firmware_version(char* versionBuf, INT32U version_buf_size)
{
    char fwfile_name[FAT_FS_MAX_LFN];
    char tempBuf[70];
	char *tok;
	INT32U tok_cnt = 1;
	INT32S buf_idx = 0;
	int ret = -1;

    if(get_firmware_name(fwfile_name, FW_UPDATE_FILE_PREFIX_UPPER) < 0) // check CL3000N Firmware file
    {
    	if(get_firmware_name(fwfile_name, FW_UPDATE_FILE_PREFIX_LOWER) < 0) // check CL3000N Firmware file
    	{
    		return -1; // file read faile
    	}
    }
    strncpy(tempBuf, fwfile_name, sizeof(tempBuf));

	tok = strtok(fwfile_name, "_");

	memset(tempBuf, 0, sizeof(tempBuf));
	while (tok != NULL) {
		if (tok_cnt > 2) { // 모델명, 날짜 스킵
			INT32U tok_len = strlen(tok);
			INT32U check_buf_idx = buf_idx+tok_len+1;

			if (check_buf_idx >= version_buf_size-1 || check_buf_idx >= sizeof(tempBuf)-1)
				break;

			if (tok_cnt >= 4 && tok_len >= 2 && tok[0] == 'r' && tok[1] >= '0' && tok[1] <= '9') {
				strncpy(versionBuf, tempBuf, strlen(tempBuf)-1); // 마지막 '.'은 복사 x
				ret = 0;
				break;
			}

			strncat(tempBuf, tok, tok_len);
			strcat(tempBuf, ".");
			buf_idx += tok_len+1;
		}
		tok = strtok(NULL, "_");
		tok_cnt++;
	}
	return ret;
}

void get_datarom_scaleType(INT32U scaleType, char* typestr)
{
    switch(scaleType)
    {
        case SCALE_TYPE_BENCH:
            sprintf(typestr, "B");
            break;
        case SCALE_TYPE_POLE:
            sprintf(typestr, "P");
            break;
        default:
            memset(typestr, 0, sizeof(typestr));
            break;
    }
}
#endif

int get_header_of_datarom (void)
{
	int ret;
	MASTER_MEMORY rom_header;

	if (dataRomHandle == SYS_FS_HANDLE_INVALID)
	{
		return -1;
	}
	
	ret = SYS_FS_FileRead(dataRomHandle, &rom_header, sizeof(MASTER_MEMORY));
	if(ret < 0)
		return -1;
	
	ret = GetCountry(rom_header.ver);
	if(ret == -1)
	{
		Datarom_country = COUNTRY_UN;
	}
	else
	{
		Datarom_country = ret;
	}

	rom_header.caption_size = 72004l+6400l+PCS_TABLE_SIZE+4096l;
	rom_header.symbol_size  = SIZE_REAL_BASICSYMBOL;
	
	Datarom_section[ROM_PARAM].pointer = rom_header.param_pnt;
	Datarom_section[ROM_SYMBOL].pointer = rom_header.symbol_pnt;
	Datarom_section[ROM_CAPTION].pointer = rom_header.caption_pnt;
	Datarom_section[ROM_ORIGIN].pointer = rom_header.origin_pnt;
	Datarom_section[ROM_KEYPAD].pointer = rom_header.keypad_pnt;
	Datarom_section[ROM_FONT].pointer = rom_header.font_pnt;
	Datarom_section[ROM_LABEL].pointer = rom_header.label_pnt;
	
	Datarom_section[ROM_PARAM].size = rom_header.param_size;
	Datarom_section[ROM_SYMBOL].size = rom_header.symbol_size;
	Datarom_section[ROM_CAPTION].size = rom_header.caption_size;
	Datarom_section[ROM_ORIGIN].size = rom_header.origin_size;
	Datarom_section[ROM_KEYPAD].size = rom_header.keypad_size;
	Datarom_section[ROM_FONT].size = rom_header.font_size;
	Datarom_section[ROM_LABEL].size = rom_header.label_size;
    
	return 0;
}

int get_header_of_labelform (void)
{
	SYS_FS_HANDLE labelFormDirHandle;
	SYS_FS_HANDLE labelFormFileHandle;
	SYS_FS_FSTAT labelFormDirStat;
	LABELHEADER labelHeader;
	char name_temp[FAT_FS_MAX_LFN];
	char fileName[FAT_FS_MAX_LFN];
	char full_path[FAT_FS_MAX_LFN];
	
	labelFormDirHandle = SYS_FS_DirOpen(DATA_ROM_LABEL_UPDATE_DIRECTORY_PATH);
	
	if(labelFormDirHandle != SYS_FS_HANDLE_INVALID)
	{
		labelFormDirStat.lfname = name_temp;
		labelFormDirStat.lfsize = FAT_FS_MAX_LFN;
		while(1)
		{
			SYS_FS_DirRead(labelFormDirHandle, &labelFormDirStat);
			if (labelFormDirHandle == SYS_FS_RES_FAILURE)
			{
				SYS_FS_DirClose(labelFormDirHandle);
				return -1;
			}

			if (labelFormDirStat.lfname[0] != 0)
			{
				strcpy(fileName, labelFormDirStat.lfname);
			}
			else if (labelFormDirStat.fname[0] != 0)
			{
				strcpy(fileName, labelFormDirStat.fname);
			}
			else
			{
				break;
			}
			if(strstr(fileName, ".lfm")|| strstr(fileName, ".LFM"))
			{
				sprintf(full_path, "%s/", DATA_ROM_LABEL_UPDATE_DIRECTORY_PATH);
				strcat(full_path, fileName);
			
				labelFormFileHandle = SYS_FS_FileOpen(full_path,(SYS_FS_FILE_OPEN_READ));
				if(labelFormFileHandle == SYS_FS_HANDLE_INVALID) return -1;
				
				SYS_FS_FileRead(labelFormFileHandle,&labelHeader, sizeof(LABELHEADER));
				SYS_FS_FileClose(labelFormFileHandle);
				
				if(store_label_bmp_data(fileName, &labelHeader) < 0)
				{
					return -1;
				}
				continue;
			}
		}
		SYS_FS_DirClose(labelFormDirHandle);
	}
	else
	{
		return -1;
	}
	return 0;
}

int store_label_bmp_data (char *file_name, LABELHEADER *l_header)
{
	int i;
	char full_path[256];
	INT32U nth_addr = 999;
	INT32U addr;
	INT32U pos;
	int ret;
	INT8U temp[BITMAP_MAX_SIZE];
	INT32U saddr_label = DFLASH_BASE + FLASH_LABEL_AREA;
	INT32U saddr_bmp = DFLASH_BASE + FLASH_BITMAP_AREA;
	LABEL_IMAGE_INFO label_info;
	LABEL_BITMAP_INFO bitmap_info;
	
	SYS_FS_HANDLE labelFormBmpHandle;

	
	sprintf(full_path, "%s/%s",DATA_ROM_LABEL_UPDATE_DIRECTORY_PATH, file_name);
	labelFormBmpHandle = SYS_FS_FileOpen(full_path,(SYS_FS_FILE_OPEN_READ));
	
	if (labelFormBmpHandle == SYS_FS_HANDLE_INVALID)
	{
		return -1;
	}
		
	if(l_header->size_label)
	{			//label
		nth_addr= (INT32U)get_new_labeladdress(l_header->id, 0);
		if(nth_addr == 999)
		{
			ret = -1;
			goto CLOSE_FP;
		}
		
		//store label info
		label_info.id = l_header->id;
		label_info.width = l_header->width;
		label_info.height = l_header->height;
		safe_strncpy(label_info.labelname, l_header->name, sizeof(label_info.labelname));
		label_info.size = l_header->size_label;

		addr = get_addr_from_label_idx(nth_addr, LABEL_AREA_HEADER);
		save_label_header(addr, (struct LABEL_HEADER_INFO*)&label_info, L_H_ALL_BIT);	

		//store label image

		memset(temp,0x00,sizeof(temp));
		switch (l_header->version[1])
		{
			case 0:	pos = 52;	break;
			case 1:	pos = 122;	break;
			case 2:	pos = 128;	break;
		}
		SYS_FS_FileSeek(labelFormBmpHandle, pos, SYS_FS_SEEK_SET);
		SYS_FS_FileRead(labelFormBmpHandle, temp, l_header->size_label);

		
		addr = get_addr_from_label_idx(nth_addr, LABEL_AREA_IMAGE);
		Flash_swrite(addr, temp, l_header->size_label);

	}
	for(i = 0; i < sizeof(l_header->stImageIf) / sizeof(l_header->stImageIf[0]) ; i++) {
		if(l_header->stImageIf[i].size) {		//bmp
			nth_addr= (INT32U)get_new_bitmapaddress(l_header->stImageIf[i].id, 0);		
			if(nth_addr == 999) {
				ret = -1;
				goto CLOSE_FP;
			}
			//store bmp info
			bitmap_info.id = l_header->stImageIf[i].id;
			bitmap_info.width = l_header->stImageIf[i].width;
			bitmap_info.height = l_header->stImageIf[i].height;
			addr  = saddr_bmp + (nth_addr * BITMAP_INFO_SIZE);
			Flash_swrite(addr, (INT8U *)&bitmap_info, BITMAP_INFO_SIZE);
			
			//store bmp image
			SYS_FS_FileSeek(labelFormBmpHandle, l_header->stImageIf[i].addr, SYS_FS_SEEK_SET);
			SYS_FS_FileRead(labelFormBmpHandle, temp, l_header->stImageIf[i].size);
			addr = saddr_bmp + BITMAP_IMAGE + (nth_addr * BITMAP_MAX_SIZE);
			Flash_swrite(addr, temp, l_header->stImageIf[i].size);
		}
	}
	ret = 0;
CLOSE_FP:
	SYS_FS_FileClose(labelFormBmpHandle);

	return ret;
}


int copy_datarom_to_flash (enum DATAROM_LIST section)
{
	char temp[512];
	INT32U store_size;
	INT32U addr;
	INT32U paraTotalCnt;
	PARAMETER_SETTING_TYPE pset;
	PARAMETER_FILE_STRUCT fpara;
	int i;
	INT32U data_ptr;
	int ret;
	INT8U  str_length;
	INT32U remainByte;
	INT32U totalReadSize;
	INT32U blockCount;
	
	if(section == ROM_PARAM && Datarom_section[ROM_PARAM].size)
	{
		paraTotalCnt = Datarom_section[ROM_PARAM].size / sizeof(PARAMETER_FILE_STRUCT);
		for (i = 0 ; i < paraTotalCnt ; i++)
		{
			data_ptr = Datarom_section[ROM_PARAM].pointer + (i * sizeof(PARAMETER_FILE_STRUCT));

			SYS_FS_FileSeek(dataRomHandle,data_ptr,SYS_FS_SEEK_SET);
			ret = SYS_FS_FileRead(dataRomHandle, &fpara, sizeof(PARAMETER_FILE_STRUCT));
            
			if(ret != sizeof(PARAMETER_FILE_STRUCT))
			{
				return -1;
			}
			SearchParamSet(fpara.code, &pset);
			str_length = strlen(fpara.data);
			if(str_length > sizeof(PARAMETER_FILE_STRUCT))
			{
				str_length = sizeof(PARAMETER_FILE_STRUCT);
			}
			if (pset.addrMemory)
			{
				addr = DFLASH_BASE + pset.addrMemory + pset.offsetMemory;
				Flash_write_data(addr,pset.sizeMemory,(INT8U *)&fpara.data,str_length,pset.bitFlag,pset.startBit,pset.countBit, fpara.code);
			}
		}
	} 
	else if (section == ROM_LABEL)
	{
		close_datarom();
		label_clear();
		bitmap_clear();
		ret = get_header_of_labelform();
		if(open_datarom()< 0) return -1;
		if(ret < 0) return -1;
	}
	else
	{
		SYS_FS_FileSeek(dataRomHandle, Datarom_section[section].pointer, SYS_FS_SEEK_SET);

		totalReadSize = Datarom_section[section].size;
		remainByte = totalReadSize % sizeof(temp);
		blockCount = totalReadSize / sizeof(temp);
		addr = commun_get_block_type_address(Datarom_section[section].type, &store_size);
		
		for(i=0; i < blockCount; i++)
		{
			ret = SYS_FS_FileSeek(dataRomHandle, Datarom_section[section].pointer+(i*sizeof(temp)), SYS_FS_SEEK_SET);
			if(ret == -1) return -1;
			ret = SYS_FS_FileRead(dataRomHandle, temp, sizeof(temp));
			if(ret == -1) return -1;
			Flash_swrite(addr+(i*(sizeof(temp))), (INT8U*)temp, sizeof(temp));
		}
		if(remainByte != 0)
		{
			ret = SYS_FS_FileSeek(dataRomHandle, Datarom_section[section].pointer+(blockCount*sizeof(temp)), SYS_FS_SEEK_SET);
			if(ret == -1) return -1;
			ret = SYS_FS_FileRead(dataRomHandle,temp, remainByte);
			if(ret == -1) return -1;
			Flash_swrite(addr+(blockCount*(sizeof(temp))), (INT8U*)temp, remainByte);
		}
	}
	return 0;
}

void dataRomStatusSave(void)
{
	INT32U addr;
	addr = EXT_MEMCARD_ADDR_X + FLASH_USB_DATA_ROM_UPDATE;
	Flash_swrite(addr, (INT8U*)&UsbDataRomUpdate, 1);
}
void dataRomStatusLoad(void)
{
	INT32U addr;
	addr = EXT_MEMCARD_ADDR_X + FLASH_USB_DATA_ROM_UPDATE;
	Flash_sread(addr, (INT8U*)&UsbDataRomUpdate, 1);
}

extern INT8U dealer_factory_mode;
INT8U data_rom_usb_update(void) // USB Memory -> Scale
{
	ROM_DOWNLOAD_INF setVal;
	int addr;
	INT8U buffer[HISTORY_BUF_SIZE];
	INT8U i;
	INT8U downCnt = 0;
	INT8U resetOn;
	INT8U mode;
	INT8U string_buf[32];
	int ret;
#ifdef USE_DSP_USB_FW_UPDATE_VERSION
	INT8U str_buf[13];
	char version[30];
	char scaleTypeStr[11];
	int readversion = -1;
	INT32U scaleType;
#endif

	if(UsbdeviceIsConnected)
	{
		if(get_download_inf(&setVal) < 0) return 0;
		for (i = 0; i < DATAROM_INI_NO; i++)
		{
			if(setVal.download[i] == 'Y') break;
			else downCnt++;
		}
		if (downCnt == DATAROM_INI_NO) return 0; //9의 의미는 setVal.Download[index]의 index 최대 갯수이다. downCnt 값이 9개라는 의미는 다운로드할 파라미터가 없다는 의미와 같다.
		
		Dsp_Fill_Buffer(0);
	
		dataRomStatusLoad();

		if(UsbDataRomUpdate&(~DATAROMUPDATE_READY))	UsbDataRomUpdate= DATAROMUPDATE_READY; // ready
		if(UsbDataRomUpdate == DATAROMUPDATE_COMPLETE) // Update 완료
		{
			UsbDataRomUpdate = DATAROMUPDATE_READY; // ready
			dataRomStatusSave();
			Flash_flush_nvbuffer();
			return 0;
		}

#ifdef USE_DSP_USB_FW_UPDATE_VERSION        
        memset(version, 0, strlen(version));
        readversion = get_firmware_version(version, sizeof(version));
        if (setVal.download[8] == 'Y')
            scaleType = setVal.scaleType;
        else 
            scaleType = 0;
#endif
		if(UsbDataRomUpdate&DATAROMUPDATE_CONFIRM) // Update confirm
		{
			if(setVal.download[10] == 'Y') // confirm 여부
			{
				BuzOn(1);
				display_alloff_primary_indication();
				VFD7_Diffuse();
				display_lcd_clear();
				DspLoadFont1(2);
				sprintf((char*)string_buf, "Update USB?");
				Dsp_Write_String(point(0, 0),(HUGEDATA char*)string_buf);
				Dsp_Diffuse();
#ifdef USE_DSP_USB_FW_UPDATE_VERSION    
				if(readversion >= 0)
				{
					get_datarom_scaleType(scaleType, scaleTypeStr);
					sprintf((char*)string_buf, "TYPE.%s", scaleTypeStr);
					display_string(DISPLAY_WEIGHT, (INT8U*)string_buf);
					display_string_pos(DISPLAY_START_POS_UNITPICE, (INT8U*)version);
					VFD7_Diffuse();
				}
#endif
				while(!KEY_Read());
				BuzOn(1);
				switch(KeyDrv.CnvCode)
				{
					case KEY_NUM_1:
					case KP_ASC_Y:
						sale_display_update(UPDATE_ALL);
						break;
					default:
					case KEY_NUM_0:
						sale_display_update(UPDATE_ALL);
						return 0;
				}
			}
			
			UsbDataRomUpdate &= ~DATAROMUPDATE_CONFIRM;
			dataRomStatusSave();
		}
		
		if(UsbDataRomUpdate&DATAROMUPDATE_FWUPDATE) // Firmware Update
		{
			resetOn = OFF;
			if(setVal.download[9] == 'Y')
			{
				mode = Flash_bread(DFLASH_BASE + FLASH_FWMODE_SET);
				mode |= 0x04; // Firmware update on
				Flash_bwrite(DFLASH_BASE + FLASH_FWMODE_SET, mode);
				
				resetOn = ON;
			}
			UsbDataRomUpdate &= ~DATAROMUPDATE_FWUPDATE;
			dataRomStatusSave();
			Flash_flush_nvbuffer();
			if (resetOn)
			{
				BuzOn(1);
				display_alloff_primary_indication();
				VFD7_Diffuse();
				display_lcd_clear();
				DspLoadFont1(2);
#ifdef USE_DSP_USB_FW_UPDATE_VERSION
				if(readversion >= 0)
				{
					get_datarom_scaleType(scaleType, scaleTypeStr);
					sprintf((char*)str_buf, "TYPE %s", scaleTypeStr);
					display_string(DISPLAY_UNITPRICE, (INT8U*)str_buf);
					display_string_pos(DISPLAY_START_POS_PRICE, (INT8U*)version);
					VFD7_Diffuse();
				}
				sprintf((char*)string_buf, "Updating...");
				display_message_page_mid_delay(string_buf, 20);
				sprintf((char*)string_buf, "Keep turn on");
#else
				sprintf((char*)string_buf, "FW UP by USB");
#endif
				Dsp_Write_String(point(0, 0),(HUGEDATA char*)string_buf);
				Dsp_Diffuse();
				
				software_reset();
			}
		}
			
		if(UsbDataRomUpdate&DATAROMUPDATE_MEMCLR) // 메모리 클리어 체크
		{
			VertScrollMsg_Init(DSP_PLU_FONT_ID);
			resetOn = OFF;
			if(setVal.download[7] == 'Y')
			{
				// boot에서 f/w update 후 수정한 flash 내용이 Nvram cache에 반영되지 않아, f/w usb update가 on으로 read되는 경우 발생
				// memory clear 단계에서는 f/w usb update flag가 on되어 있을 경우가 없으므로 강제로 off 시킴
				mode = Flash_bread(DFLASH_BASE + FLASH_FWMODE_SET);
				mode &= ~0x04; // Firmware update off
				Flash_bwrite(DFLASH_BASE + FLASH_FWMODE_SET, mode);
				////////////////////////////////////////////////////////////////
				
				VertScrollMsg_Add("MEM CLEAR...");
				history_flash2ram(buffer);
#ifdef USE_INIT_MASK
				init_mask(INIT_MASK_BACKUP);
#endif
				Flash_init_nvbuffer();
				FlashDChipErase();
				history_ram2flash(buffer);
#ifdef USE_INIT_MASK
				init_mask(INIT_MASK_RESTORE);
#endif
				resetOn = ON;
			}
			UsbDataRomUpdate &= ~DATAROMUPDATE_MEMCLR;
			dataRomStatusSave();
			Flash_flush_nvbuffer();
			if (resetOn)
			{
				software_reset();
			}
		}
		if(UsbDataRomUpdate&DATAROMUPDATE_OTHERS) // 셋팅롬 다운로드 체크
		{
#ifdef USE_INIT_MASK
			init_mask(INIT_MASK_BACKUP);
#endif
			if(open_datarom() < 0)
			{
				UsbDataRomUpdate &= ~DATAROMUPDATE_OTHERS;
				dataRomStatusSave();
				Flash_flush_nvbuffer();
				return 0;
			}
			get_header_of_datarom();
			set_global_wparam(GLOBAL_AREA_VENDOR_COUNTRY, Datarom_country);
			VertScrollMsg_Init(DSP_PLU_FONT_ID);
			resetOn = OFF;
			if(setVal.download[0] == 'Y')//parameter
			{
				VertScrollMsg_Add("PARAMETER...");
				//DefaultDealerParamSetting(Datarom_country, dealer_factory_mode);
				global_set_country(Datarom_country, OFF);
				ret = copy_datarom_to_flash(ROM_PARAM);
				if(ret == -1)
				{
					VertScrollMsg_Add("PARAMETER : FAIL");
					Delay_10ms(100);
				}
				else
				{
					resetOn = ON;
				}

			}
			if(setVal.download[1] == 'Y') //Symbol
			{
				VertScrollMsg_Add("SYMBOL...");
				ret = copy_datarom_to_flash(ROM_SYMBOL);
				if(ret == -1)
				{
					VertScrollMsg_Add("SYMBOL : FAIL");
					Delay_10ms(100);
				}
				else
				{
					resetOn = ON;
				}
			}
			
			if(setVal.download[2] == 'Y') //Caption
			{
				VertScrollMsg_Add("CAPTION...");
				ret = copy_datarom_to_flash(ROM_CAPTION);
				if(ret == -1)
				{
					VertScrollMsg_Add("CAPTION : FAIL");
					Delay_10ms(100);
				}
				else
				{
					resetOn = ON;
				}
			}
			
			if(setVal.download[3] == 'Y') //ORIGIN
			{
				VertScrollMsg_Add("ORIGIN...");
				addr = DFLASH_BASE + FLASH_ORIGIN_AREA;
				Flash_clear(addr, 0x00, ORIGIN_STRUCT_SIZE * MAX_ORIGIN_NO);
				ret = copy_datarom_to_flash(ROM_ORIGIN);
				if(ret == -1)
				{
					VertScrollMsg_Add("ORIGIN : FAIL");
					Delay_10ms(100);
				}
				else
				{
					resetOn = ON;
				}
			}
			
			if(setVal.download[4] == 'Y')//keypad
			{
				VertScrollMsg_Add("KEYPAD...");
				//KEY_CopyKeyTable(Datarom_country);
				ret = copy_datarom_to_flash(ROM_KEYPAD);
				if(ret == -1)
				{
					VertScrollMsg_Add("KEYPAD : FAIL");
					Delay_10ms(100);
				}
				else
				{
					resetOn = ON;
				}
			}
			
			if(setVal.download[5] == 'Y')
			{
				VertScrollMsg_Add("FONT...");
				ret = copy_datarom_to_flash(ROM_FONT);
//				printer_modeset_default();
//				if(Datarom_country == COUNTRY_EUT)
//					printer_modeset(1);
//				else
//					printer_modeset(0);
				if(ret == -1)
				{
					VertScrollMsg_Add("FONT : FAIL");
					Delay_10ms(100);
				}
				else
				{
					resetOn = ON;
				}
			}
			
			if(setVal.download[6] == 'Y') //LABEL
			{
				
				VertScrollMsg_Add("LABEL...");
				ret = copy_datarom_to_flash(ROM_LABEL);
				if(ret == -1)
				{
					VertScrollMsg_Add("LABEL : FAIL");
					Delay_10ms(100);
				}
				else
				{
					resetOn = ON;
				}
			}
			
			if(setVal.download[8] == 'Y')
			{
				VertScrollMsg_Add("SCALE TYPE..");
#if defined (USE_CL5200_KEY)
				if(setVal.scaleType == SCALE_TYPE_BENCH)	//Firmware update 프로그램에서 bench로 보냈을 경우 6으로 저장. 
				{
					setVal.scaleType = SCALE_TYPE_BENCH_CL5200;
				}
#elif defined (USE_CL3000_KEY)
				if(setVal.scaleType == SCALE_TYPE_BENCH)	//Firmware update 프로그램에서 bench로 보냈을 경우 7로 저장. 
				{
					setVal.scaleType = SCALE_TYPE_BENCH_CL3000;
				}
#endif
				SetScaleType(setVal.scaleType, OFF, ON);
//				VertScrollMsg_Add("Change Type");
//				VertScrollMsg_Add("Conv Key TBL");
//				KEY_CnvTable(0, setVal.scaleType, 0x3f);
//				
//				VertScrollMsg_Add("Init Key TBL");
//				KEY_CopyKeyTable(-1);
//				
//				VertScrollMsg_Add(" Init A/D Fil");
//				CalRun_InitAdFilter(setVal.scaleType);	// AD Filter settings
//				ADM_WaitData(ADM_CMD_SAVE_SETS, 83419, ADM_PARAM_ON);
				resetOn = ON;
			}
			
			UsbDataRomUpdate &= ~DATAROMUPDATE_OTHERS;
			dataRomStatusSave();
			Flash_flush_nvbuffer();
			close_datarom();
#ifdef USE_INIT_MASK
			init_mask(INIT_MASK_RESTORE);
#endif
			if (resetOn)
			{
				set_global_wparam(GLOBAL_AREA_VENDOR_COUNTRY,(INT16U)Datarom_country);
				software_reset();
			}
		}
	}
	return 1;
}

INT8U restore_data(void) // USB Memory -> Scale
{
	INT8U resetOn;
	CAPTION_STRUCT 	cap;
	INT8U	usbBackupFlag;
	INT32U set_val[4];
	
	set_val[0] = 0; // M1877 초기값 'N'으로
	
	// 메뉴 추가
	Menu_Init();
	caption_split_by_code(0x1878, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);
	caption_split_by_code(0x38A5, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
					cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);
	Menu_InputDisplay();
	
	if (UsbdeviceIsConnected)
	{
		if (open_datfile() < 0)
		{
			VertScrollMsg_Add("RESTORE FAIL");
			Delay_100ms(10);
			return 0;
		}
		
		usbBackupFlag = (INT8U)set_val[0];
		if (usbBackupFlag)
		{
				resetOn = OFF;
				VertScrollMsg_Add("RESTORE...");

				read_datfile(); // dat파일에서 데이터 읽고, 메모리에 업데이트

				VertScrollMsg_Add("RESTORE OK!");
					
				resetOn = ON;
				UsbDataRomUpdate = DATAROMUPDATE_COMPLETE;
				dataRomStatusSave();
				Flash_flush_nvbuffer();

				if (resetOn)
				{
					Delay_100ms(5); // "RESTORE OK!"표시를 위해 0.5초 지연
					close_datarom();
					software_reset();
				}
		}
		close_datarom();
		return 1;
	}
	else
	{
		VertScrollMsg_Add("RESTORE FAIL");
		Delay_100ms(10);
		return 0;
	}
}

void make_datfile_name(char *path)
{
	char full_path[FAT_FS_MAX_LFN];
	FW_VERSION fversion;
	char country[4];
	char countrycode[3];
	
	version_get(&fversion);

	sprintf(full_path, "%s/", DATA_ROM_UPDATE_DIRECTORY_PATH);

#ifdef CL3000_PROJECT
	strcat(full_path, "CL30-MA-");
#else
	strcat(full_path, "CL52J-MA-");
#endif
	country[0] = fversion.country[0];
	country[1] = fversion.country[1];
	country[2] = fversion.country[2];
	country[3] = 0x00;
	countrycode[0] = fversion.subversion[0];
	countrycode[1] = fversion.subversion[1];
	countrycode[2] = 0x00;

	strcat(full_path, countrycode);
	strcat(full_path, country);
	strcat(full_path, "-backup.dat");
	sprintf(path,"%s\r\n",full_path);
}

INT8U backup_data(void) // scale -> USB Memory
{
    INT8U full_path[FAT_FS_MAX_LFN];
	INT8U remove_path[FAT_FS_MAX_LFN];
	CAPTION_STRUCT 	cap;
	INT8U	usbBackupFlag;
	INT32U set_val[4];
	POINT disp_p;
	INT8U datfile_name[FAT_FS_MAX_LFN];
	
	set_val[0] = 0; // M1877 초기값 'N'으로
	
	// 메뉴 추가
	Menu_Init();
	caption_split_by_code(0x1877, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);
	caption_split_by_code(0x38A4, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
					cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);
	Menu_InputDisplay();
	
	if(UsbdeviceIsConnected)
	{
		//해당 경로에 dat파일이 존재한다면 삭제
		if (get_datfile_name((char *)datfile_name, FAT_FS_MAX_LFN) >= 0)
		{
			sprintf(remove_path, "%s/", DATA_ROM_UPDATE_DIRECTORY_PATH);
			strcat(remove_path, (char*)datfile_name);
			SYS_FS_FileDirectoryRemove(remove_path);
		}
		
		make_datfile_name((char *)full_path);
		
		// wait 출력
		disp_p.x = 0;
		disp_p.y = 0;
		caption_split_by_code(0x1aa5, &cap,0);
		Dsp_Write_String(disp_p, (HUGEDATA char *)cap.form);
		Dsp_Diffuse();

		usbBackupFlag =(INT8U)set_val[0];
		
		if (usbBackupFlag)
		{
			dataRomHandle = SYS_FS_FileOpen((char *)full_path,(SYS_FS_FILE_OPEN_WRITE_PLUS));
			usb_backup_flashmemory();
			usb_backup_memorycard();
			SYS_FS_FileClose(dataRomHandle);

			Delay_100ms(10); // "waiting"표시를 위해 1초 지연
		}
		return 1;
	}
	else
	{
		VertScrollMsg_Add("BACKUP FAIL!");
		Delay_100ms(10);
		return 0;
	}
}

int usb_backup_flashmemory(void) // Scale -> USB Memory
{
	INT8U temp[USB_UPDATE_BLOCK_SIZE];
	INT32U addr;
	INT32U i;
	INT32U remainByte;
	INT32U totalReadSize;
	INT32U blockCount;
	
	addr = DFLASH_BASE;
	totalReadSize = 0x200000;
	remainByte = totalReadSize % sizeof(temp);
	blockCount = totalReadSize / sizeof(temp);

	for(i=0; i < blockCount; i++)
	{
		Flash_sread(addr+(i*(sizeof(temp))), (INT8U*)temp, sizeof(temp));
		SYS_FS_FileWrite(dataRomHandle, (const void*)temp, sizeof(temp));
	}
	if(remainByte != 0)
	{
		Flash_sread(addr+(blockCount*(sizeof(temp))), (INT8U*)temp, remainByte);
		SYS_FS_FileWrite(dataRomHandle,(const void*)temp, remainByte);
	}
	return 0;
}

/* (CL5500/CL5200J전용)
 * EXT_MEMCARD 영역 dat파일에 Backup 
 * dat 파일 내 저장 영역 : Flash 영역(0x200000)이후 부터 Backup
 */
int usb_backup_memorycard(void) // Scale -> USB Memory
{
	INT8U temp[USB_UPDATE_BLOCK_SIZE];
	INT32U addr;
	INT32U i;
	INT32U remainByte;
	INT32U totalReadSize;
	INT32U blockCount;
	
	addr = EXT_MEMCARD_ADDR_X;
	totalReadSize = 0x200000;
	remainByte = totalReadSize % sizeof(temp);
	blockCount = totalReadSize / sizeof(temp);

	for(i = 0; i < blockCount; i++)
	{
		Flash_sread(addr+(i*(sizeof(temp))), temp, sizeof(temp));
		SYS_FS_FileWrite(dataRomHandle, (const void*)temp, sizeof(temp));
	}
	if(remainByte != 0)
	{
		Flash_sread(addr+(blockCount*(sizeof(temp))), temp, remainByte);
		SYS_FS_FileWrite(dataRomHandle,(const void*)temp, remainByte);
	}
	return 0;
}

/*
 * dat 파일을 열고, 버전정보를 확인한다.
 */
int open_datfile(void)
{
	char datfile_name[FAT_FS_MAX_LFN];
	char full_path[FAT_FS_MAX_LFN];
	char temp[3];
	int version; //저울의 버전
	int fversion; //dat파일에서의 버전
	
	if (get_datfile_name(datfile_name, FAT_FS_MAX_LFN) < 0)
	{
		return -1;
	}
	sprintf(full_path, "%s/", DATA_ROM_UPDATE_DIRECTORY_PATH);
	strcat(full_path, datfile_name);
	
	dataRomHandle = SYS_FS_FileOpen(full_path,(SYS_FS_FILE_OPEN_READ));
	
	memset(temp, 0x00, sizeof(temp));
	SYS_FS_FileSeek(dataRomHandle, 0, SYS_FS_SEEK_SET);
	SYS_FS_FileRead(dataRomHandle, temp, 2);
	fversion = atoi(temp);
	version = atoi(FLASH_STRUCT_VERSION);
	
	// 저울과 dat파일의 버전이 다르면 업데이트 하지 않음
	if (version != fversion)
	{
		return -1;
	}
	if (dataRomHandle == SYS_FS_HANDLE_INVALID)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/*
 * "cl5200j/rom","cl3000/rom"경로에 dat파일이 존재하는 경우 파일 이름을 가져온다.
 */
int get_datfile_name (char *name, int size)
{
	SYS_FS_HANDLE dirHandle;
	SYS_FS_FSTAT stat;
	char longFileName[FAT_FS_MAX_LFN];
	char temp[FAT_FS_MAX_LFN];
	
	memset(name, 0, size);
	stat.lfname = longFileName;
	stat.lfsize = FAT_FS_MAX_LFN;
	
	// USB에 경로 지정이 안되어 있다면, 경로 생성
	dirHandle = SYS_FS_DirOpen("/mnt/myDrive1");
	
	if(SYS_FS_DirSearch(dirHandle,DATA_ROM_ROOT_DIRECTORY_REL_PATH,SYS_FS_ATTR_DIR,&stat) == SYS_FS_RES_FAILURE)
	{
		SYS_FS_DirectoryMake(DATA_ROM_ROOT_DIRECTORY_REL_PATH); // "ecg: cl5200j or cl3000, efg: cl5200jn or cl3000n"
		SYS_FS_DirectoryMake(DATA_ROM_UPDATE_DIRECTORY_REL_PATH); // "ecg: cl5200j/rom or cl3000/rom, efg: cl5200jn/rom or cl3000n/rom"
	}
	else
	{
		SYS_FS_DirClose(dirHandle);
		dirHandle = SYS_FS_DirOpen(DATA_ROM_ROOT_DIRECTORY_PATH); // "ecg: /../../cl3000,/../../cl5200j, efg: /../../cl3000n, /../../cl5200jn"
		if(SYS_FS_DirSearch(dirHandle,"rom",SYS_FS_ATTR_DIR,&stat) == SYS_FS_RES_FAILURE)
		{
			SYS_FS_DirectoryMake(DATA_ROM_UPDATE_DIRECTORY_REL_PATH); // "cl3000/rom or cl5200j/rom"
		}
	}
	SYS_FS_DirClose(dirHandle);
	
	// 경로에 dat 파일이 있는지 Search
	dirHandle = SYS_FS_DirOpen(DATA_ROM_UPDATE_DIRECTORY_PATH);
	
	if (dirHandle == SYS_FS_HANDLE_INVALID)
	{
		SYS_FS_DirClose(dirHandle);
		return -1;
	}
	
	if(SYS_FS_DirSearch(dirHandle,DATA_UPDATE_FILE_PREFIX,SYS_FS_ATTR_ARC,&stat) == SYS_FS_RES_FAILURE)
	{
		SYS_FS_DirClose(dirHandle);
		return -1;
	}
	if(SYS_FS_DirClose(dirHandle) == SYS_FS_RES_FAILURE)
	{
		return -1;
	}
	else
	{
		if (stat.lfname[0] != 0)
		{
			strcpy(temp, stat.lfname);
		}
		else if (stat.fname[0] != 0)
		{
			strcpy(temp, stat.fname);
		}
		strcpy(name,temp);
	}
	
	if(name[0])
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

#define MASK_START_ADDR     0
#define MASK_SIZE           1
#define LAST_MASK_INDEX		(MAX_FLASH_UPDATE_MASK_NUM-1)
/*
 * 업데이트 mask영역을 제외하고, dat파일의 데이터로 업데이트
 * 업데이트 mask영역은 update_mask_range[][]에 추가 (주소 순서대로 등록)
 */
void read_datfile(void) // USB Memory -> Scale
{
	INT32U startAddr = 0; //업데이트 할 시작 주소
	INT32U size = 0;  // 업데이트 할 영역의 사이즈
	INT32U maskStartAddr = 0; //업데이트 하지 않는 영역의 시작 주소
	INT32U maskSize = 0;      //업데이트 하지 않는 영역의 사이즈
	INT32U maskIndex = 0;
	INT32U nextMaskStartAddr = 0;
	INT32U nextMaskIndex = 0;

	// DFLASH_BASE ~ 첫번째 mask영역
	startAddr = DFLASH_BASE;
	size = update_mask_range[0][MASK_START_ADDR] - DFLASH_BASE;
	update_flashmemory(startAddr, size);
	
	for (maskIndex = 0; maskIndex < LAST_MASK_INDEX; maskIndex++)
	{
		nextMaskIndex = maskIndex + 1;
		maskStartAddr = update_mask_range[maskIndex][MASK_START_ADDR];
		maskSize = update_mask_range[maskIndex][MASK_SIZE];
		nextMaskStartAddr = update_mask_range[nextMaskIndex][MASK_START_ADDR];
				
		startAddr = maskStartAddr + maskSize;
		
		size = nextMaskStartAddr - startAddr;
		update_flashmemory(startAddr, size);
	}
	// 마지막 mask영역 ~ FLASH_END_POINT
	startAddr = update_mask_range[LAST_MASK_INDEX][MASK_START_ADDR] + update_mask_range[LAST_MASK_INDEX][MASK_SIZE];
	size = (DFLASH_CHIP_MAX_NUM * DFLASH_VOLUME) - startAddr;
	update_flashmemory(startAddr, size);
}

/*
 * dat파일에서 데이터를 읽어서 Flash memory를 업데이트한다.
 */
void update_flashmemory (INT32U addr, INT32U size) // USB Memory -> Scale
{
	INT8U temp[USB_UPDATE_BLOCK_SIZE];
	INT32U i;
	INT32U remainByte;
	INT32U blockCount;
	INT32U totalReadSize;
	
	char	string_buf[32];
	
	totalReadSize = size;
	remainByte = totalReadSize % sizeof(temp);
	blockCount = totalReadSize / sizeof(temp);

	sprintf(string_buf,"RESTORE %2d%%", (int)(((float)(addr)/DFLASH_VOLUME)*100));
	VertScrollMsg_Add(string_buf);
	
	for(i=0; i < blockCount; i++)
	{
		SYS_FS_FileSeek(dataRomHandle, addr+(i*sizeof(temp)), SYS_FS_SEEK_SET);
		SYS_FS_FileRead(dataRomHandle, (void *)temp, sizeof(temp));
		Flash_swrite(addr+(i*(sizeof(temp))), temp, sizeof(temp)); // 블럭 단위

		if(i % 0x200 == 0)
		{
			sprintf(string_buf,"RESTORE %2d%%", (int)(((float)(addr+(i*sizeof(temp)))/DFLASH_VOLUME)*100));
			VertScrollMsg_Add(string_buf);
		}
	}
	if(remainByte != 0)
	{
		SYS_FS_FileSeek(dataRomHandle, addr+(blockCount*sizeof(temp)), SYS_FS_SEEK_SET);
		SYS_FS_FileRead(dataRomHandle, (void *)temp, remainByte);
		Flash_swrite(addr+(blockCount*(sizeof(temp))), temp, remainByte);

		sprintf(string_buf,"RESTORE %2d%%", (int)(((float)(addr+(blockCount*sizeof(temp)))/DFLASH_VOLUME)*100));
		VertScrollMsg_Add(string_buf);
	}
}

#endif

#ifdef USE_INIT_MASK
/**
 * @brief		설정 초기화 방어(마스킹) (MENU 1891)
 * 					- Keep Init Settings
 * @param		void
 * @return 	void
 * @remark	
 * 				- 초기화 방어 항목
 * 					- Printer Hardware
 * 						- 1730 (Printer Hardware) 
 * 							- 1731 ~ 1738
 * 						- 1810 (Sale Mode)
 * 					- Network
 * 						- Parameter 731 (Wireless Bridge)
 * 						- 1910 (Network Setting)
 * 							- 1911 ~ 1916
 * 					- Label Format
 * 						- 1510 (Label Format)
 * 						- 라벨 헤더, 이미지는 마스킹하지 않음(지워짐)
 *						- Menu 1510 설정 값만 유지 
 * 						- 사용자에게 라벨은 지워지니 다시 다운로드해야 함을 고지해야함으로 결정
 */
void partial_keep_init_set(void)
{
	CAPTION_STRUCT 	cap;
	INT16U result;
	const INT16U MENU_1891 			= 0x1891;
	const INT16U CAPTION_CODE_3A01 	= 0x3A01; // "Printer Hardware : "
	const INT16U CAPTION_CODE_3A02 	= 0x3A02; // "Network : "
	const INT16U CAPTION_CODE_3A03 	= 0x3A03; // "Label Format : "
	struct M1891_SETTINGS menu_settings;
	INT32U user_input[3] = {0};

	menu_settings.bits = get_base_wparam(FLASH_KEEP_INIT_SETTINGS);
	user_input[0] = menu_settings.printer_hardware;
	user_input[1] = menu_settings.network;
	user_input[2] = menu_settings.label_format;

	Menu_Init();
	caption_split_by_code(MENU_1891, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(CAPTION_CODE_3A01, &cap,0);
	caption_adjust(&cap,NULL);

	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &user_input[0], NULL, NULL, NULL);
	
	caption_split_by_code(CAPTION_CODE_3A02, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &user_input[1], NULL, NULL, NULL);

	caption_split_by_code(CAPTION_CODE_3A03, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &user_input[2], NULL, NULL, NULL);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE)
	{
		menu_settings.printer_hardware = user_input[0];
		menu_settings.network          = user_input[1];
		menu_settings.label_format     = user_input[2];
		set_base_wparam(FLASH_KEEP_INIT_SETTINGS, menu_settings.bits);
	}
	return;
}
#endif // #ifdef USE_INIT_MASK ... end

#ifdef USE_PARTIAL_MEM_INIT
/**
 * @brief		메모리 부분 초기화(MENU 1892)
 * 					- Select PLU Item Init
 * @param		void
 * @return 	void
 * @remark	
 * 				- CL7200 부분 초기화 메뉴와 다른 노선으로
 * 					- CL7200 Menu 1876 (Init Settings)
 * 				- 초기화 항목
 * 					- Select PLU Item (1144)
 */
void partial_init_sel_plu(void)
{
	/*
	 * Menu 1144는 캡션을 하드코딩으로 생성하는듯 함
	 * 		- plu_device.c
	 * 		- plu_struct_set_base()
	 * 
	 * DataFlash_Structure 문서의 258 라인 - pType
	 * 
	 * 1xxx xxxx xxxx xxxx
	 * 1이면 N
	 * 0이면 Y
	 */
	CAPTION_STRUCT 	cap;
	INT16U result;
	const INT16U MENU_1892 			= 0x1892;
	const INT16U CAPTION_CODE_1892 	= 0x1892;
	INT32U input_yn;

	Menu_Init();
	caption_split_by_code(MENU_1892, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(CAPTION_CODE_1892, &cap,0);
	caption_adjust(&cap,NULL);
	
	input_yn = 0;
	
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &input_yn, NULL, NULL, NULL);
	
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE)
	{
		if(input_yn)
		{
			set_plu_inhibit_default();	
		}
	}
	return;
}

/**
 * @brief		메모리 부분 초기화(MENU 1893)
 * 					- Custom Label Init
 * @param		void
 * @return 	void
 * @remark	
 * 				- 초기화 항목
 * 					- User Label 51 ~ 99 
 */
void partial_init_cus_label(void)
{
	CAPTION_STRUCT 	cap;
	INT16U result;
	const INT16U MENU_1893 			= 0x1893;
	const INT16U CAPTION_CODE_1893 	= 0x1893;
	INT32U input_yn;

	Menu_Init();
	caption_split_by_code(MENU_1893, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(CAPTION_CODE_1893, &cap,0);
	caption_adjust(&cap,NULL);
	
	input_yn = 0;
	
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &input_yn, NULL, NULL, NULL);
	
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE)
	{
		if(input_yn)
		{
			/*
			 * 라벨 영역 메모리 클리어
			 * 라벨 포맷 영역
			 * 		- 라벨 헤더
			 */
			label_clear();
		}
	}
	return;
}

/**
 * @brief		메모리 부분 초기화(MENU 1894)
 * 					- Scale Parameter Init
 * @param		void
 * @return 	void
 * @remark	
 * 				- 초기화 항목
 * 					- 1870
 * 						- Display      (1871) 
 * 						- Printing     (1872) 
 * 						- Sale setup   (1873) 
 * 						- Clerk Logout (1874) 
 */
void partial_init_scale_param(void)
{
	CAPTION_STRUCT 	cap;
	INT16U result;
	const INT16U MENU_1894 			= 0x1894;
	const INT16U CAPTION_CODE_1894 	= 0x1894;
	INT32U input_yn;

	Menu_Init();
	caption_split_by_code(MENU_1894, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(CAPTION_CODE_1894, &cap,0);
	caption_adjust(&cap,NULL);
	
	input_yn = 0;
	
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &input_yn, NULL, NULL, NULL);
	
	result = Menu_InputDisplay();

	if(result == MENU_RET_SAVE)
	{
		if(input_yn)
		{
			INT32S i;
			const INT16U default_funcCode[] = { 
				1810, 1811, 1812, 1845,															// Menu 1871
				1814, 1815, 1816, 1817, 1818, 1819, 1820, 1821, 1822, 1823, 1848,				// Menu 1872
				1824, 1825, 1826, 1827, 1828, 1829, 1830, 1831, 1832, 1833, 1834, 1835, 1836, 1837, 1854, 1939,	// Menu 1873
				1838, 1839																		// Menu 1874 
			};

			for(i = 0; i < sizeof(default_funcCode)/sizeof(default_funcCode[0]); i++)
			{
				CodeDefaultParameterByCode(default_funcCode[i]);
			}
		}
	}
	return;
}

/**
 * @brief		메모리 부분 초기화(MENU 1895)
 * 					- Clerk & Permission Init
 * @param		void
 * @return 	void
 * @remark	
 * 				- 초기화 항목
 * 					- 1850
 * 						- User              (1851, 1853, 1854) 
 * 						- Change Password   (1852) 
 * 						- Config Permission (1855) 
 */
void partial_init_clerk_perm(void)
{
	CAPTION_STRUCT 	cap;
	INT16U result;
	const INT16U MENU_1895 			= 0x1895;
	const INT16U CAPTION_CODE_1895 	= 0x1895;
	INT32U input_yn;

	Menu_Init();
	caption_split_by_code(MENU_1895, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(CAPTION_CODE_1895, &cap,0);
	caption_adjust(&cap,NULL);
	
	input_yn = 0;
	
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &input_yn, NULL, NULL, NULL);
	
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE)
	{
		if(input_yn)
		{
			INT16U country;
			INT32U addr = DFLASH_BASE + FLASH_CLERK_AREA;
			INT32S i;

			/* Clerk */
			Flash_clear(addr, 0xFF, CLERK_STRUCT_SIZE * CLERK_STRUCT_QTY);
			set_clerk_init();
			
			/* Permission */
			addr = DFLASH_BASE + FLASH_CLERK_PERMISSION;
			Flash_clear(addr, 0xFF, PERMISSION_STRUCT_SIZE * PERMISSION_STRUCT_QTY);

			country = get_global_wparam(GLOBAL_AREA_VENDOR_COUNTRY);
			set_global_permission_init(country);
		}
	}
	return;
}

/**
 * @brief		메모리 부분 초기화(MENU 1896)
 * 					- Scanner Init
 * @param		void
 * @return 	void
 * @remark	
 * 				- 초기화 항목
 * 					- 1460
 * 						- Scanner Conversion Table (1461 ~ 1463) 
 * 							- 삭제
 * 						- Scanner Config (1464)
 * 							- 삭제
 */
void partial_init_scanner(void)
{
	CAPTION_STRUCT 	cap;
	INT16U result;
	const INT16U MENU_1896 			= 0x1896;
	const INT16U CAPTION_CODE_1896 	= 0x1896;
	INT32U input_yn;

	Menu_Init();
	caption_split_by_code(MENU_1896, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(CAPTION_CODE_1896, &cap,0);
	caption_adjust(&cap,NULL);
	
	input_yn = 0;
	
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &input_yn, NULL, NULL, NULL);
	result = Menu_InputDisplay();

	if(result == MENU_RET_SAVE)
	{
		if(input_yn)
		{
			Flash_clear(DFLASH_BASE + FLASH_SCANNER_FORMAT , 0xFF, 40 * 10);
			Flash_clear(DFLASH_BASE + FLASH_SCANNER_TABLE_AREA , 0xFF, sizeof(SCANNER_CONV_TABLE_STR)* 2000);

			set_scanner_default();
		}
	}
	return;
}

/**
 * @brief		메모리 부분 초기화(MENU 1897)
 * 					- Printer Settings Init
 * @param		void
 * @return 	void
 * @remark	
 * 				- 초기화 항목
 * 					- 1730
 * 						- Printer Mode       (1731) 
 * 						- Label/Ticket Size  (1732) 
 * 						- Sensor Calibration (1733) 
 * 						- Sensor & Motor     (1734) 
 * 						- Print Intensity    (1735) 
 * 						- Adjust Feed Length (1736) 
 * 						- Label Preprint     (1737) 
 */
void partial_init_prt_set(void)
{
	CAPTION_STRUCT 	cap;
	INT16U result;
	const INT16U MENU_1897 			= 0x1897;
	const INT16U CAPTION_CODE_1897 	= 0x1897;
	INT32U input_yn;

	Menu_Init();
	caption_split_by_code(MENU_1897, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(CAPTION_CODE_1897, &cap,0);
	caption_adjust(&cap,NULL);
	
	input_yn = 0;
	
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &input_yn, NULL, NULL, NULL);
	
	result = Menu_InputDisplay();

	if(result == MENU_RET_SAVE)
	{
		INT8U prt_mode = 0;
		if(input_yn)
		{
			/*
			 * 1731 (Print Mode)
			 * 		1752
			 * 1732 (Label Size)
			 * 		1753~1761
			 * 		(default value X)
			 * 1733 (Sensor Calibration)
			 * 		1762 : Gap Threshold
			 * 		1763 : Peel Threshold
			 * 1734 (Sensor & Motor)
			 * 		1764~1773
			 * 		(default value X)
			 * 		1773 : Active Headup Sensor
			 * 1735 (Print Intensity)
			 * 		1774 : Print Intensity
			 * 1736 (Printer Hardware)
			 * 		1775 : Feed Adjustment
			 * 1737 (Label Pre-print)
			 * 		1776 : Enable Preprint
			 * 		1777 : Preprint Length
			 * 1738 (Adjust Reverse Feed Length)
			 * 		1798 : Reverse Feed Adjustment
			 */

			INT32S i;
			const INT16U default_funcCode[] = { 1752, 1762, 1763, 1773, 1774, 1775, 1776, 1777, 1798 };

			for(i = 0; i < sizeof(default_funcCode)/sizeof(default_funcCode[0]); i++)
			{
				CodeDefaultParameterByCode(default_funcCode[i]);
			}

			set_printer_default(prt_mode);

			_SOFTWARE_RESET;
		}
	}
	return;
}

/**
 * @brief		메모리 부분 초기화(MENU 1898)
 * 					- Network Settings Init
 * @param		void
 * @return 	void
 * @remark	
 * 				- 초기화 항목
 * 					- 1910
 * 						- Service Type (1911) 
 * 						- DHCP         (1912) 
 * 						- IP           (1913) 
 * 						- Remote IP    (1914) 
 * 						- RS232C       (1915) 
 * 						- WLAN setting (1916) 
 * 					- 1920 
 * 						- Application (1920) 
 */
void partial_init_net_set(void)	
{
	CAPTION_STRUCT 	cap;
	INT16U result;
	const INT16U MENU_1898 			= 0x1898;
	const INT16U CAPTION_CODE_1898 	= 0x1898;
	INT32U input_yn;

	Menu_Init();
	caption_split_by_code(MENU_1898, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(CAPTION_CODE_1898, &cap,0);
	caption_adjust(&cap,NULL);
	
	input_yn = 0;
	
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &input_yn, NULL, NULL, NULL);
	
	result = Menu_InputDisplay();

	if(result == MENU_RET_SAVE)
	{
		if(input_yn)
		{
			init_network();
			_SOFTWARE_RESET;
		}
	}
	return;
}
#endif // #ifdef USE_PARTIAL_MEM_INIT ... end

/**
 * @brief		라벨 영역의 플래시 주소 얻는 함수	
 * @param		label_idx
 * 					- 라벨 배열에서의 인덱스	
 * 					- 범위
 * 					- 0 ~ 20 (기본 영역만 사용할 경우)
 * 					- 0 ~ LABEL_CUSTOM_TOTAL_NUM (확장 영역 사용할 경우)
 * @param		label_area_select
 * 					- 라벨 영역 중 어느 영역 주소 찾고 싶은지
 * 					- LABEL_AREA_HEADER : 라벨 헤더 영역
 * 					- LABEL_AREA_IMAGE  : 라벨 이미지 영역
 * 					- 인자 잘못 전달 시 LABEL_ADDR_ERR 반환
 * @return 	라벨 영역의 플래시 주소	
 * @remark	
 */
INT32U get_addr_from_label_idx(const INT16U label_idx, const enum LABEL_AREA label_area_select)
{
	INT32U addr; // 라벨 영역 시작 주소
	INT16U idx_offset = label_idx;
	INT16U area_size; // 라벨 영역별 크기(HEADER : 24, IMAGE : 4096)

	switch(label_area_select)
	{
		case LABEL_AREA_HEADER:
			area_size = LABEL_HEADER_SIZE;

			if(label_idx < LABEL_CUSTOM_2MB_NUM)
			{
				addr = DFLASH_BASE + FLASH_LABEL_AREA; 
			}
			#ifdef USE_LABEL_FORMAT_EXT
			else
			{
				addr = FLASH_LABEL_FORMAT_EXT_HEADER_AREA; 
				idx_offset = label_idx - LABEL_CUSTOM_2MB_NUM; // 라벨 인덱스가 23이면 오프셋은 3
			}
			#endif
			break;
		case LABEL_AREA_IMAGE:
			area_size = LABEL_IMAGE_SIZE;

			if(label_idx < LABEL_CUSTOM_2MB_NUM)
			{
				addr = FLASH_LABEL_IMAGE_AREA_ADDR; 
			}
			#ifdef USE_LABEL_FORMAT_EXT
			else
			{
				addr = FLASH_LABEL_FORMAT_EXT_IMAGE_AREA; 
				idx_offset = label_idx - LABEL_CUSTOM_2MB_NUM; // 라벨 인덱스가 23이면 오프셋은 3
			}
			#endif
			break;
		default:
			return LABEL_ADDR_ERR;
	}

	// 주소 = 기본 시작 주소 + 인덱스 오프셋 * 라벨 영역별 크기
	// 주소 = 기본 라벨 시작 주소(2MB) 중 헤더 영역 or 이미지 영역 + 3 * 24
	//                             3 * 480
	addr += idx_offset * area_size;

	return addr;
}

/**
 * @brief		라벨 헤더 영역 저장하는 함수	
 * @param		label_addr	
 * 					- 라벨 저장할 주소	
 * @param		info	
 * 					- 라벨 헤더 정보	
 * @param		bit_flag	
 * 					- 라벨 헤더 정보 중 저장하고 싶은 것을 표현하는 비트 플래그	
 *					- L_H_NONE_BIT = 0x00, // none
 * 					- L_H_ID_BIT   = 0x01, // Label Header Label id
 *					- L_H_I_W_BIT  = 0x02, // Label Header Image width
 *					- L_H_I_H_BIT  = 0x04, // Label Header Image height
 *					- L_H_I_N_BIT  = 0x08, // Label Header Image name
 *					- L_H_I_S_BIT  = 0x10, // Label Header Image size
 *					- L_H_ALL_BIT  = L_H_ID_BIT | L_H_I_W_BIT | L_H_I_H_BIT | L_H_I_N_BIT | L_H_I_S_BIT, // all
 * @return 	void	
 * @remark	
 * 				
 */
void save_label_header(INT32U label_addr, const struct LABEL_HEADER_INFO *info, const enum LABEL_HEADER_BIT_FLAG bit_flag)
{
	INT32S i;

	for(i = 0; i < get_label_header_bit_flag_cnt(); i++)
	{
		// ... 000x xxxx : bit flag
		switch(bit_flag & (0x01 << i))
		{
			case L_H_ID_BIT:   // 0000 0001
				Flash_wwrite(label_addr + LABEL_HEADER_OFFSET_ID, info->label_id);
				break;
			case L_H_I_W_BIT:  // 0000 0010
				Flash_wwrite(label_addr + LABEL_HEADER_OFFSET_W, info->img_w);
				break;
			case L_H_I_H_BIT:  // 0000 0100
				Flash_wwrite(label_addr + LABEL_HEADER_OFFSET_H, info->img_h);
				break;
			case L_H_I_N_BIT:  // 0000 1000
				Flash_swrite(label_addr + LABEL_HEADER_OFFSET_NAME, (INT8U*)(info->img_name), LABEL_HEADER_IMG_NAME_SIZE);
				break;
			case L_H_I_S_BIT:  // 0001 0000
				Flash_wwrite(label_addr + LABEL_HEADER_OFFSET_SIZE, info->img_size);
				break;
			default:
				break;
		}
	}
	return;
}

/**
 * @brief		라벨 헤더 영역 읽어오는 함수	
 * @param		label_addr	
 * 					- 읽어올 라벨 플래시 주소	
 * @param		info	
 * 					- 라벨 헤더 정보를 담을 파라미터	
 * @param		bit_flag	
 * 					- 라벨 헤더 정보 중 저장하고 싶은 것을 표현하는 비트 플래그	
 *					- L_H_NONE_BIT = 0x00, // none
 * 					- L_H_ID_BIT   = 0x01, // Label Header Label id
 *					- L_H_I_W_BIT  = 0x02, // Label Header Image width
 *					- L_H_I_H_BIT  = 0x04, // Label Header Image height
 *					- L_H_I_N_BIT  = 0x08, // Label Header Image name
 *					- L_H_I_S_BIT  = 0x10, // Label Header Image size
 *					- L_H_ALL_BIT  = L_H_ID_BIT | L_H_I_W_BIT | L_H_I_H_BIT | L_H_I_N_BIT | L_H_I_S_BIT, // all
 * @return 	void	
 * @remark	
 * 				
 */
void read_label_header(INT32U label_addr, struct LABEL_HEADER_INFO *info, const enum LABEL_HEADER_BIT_FLAG bit_flag)
{
	INT32S i;

	for(i = 0; i < get_label_header_bit_flag_cnt(); i++)
	{
		// 000x xxxx : bit flag
		switch(bit_flag & (0x01 << i))
		{
			case L_H_ID_BIT:   // 0000 0001
				info->label_id = Flash_wread(label_addr + LABEL_HEADER_OFFSET_ID);
				break;
			case L_H_I_W_BIT:  // 0000 0010
				info->img_w = Flash_wread(label_addr + LABEL_HEADER_OFFSET_W);
				break;
			case L_H_I_H_BIT:  // 0000 0100
				info->img_h = Flash_wread(label_addr + LABEL_HEADER_OFFSET_H);
				break;
			case L_H_I_N_BIT:  // 0000 1000
				Flash_sread(label_addr + LABEL_HEADER_OFFSET_NAME, (INT8U*)(info->img_name), LABEL_HEADER_IMG_NAME_SIZE);
				break;
			case L_H_I_S_BIT:  // 0001 0000
				info->img_size = Flash_wread(label_addr + LABEL_HEADER_OFFSET_SIZE);
				break;
			default:
				break;
		}
	}
	return;
}

/**
 * @brief		라벨 헤더 구분하기 위한 비트 플래그 총 개수 반환하는 함수	
 * @param		void	
 * @return 	비트 플래그 총 개수	
 * @remark	
 */
INT32U get_label_header_bit_flag_cnt(void)
{
	static INT32S label_header_bit_flag = 1;
	static INT32U label_header_bit_flag_cnt = 0;
	INT32S i;

	if(label_header_bit_flag)
	{
		label_header_bit_flag = 0;
		for(i = 0; i < 32; i++)
		{
			if((L_H_ALL_BIT >> i) & 0x01)
			{
				label_header_bit_flag_cnt++;	
			}
		}
	}

	return label_header_bit_flag_cnt;
}

#ifdef USE_INIT_MASK
/**
 * @brief		프린터 하드웨어 설정 값 초기화 방어 함수 등록		
 * @param		void	
 * @return 	void	
 * @remark	
 * 				- Menu 1730 (Printer Hardware)
 * 					- (1731~1738) 초기화 방어 함수
 * 				- 백업, 복구할 함수 생성 후 밑의 구조체 배열에 함수 등록
 */
void init_mask_printer_hardware_func_enroll(void)
{
	struct INIT_MASK_BACKUP_RESTORE_FUNC functions[] = {
		/* m1731은 1810 등 엮여 있는 부분들이 있어 백업 안하는 것으로 결정 */
		//{.backup_func = backup_m1731, 			.restore_func = restore_m1731},
		{.backup_func = backup_m1732, 			.restore_func = restore_m1732},
		{.backup_func = backup_m1733, 			.restore_func = restore_m1733},
		{.backup_func = backup_m1734, 			.restore_func = restore_m1734},
		{.backup_func = backup_m1735, 			.restore_func = restore_m1735},
		{.backup_func = backup_m1736, 			.restore_func = restore_m1736},
		{.backup_func = backup_m1737, 			.restore_func = restore_m1737},
		{.backup_func = backup_m1738, 			.restore_func = restore_m1738},
		{.backup_func = backup_m1810, 			.restore_func = restore_m1810},
		{.backup_func = backup_printer_area, 	.restore_func = restore_printer_area},
	};

	INT32S i;
	for(i = 0; i < sizeof(functions) / sizeof(functions[0]); i++)
	{
		init_mask_put(&functions[i]);
	}

	return;
}

/**
 * @brief		Menu 1891 설정 값 초기화 방어 함수 등록	
 * @param		void	
 * @return 	void	
 * @remark	
 */
void init_mask_m1891_settings_func_enroll(void)
{
	struct INIT_MASK_BACKUP_RESTORE_FUNC functions[] = {
		{.backup_func = backup_m1891, .restore_func = restore_m1891},
	};

	INT32S i;
	for(i = 0; i < sizeof(functions) / sizeof(functions[0]); i++)
	{
		init_mask_put(&functions[i]);
	}

	return;
}

/**
 * @brief		네트워크 설정 초기화 방어 함수 등록	
 * @param		void	
 * @return 	void	
 * @remark	
 * 				- 마스킹 항목
 * 					- Parameter 731 (Wireless Bridge)
 * 					- 1910 (Network Setting)
 * 						- 1911 ~ 1916
 * 				- 백업, 복구할 함수 생성 후 밑의 구조체 배열에 함수 등록
 */
void init_mask_network_func_enroll(void)
{
	struct INIT_MASK_BACKUP_RESTORE_FUNC functions[] = {
		{.backup_func = backup_parameter_731, 	.restore_func = restore_parameter_731},
		{.backup_func = backup_m1911, 			.restore_func = restore_m1911},
		{.backup_func = backup_m1912, 			.restore_func = restore_m1912},
		{.backup_func = backup_m1913, 			.restore_func = restore_m1913},
		{.backup_func = backup_m1914, 			.restore_func = restore_m1914},
		{.backup_func = backup_m1915, 			.restore_func = restore_m1915},
		{.backup_func = backup_m1916, 			.restore_func = restore_m1916},
	};

	INT32S i;
	for(i = 0; i < sizeof(functions) / sizeof(functions[0]); i++)
	{
		init_mask_put(&functions[i]);
	}

	return;
}

/**
 * @brief		라벨 포맷 초기화 방어 함수 등록	
 * @param		void	
 * @return 	void	
 * @remark	
 * 				- Menu 1510 (Label Format) 설정 항목 마스킹
 * 				- 백업, 복구할 함수 생성 후 밑의 구조체 배열에 함수 등록
 */
void init_mask_label_format_func_enroll(void)
{
	struct INIT_MASK_BACKUP_RESTORE_FUNC functions[] = {
		{.backup_func = backup_m1510, .restore_func = restore_m1510},
	};

	INT32S i;
	for(i = 0; i < sizeof(functions) / sizeof(functions[0]); i++)
	{
		init_mask_put(&functions[i]);
	}
	
	return;
}

/**
 * @brief		M1891 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1891(void)
{
	struct M1891_SETTINGS *menu_settings;
	menu_settings = (struct M1891_SETTINGS*)malloc(sizeof(struct M1891_SETTINGS));

	RETURN_IF_NULL(menu_settings, "m1891 b fail", NULL);

	menu_settings->bits = get_base_wparam(FLASH_KEEP_INIT_SETTINGS);

	return (void*)menu_settings;
}

/**
 * @brief		M1891 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1891(void *p)
{
	RETURN_IF_NULL(p, "m1891 r fail", );
	
	set_base_wparam(FLASH_KEEP_INIT_SETTINGS, ((struct M1891_SETTINGS*)p)->bits);
	free(p);

	return;
}

/**
 * @brief		M1731 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1731(void)
{
	INT8U *mode;
	mode = (INT8U*)malloc(sizeof(INT8U));

	RETURN_IF_NULL(mode, "m1731 b fail", NULL);

	*mode = get_global_bparam(GLOBAL_PAPER_TYPE);
	
	return (void*)mode;
}

/**
 * @brief		M1731 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1731(void *p)
{
	RETURN_IF_NULL(p, "m1731 r fail", );

	set_global_bparam(GLOBAL_PAPER_TYPE, *(INT8U*)p);
	free(p);
	
	return;
}

/**
 * @brief		M1732 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1732(void)
{
	struct PRINTER_PAPER *prt_paper; 

	prt_paper = (struct PRINTER_PAPER*)malloc(sizeof(struct PRINTER_PAPER));
	
	RETURN_IF_NULL(prt_paper, "m1732 b fail", NULL);

	Flash_sread(FLASH_GLOBAL_AREA + GLOBAL_PRT_PAPER_WIDTH, (INT8U*)prt_paper, sizeof(struct PRINTER_PAPER));

	return (void*)prt_paper;
}

/**
 * @brief		M1732 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1732(void *p)
{
	RETURN_IF_NULL(p, "m1732 r fail", );
	
	Flash_swrite(FLASH_GLOBAL_AREA + GLOBAL_PRT_PAPER_WIDTH, (INT8U*)p, sizeof(struct PRINTER_PAPER));
	free(p);

	return;
}

/**
 * @brief		M1733 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1733(void)
{
	struct SENSOR_CALIBRATION *sc;
	sc = (struct SENSOR_CALIBRATION*)malloc(sizeof(struct SENSOR_CALIBRATION));

	RETURN_IF_NULL(sc, "m1733 b fail", NULL);

	Flash_sread(PRT_FLASH_BASE + FLASH_OFFSET_GapThreshold, (HUGEDATA INT8U *)&sc->gap, 1);
	Flash_sread(PRT_FLASH_BASE + FLASH_OFFSET_PeelThreshold, (HUGEDATA INT8U *)&sc->peel, 1);

	return (void*)sc;
}

/**
 * @brief		M1733 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1733(void *p)
{
	RETURN_IF_NULL(p, "m1733 r fail", );
	
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapThreshold, (HUGEDATA INT8U *)&(((struct SENSOR_CALIBRATION*)p)->gap), 1);
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PeelThreshold, (HUGEDATA INT8U *)&(((struct SENSOR_CALIBRATION*)p)->peel), 1);
	free(p);

	return;
}

/**
 * @brief		M1734 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 * 				Printer Sensor 비트 구성(1byte 기준)
 *  				- 개별로 적용되는 설정(Label, Ticket, Cont_Label) 
 * 					- bit : [7 6 5 4 3 2 1 0]
 * 					- bit[0] : active peel-off
 * 					- bit[1] : ifndef _CL3000_PRINTER_MECH
 * 					- bit[2] : Label Paper(0:NoGap, 1:Gap)
 */
void* backup_m1734(void)
{
	struct PRINTER_SENSOR *ps;
	INT32S i;

	ps = (struct PRINTER_SENSOR*)malloc(sizeof(struct PRINTER_SENSOR));

	RETURN_IF_NULL(ps, "m1734 b fail", NULL);

	for(i = 0; i < PRINT_MODE_TYPE_QTY; i++)
	{
		ps->prt_sensor[i].bits = get_global_bparam(GLOBAL_PRT_SENSOR_MOTOR + i);
	}
	ps->headup = get_global_bparam(GLOBAL_PRT_HEADUP_SENSOR);

	return (void*)ps;
}

/**
 * @brief		M1734 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1734(void *p)
{
	INT32S i;
	struct PRINTER_SENSOR *prt_sensor_p;

	RETURN_IF_NULL(p, "m1734 r fail", );

	prt_sensor_p = (struct PRINTER_SENSOR*)p;

	for(i = 0; i < PRINT_MODE_TYPE_QTY; i++)
	{
		set_global_bparam(GLOBAL_PRT_SENSOR_MOTOR + i, prt_sensor_p->prt_sensor[i].bits);
	}
	set_global_bparam(GLOBAL_PRT_HEADUP_SENSOR, prt_sensor_p->headup);
	free(p);

	return;
}

/**
 * @brief		M1735 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1735(void)
{
	INT16U *intensity;
	intensity = (INT16U*)malloc(sizeof(INT16U));

	RETURN_IF_NULL(intensity, "m1735 b fail", NULL);

	Flash_sread(PRT_FLASH_BASE + FLASH_OFFSET_StbTimeTrimValue, (HUGEDATA INT8U *)intensity, 2);
	
	return (void*)intensity;
}

/**
 * @brief		M1735 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1735(void *p)
{
	RETURN_IF_NULL(p, "m1735 r fail", );

	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_StbTimeTrimValue, (HUGEDATA INT8U *)p, 2);
	free(p);

	return;
}

/**
 * @brief		M1736 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1736(void)
{
	INT16U *feed_length;
	feed_length = (INT16U*)malloc(sizeof(INT16U));

	RETURN_IF_NULL(feed_length, "m1736 b fail", NULL);

	Flash_sread(PRT_FLASH_BASE + FLASH_OFFSET_GapBufInPtrTrimValue, (HUGEDATA INT8U *)feed_length, 2);

	return (void*)feed_length;
}

/**
 * @brief		M1736 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1736(void *p)
{
	RETURN_IF_NULL(p, "m1736 r fail", );

	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_GapBufInPtrTrimValue, (HUGEDATA INT8U *)p, 2);
	free(p);

	return;
}

/**
 * @brief		M1737 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1737(void)
{
	struct LABEL_PRE_PRINT *pre_print;
	pre_print = (struct LABEL_PRE_PRINT*)malloc(sizeof(struct LABEL_PRE_PRINT));

	RETURN_IF_NULL(pre_print, "m1737 b fail", NULL);

	Flash_sread(PRT_FLASH_BASE + FLASH_OFFSET_PrePrintMode, &pre_print->mode, 1);
	Flash_sread(PRT_FLASH_BASE + FLASH_OFFSET_PrePrintLength, &pre_print->length, 1);

	return (void*)pre_print;
}

/**
 * @brief		M1737 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1737(void *p)
{
	RETURN_IF_NULL(p, "m1737 r fail", );
	
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PrePrintMode, (HUGEDATA INT8U*)&(((struct LABEL_PRE_PRINT*)p)->mode), 1);
	Flash_swrite(PRT_FLASH_BASE + FLASH_OFFSET_PrePrintLength, (HUGEDATA INT8U*)&(((struct LABEL_PRE_PRINT*)p)->length), 1);
	free(p);

	return;
}

/**
 * @brief		M1738 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1738(void)
{
	INT16U *reverse_feed_adjustment;
	reverse_feed_adjustment = (INT16U*)malloc(sizeof(INT16U));

	RETURN_IF_NULL(reverse_feed_adjustment, "m1738 b fail", NULL);

	*reverse_feed_adjustment = Flash_wread(PRT_FLASH_BASE + FLASH_OFFSET_GapBufInPtrReverseTrimValue);

	return (void*)reverse_feed_adjustment;
}

/**
 * @brief		M1738 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1738(void *p)
{
	RETURN_IF_NULL(p, "m1738 r fail", );

	Flash_wwrite((PRT_FLASH_BASE + FLASH_OFFSET_GapBufInPtrReverseTrimValue), *(INT16U*)p);
	free(p);

	return;
}

/**
 * @brief		M1810 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1810(void)
{
	INT8U *sale_mode;
	sale_mode = (INT8U*)malloc(sizeof(INT8U));

	RETURN_IF_NULL(sale_mode, "m1810 b fail", NULL);

	*sale_mode = get_global_bparam(GLOBAL_SALERUN_MODE);

	return (void*)sale_mode;
}

/**
 * @brief		M1810 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1810(void *p)
{
	RETURN_IF_NULL(p, "m1810 r fail", );

	set_global_bparam(GLOBAL_SALERUN_MODE, *(INT8U*)p);
	free(p);

	return;
}

/**
 * @brief		printer_area 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_printer_area(void)
{
	PRT_FLASH_STRUCT *prt_flash;
	prt_flash = (PRT_FLASH_STRUCT*)malloc(PRT_FLASH_SET_STRUCT_SIZE);

	RETURN_IF_NULL(prt_flash, "prt b fail", NULL);

	Flash_sread(PRT_FLASH_BASE, (HUGEDATA INT8U *)prt_flash, PRT_FLASH_SET_STRUCT_SIZE);

	return (void*)prt_flash;
}

/**
 * @brief		printer_area 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_printer_area(void *p)
{
	RETURN_IF_NULL(p, "prt r fail", );

	Flash_swrite(PRT_FLASH_BASE, (HUGEDATA INT8U *)p, PRT_FLASH_SET_STRUCT_SIZE);
	free(p);

	return;
}

/**
 * @brief		Parameter 731 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_parameter_731(void)
{
	struct PARAMETER_730_731 *param, tmp;

	param = (struct PARAMETER_730_731*)malloc(sizeof(struct PARAMETER_730_731));

	RETURN_IF_NULL(param, "p 731 b fail", NULL);

	memset((void*)param, 0, sizeof(struct PARAMETER_730_731));

	tmp.bits = Flash_bread(GLOBAL_AREA_COMMUNICATION + FLASH_GLOBAL_AREA);
	param->wireless_bridge = tmp.wireless_bridge;

	return (void*)param;
}

/**
 * @brief		Parameter 731 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_parameter_731(void *p)
{
	RETURN_IF_NULL(p, "p 731 r fail", );

	INT8U tmp = Flash_bread(GLOBAL_AREA_COMMUNICATION + FLASH_GLOBAL_AREA);

	tmp |= ((struct PARAMETER_730_731 *)p)->wireless_bridge << 1;
	Flash_bwrite(GLOBAL_AREA_COMMUNICATION + FLASH_GLOBAL_AREA, tmp);

	free(p);

	return;
}

/**
 * @brief		M1911 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1911(void)
{
	struct SERVICE_TYPE *st;

	st = (struct SERVICE_TYPE*)malloc(sizeof(struct SERVICE_TYPE));

	RETURN_IF_NULL(st, "m1911 b fail", NULL);

	st->scale_no     = get_net_wparam(NETWORK_SCALE_ID);
	st->service_type = get_net_bparam(NETWORK_SERVICE_TYPE);

	return (void*)st;
}

/**
 * @brief		M1911 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1911(void *p)
{
	RETURN_IF_NULL(p, "m1911 r fail", );

	set_net_wparam(NETWORK_SCALE_ID, ((struct SERVICE_TYPE*)p)->scale_no);
	set_net_bparam(NETWORK_SERVICE_TYPE, ((struct SERVICE_TYPE*)p)->service_type);
	free(p);

	return;
}

/**
 * @brief		M1912 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1912(void)
{
	INT8U *enable_dhcp;

	enable_dhcp = (INT8U*)malloc(sizeof(INT8U));

	RETURN_IF_NULL(enable_dhcp, "m1912 b fail", NULL);
	
	*enable_dhcp = get_net_bparam(NETWORK_LOCAL_ETHERNET_FLAG);

	return (void*)enable_dhcp;
}

/**
 * @brief		M1912 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1912(void *p)
{
	RETURN_IF_NULL(p, "m1912 r fail", );

	set_net_bparam(NETWORK_LOCAL_ETHERNET_FLAG, *(INT8U*)p);
	free(p);

	return;
}

/**
 * @brief		M1913 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1913(void)
{
	struct IP_CONFIG *ip_config;

	ip_config = (struct IP_CONFIG*)malloc(sizeof(struct IP_CONFIG));

	RETURN_IF_NULL(ip_config, "m1913 b fail", NULL);

	get_net_sparam(NETWORK_LOCAL_IP, ip_config->ip, IP_SIZE);
	get_net_sparam(NETWORK_LOCAL_GATEWAY, ip_config->gateway, GATEWAY_SIZE);
	get_net_sparam(NETWORK_LOCAL_SUBNETMASK, ip_config->subnet_mask, SUBNET_MASK_SIZE);
	ip_config->tcp_port = get_net_wparam(NETWORK_LOCAL_TCP_PORT);

	return (void*)ip_config;
}

/**
 * @brief		M1913 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1913(void *p)
{
	RETURN_IF_NULL(p, "m1913 r fail", );

	set_net_sparam(NETWORK_LOCAL_IP, ((struct IP_CONFIG*)p)->ip, IP_SIZE);
	set_net_sparam(NETWORK_LOCAL_GATEWAY, ((struct IP_CONFIG*)p)->gateway, GATEWAY_SIZE);
	set_net_sparam(NETWORK_LOCAL_SUBNETMASK, ((struct IP_CONFIG*)p)->subnet_mask, SUBNET_MASK_SIZE);
	set_net_wparam(NETWORK_LOCAL_TCP_PORT, ((struct IP_CONFIG*)p)->tcp_port);

	free(p);

	return;
}

/**
 * @brief		M1914 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1914(void)
{
	struct REMOTE_IP_CONFIG *remote;

	remote = (struct REMOTE_IP_CONFIG*)malloc(sizeof(struct REMOTE_IP_CONFIG));

	RETURN_IF_NULL(remote, "m1914 b fail", NULL);

	get_net_sparam(NETWORK_MACHINE_REMOTE_IP, remote->ip, REMOTE_IP_SIZE);
	remote->tcp_port = get_net_wparam(NETWORK_MACHINE_REMOTE_TCPPORT);

	return (void*)remote;
}

/**
 * @brief		M1914 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1914(void *p)
{
	RETURN_IF_NULL(p, "m1914 r fail", );
	
	set_net_sparam(NETWORK_MACHINE_REMOTE_IP, ((struct REMOTE_IP_CONFIG*)p)->ip, REMOTE_IP_SIZE);
	set_net_wparam(NETWORK_MACHINE_REMOTE_TCPPORT, ((struct REMOTE_IP_CONFIG*)p)->tcp_port);

	free(p);

	return;
}

/**
 * @brief		M1915 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 */
void* backup_m1915(void)
{
	INT8U *baud_rate;

	baud_rate = (INT8U*)malloc(sizeof(INT8U));

	RETURN_IF_NULL(baud_rate, "m1915 b fail", NULL);

	*baud_rate = get_global_bparam(GLOBAL_AREA_RS232C_BAUD);

	return (void*)baud_rate;
}

/**
 * @brief		M1915 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1915(void *p)
{
	RETURN_IF_NULL(p, "m1915 r fail", );

	set_global_bparam(GLOBAL_AREA_RS232C_BAUD, *(INT8U*)p);
	free(p);

	return;
}

/**
 * @brief		M1916 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 * 				- security_setting(use_wep) 값에 따라 값을 저장하지 않고 WLAN AP 영역 전체 백업하기로 결정 
 */
void* backup_m1916(void)
{
	struct WLAN_AP *wlan_ap;

	wlan_ap = (struct WLAN_AP*)malloc(sizeof(struct WLAN_AP));

	RETURN_IF_NULL(wlan_ap, "m1916 b fail", NULL);

	memset((void*)wlan_ap, 0, sizeof(struct WLAN_AP));
	Flash_sread(DFLASH_BASE + FLASH_NET_AREA + NETWORK_WLAN_SSID, (HUGEDATA INT8U*)wlan_ap, sizeof(struct WLAN_AP));

	return (void*)wlan_ap;
}

/**
 * @brief		M1916 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- security_setting(use_wep) 값에 따라 값을 저장하지 않고 WLAN AP 영역 전체 백업 데이터를 그대로 복원 
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1916(void *p)
{
	RETURN_IF_NULL(p, "m1916 r fail", );

	Flash_swrite(DFLASH_BASE + FLASH_NET_AREA + NETWORK_WLAN_SSID, (HUGEDATA INT8U*)p, sizeof(struct WLAN_AP));
	free(p);

	return;
}

/**
 * @brief		M1510 백업 및 malloc		
 * @param		void	
 * @return 		
 * 				동적할당된 주소	
 * @remark	
 * 				- UseFSP의 값에 상관없이 Label - FSP Label id 값 유지
 */
void* backup_m1510(void)
{
	struct LABEL_FORMAT_M1510_T *label_format;

	label_format = (struct LABEL_FORMAT_M1510_T*)malloc(sizeof(struct LABEL_FORMAT_M1510_T));

	RETURN_IF_NULL(label_format, "m1510 b fail", NULL);

	label_format->label_priority    = get_global_bparam(GLOBAL_LABEL_PRIORITY); 
	label_format->standard_label_id = get_global_wparam(GLOBAL_LABEL_STANDARD);
	label_format->misc_label_id     = get_global_wparam(GLOBAL_LABEL_MISC);
	label_format->addup_label_id    = get_global_wparam(GLOBAL_LABEL_ADDUP);
	label_format->fsp_label_id      = get_global_wparam(GLOBAL_LABEL_FSP);

	label_format->addup_print_flag2.reverse_total_label = (get_global_bparam(GLOBAL_PRINT_ADDUP_FLAG2) >> REVERSE_TOTAL_LABEL_BIT) & 0x01;
	label_format->addup_print_flag2.reverse_item_label  = (get_global_bparam(GLOBAL_PRINT_ADDUP_FLAG2) >> REVERSE_ITEM_LABEL_BIT) & 0x01;

	label_format->label2            = get_global_bparam(GLOBAL_LABEL_2);

	get_base_sparam(FLASH_EXT_PRT_LABEL_NAME, label_format->ext_label_name, EXT_LABEL_NAME_SIZE);

	return (void*)label_format;
}

/**
 * @brief		M1510 복원 및 free	
 * @param		p
 * 					- 이전에 동적할당한 주소	
 * @return 	void	
 * @remark	
 * 				- 복원하는 함수에서 동적 할당 해제하는 방식으로 처리
 */
void restore_m1510(void *p)
{
	struct LABEL_FORMAT_M1510_T *label_format = (struct LABEL_FORMAT_M1510_T*)p;
	INT8U read_tmp;
	INT8U tmp = 0;

	RETURN_IF_NULL(p, "m1510 r fail", );

	set_global_bparam(GLOBAL_LABEL_PRIORITY, label_format->label_priority); 
	set_global_wparam(GLOBAL_LABEL_STANDARD, label_format->standard_label_id);
	set_global_wparam(GLOBAL_LABEL_MISC,     label_format->misc_label_id);
	set_global_wparam(GLOBAL_LABEL_ADDUP,    label_format->addup_label_id);
	set_global_wparam(GLOBAL_LABEL_FSP,      label_format->fsp_label_id);

	// 복원할 때 다른 나머지 비트플래그들의 초기 상태는 건드리면 안되므로 
	// 일단 플래시에서 읽어온 다음 reverse_total_label과 reverse_item_label 비트 플래그만 업데이트하려는 작업
	read_tmp = get_global_bparam(GLOBAL_PRINT_ADDUP_FLAG2);

	// reverse_total_label, reverse_item_label 비트만 0으로 초기화하는 코드
	tmp = (0x01 << REVERSE_TOTAL_LABEL_BIT) | (0x01 << REVERSE_ITEM_LABEL_BIT);
	read_tmp &= ~tmp;

	// reverse_total_label, reverse_item_label 비트 업데이트
	read_tmp |= label_format->addup_print_flag2.reverse_total_label << REVERSE_TOTAL_LABEL_BIT;
	read_tmp |= label_format->addup_print_flag2.reverse_item_label  << REVERSE_ITEM_LABEL_BIT;

	// 플래시에 저장
	set_global_bparam(GLOBAL_PRINT_ADDUP_FLAG2, read_tmp);

	set_global_bparam(GLOBAL_LABEL_2, label_format->label2);

	set_base_sparam(FLASH_EXT_PRT_LABEL_NAME, label_format->ext_label_name, EXT_LABEL_NAME_SIZE);

	free(p);

	return;
}

/**
 * @brief		초기화 방어 함수	
 * @param		br
 * 					- INIT_MASK_BACKUP
 * 					- INIT_MASK_RESTORE
 * @return 	void
 * @remark	
 * 				- init_mask(INIT_MASK_BACKUP) 수행 후 적절한 타이밍에 반드시 init_mask(INIT_MASK_RESTORE)를 수행할 것
 * 					- 백업 시 동적으로 생성되므로 복원 함수를 통해 해제를 해주기 위함
 * 				- 백업 파라미터를 넘기면 이전에 등록한 백업 함수들을 연결 리스트를 동적으로 생성하면서 차례대로 실행
 * 				- 복구 파라미터를 넘기면 백업 파라미터로 넘겼을 때 생성된 연결 리스트를 참조하여 복구 함수를 차례대로 실행
 */
void init_mask(const enum INIT_MASK_BACKUP_RESTORE br)
{
	struct M1891_SETTINGS menu_settings;

	// Menu 1891 설정 각각에 대하여 관련 함수 등록
	void (*func_enroll[])(void) = {
		init_mask_printer_hardware_func_enroll,
		init_mask_network_func_enroll,
		init_mask_label_format_func_enroll
	};
	INT32S i;

	menu_settings.bits = 0;
	menu_settings.bits = get_base_wparam(FLASH_KEEP_INIT_SETTINGS);

	switch(br)
	{
		case INIT_MASK_BACKUP:
			if (init_check_m1891(&menu_settings)) {
				return;
			}

			// Menu 1891 설정 값 전체 등록 함수 - 무조건 실행
			init_mask_m1891_settings_func_enroll();

			// Menu 1891 설정 각각에 대하여 1로 설정되어 있어야 등록하는 함수
			for(i = 0; i < sizeof(func_enroll) / sizeof(func_enroll[0]); i++)
			{
				if((menu_settings.bits >> i) & 0x01)
				{
					func_enroll[i]();
				}
			}
			break;
		case INIT_MASK_RESTORE:
			init_mask_restore();
			init_mask_free();
			break;
		default:
			break;
	}

	return;
}

/**
 * @brief		초기화 방어 관련 다음 연결 리스트 찾는 함수
 * @param		void
 * @return 	다음 연결 리스트 주소	
 * @remark	
 */
struct INIT_MASK_LINKED_LIST* init_mask_get_search_next(void)
{
	struct INIT_MASK_LINKED_LIST_META* meta = (struct INIT_MASK_LINKED_LIST_META*)init_mask_get_global_meta_pointer();

	return meta->search->next;
}

/**
 * @brief		초기화 방어 관련 연결 리스트에 등록된 복구 함수를 차례대로 실행하는 함수
 * @param		void
 * @return 	void
 * @remark	
 */
void init_mask_restore(void)
{
	struct INIT_MASK_LINKED_LIST_META* meta = (struct INIT_MASK_LINKED_LIST_META*)init_mask_get_global_meta_pointer();

	init_mask_set_search_first();

	while(meta->search != NULL)
	{
		meta->search->restore_func(meta->search->malloc_p);
		meta->search = meta->search->next;
	}
	
	return;
}

/**
 * @brief		초기화 방어 관련 연결 리스트를 동적으로 생성하여 백업, 복구 함수를 등록하는 함수
 * @param		func	
 * 					- 백업 함수와 복구 함수 주소를 구성하고 있는 구조체
 * @return 	void
 * @remark	
 * 				- func로 넘긴 백업 및 복구 함수를 연결 리스트에 등록
 * 				- func로 넘긴 백업 함수를 수행하고, 동적 할당된 주소를 반환받으면 연결 리스트에 등록
 * 				- 연결 리스트에서 추후에 복구 할 때 등록된 복구 함수를 수행하면서 동적 할당된 주소를 파라미터로 넘길 것임  
 */
void init_mask_put(struct INIT_MASK_BACKUP_RESTORE_FUNC* func)
{
	struct INIT_MASK_LINKED_LIST* 		g_init_mask;
	struct INIT_MASK_LINKED_LIST* 		new;
	struct INIT_MASK_LINKED_LIST_META* 	g_meta;

	g_init_mask = (struct INIT_MASK_LINKED_LIST*     )init_mask_get_dummy_pointer();
	g_meta      = (struct INIT_MASK_LINKED_LIST_META*)init_mask_get_global_meta_pointer();

	new = (struct INIT_MASK_LINKED_LIST*)malloc(sizeof(struct INIT_MASK_LINKED_LIST));

	RETURN_IF_NULL(new, "init mask backup fail", );

	new->backup_func  = func->backup_func;
	new->restore_func = func->restore_func;
	new->malloc_p = (new->backup_func)();
	new->next = NULL;

	if(g_init_mask->next == NULL)
	{
		g_init_mask->next = new;
	}
	else
	{
		g_meta->curr->next = new;
	}
	g_meta->curr = new;
	
	return;
}

/**
 * @brief		초기화 방어 관련 연결 리스트의 첫 번째 노드를 가리키도록 하는 함수
 * @param		void	
 * @return 	void
 * @remark	
 */
void init_mask_set_search_first(void)
{
	struct INIT_MASK_LINKED_LIST* 		g_init_mask = (struct INIT_MASK_LINKED_LIST*     )init_mask_get_dummy_pointer();
	struct INIT_MASK_LINKED_LIST_META* 	meta        = (struct INIT_MASK_LINKED_LIST_META*)init_mask_get_global_meta_pointer();
	meta->search = g_init_mask->next;

	return;
}

/**
 * @brief		초기화 방어 관련 연결 리스트 노드 해제하는 함수
 * @param		void	
 * @return 	void	
 * @remark	
 */
void init_mask_free(void)
{
	struct INIT_MASK_LINKED_LIST* 		g_init_mask = NULL;
	struct INIT_MASK_LINKED_LIST* 		next_tmp    = NULL;
	struct INIT_MASK_LINKED_LIST* 		cur_tmp     = NULL;
	struct INIT_MASK_LINKED_LIST_META* 	g_meta;

	g_init_mask = (struct INIT_MASK_LINKED_LIST*     )init_mask_get_dummy_pointer();	
	g_meta      = (struct INIT_MASK_LINKED_LIST_META*)init_mask_get_global_meta_pointer();
	cur_tmp     = g_init_mask->next;

	g_init_mask->next = NULL;

	while(cur_tmp != NULL)
	{
		next_tmp = cur_tmp->next;
		free(cur_tmp);
		cur_tmp = next_tmp;
	}

	g_meta->curr = NULL;
	g_meta->search = NULL;
		
	return;
}

/**
 * @brief		1891 플래시 영역이 초기화 상태인지 체크하는 함수
 * @param		menu_settings - 플래시에서 읽어온 것을 가정
 * @return 		1 : 완전 초기화 상태, 0 : 다른 값이 쓰여있는 상태
 * @remark	
 */
INT8U init_check_m1891(struct M1891_SETTINGS *menu_settings)
{
	if(menu_settings->bits == 0xFFFF) {
		INT32S i;
		const INT16U default_funcCode[] = { 
			1940, 1941, 1942
		};

		set_base_wparam(FLASH_KEEP_INIT_SETTINGS, 0);

		for(i = 0; i < sizeof(default_funcCode)/sizeof(default_funcCode[0]); i++)
		{
			CodeDefaultParameterByCode(default_funcCode[i]);
		}
		return 1;
	}
	return 0;
}

#endif // #ifdef USE_INIT_MASK ... end
