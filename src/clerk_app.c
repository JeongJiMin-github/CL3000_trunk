/*****************************************************************************
  Copyright		:	(c) 2004 CAS
  Filename		:	clerk_app.c
  Version		:	v 0.9
  Programmer(s)		:	
  Created		:	2004/03/16
  Description		:	plu edit       				
  edit			:	20040319 -> 1.  manager_permission_edit 함수추가 
                                                     menu_function에 등록하고 cap추가 필요함
			                    2.  clerk_manager_edit 부분수정
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
#include "prt_common.h"
#include "prt_interpreter.h"
#include "dsp_vfd7.h"
#include "dsp_app.h"

#include "adm.h"
#include "key.h"
#include "key_typedef.h"
#include "key_app.h"
#include "key_function.h"
#include "main.h"
#include "cal.h"
#include "debug.h"
#include "menu.h"
#include "initial.h"
#include "common.h"
#include "mode_main.h"
#include "mode_error.h"
#include "plu_device.h"
#include "plu_manager.h"
#include "flash_app.h"
#include "caption_app.h"
#include "clerk_app.h" 
#include "mode_sale.h"
#include "pgm_app.h"
//#include "sale_report.h"	// print cmd용
#include "prt_cmd.h"
#include "login.h"
#include "prt_list.h"
#include "menu_code_text1.h"

//reload 할필요 없음 
INT8U clerk_print_list(INT16U start_id, INT16U end_id);
INT8U clerk_zero_allow;

CLERK_STRUCT	g_clerk_str;

////INT8U clerk_edit_check(CLERK_STRUCT *clerk_str)
////{
////	INT16S i,err;
//// 
////	err=0;
////	if (!((status_scale.clerkid==0) && (status_scale.level==1))) {
////		i=password_input_clerk(clerk_str,0);
////		if (!i) err=1;
////		if (!((clerk_str->code==0) && (clerk_str->permissionLevel==1))) err=1;
////	}
////	if (err) {
////		error_display_caption(0x38D1);
////		return OFF;
////	}
////	return ON;
////}
void callback_clerk_id(INT16S num)
{
	CAPTION_STRUCT 	cap;

	POINT  disp_p;
	INT8U  buffer[64], i;
	INT16U uitemp;
	INT32U addr;

	addr = num;
	addr *= CLERK_STRUCT_SIZE;
	addr += FLASH_CLERK_AREA;
	uitemp = get_base_wparam(addr);
	if (uitemp != num) {
		memset((INT8U*)&g_clerk_str, 0x00, CLERK_STRUCT_SIZE);
	} else {
		get_base_sparam( addr, (INT8U *)&g_clerk_str, CLERK_STRUCT_SIZE); 
	}
	g_clerk_str.code = num;

	disp_p = point((Startup.menu_type?2:1)*Startup.menu_ygap, 0);
	memset((INT8U *)&buffer,0x20, 64);
	memset((INT8U *)&cap.form, 0x20, sizeof(cap.form));
	buffer[63] = 0;

	caption_split_by_code(0x1d02, &cap,0);
	//caption_adjust(&cap,NULL);
#ifdef HEBREW_FONT
	buffer[0] = ']';
#else
	buffer[0] = '[';
#endif
	for (i = 0; i < strlen((HUGEDATA char *)g_clerk_str.name); i++)
		buffer[i+1] = g_clerk_str.name[i];
#ifdef HEBREW_FONT
	buffer[cap.input_dlength] = '[';
#else
	buffer[cap.input_dlength] = ']';
#endif
	buffer[cap.input_dlength+1] = 0;

	strcat((HUGEDATA char *)cap.form, (HUGEDATA char *)buffer);
	DspStruct.DirectDraw = 1;
	Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form);
	
	if (Startup.menu_type)
	{
		disp_p = point(3*Startup.menu_ygap, 0);
		memset((INT8U *)&buffer,0x20, 64);
		memset((INT8U *)&cap.form, 0x20, sizeof(cap.form));
		buffer[63] = 0;

		caption_split_by_code(0x1d03, &cap,0);
		//caption_adjust(&cap,NULL);
#ifdef HEBREW_FONT
		buffer[0] = ']';
#else
		buffer[0] = '[';
#endif
		for (i = 0; i < strlen((HUGEDATA char *)g_clerk_str.nickname); i++)
			buffer[i+1] = g_clerk_str.nickname[i];
#ifdef HEBREW_FONT
		buffer[cap.input_dlength] = '[';
#else
		buffer[cap.input_dlength] = ']';
#endif
		buffer[cap.input_dlength+1] = 0;

		strcat((HUGEDATA char *)cap.form, (HUGEDATA char *)buffer);
		DspStruct.DirectDraw = 1;
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)&cap.form, strlen(cap.form), 0, 0); 
		hebrew_codetemp[strlen(cap.form)] = 0;
		disp_p.x = 202 - disp_p.x;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)cap.form, strlen((char *)cap.form), 0, 0, 0); 
		arab_codetemp[strlen((char *)cap.form)] = 0;
		disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
		Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form);
#endif
	}

}


void callback_clerk_name(long num)
{	
	CAPTION_STRUCT 		cap;
	POINT 			disp_p;
	CLERK_STRUCT		clerk_str;
	INT8U	buffer[64];
	INT32U	addr;
	INT16S  	str_size;	
	INT16U 	w;
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
    if(menuDispOn) return;
#endif	  
	str_size = CLERK_STRUCT_SIZE;

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

	if (!clerk_zero_allow) {
		if (num == 0) goto M1;
	}
	caption_split_by_code(0x1D01, &cap,0);
	if(num < cap.input_min || num > cap.input_max){
M1:		
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)&global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0); 
		hebrew_codetemp[strlen(global_message[MSG_INVALID_VALUE])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)&global_message[MSG_INVALID_VALUE], strlen(global_message[MSG_INVALID_VALUE]), 0, 0, 0); 
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
	} else {
		addr = num;
		addr *= str_size;
		addr += FLASH_CLERK_AREA;
		get_base_sparam(addr, (INT8U*)&clerk_str, str_size);
		if(num != clerk_str.code) {
			caption_split_by_code(0xE00C, &cap,0);
#ifdef HEBREW_FONT
			Convert_HebrewPreview((INT8U *)&cap.form, strlen(cap.form), 0, 0); 
			hebrew_codetemp[strlen(cap.form)] = 0;
			Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
			Convert_ArabPreview((INT8U *)cap.form, strlen(cap.form), 0, 0, 0); 
			arab_codetemp[strlen(cap.form)] = 0;
			Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
			Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)&cap.form);
  #else
			Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form);
  #endif
#endif
			return;
		} else {
			memcpy((INT8U*)buffer, (INT8U *)clerk_str.name, str_size);
			buffer[10]=0;
#ifdef HEBREW_FONT
			Convert_HebrewPreview((INT8U *)&buffer, strlen(buffer), 0, 0); 
			hebrew_codetemp[strlen(buffer)] = 0;
			Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
			Convert_ArabPreview((INT8U *)buffer, strlen(buffer), 0, 0, 0); 
			arab_codetemp[strlen(buffer)] = 0;
			Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
   #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
			Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)&buffer);
  #else
			Dsp_Write_String(disp_p,(HUGEDATA char *)&buffer);
  #endif
#endif
			return;
		}
	} 
}

void clerk_dataUpdate(INT32U num)
{
	CLERK_STRUCT clerk_str;
	INT32U	addr;
	INT16U  clerkIndex;

	addr = DFLASH_BASE;
	addr += FLASH_CLERK_AREA;
	addr += (num * CLERK_STRUCT_SIZE);
	clerkIndex = Flash_wread(addr);
	if(clerkIndex == num){
		Flash_sread(addr, (HUGEDATA INT8U *)&clerk_str, CLERK_STRUCT_SIZE);
	} else {
		memset(&clerk_str, 0, CLERK_STRUCT_SIZE);
		clerk_str.code = num;
	}

	*Menu[0].pval = clerk_str.code;
	memcpy(Menu[1].pstr, clerk_str.name, Menu[1].length);
	memcpy(Menu[2].pstr, clerk_str.nickname, Menu[2].length);
	memcpy(Menu[3].pstr, clerk_str.password, Menu[3].length);
	*Menu[4].pval = clerk_str.permissionLevel;
	*Menu[5].pval = clerk_str.training;

	MenuRefresh = ON;
}

void clerk_new(void)
{	
	CAPTION_STRUCT cap;
	CLERK_STRUCT clerk_str;
	INT8U	i;
	INT8U	result;
	INT32U	addr;
	INT8U   name_str[24+1];
	INT8U   nickname_str[8+1];
	INT8U   passwd_str[8+1];
	INT32U  clerk_no, level, training;
	
	if(!permission_check(PMS_SECURITY,0))	return;
	
	caption_split_by_code(0x1D01, &cap,0); //Clerk No. :
	i=0;
	while(1) {	
		addr = i;
		addr *= CLERK_STRUCT_SIZE;
		addr += FLASH_CLERK_AREA;
		addr += DFLASH_BASE;
		clerk_no = Flash_wread(addr);
		if( clerk_no == i ) {
			i++;
		} else {
			break;
		}
	}
	if(clerk_no == cap.input_max) {
		error_display_caption(0xF035);	// data full일때는 manager의 데이터를 보여준다.
		addr = FLASH_CLERK_AREA;
		addr += DFLASH_BASE;
		Flash_swrite(addr, (INT8U *)&clerk_str, CLERK_STRUCT_SIZE);
	} else {
		memset((INT8U*)&clerk_str, 0x00, CLERK_STRUCT_SIZE);
		clerk_str.code = i;	
	}
	if (clerk_str.code==0) clerk_str.code=1;
	clerk_no = (INT32U)clerk_str.code;
	strcpy((char*)name_str, (char*)clerk_str.name);
	strcpy((char*)nickname_str, (char*)clerk_str.nickname);
	strcpy((char*)passwd_str, (char*)clerk_str.password);
	level = (INT32U)clerk_str.permissionLevel;
	training = (INT32U)clerk_str.training;
	
	
	Menu_Init();

	caption_split_by_code(0x1851, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x1d01, &cap,0); // Clerk No. :
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &clerk_no, NULL, NULL, NULL);
	Menu_AddEndFunction(1, clerk_dataUpdate);
	
	caption_split_by_code(0x1d02, &cap,0); // Clerk Name:
	caption_adjust(&cap,NULL);
	Menu_InputCreate(2, (char *)cap.form, MENU_TYPE_STR, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, name_str, NULL, NULL);

	caption_split_by_code(0x1d03, &cap,0); // Nickname  :
	caption_adjust(&cap,NULL);
	Menu_InputCreate(3, (char *)cap.form, MENU_TYPE_STR, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, nickname_str, NULL, NULL);

	caption_split_by_code(0x1d04, &cap,0); // Clerk Password:
	caption_adjust(&cap,NULL);
	Menu_InputCreate(4, (char *)cap.form, MENU_TYPE_STR, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, passwd_str, NULL, NULL);

	caption_split_by_code(0x1d05, &cap,0); // Clerk Level  :
	caption_adjust(&cap,NULL);
	Menu_InputCreate(5, (char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &level, NULL, NULL, NULL);

	caption_split_by_code(0x1d06, &cap,0); // Use for Training : 
	caption_adjust(&cap,NULL);
	Menu_InputCreate(6, (char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &training, NULL, NULL, NULL);

	while (1)
	{
		result = Menu_InputDisplay();
		if(result == MENU_RET_SAVE) {
			str_trim((char *)passwd_str);
			if (strlen((char *)(passwd_str))==0) {
				error_display_caption(0x38D2);
				continue;
			}

			clerk_str.code = (INT16U)clerk_no;
			strcpy((char*)clerk_str.name, (char*)name_str);
			strcpy((char*)clerk_str.nickname,(char*)nickname_str);
			str_trim((char *)passwd_str);
			strcpy((char*)clerk_str.password, (char*)passwd_str);
			clerk_str.permissionLevel = (INT8U)level;
			clerk_str.training = (INT8U)training;
			clerk_str.dept            = 0;
			clerk_str.headid          = 0;
			clerk_str.shopid          = 0;
			clerk_str.permissionID    = 0;

			addr = clerk_str.code;
			addr *= CLERK_STRUCT_SIZE;
			addr += FLASH_CLERK_AREA;
			addr += DFLASH_BASE;
			Flash_swrite(addr, (INT8U *)&clerk_str, CLERK_STRUCT_SIZE);
			break;
		} else if(result == MENU_RET_ESC) break;
	}
}

void clerk_password_dataUpdate(INT32U num)
{
	CLERK_STRUCT clerk_str;
	INT32U	addr;
	INT16U  clerkIndex;

	addr = DFLASH_BASE;
	addr += FLASH_CLERK_AREA;
	addr += (num * CLERK_STRUCT_SIZE);
	clerkIndex = Flash_wread(addr);
	if(clerkIndex == num){
		Flash_sread(addr, (HUGEDATA INT8U *)&clerk_str, CLERK_STRUCT_SIZE);
	} else {
		memset(&clerk_str, 0, CLERK_STRUCT_SIZE);
		clerk_str.code = num;
	}

	memcpy(Menu[1].pstr, clerk_str.password, Menu[1].length);

	MenuRefresh = ON;
}

void clerk_password_change(void)
{
	CAPTION_STRUCT cap;
	CLERK_STRUCT clerk_str;
	char passwd_str[32];
	INT8U i;
	INT8U result;
	INT32U clerk_id;
	INT32U addr;
	INT8U clerkuse;
	INT8U string_buf[50];
	INT8U dealer_password;

	clerkuse=0;
	if ((status_scale.restrict&FUNC_CLERK)) clerkuse=1;

	i=password_input_clerk(&clerk_str, 0);
	if (!i) {
		error_display_caption(0x38D1);
		return;
	}
	if (clerkuse) {
	} else {
		clerk_str.code=1;
	}
	clerk_id = clerk_str.code;
	caption_split_by_code(0x1d01, &cap, 0);
	clerk_zero_allow=1;

	memset(passwd_str, 0, 8);
	dealer_password = OFF;
	if(clerk_str.code < cap.input_min || clerk_str.code > cap.input_max) {
		dealer_password = ON;
		clerk_id=1;
		clerk_str.code=1;
	}
	addr = clerk_str.code;
	addr *=	CLERK_STRUCT_SIZE;
	addr += FLASH_CLERK_AREA;
	addr += DFLASH_BASE;
	Flash_sread(addr, (INT8U *)&clerk_str, CLERK_STRUCT_SIZE);
	memcpy((INT8U*)passwd_str, (INT8U*)clerk_str.password, 8);


	Menu_Init();

	caption_split_by_code(0x1852, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	if (clerkuse && dealer_password)
	{
		caption_split_by_code(0x1d01, &cap, 0); // Clerk No. :
		caption_adjust(&cap,NULL);
		Menu_InputCreate(1, (char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
							cap.input_max, cap.input_min, 0, &clerk_id, NULL, NULL, NULL);
		Menu_AddEndFunction(1, clerk_password_dataUpdate);
	}

	caption_split_by_code(0x1d09, &cap,0); // Clerk Password:
#if defined(USE_SINGLE_LINE_GRAPHIC)
	cap.form[4] = 0;
	sprintf((char*)string_buf, "%-4s.%02ld", cap.form, clerk_id);
#else
#ifdef HEBREW_FONT
	sprintf((char*)string_buf, "%s)%02d(", cap.form, clerk_id);
#else
	sprintf((char*)string_buf, "%s(%02d)", cap.form, clerk_id);
#endif
#endif
	caption_adjust(&cap,NULL);
	Menu_InputCreate(2, (char *)string_buf, MENU_TYPE_STR, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, (INT8U*)passwd_str, NULL, NULL);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		addr = clerk_id;
		addr *= CLERK_STRUCT_SIZE;
		addr += FLASH_CLERK_AREA;
		addr += DFLASH_BASE;
		Flash_sread(addr, (INT8U *)&clerk_str, CLERK_STRUCT_SIZE);

		str_trim((char *)passwd_str);
		strcpy((char*)clerk_str.password, (char*)passwd_str);

		Flash_swrite(addr, (INT8U *)&clerk_str, CLERK_STRUCT_SIZE);
	}
}

extern INT8U menu_data_from_zero;
void clerk_delete(void)
{
	if(!permission_check(PMS_SECURITY,0))	return;
	menu_data_from_zero = 1;
	menu1_delete(0x1854, 0x1D01, FLASH_CLERK_AREA, CLERK_STRUCT_SIZE);
	menu_data_from_zero = 0;
}


void manager_permission_edit(void)
{
	CLERK_PERMISSION_STRUCT 	permission_str;
	// CLERK_STRUCT			clerk_str;

	INT8U	permi_id, sale_mode;	       	//	permi_id  0: manager permission when normal_mode
						// 	permi_id  1: manager permission when clerk_mode	
						//      permi_id  2~8: clerk permission when clerk_mode
	INT16U	result, str_size;
	
	INT32U  addr;

	str_size = PERMISSION_STRUCT_SIZE;
	sale_mode = get_global_bparam(GLOBAL_AREA_SALEMODE);
	if(!permission_check(PMS_SECURITY,0))	return;
	if(sale_mode ==OPER_NORMAL) {
		permi_id = 0;
	} else {
		permi_id = 1;
	}
FUN_START:
	result = permission_setting(permi_id, &permission_str);
	switch(result) {
		case KP_ENTER:
		case KP_ARROW_DN:
		case KP_SAVE:
		case KP_PAGE_DN:
			break;

		case KP_ESC:
			goto RET_ESC;

		default:
			BuzOn(2);
			goto FUN_START;
	}
	addr =(INT32U)permi_id;
	addr*=str_size;
	addr+=FLASH_CLERK_PERMISSION;
	set_base_sparam(addr, (INT8U*)&permission_str, str_size);
RET_ESC:
 	return;	
	// id check
}
#ifdef USE_REALTIME_DATA_EVROOPT
void remote_permission_setting(INT8U permission_id, CLERK_PERMISSION_STRUCT *permission_str) 
{	
	CAPTION_STRUCT	cap;	
	INT8U 	menu_cnt;
	INT16S	i, result, str_size;
	INT8U 	input_data;
	INT32U  addr;

	str_size = PERMISSION_STRUCT_SIZE;

	addr = permission_id;
	addr*= str_size;
	addr+=FLASH_CLERK_PERMISSION;
    
	get_base_sparam(addr,(INT8U*)permission_str, str_size );

    input_data = get_global_bparam(GLOBAL_SALE_SETUP_FLAG19) & 0x04;
    input_data = input_data>>2;

	permission_str->edit_menu = input_data;

	set_base_sparam(addr,(INT8U*)permission_str, str_size );
			
}

void remote_manager_permission_edit(void)
{
	CLERK_PERMISSION_STRUCT 	permission_str;
	INT8U	permi_id, sale_mode;	       	//	permi_id  0: manager permission when normal_mode
						                        // 	permi_id  1: manager permission when clerk_mode	
						                        //      permi_id  2~8: clerk permission when clerk_mode
	INT16U	result, str_size;
	INT32U  addr;

	str_size = PERMISSION_STRUCT_SIZE;
	sale_mode = get_global_bparam(GLOBAL_AREA_SALEMODE);
	/*if(!permission_check(PMS_SECURITY,0))	return;
	if(sale_mode ==OPER_NORMAL) {
		permi_id = 0;
	} else {
		permi_id = 1;
	}*/

	//remote_permission_setting(permi_id, &permission_str);
	permi_id = 0;
	remote_permission_setting(permi_id, &permission_str);

	addr =(INT32U)permi_id;
	addr*=str_size;
	addr+=FLASH_CLERK_PERMISSION;
	set_base_sparam(addr, (INT8U*)&permission_str, str_size);

}
#endif

/*
void clerk_permission_edit(void)
{
	CAPTION_STRUCT	cap;	
	POINT 		disp_p;
		
	INT8U	y_gab=8, Startup.menu_xgap=6;				// 폰트의 x, y 크기

	CLERK_PERMISSION_STRUCT 	permission_str;
	CLERK_STRUCT			clerk_str;

	char 	buffer[32];
	INT8U	permi_id, sale_mode;	       	//	permi_id  0: manager permission when normal_mode
						// 	permi_id  1: manager permission when clerk_mode	
						//      permi_id  2~8: clerk permission when clerk_mode
	INT16U	result, str_size;
	long 	ret_long;
	INT32U  addr;

	str_size = PERMISSION_STRUCT_SIZE;
	sale_mode = get_global_bparam(GLOBAL_AREA_SALEMODE);
	if(sale_mode ==OPER_NORMAL) {
		sprintf(buffer, "Change Clerk mode");
		error_display_message(buffer);	      
		return;
	}
//	get_base_sparam(FLASH_CLERK_AREA, (INT8U*)&clerk_str, CLERK_STRUCT_SIZE); // load data of manager

	// JHC 20040331 <MODIFY>
	// [
	// <Original>
	// if (!clerk_edit_check(&clerk_str)) return;
	//
	// <Modify>
	if(!permission_check(PMS_SECURITY))	return;
	// ]

	display_title_page_change(0x1855, 1, 1);
	permi_id=2;
INPUT_PERMISSION:
	result = permission_setting(permi_id, &permission_str);
	switch(result) {
		case KP_ENTER:
		case KP_ARROW_DN:
		case KP_SAVE:
		case KP_PAGE_DN:
			break;

		case KP_ARROW_UP:
		case KP_PAGE_UP:
//			goto KI_IN;
			goto INPUT_PERMISSION;
		case KP_ESC:
			goto RET_ESC;

		default:
			BuzOn(2);
			goto INPUT_PERISSION;
	}
	addr =(INT32U)permi_id;
	addr*=str_size;
	addr+=FLASH_CLERK_PERMISSION;
	set_base_sparam(addr, (INT8U*)&permission_str, str_size);
RET_ESC:
	Dsp_Fill_Buffer(0);						// LCD Buffer clear
	Dsp_Diffuse();
 	return;	
	// id check
}
*/

INT16U permission_setting(INT8U permission_id, CLERK_PERMISSION_STRUCT *permission_str) 
{	
	CAPTION_STRUCT	cap;	
	INT8U 	menu_cnt;
	INT16S	i, result, str_size;
	INT32U 	input_data[37];
	INT32U  addr;
    INT8U oldtemp,newtemp;

	str_size = PERMISSION_STRUCT_SIZE;

	addr = permission_id;
	addr*= str_size;
	addr+=FLASH_CLERK_PERMISSION;
	get_base_sparam(addr,(INT8U*)permission_str, str_size );

	input_data[0]	= permission_str->change_item;
	input_data[1]	= permission_str->change_price;
	input_data[2]	= permission_str->key_dept;
	input_data[3]	= permission_str->key_pay;
	input_data[4]	= permission_str->key_date_chage;
	input_data[5]	= permission_str->key_type_chage;
	input_data[6]	= permission_str->key_inhibit;
	input_data[7]	= permission_str->key_inweight;
	input_data[8]	= permission_str->key_discount;

	input_data[9]	= permission_str->clerk_sales;
	input_data[10]	= permission_str->sale_void;
	input_data[11]	= permission_str->sale_addup;
	input_data[12]	= permission_str->sale_return;
	input_data[13]	= permission_str->sale_override;
	input_data[14]  = permission_str->cash_open;
	input_data[15]	= permission_str->edit_menu;
	input_data[16]	= permission_str->edit_plu;
	input_data[17]	= permission_str->edit_price;	// 기능없음
	input_data[18]	= permission_str->edit_global;
	input_data[19]	= permission_str->edit_x_report;
	input_data[20]	= permission_str->edit_z_report;
	input_data[21]	= permission_str->edit_store_data;
//DEL060912	input_data[22]	= permission_str->edit_dc_table;
	input_data[23]	= permission_str->edit_tax_table;
//DEL060912	input_data[24]	= permission_str->edit_tare_table; /* 7200 - MISC PLU KEY? */
//DEL060912	input_data[25]	= permission_str->edit_trace_table;
	input_data[25]  = permission_str->initialization;
//DEL060912	input_data[26]	= permission_str->edit_custom_table;
	input_data[27]	= permission_str->edit_scale_con;
//	input_data[28]	= permission_str->edit_plu_manage;
	input_data[29]  = permission_str->edit_data_table1;
	input_data[30]  = permission_str->edit_data_table2;
	input_data[31]  = permission_str->print_set       ;
	input_data[32]  = permission_str->oper_mode       ;
	input_data[33]  = permission_str->user_parameter  ;
	input_data[34]  = permission_str->communication   ;
	input_data[35]	= permission_str->key_save;
	input_data[36]	= permission_str->reopen;
	menu_cnt = 37;
    	Menu_Init();
	caption_split_by_code(0x1855, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_continuous_search = ON;
	caption_continuous_search_addr = 0;
	for (i = 0; i<menu_cnt; i++) {
		if(i == 22 || i == 24 || i == 26 || i == 28)	// 임시 제거
		{
			continue;
		}
		caption_split_by_code(0x1D51+i, &cap,0);
		caption_adjust(&cap,NULL);
		if (caption_run_check(cap.code,cap.runflag)) {
			Menu_InputCreate(1, 
				 (HUGEDATA char *)cap.form, 
				 MENU_TYPE_NUM, 
				 cap.input_x, 
				 cap.input_dlength, 
				 cap.input_length,
				 cap.input_max, 
				 cap.input_min, 
				 0, 
				 &input_data[i], 
				 NULL, NULL, NULL);
		}
	}
	caption_continuous_search = OFF;

PAGE_VIEW:
       	result = Menu_InputDisplay();
	switch(result)
	{
	case MENU_RET_SAVE:
		permission_str->change_item 		= (INT8U)input_data[0];
		permission_str->change_price 		= (INT8U)input_data[1];
		permission_str->key_dept                = (INT8U)input_data[2];
		permission_str->key_pay                 = (INT8U)input_data[3];
		permission_str->key_date_chage          = (INT8U)input_data[4];
		permission_str->key_type_chage          = (INT8U)input_data[5];
		permission_str->key_inhibit             = (INT8U)input_data[6];
		permission_str->key_inweight            = (INT8U)input_data[7];
		permission_str->key_discount            = (INT8U)input_data[8];
		permission_str->clerk_sales             = (INT8U)input_data[9];
		permission_str->sale_void               = (INT8U)input_data[10];
		permission_str->sale_addup              = (INT8U)input_data[11];
		permission_str->sale_return             = (INT8U)input_data[12];
		permission_str->sale_override           = (INT8U)input_data[13];
		permission_str->cash_open               = (INT8U)input_data[14];
		permission_str->edit_menu               = (INT8U)input_data[15];
		permission_str->edit_plu                = (INT8U)input_data[16];
		permission_str->edit_price              = (INT8U)input_data[17];
		permission_str->edit_global             = (INT8U)input_data[18];
		permission_str->edit_x_report           = (INT8U)input_data[19];
		permission_str->edit_z_report           = (INT8U)input_data[20];
		permission_str->edit_store_data         = (INT8U)input_data[21];
//DEL060912		permission_str->edit_dc_table           = (INT8U)input_data[22];
		permission_str->edit_tax_table          = (INT8U)input_data[23];
//DEL060912		permission_str->edit_tare_table         = (INT8U)input_data[24];
//DEL060912		permission_str->edit_trace_table        = (INT8U)input_data[25];
		permission_str->initialization          = (INT8U)input_data[25];
//DEL060912		permission_str->edit_custom_table       = (INT8U)input_data[26];
		permission_str->edit_scale_con          = (INT8U)input_data[27];
//		permission_str->edit_plu_manage         = (INT8U)input_data[28];
		permission_str->edit_data_table1        = (INT8U)input_data[29];
		permission_str->edit_data_table2        = (INT8U)input_data[30];
		permission_str->print_set               = (INT8U)input_data[31];
		permission_str->oper_mode               = (INT8U)input_data[32];
		permission_str->user_parameter          = (INT8U)input_data[33];
		permission_str->communication           = (INT8U)input_data[34];
		permission_str->key_save		= (INT8U)input_data[35];
		permission_str->reopen			= (INT8U)input_data[36];
		set_base_sparam(addr,(INT8U*)permission_str, str_size );
#ifdef USE_REALTIME_DATA_EVROOPT
        oldtemp = get_global_bparam(GLOBAL_SALE_SETUP_FLAG19);

        if(permission_str->edit_menu) newtemp = oldtemp | 0x04; //파라미터 1856번은 1과 0로만 세팅하도록 요청됨. 추후 필요시 0~3까지 되도록 수정
        else newtemp = oldtemp &0xfb;
        set_global_bparam(GLOBAL_SALE_SETUP_FLAG19,newtemp);
#endif
		return KP_SAVE;
	case MENU_RET_ESC:
		goto RET_ESC;
	default :
		goto PAGE_VIEW;
	}

RET_ESC:
	return KP_ESC;
	// ]			
}

void clerk_save(CLERK_STRUCT *clerk)
{
	INT32U addr;
	if (clerk->code<0) return;
	addr  = (DFLASH_BASE + FLASH_CLERK_AREA);
	addr += clerk->code*CLERK_STRUCT_SIZE;
	Flash_swrite(addr, (HUGEDATA INT8U *)clerk, sizeof(CLERK_STRUCT));
}

#ifndef DISABLE_LIST_MENU
void clerk_list(void)
{		
	CAPTION_STRUCT 	cap;
	POINT 		disp_p;
	CLERK_STRUCT	clerk_str;
	
//	INT8U	y_gab=8;				// 폰트의 y 크기
//	INT8U	x_gab=6;				// 폰트의 x 크기

	char   	string_buf[64], end_flag, reverse_num;
	long 	ret_long, i;
	INT16S	start_num, end_num, cur_id, str_size;
	INT16S	j, result, min_num, max_num, lcd_size;	
	INT16S	line_id[4];

	INT16U	read_code, title_cap, boundary_cap, input_cap;//, list_title_cap;
	INT32U	addr, addr_flash;
	INT8U	linenum;			    //se-hyung 20071004
		
	// JHC 20040331	<MODIFY>
	// [
	// <Original>
	// if (!clerk_edit_check(&clerk_str)) return;
	// 
	// <Modify>
	if(!permission_check(PMS_SECURITY,0))	return;
	// ]
		
	title_cap    = 0x1853;
	boundary_cap = 0x1D01;
	input_cap    = 0x1E00;
//	list_title_cap = 0x1859;

	str_size = CLERK_STRUCT_SIZE;
	addr_flash = DFLASH_BASE + FLASH_CLERK_AREA;

	lcd_size = Dsp_GetChars();

	caption_split_by_code(boundary_cap, &cap,0);     	// Decide inpput boundary
	min_num	= 1;					// manager list는 출력하지 않는다. 
	max_num = cap.input_max; 			// 



LIST_START:
	end_flag=1;	
	
	Dsp_Fill_Buffer(0);
	//display_title_page(title_cap);
	display_title_page_change(title_cap,1,1);
	linenum = Startup.line_max;			      //se-hyung 20071004 
	if (linenum > 3) linenum = 3;			      //se-hyung 20071004  
	for(i = 0;i<linenum; i++)  {			      //se-hyung 20071004
//	for(i = 0; i<Startup.line_max; i++)  {
		caption_split_by_code(input_cap+i, &cap,0);
		if (i<2) caption_adjust(&cap,NULL);
		disp_p = point((i+Startup.menu_type)*Startup.menu_ygap,0);
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
		Dsp_Write_String(disp_p,(HUGEDATA char*)cap.form);
#endif
	}
	Dsp_Diffuse();

	start_num = min_num;
	end_num = max_num;
	
	for(i=0; i<2; i++)  {
REN1:		switch(i) {
			case 0:	ret_long = start_num;	
				break;
			case 1:	ret_long = end_num;
				break;
 		}

		caption_split_by_code(input_cap+i, &cap,0);
		caption_adjust(&cap,NULL);
		disp_p = point((i+Startup.menu_type)*Startup.menu_ygap,0);
#ifdef HEBREW_FONT
		 Convert_HebrewPreview((INT8U *)cap.form, strlen(cap.form), 0, 0); 
		 hebrew_codetemp[strlen(cap.form)] = 0;
		 disp_p.x = 202 - disp_p.x;
		 Dsp_Write_1stringLong(disp_p, hebrew_codetemp);

		 result=keyin_ulong(KI_GRAPHIC, KI_FORM_ILNN,	0,(long*)&ret_long, cap.input_dlength, 
				   				(i+Startup.menu_type)*Startup.menu_ygap,  cap.input_x*Startup.menu_xgap, 0,KI_NOPASS );
#elif defined(USE_ARAB_CONVERT)
		 Convert_ArabPreview((INT8U *)cap.form, strlen(cap.form), 0, 0, 0); 
		 arab_codetemp[strlen(cap.form)] = 0;
		 disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
		 Dsp_Write_1stringLong(disp_p, arab_codetemp);

		 result=keyin_ulong(KI_GRAPHIC, KI_FORM_ILNN,	0,(long*)&ret_long, cap.input_dlength, 
			   				(i+Startup.menu_type)*Startup.menu_ygap,  cap.input_x*Startup.menu_xgap, 0,KI_NOPASS );
#else
		Dsp_Write_String(disp_p,(HUGEDATA char*)cap.form);
			
		result=keyin_ulong(KI_GRAPHIC, KI_FORM_NORMAL,	0,(long*)&ret_long, cap.input_dlength, 
				   				(i+Startup.menu_type)*Startup.menu_ygap,  cap.input_x*Startup.menu_xgap, 0,KI_NOPASS );
#endif
		switch (result){
						 
			case KP_ARROW_UP:  if(i == 0)	
						goto REN1;
					   i--;	
					   goto REN1;

			case KP_PAGE_UP:
			case KP_ESC:
				   	goto RET_ESC;

			case KP_SAVE:	   start_num = min_num;
					   end_num = max_num;
					   i++;
					   break;
			case KP_PAGE_DN:
			case KP_ARROW_DN:
			case KP_ENTER:	   
					   if ( ret_long > max_num || ret_long < min_num ) {
						BuzOn(2);	goto REN1;	
				           }
					   if(i == 0)	start_num = ret_long;
					   if(i == 1)	end_num = ret_long;
					   break;
					   				  				
		     	default:	BuzOn(2);	goto REN1;		     	 	
		} //switch (result)
	}//for(i=1; i<3; i++)

	for(i=start_num; i<=end_num; i++) {					// There is no data 
		
		addr = addr_flash + (long)str_size * i;				//
		read_code = Flash_wread(addr);					//													 
 		if(read_code >= min_num && read_code <= max_num ) {	 	//
 			end_flag = 0;						//
			break;							//
		}		 						//
	}									//	
	if(end_flag == 1) {							//
			error_display_caption(0xE00C);				//
			goto LIST_START;					//
	}									//
	cur_id = start_num;
READ_START:	
	j = 0;
	for(i=cur_id; i<=end_num; i++)
	{						
		addr =  addr_flash + (long)str_size * i;
		read_code = Flash_wread(addr);		
		
 		if(read_code >= min_num && read_code <= max_num )
		{	
			line_id[j] = read_code;
			if(j == 0) {
				Dsp_Fill_Buffer(0);
				caption_split_by_code(0x7853, &cap,0);
//				disp_p = point(0,0);
				disp_p = point((Startup.menu_type-1) * Startup.menu_ygap, 0); //se-hyung 20071004  
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
				Dsp_Write_String(disp_p,(HUGEDATA char*)cap.form);
#endif
			}

			Flash_sread(addr, (HUGEDATA INT8U*)&clerk_str, str_size ); 			
		 	sprintf(string_buf, "%2d   %2d    %s", clerk_str.code, clerk_str.permissionLevel, clerk_str.name);
			string_buf[lcd_size] = 0;	       		
			disp_p = point((j+Startup.menu_type)*Startup.menu_ygap, 0);	       		 

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
 			Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
#endif
 			Dsp_Diffuse();	
			j++;
		}
				
		if(i == end_num )  end_flag = 1;
		else	end_flag = 0; 

KEY_WAITE:	
		if(j > 2 || end_flag == 1) {
			while(!KEY_Read());		BuzOn(1);
			if(KeyDrv.CnvCode == KP_ESC)	goto RET_ESC;			
			
			switch(KeyDrv.CnvCode) {

				case KP_ARROW_UP:
						cur_id = line_id[0]-1;						
						while(1)  {
							if(cur_id <= start_num) {
								cur_id = start_num;
								goto READ_START;
							}
							addr =  addr_flash + (long)str_size * (cur_id);			
							read_code = Flash_wread(addr);
							if(read_code >= min_num && read_code <= max_num )  {
								cur_id = read_code;								    
								goto READ_START;
							}
							cur_id--;
						}
									       
				case KP_PAGE_UP :
						cur_id = line_id[0]-1;						
								
						reverse_num = 0;

						while(1)  {
							if(cur_id <= start_num) {
								cur_id = start_num;
								goto READ_START;
							}
							addr =  addr_flash + (long)str_size * (cur_id);			
							read_code = Flash_wread(addr);
  							
   							if(read_code >= min_num && read_code <= max_num )  {
							       	reverse_num++;
								if(reverse_num ==3) {									
									cur_id = read_code;  
									goto READ_START;
								}
							}
							cur_id--;
						}

				case KP_PAGE_DN :
				case KP_ENTER:
						if(end_flag == 1)  {
							error_display_caption(0xE00E); 
							goto KEY_WAITE;
						} else {
							cur_id = line_id[2]+1;
							goto  	READ_START;
						}
						
				case KP_ARROW_DN:
						if(end_flag == 1)  {
							error_display_caption(0xE00E); 
							goto KEY_WAITE;
						} else {
							cur_id = line_id[1];
							goto   	READ_START;
						}						
				
				// Added by CJk
				case KP_TEST:
						clerk_print_list(start_num, end_num);
						goto KEY_WAITE;		
				///////////////////////////////
				case KP_FEED:
						keyapp_feed(OFF);
						goto KEY_WAITE;

				default 	:	
				    BuzOn(1);
				    goto KEY_WAITE;
			} //switch(KeyDrv.CnvCode)

		} //if(j > 2)
	}//for(i=start_num; i<=end_num ; i++)

	RET_ESC:    return;
}
#endif //#ifndef DISABLE_LIST_MENU

#define M_ID_CLERKKEY_KNO	1
#define M_ID_CLERKKEY_TNO	2

void callback_end_clerkkey(INT32U num)
{
	INT32U addr_key;
	INT8U  id_clerk_no;
	INT32U clerk_no;

	id_clerk_no = Menu_GetMenuId(M_ID_CLERKKEY_TNO);

	addr_key = FLASH_KEY_CDTC_AREA + DFLASH_BASE;
	addr_key += (num - 1) * 2;
	clerk_no = Flash_wread(addr_key);

	if(id_clerk_no != 0xff) *Menu[id_clerk_no].pval = clerk_no;

//      	MenuRefresh = ON;
}

void clerk_key_define(void)
{
	CAPTION_STRUCT cap;
	INT16S 	result;
	INT32U  addr_key;
	INT32U  clerk_key_no, clerk_no;
	INT32U  clerk_key_max;

	clerk_key_no = 1;
	clerk_no = 1;

	clerk_zero_allow=0;
	while(1)
	{
		addr_key = FLASH_KEY_CDTC_AREA + DFLASH_BASE;
		addr_key += (clerk_key_no - 1) * 2;
		clerk_no = Flash_wread(addr_key);

		Menu_Init();

		caption_split_by_code(0x1856, &cap,0);//""
		Menu_HeadCreate((HUGEDATA char *)cap.form);

		caption_split_by_code(0x3429, &cap,0);//""
		clerk_key_max = cap.input_max;
		//if (clerk_key_max > MAX_TARE_KEY) clerk_key_max = MAX_TARE_KEY;
		caption_adjust(&cap,NULL);
		Menu_InputCreate(M_ID_CLERKKEY_KNO, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
							cap.input_max, cap.input_min, 0, &clerk_key_no, NULL, NULL, NULL);
		Menu_AddEndFunction(M_ID_CLERKKEY_KNO, callback_end_clerkkey);
		Menu_SetKeyMode(M_ID_CLERKKEY_KNO, 3);

		caption_split_by_code(0x1D01, &cap,0);//""
		caption_adjust(&cap,NULL);
		Menu_InputCreate(M_ID_CLERKKEY_TNO, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
							cap.input_max, cap.input_min, 0, &clerk_no, NULL, NULL, callback_clerk_name);						

		result = Menu_InputDisplay();
		if (result == MENU_RET_SAVE) 
		{
			addr_key = FLASH_KEY_CDTC_AREA + DFLASH_BASE;
			addr_key += (clerk_key_no - 1) * 2;
			Flash_wwrite(addr_key, clerk_no);
			clerk_key_no++;
			if (clerk_key_no > clerk_key_max) clerk_key_no = clerk_key_max;
		}
		else if (result == MENU_RET_ESC)
		{
			break;
		}
	}
}

