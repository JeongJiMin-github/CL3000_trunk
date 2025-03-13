/*****************************************************************************
  Copyright		:	(c) 2002 CAS
  Filename		:	init.c
  Version			:	0.1
  Programmer(s)	:	Byung Moo Lim (LIM)
  Created			:	2003/09/04
  Created			:	2004/08/20
***************************************************************************/
#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "serial.h"
#include "flash.h"
#include "commbuf.h"
#include "prt_common.h"
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
#include "prt_interpreter.h"
#include "prt_advertise.h"
#include "dsp_vfd7.h"
#include "adm.h"
#include "key.h"
#include "cal.h"
#include "prt_cmdtest.h"
#include "programmode.h"
//#include "file.h"
#include "rtc.h"
#include "debug.h"
#include "ethernet.h"
#include "main.h"
#include "initial.h"
#include "flash_app.h"
#include "common.h"
#include "menu.h"
#include "plu_device.h"
#include "plu_manager.h"
#include "caption_app.h"
#include "mode_commun.h"
#include "mode_sale.h"
#include "dsp_app.h"
#include "key_app.h"
#include "key_typedef.h"
#include "key_function.h"

#include "comm_app.h"
#include "mode_calibration.h"
#include "ps2.h"
#include "label_caption.h"
#include "mode_main.h"
#include "ethernet_comm.h"
#include "global_set.h"
#include "pgm_app.h"
#include "network_command.h"
#include "clerk_app.h"
#include "login.h"
#include "discount.h"
#include "initial_glb.h"
#include "adm_app.h"
#include "prt_app.h"
#include "network_command.h"

#include "wlan_set_lantronix.h"
#include "traceability.h"
#include "license.h"
#include "sqi.h"
#include "wdt.h"
#include "scale_config.h"

//#define ETH_JIG_PGM4
#ifdef USE_PIC32MZ_EFG
#ifdef CL3000_PROJECT
#define DISPLAY_MODELNAME_FOR_EFG_VERSION   "  CL3000"
#define DISPLAY_START_POS_VERSION 18
#else
#define DISPLAY_MODELNAME_FOR_EFG_VERSION   " CL5200j"
#define DISPLAY_START_POS_VERSION 17
#endif
#define DISPLAY_CPUTYPE_FOR_EFG_VERSION   "     EFG"
#endif

HUGEDATA PACKDATE_STRUCT mPackDate; //SG060309
extern INT16U plu_n_field;
extern INT8U CheckingWirelessLAN;

extern INT16U plu_ptype_reset(void);
#ifdef USE_EXT_SETTING_MEM
extern INT8U  Data_Save_Restore(INT8U restore);
#endif
extern INT32U rtc_makedate_str(char *str, INT8U year, INT8U month, INT8U date);
extern INT32U rtc_maketime_str(char *str, INT8U hour, INT8U min  , INT8U sec );
extern void   get_max_pluno(void);
extern INT8U  caption_init(void);
extern void plu_ptype_enable_code(void);
extern void check_delay_time(INT16S ch);
extern void netClerkLockInit(void);
extern void scroll_PLU_init(void);
#ifdef USE_LOT_NUMBER_FUNCTION
extern void	hungary_lotnumber_default(void);
#endif

INT32U NVRAM_CLEARK_LASTSALE_PNT;
INT32U FLASH_PLU_AREA_PNT;
INT32U FLASH_PLU_STRUCT_PNT;
INT32U FLASH_ING_EXT_AREA_PNT;
INT32U FLASH_ING_NOR_AREA_PNT;
#ifdef USE_USB_DATA_ROM_UPDATE
INT8U UsbDataRomUpdate;
INT8U CheckUsbDataRomUpdate;
#endif
#ifndef STARTUP_NOT_DEFINE
#define STARTUP_NOT_DEFINE
//STARTUP_CONFIG Startup;
NEARDATA STARTUP_CONFIG Startup;//HYP 20060412 _huge --> _near for small code size
#endif
#ifdef USE_CTS_FUNCTION
CTS_STRUCT CtsStruct;
CTS_STRUCT2 CtsStruct2;
CTS_ENCRYPT_BUF_STRUCT CtsEncryptBufStruct;
#endif
INT8U   UseSaleModeEditFlag;
INT8U InitFtlChkFlag=0;

#if defined(USE_TRACE_STANDALONE)
#define FIRMWARE_TYPE_NUMBER 0x31  
#else
#define FIRMWARE_TYPE_NUMBER 0x30  
#endif

#ifdef USE_INIT_MASK
static struct INIT_MASK_LINKED_LIST 		g_init_mask; 		// 초기화 방어 링크드 리스트 더미 노드 변수
static struct INIT_MASK_LINKED_LIST_META 	g_init_mask_meta; 	// 초기화 방어 링크드 리스트 메타 변수
#endif

//#define UPDATE_VERSION 11 // 2012.06.05 param750 추가
//#define UPDATE_VERSION 12 // 2012.07.02 param718 추가
//#define UPDATE_VERSION 13 // 2012.07.09 param782 추가
//#define UPDATE_VERSION 15 // 2014.07.01 CL7200 전용. 이력관리에 data.db 사용.
//#define UPDATE_VERSION 16 // 2014.09.23 CL7200 전용. 개체번호키 data 구조 변경. 
//#define UPDATE_VERSION 17 // 2014.11.13 F/W Updated Version 저장위치 오류 수정(FLASH_FW_PDATE_VER)
//#define UPDATE_VERSION 18 // 2015.02.02 CL7200 전용. touch key 유효성 체크를 위한 time offest
//(이후로 CL7200과 UPDATE_VERSION 별도 관리)
//#define UPDATE_VERSION 19 // 2016.03.07 param746 추가. Auto zero setting
//#define UPDATE_VERSION 20 // 2016.03.08 key on off 기능 추가
//#define UPDATE_VERSION 21 // 2018.01.29 CTS Struct 2 추가 관련 History 갯수 5->4
//#define UPDATE_VERSION 22 // 2018.06.05 Param 729추가 : tare 필드를 member price로 사용 
//#define UPDATE_VERSION 23 // 2018.07.16 Menu 1867, Contrast 조절 기능 추가(CL3000 전용기능)
#define UPDATE_VERSION 24 // 2022.09.29 Param 795추가 : 무게 있을 때 Key Tare 사용 설정

extern void ETHERNET_MACInit(void);
extern void ETHERNET_Reset(void);
#ifdef USE_ADM_AUTO_RESTORE
void check_ADM_Err_count(void);
#endif
extern void debug_Factory(void);
//void init_default(void)
//{
//	CalRun_InitADM(0);
//	init_factory();
//	display_init();
//	global_net_init();
//	KEY_CopyKeyTable(0);
//	PrtFlashSetDefault();
//	plu_all_delete();
//	label_caption_init();
//	print_inhibit_init();
//	global_set_default_parameter();
//}

void init_factory_nvram(INT8U mode)
{
	INT8U chkversion[4],cur_version[4];
	strcpy((char *)cur_version,NVRAM_STRUCT_VERSION);
//	FW_VERSION fversion;
	get_nvram_sparam(NVRAM_VERSION,chkversion,4);
//	version_get(&fversion);
//	Startup.fwversion[0] = fversion.version;
//	Startup.fwversion[1] = fversion.reversion[0];
//	Startup.fwversion[2] = fversion.reversion[1];
	if (mode&CLEAR_CHECK) {
		if (memcmp(chkversion,cur_version,3)==0) {
			return;
		}
	}
	if (mode&CLEAR_NVRAM) {
		if (mode&CLEAR_VERBOSE)
			VertScrollMsg_Add("Clear RAM");
		nvram_clear();
		Nvram_init_nvbuffer(); // nvram cache
	}
	if (mode&CLEAR_REPORT) {
		if (mode&CLEAR_VERBOSE)
			VertScrollMsg_Add("Reset Report");
		Report_InitResetTime();
		if (mode&CLEAR_VERBOSE)
			VertScrollMsg_Add("Clear Tran");
		ClerkClearTransaction();
#if defined(USE_INDIA_FUNCTION) || defined(USE_SEND_TICKET_DATA)
//#ifdef USE_QR_ADDTOTAL
		ClearAdditionalData();
//#endif
#endif
		if (mode&CLEAR_VERBOSE)
			VertScrollMsg_Add("Clear Summary"); //check to caption.
		ClearSalesSummaries(99);
		ClearClerkTareList();
//#ifdef USE_ZREPORT_COUNTER
//		ClearZreportCounter();
//#endif
		commun_net_mask(0, NULL, 0xff, 98);	// sync plu
		commun_net_mask(1, NULL, 0xff, 98);	// sync d/c
		commun_net_mask(2, NULL, 0xff, 98);	// sync ingredient
		commun_net_mask(3, NULL, 0xff, 98);	// sync scroll msg

#ifdef USE_TRACE_STANDALONE
		TNT_InitTraceNoTable();	// Trace No Table 초기화
#endif
	}
	set_nvram_sparam(NVRAM_VERSION,cur_version,4);

	FtlDataBackupAllDataProgram(OFF);
	FtlDataBackupFlagProgram(FTL_INIT_FLAG_COMPLETE);

}

void boot_log(void)
{
	INT16U boot_cnt;
	INT16U boot_cnt_high; // boot_cnt의 상위 2byte
	INT32U boot_cnt_total;
	INT32U addr;
	INT8U  date[6];
	
	boot_cnt_high=get_base_wparam(FLASH_START_COUNT_HIGH);
	boot_cnt=get_base_wparam(FLASH_START_COUNT);
	if (boot_cnt==0xffff) {
		boot_cnt=0;
		if (boot_cnt_high==0xffff)
			boot_cnt_high=0;
		else
			boot_cnt_high++;
	} else {
		boot_cnt++;
	}
	set_base_wparam(FLASH_START_COUNT_HIGH,boot_cnt_high);
	set_base_wparam(FLASH_START_COUNT,boot_cnt);

	boot_cnt_total = boot_cnt_high;
	boot_cnt_total <<= 16;
	boot_cnt_total += boot_cnt;
	addr = boot_cnt_total;
	addr%=5;
	addr*=6;
	addr+=DFLASH_BASE;
	addr+=FLASH_START_LOG;
#ifdef USE_PERSIAN_CALENDAR
	RTC_Convert_Persian_Read();
#else
	RTC_CLK_Burst_Read();
#endif
	date[0] = bcd2hex(RTCStruct.year);
	date[1] = bcd2hex(RTCStruct.month);
	date[2] = bcd2hex(RTCStruct.date);
	date[3] = bcd2hex(RTCStruct.hour);
	date[4] = bcd2hex(RTCStruct.min);
	date[5] = bcd2hex(RTCStruct.sec);
	Flash_swrite(addr,date,6);
	
//sprintf(MsgBuf, "Boot Time : 20%02d/%02d/%02d %02d:%02d:%02d\r\n", date[0], date[1], date[2], date[3], date[4], date[5]);
//MsgOut(MsgBuf);
}

void init_show_lasttime(void)
{
	INT16U boot_cnt;
	INT16U boot_cnt_high; // boot_cnt의 상위 2byte
	INT32U boot_cnt_total;
	INT32U addr;
	INT8U  date[6];
	char   str[32];
	POINT  disp_p;
	INT16S    old_id;
		
	boot_cnt_high=get_base_wparam(FLASH_START_COUNT_HIGH);
	boot_cnt=get_base_wparam(FLASH_START_COUNT);
	boot_cnt_total = boot_cnt_high;
	boot_cnt_total <<= 16;
	boot_cnt_total += boot_cnt;
	
	if (Startup.start_flag&0x20)
	{
		if (boot_cnt_total) // boot_log()를 먼저 실행하는 경우
			addr = boot_cnt_total-1;
		else
			addr = 0;
		addr%=5;
		addr*=6;
		addr+=DFLASH_BASE;
		addr+=FLASH_START_LOG;
		Flash_sread(addr,date,6);
		sprintf(str,"%02d%02d%02d%02d%02d%02d",date[0],date[1],date[2],date[3],date[4],date[5]);
	}
	else if (Startup.start_flag&0x40)
	{
		sprintf(str,"on:%d",boot_cnt_total);
	}
	Dsp_Fill_Buffer(0);
	old_id = DspStruct.Id1;
	DspLoadFont1(DSP_PLU_FONT_ID);
	disp_p.y = 0; disp_p.x = 0;
	Dsp_Write_String(disp_p,(HUGEDATA char *)str);
	Dsp_Diffuse();
	DspLoadFont1(old_id);
}

INT8U init_map(void)
{
	INT8U r;

	Startup.start_addr = DFLASH_BASE;
	Flash_sread(Startup.start_addr+FLASH_STR_VERSION,Startup.version,2);
	if (_isdigit(Startup.version[0])!=0) {
		r=TRUE;
	} else {
		r=FALSE;
	}
	return r;
}

void display_style(void)
{
	INT32U set_val;
	CAPTION_STRUCT cap;
	INT8U cc,result;

	Menu_Init();
	set_val=0;

	caption_split_by_code(0x4997, &cap,0);// Factory Default	// Modified by CJK 20050928
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x4997, &cap,0);// Question
	caption_adjust(&cap,NULL);
	cc=get_global_bparam(GLOBAL_DISPLAY_TARELENGTH);
	set_val=3;
	if (cc) set_val=4;
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
			cap.input_max, cap.input_min, 0, &set_val, NULL, NULL, NULL);
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
	    if (set_val==4) {
		global_display_set_4digit(DISPLAY_PRICE_DIGIT, Startup.country);
	    } else if (set_val==3) {
		global_display_set_3digit();
	    }
	    display_alloff_primary_indication();
//	    display_clear(DISPLAY_TARE);
//	    display_clear(DISPLAY_PRICE);
//	    display_clear(DISPLAY_WEIGHT);
//	    display_clear(DISPLAY_UNITPRICE);
//#pragma	asm
//		reset;
//#pragma endasm
	}
}

void restore_factory_default(INT16U param)
{
	INT32U set_val;
	INT32U sorc_pos;
	INT16U country;
	INT8U result,str[16];
	CAPTION_STRUCT cap;

	Menu_Init();
	set_val=0;

	caption_split_by_code(0x8600, &cap,0);// Parameter
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x4998, &cap,0);
#if defined(USE_SINGLE_LINE_GRAPHIC)
	sprintf(cap.form, "FN %03d", param);
#endif
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
			cap.input_max, cap.input_min, 0, &set_val, NULL, NULL, NULL);
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
	    if (set_val) {
#ifndef USE_CHINA_DEFAULT
		sorc_pos = NVRAM_DEFAULT_SETTING;
		//sorc_pos+= NVRAM_BASE;	// Deleted by CJK 20060510
		get_nvram_sparam(sorc_pos,str,4);
		str[4]=0;
		if ((str[0]==0) || (str[0]==0xff)) {
			country = get_global_wparam(GLOBAL_AREA_VENDOR_COUNTRY);
			if (country>999) country=0;
			global_set_country(country, OFF);
		} else {
			parameter_restore();
			KEY_CopyKeyTable(-1);
		}
#else
		init_factory_nvram(CLEAR_CAL);
		plu_init(0);
		discount_delete_all();
		plu_all_delete(NULL,0);
		CalRun_ClearTable();
		CalRun_TableDefault();
		global_set_country(COUNTRY_CN, ON);
#endif
		_SOFTWARE_RESET;		
	   }
	}
}

void FirmwareTypeCheck(void)
{
	FW_VERSION fversion;
	char data[32];

	version_get(&fversion);
	if( fversion.country[2] != FIRMWARE_TYPE_NUMBER )
	{
		display_alloff_primary_indication();
		sprintf(data,"CHECK");
		display_string(0,(INT8U *)data);
		sprintf(data,"TYPE ");
		display_string(2,(INT8U *)data);
		VFD7_Diffuse();
		while(1);
	}
}

#define DELETE_BLOCK_SIZE	0x4000	//8192l
//#define DELETE_BLOCK_SIZE 	DFLASH_BLOCK_SIZE

void init_flash_check(void)
{
	INT32U addr;
	INT32U flash_size;
	INT16S    i;
	char   data[32],run_init;
	INT8U  cc,oc;
	INT32U nblock,nrem;
	FW_VERSION fversion;
#ifdef USE_GSMART_TRACE_DEFAULT
	INT8U remote_ip[4] = {10, 56, 27, 1};
#endif
#ifdef USE_EXT_SETTING_MEM
	POINT disp_p;
#endif
	INT8U buffer[HISTORY_BUF_SIZE];
	INT8U updateLog;

	addr=DFLASH_BASE;
	oc  =Flash_bread(addr);
	run_init=0;
	///////
//	VFD7_Init();
//	LCD_Init();
//	Dsp_InitHardware();
	display_init();
	KEY_Init();
#ifdef USE_EXT_SETTING_MEM
	///////
	if (Data_Save_Restore(2)) { // if it is exist EXT Memory
//DEL080226		VFD7_Init();
//DEL080226		LCD_Init();
//		Dsp_InitHardware();
//		display_init();
		display_alloff_primary_indication();
		VFD7_Diffuse();
		for (i=0; i<10; i++) {
			cc='0'+i;
			display_use_allon_digit(cc);
			init_lcdchess();
			Delay_100ms(3);
		}
		display_alloff_primary_indication();
		VFD7_Diffuse();
		display_temp_setting();
		if (Data_Save_Restore(3)) {	// then already set parameter
//DEL080226			KEY_Init();
//DEL080226			Dsp_default_fontload();
			Dsp_Fill_Buffer(0);
			DspLoadFont1(DSP_SYSTEM_FONT_ID);	
			sprintf(data,"Restore (Y/N) ?");
			disp_p = point(0,0);
			Dsp_Write_String(disp_p,(HUGEDATA char *)data);
			Dsp_Diffuse();
//DEL080226			KEY_Init();
			while (1) {
				while (!KEY_Read());
				switch (KeyDrv.CnvCode) {
					case KEY_NUM_1: 
					case KP_ASC_Y : BuzOn(1); goto NEXT;
					case KEY_NUM_0: 
					case KP_ASC_N : BuzOn(1); return;
					default : BuzOn(2);
				}
			}
		NEXT:
			Dsp_Fill_Buffer(0);
			Dsp_Diffuse();
		}
		BuzOn(1);
		// ----------ADM Check
		cc=ADM_Init();
		if (cc==ADM_CHK_ERR_INIT) {
			CalRun_InitADM(2, OFF);// Modified by CJK 20050816
		}
		display_alloff_primary_indication();
		VFD7_Diffuse();
		Data_Save_Restore(1);
		display_alloff_primary_indication();
		VFD7_Diffuse();
		sprintf(data,"END");
		display_string(0,(INT8U *)data);
		sprintf(data,"WAIT");
		display_string(1,(INT8U *)data);
		sprintf(data,"RESET");
		display_string(2,(INT8U *)data);
		VFD7_Diffuse();
		ADM_RestoreEeprom();
		BuzOnAdd(1);
		while (1) {
			cc=KEY_Read();
		}
	}
#endif
	Flash_sread(addr+FLASH_STR_VERSION,Startup.version,2);
	sprintf(data,"%c%c",0xff,0xff);
	if (memcmp(Startup.version,data,2)!=0) {	// Added by CJK 20051114 : memory가 초기화 되었을 때 PCB체크가 불가능하던것 수정
		sprintf(data,"%s",FLASH_STRUCT_VERSION);
		if (memcmp(Startup.version,data,2)!=0) {	// 초기화는 되어있고, STR Version이 틀릴경우
			run_init=1;
			goto INIT;
		}
	}
	for (i=0; i<20; i++) {
		cc=Flash_bread(addr++);
		if (cc!=oc) {
			goto END;
		}
	}
#ifdef USE_INIT_MASK
	init_mask(INIT_MASK_BACKUP);
#endif
	if (CAL) {
		addr =DFLASH_BASE;
		addr+=FLASH_BASIC_PARAMETER;
		Flash_clear(addr,0x00,FLASH_NET_AREA-FLASH_BASIC_PARAMETER);
	}
#ifdef USE_INIT_MASK
	init_mask(INIT_MASK_RESTORE);
#endif
INIT:
	display_temp_setting();
	DspLoadFont1(DSP_SYSTEM_FONT_ID);

	Uart0Init(BAUD_RATE_115200);
	CommBufInit(&CommBuf,COMM_TYPE_HOST,0);////HYP 20041222

	if (run_init) goto NEXT2;
	if (!CAL) {
//DIAG:
#ifndef ETH_JIG_PGM4
		debug_Factory();
		return;
#endif
	}
NEXT2:
	//addr=DFLASH_BASE;
	//flash_size = FlashCheckMem();
	Dsp_Fill_Buffer(0);
	Dsp_Diffuse();
	display_temp_setting();
	version_get(&fversion);
	display_alloff_primary_indication();
	VFD7_Diffuse();
#ifdef USE_DSP_MESSAGE_FOR_MEM_CLEAR
	DspLoadFont1(3);
	sprintf(data, "Keep turn on");
	Dsp_Write_String(point(0, 0),(HUGEDATA char*)data);
	Dsp_Diffuse();				
#endif

#ifdef USE_INIT_MASK
	init_mask(INIT_MASK_BACKUP);
#endif
	sprintf(data,"NVRAM");
	display_string(0,(INT8U *)data);
	sprintf(data,"CLEAR ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	nvram_clear();
	Nvram_init_nvbuffer(); // nvram cache
	Flash_init_nvbuffer();
	
	sprintf(data,"FLASH");
	display_string(0,(INT8U *)data);
	sprintf(data,"CLEAR  ");
	display_string(1,(INT8U *)data);
	history_flash2ram(buffer);
	addr=DFLASH_BASE;
	//flash_size = FlashMemSizeByte(0);	// 0:all
	flash_size = FLASH_CLERK_AREA;
//	flash_size = FLASH_NET_AREA;
	nblock = flash_size/DELETE_BLOCK_SIZE;
	nrem   = flash_size%DELETE_BLOCK_SIZE;
	for (i=0; i<nblock; i++) {
		Flash_clear(addr,0,DELETE_BLOCK_SIZE);
		sprintf(data,"B %04lX",nblock-i);
		display_string(2,(INT8U *)data);
		VFD7_Diffuse();
		addr+=nblock;
	}
	if (nrem) {
		//sprintf(data,"B %04X",i);
		//display_string(2,(INT8U *)data);
		//VFD7_Diffuse();
		Flash_clear(addr,0,nrem);
	}
	history_ram2flash(buffer);
	Flash_flush_nvbuffer();
// Move down : CJK080227
// Write Flash Structure Version
//	strcpy(data,FLASH_STRUCT_VERSION);
//	addr=DFLASH_BASE;
//	Flash_swrite(addr+FLASH_STR_VERSION,(HUGEDATA INT8U *)data,2);
//////////////////////////////////
//DEL CJK070319	set_global_bparam(GLOBAL_ERROR_TIMEOUT,12);
	set_global_wparam(GLOBAL_START_FLAG   ,0);
	set_global_wparam(GLOBAL_AREA_VENDOR_COUNTRY,COUNTRY_UN);
	global_set_number();
	sprintf(data,"       ");
	display_string(1,(INT8U *)data);

//	// DEL CJK080222 : PLU Clear와 중복		
	sprintf(data,"INIT ");
	display_string(0,(INT8U *)data);
//	sprintf(data,"DISCNT");
//	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
//	discount_delete_all();
	discount_delete_global();

	sprintf(data,"REPORT");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	init_factory_nvram(CLEAR_REPORT);
//	Report_InitResetTime();
//	ClerkClearTransaction();
//	ClearSalesSummaries(99);

	sprintf(data,"FLASH");
	display_string(0,(INT8U *)data);
	sprintf(data,"DEFLT ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	// default
#ifdef USE_TRACE_MEATPART
#ifdef USE_TRACE_MEATPART_DEFAULT
	trace_meatpart_default_KR();
#endif
#endif
#ifdef USE_TRACE_MEATGRADE
#if (defined(USE_TRACE_MEATGRADE_EMART_DEFAULT) || defined(USE_TRACE_MEATGRADE_STANDARD_DEFAULT))
	trace_meatgrade_default();
#endif
#endif
#ifdef USE_LOT_NUMBER_FUNCTION
	hungary_lotnumber_default();
#endif



	//sprintf(data,"FLASH");
	//display_string(0,(INT8U *)data);    
	init_network();
	set_clerk_init();
	set_department_init();

	sprintf(data,"KEY--%c",fversion.keytype);
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	
	cc=0;
	switch (fversion.keytype) {
		case 'S': cc=1; break;
		case 'P': cc=2; break;
		case 'H': cc=3; break;
		case 'D': cc=5; break;
	}
	set_global_bparam(GLOBAL_AREA_SCALETYPE,cc);
	KEY_CopyKeyTable(COUNTRY_UN);

	sprintf(data,"PRINT ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	PrtFlashSetDefault();

	printer_modeset_default();	// CJK070206
	printer_modeset(0);
///
///	sprintf(data,"PLU---");
///	display_string(2,(INT8U *)data);
///	VFD7_Diffuse();
///
///	sprintf(data,"LABEL ");
///	display_string(2,(INT8U *)data);
///	VFD7_Diffuse();
//	plu_record_size=218;

	sprintf(data,"PLUSET ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	plu_struct_set_base();
	get_max_pluno(); // MAX_PLU_NO = 3000 | 10000

	sprintf(data,"PLUINFO");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	plu_n_field     = plu_struct_get_nfield();
	plu_record_size = plu_get_recordsize();
	if (plu_record_size==0) {
		while (1) DebugTerminal(&CommBuf);
	}
//	plu_ptype_enable_code();
	global_set_pluinhibit(VERIFY_FUNC,0);	 //se-hyung 070802
	plu_ptype_reset();

	sprintf(data,"PLUCLR ");
	display_string(1,(INT8U *)data);
	VFD7_Diffuse();
	plu_all_delete(NULL,9);
	sprintf(data,"       ");
	display_string(1,(INT8U *)data);
	VFD7_Diffuse();

	sprintf(data,"PLUINI ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	plu_init(0);

	sprintf(data,"P-INH  ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	print_inhibit_init();

	sprintf(data,"PARAM  ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
#ifdef USE_CHINA_DEFAULT
	global_set_country(7, ON);
#else
	global_set_country(0, OFF);
#endif

// Write Flash Structure Version
	strcpy(data,FLASH_STRUCT_VERSION);
	addr=DFLASH_BASE;
	Flash_swrite(addr+FLASH_STR_VERSION,(HUGEDATA INT8U *)data,2);

//////////////////////////////////

	sprintf(data,"-END-  ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
END:
	
/*************************************************************************************************
  F/W Updated Version에 따른 부분적 초기화
	<F/W Updated Version V17 이전 버전의 저장위치 오류 문제점>
	  F/W Updated Version의 저장 주소인 FLASH_FW_UPDATE_VER 값을
	  Flash_bread(), Flash_bwrite() 함수를 사용해서 엑세스 해야하나
	  get_global_bparam(), set_global_bparam() 함수를 사용함으로 인해
	  원하지 않는 위치(FLASH_GLOBAL_AREA+FLASH_FW_UPDATE_VER)에 엑세스 함
	<해결책>
	  다행히 FLASH_GLOBAL_AREA+FLASH_FW_UPDATE_VER 주소는 현재 사용하지 않으므로
	  저울 기능에는 이상이 없음.
	  V17 버전 부터는 원래의 위치 FLASH_FW_UPDATE_VER에 엑세스하도록 하고,
	  FLASH_GLOBAL_AREA+FLASH_FW_UPDATE_VER 주소는 값을 0으로 만들어 
	  V17 이후부터는 사용할 수 있도록 함
*************************************************************************************************/

	addr = DFLASH_BASE;
	addr += FLASH_FW_UPDATE_VER;
	updateLog = Flash_bread(addr);// old version

/*************************************************************************************************
  F/W Updated Version 저장위치 오류 수정 코드
	updateLog가 0인 경우 
	 case (1) flash 가 clear 된 경우 => 전체 초기화(V10~)를 수행
	 case (2) 현재 버전이 V10~V16인 경우 
			   => 1) 기존 참조했던 주소의 Version 읽음
			      2) 현재 F/W Updated Version이 V10~V16일 경우 해당 버전 이후에 해당하는 초기화 실행
			      3) 향후 FLASH_FW_PDATE_VER(81번지)에 저장
					   ex) Flash_bwrite(FLASH_FW_UPDATE_VER, UPDATE_VERSION);
*************************************************************************************************/
	if(updateLog == 0)
	{
		INT8U updateLog_old;
		updateLog_old = get_global_bparam(FLASH_FW_UPDATE_VER);// old version
		if((updateLog_old >= 10) && (updateLog_old <= 16)) // 버전을 잘못된 위치에 저장했던 버전들
			updateLog = updateLog_old; // 기존 버전을 그대로 사용함
	}
/*************************************************************************************************
  F/W Updated Version 저장위치 오류 수정 코드 끝.
*************************************************************************************************/

	if((updateLog < 10) || (updateLog == 0xff))
	{
		//set_global_bparam(FLASH_FW_UPDATE_VER, 10);
		updateLog = 10;
	}
	//SetDefaultByVersion();
	if (updateLog < UPDATE_VERSION)
	{
		
		switch (updateLog)
		{
			default:
			case 10:// start
				CodeDefaultParameterByCode(750);
			case 11:
#ifdef USE_EMART_BACK_LABEL
				CodeDefaultParameterByCode(718);
#endif
			case 12: 
				CodeDefaultParameterByCode(782);
			case 13:
			case 14: // CL7200 사용
			case 15: // CL7200 사용
			case 16:
				// (FLASH_GLOBAL_AREA+81) 주소는 V17버전(V16 미포함)부터는 다른용도로 사용 가능
				set_global_bparam(FLASH_FW_UPDATE_VER, 0); // 초기화
			case 17: // CL7200 사용
			case 18:
				CodeDefaultParameterByCode(746);
			case 19:
#ifdef USE_FUNCTION_KEY_BLOCK
				set_global_bparam(GLOBAL_FUNCTION_KEY_BLOCK,0);
				set_global_bparam(GLOBAL_FUNCTION_KEY_BLOCK+1,0);
				set_global_bparam(GLOBAL_FUNCTION_KEY_BLOCK+2,0);
#endif
			case 20:
				history_qty_reduction();
			case 21:
				CodeDefaultParameterByCode(729);
			case 22:
				set_global_bparam(GLOBAL_DISPLAY_BRIGHT, 5); // 초기화
			case 23:
				CodeDefaultParameterByCode(795);
			case 24:
				break;

		}
		Flash_bwrite(addr, UPDATE_VERSION);
		// 설정값을 다시읽기 위해 리부팅함.
//		Flash_flush_nvbuffer();

#ifdef USE_MORNITORING_CNT
	rtc_load_operated_time();
	PrtLoadTphUseData();
#endif 

#ifdef USE_INIT_MASK
		init_mask(INIT_MASK_RESTORE);
#endif
		_SOFTWARE_RESET;
	}
}


 #define MODELNAME_FONT_ID	DSP_PLU_FONT_ID
 #define MODELNAME_FONT_SZ	1
 #define VERSION_FONT_ID	DSP_SYSTEM_FONT_ID
 #define VERSION_FONT_SZ	1

#ifdef USE_CHN_CERTIFICATION
 #define CHECKSUM_FW_START	0
 #define CHECKSUM_FW_END	0x200000
#endif

#ifdef USE_PIC32MZ_EFG
extern INT8U UseDspReverse;
#endif
#ifdef USE_WHITESCREEEN_DEBUG
extern INT8U UseBootCountModeForDebug;
#endif

void init_displayLogo(INT16S m, INT8U dispFlag)	// Added by CJK
{
	char string_buf[64];
	char country_str[10];
	char dealer_str[10];
	char subversion[3];
	char cts_str[3];
#ifndef USE_SINGLE_LINE_GRAPHIC
	char type_str[10];
#endif
	POINT p;
	SCROLL_MESSAGE_STRUCT scroll;
	INT16S x,l;
	INT8U i;
	FW_VERSION fversion;
	FW_VERSION2 fversion2;
#ifdef USE_CHN_CERTIFICATION
	ROMDATA INT8U *cp;
	static INT16U fw_checksum=0;
	INT32U k,addr;
	INT16U flash_checksum = 0;
#endif
#ifdef USE_WHITESCREEEN_DEBUG
	INT32U bootCntForWhiteScreen;
#endif

	version_get(&fversion);
	version2_get(&fversion2);

	//Dsp_Fill_Buffer(0);
	DspLoadFont1(MODELNAME_FONT_ID);
	Dsp_FontSize(MODELNAME_FONT_SZ, MODELNAME_FONT_SZ);
	
	memcpy(dealer_str, fversion2.dealer, 3);
	dealer_str[3] = 0;
	str_trim(dealer_str);
	memcpy(country_str, fversion2.country, 2);
	country_str[2] = 0;
	str_trim(country_str); 
#ifndef USE_SINGLE_LINE_GRAPHIC // CL5200J 은 버전만 표시
	type_str[0] = 0;
	switch (get_global_bparam(GLOBAL_AREA_SCALETYPE))
	{
		case 1:	// B-Type
		case 6: //CL5200-B Type, CL5200J-B Type
			strcpy(type_str, "-B");
			break;
		case 2:	// P-Type, R-Type
			strcpy(type_str, "");
			break;
		case 3:	// H-Type
			strcpy(type_str, "-H");
			break;
		case 4:	// S-Type
			strcpy(type_str, "-S");
			break;
		case 5:	// D-Type
			strcpy(type_str, "-D");
			break;
		default:
			break;
	}
#endif
#if defined (USE_RUSSIA_VER_DISPLAY)|| defined (USE_BELARUS_VER_DISPLAY)
	sprintf(string_buf, "%s", "V2.89.5(R)"); // 인증시 표시버전 결정
#else	//#ifdef USE_RUSSIA_VER_DISPLAY
	i = 0;
	if(fversion.subversion[0] != '0') subversion[i++] = fversion.subversion[0];
	subversion[i++] = fversion.subversion[1];
	subversion[i] = 0;
	
#ifdef USE_CHN_CERTIFICATION
	cp = (ROMDATA INT8U*)CHECKSUM_FW_START;
	fw_checksum = 0;
	for(k = CHECKSUM_FW_START; k < CHECKSUM_FW_END; k++) {
		fw_checksum += *cp++;
	}

	addr = DFLASH_BASE;
	addr += FLASH_FW_CHECKSUM;
	flash_checksum = Flash_wread(addr);
	if(flash_checksum == 0xFFFF)
	{
		Flash_wwrite(addr, fw_checksum);
		flash_checksum = fw_checksum;
	}
	if(flash_checksum != fw_checksum)
	{
		subversion[0] = 'F';
	}
#endif
	
	cts_str[0] = 0;
#ifdef USE_CONTINUOUS_PRINT_DELAY_1500ms
	strcpy(cts_str, "P");
#endif
#if defined(USE_CTS_FUNCTION)
	strcpy(cts_str, "-C");
#endif
#ifdef USE_VERSTR_CERTI	// Brexit 이전 인증 버전에는 버전 앞에 country str이 없어야함
	sprintf(string_buf, "V%c.%c%c.%s", fversion.version,fversion.reversion[0],fversion.reversion[1],subversion);
#elif defined (USE_CONTINUOUS_PRINT_DELAY_1500ms)
	if (country_str[0])
		sprintf(string_buf, "%s%c.%c%c.%s%s%s", country_str, fversion.version,fversion.reversion[0],fversion.reversion[1],subversion,cts_str,dealer_str);
	else
		sprintf(string_buf, "V%c.%c%c.%s%s%s", fversion.version,fversion.reversion[0],fversion.reversion[1],subversion,cts_str,dealer_str);
#elif defined (USE_KOREA_VERSION_DISPLAY)
  #ifdef USE_CTS_FUNCTION
  	if (country_str[0])
		sprintf(string_buf, "%s%c.%c%c.%s%s%s", country_str, fversion.version,fversion.reversion[0],fversion.reversion[1],subversion,cts_str,dealer_str);	//"-C"
	else
		sprintf(string_buf, "V%c.%c%c.%s%s%s", fversion.version,fversion.reversion[0],fversion.reversion[1],subversion,cts_str,dealer_str);	//"-C"
  #else
  	if (country_str[0])
		sprintf(string_buf, "%s%c.%c%c.%s%s", country_str, fversion.version,fversion.reversion[0],fversion.reversion[1],subversion,dealer_str);
	else
		sprintf(string_buf, "V%c.%c%c.%s%s", fversion.version,fversion.reversion[0],fversion.reversion[1],subversion,dealer_str);
  #endif
#else
	if (country_str[0])
		sprintf(string_buf, "%s%c.%c%c.%s%s", country_str, fversion.version,fversion.reversion[0],fversion.reversion[1],subversion,dealer_str);
	else
		sprintf(string_buf, "V%c.%c%c.%s%s", fversion.version,fversion.reversion[0],fversion.reversion[1],subversion,dealer_str);
#endif
#endif	//#ifdef USE_RUSSIA_VER_DISPLAY

#ifdef USE_CHN_CERTIFICATION
	l=strlen(string_buf);
	sprintf(string_buf+l, "(%04X)", fw_checksum);
#endif
	if (fversion.testversion)
	{
		strcat(string_buf, " Beta");
	}
	
#ifdef USE_WHITESCREEEN_DEBUG
	if (UseBootCountModeForDebug)
	{
		bootCntForWhiteScreen = get_nvram_wparam(NVRAM_BOOT_COUNT_FOR_DEBUG);
		sprintf(string_buf, "boot%8d", bootCntForWhiteScreen + 1); // booting후 아직 증가되기 전 이므로 +1 하여 표시함. sale mode 표시와 일치하기 위함
	}
#endif
	DspLoadFont1(VERSION_FONT_ID);
	Dsp_FontSize(VERSION_FONT_SZ, VERSION_FONT_SZ);
	l=strlen(string_buf);
	x=(LCD_Y_MARGIN-display_font_get_width()*l)/2;
	p = point(0, x);
#ifdef USE_PIC32MZ_EFG
#ifdef CL3000_PROJECT
	UseDspReverse = ON;
	DspStruct.Reverse = 1;
#endif
	Dsp_Write_String(p,string_buf);	// display version
#ifdef CL3000_PROJECT
	DspStruct.Reverse = 0;
	UseDspReverse = OFF;
#endif
#else
	Dsp_Write_String(p,string_buf);	// display version
#endif
	
//	sprintf(MsgBuf, "FTL_ERR_COUNT = %d Avg Erase Cnt = %d\r\n",FtlErrManagement.FtlTableErrCnt, FtlErrManagement.AvgLifeTime);
//	MsgOut(MsgBuf);// TEST용도이므로 꼭 제거해야함.
	if(dispFlag) Dsp_Diffuse();
}

void init_adm_error(INT8U err)
{
	char data[10];
	display_temp_setting();

	display_alloff_primary_indication();
	sprintf(data,"ADM");
	display_string(0,(INT8U *)data);
	sprintf(data,"NOT IN");
	display_string(1,(INT8U *)data);
	sprintf(data,"CODE%02X",err);
	display_string(2,(INT8U *)data);
	//init_lcdchess();
	VFD7_Diffuse();
	CheckUsbDataRomUpdate = OFF; // adm error시 셋팅롬 업데이트 안되게 함
	while (1) KEY_Read();
}

extern void Check_Peel_Head_Sensor(void);
#ifndef CPU_PIC32MZ
extern INT32U   sendto(SOCKET s, HUGEDATA INT8U * buf, INT32U len, HUGEDATA INT8U * addr, INT16U port);
#endif
INT16U SysBootTime;

INT8U IsCalState(void)
{
//	INT8U calComm;
	INT8U calPort;
	calPort = CAL_PORT;
	if( !calPort ) return 1;
#if 0   //parameter로 설정할지 체크
	calComm = ADM_WaitDataCnt(ADM_CMD_GET_PARAMETER, 120000, ADM_PARAM_ON, 1);
	if( !calComm ) return 1;
#endif	
	return 0;
}

/**
********************************************************************************
* @brief	Main CPU Module VER CHK1(RK2) 포트의 Pull-up, douwn 저항을 체크한다.
*			플로팅(Floating)현상 방지를 위해 GND 연결시 참으로 판단.
*	 	    Pull-up : 신규 DISP IC(RW1087)와 매칭되는 CPU Module
*		    Pull-down : 기존 DISP IC(ST7522)와 매칭되는 CPU Module
* @return	반환값 또는 에러값 설명
*			1 : 신규 DISP IC
*			0 : 기존 DISP IC
* @remark	각 IC Instruction Table이 다르므로 해당 변수(disp_ver_chk_port)값 유의해서 연결할 것
********************************************************************************
*/
INT8U Is_Disp_Ic_Ver_Chk1_State(void)
{
	INT8U disp_ver_chk1_port;

	disp_ver_chk1_port = DISP_IC_VER_CHK1_PORT;
	if (!disp_ver_chk1_port)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#ifdef USE_CTS_FUNCTION
void CtsAlarm(INT8U key_mode)
{
	CAPTION_STRUCT cap;
	INT8U old_page, old_font, old_mode;
	INT16U x, y;
	INT16U len;
	POINT disp_p1, disp_p2;
	char string_buf1[CAP_FORM_LENGTH];
	char string_buf2[CAP_FORM_LENGTH];
	INT8U state;
	
	old_page = DspStruct.Page;
	old_font = DspStruct.Id1;
	old_mode = UseSaleModeEditFlag;
	UseSaleModeEditFlag = 1;
	
	Dsp_SetPage(DSP_ERROR_PAGE);
	DspLoadFont1(DSP_MENU_FONT_ID);

	state = 0;
	while (state <=15)
	{
		switch (state)
		{
			case 0:	// CTS 메시지 준비 
				//sprintf((char*)string_buf1, "정기점검필요"); // 차후 캡션으로 변경 요망
				//len = strlen((char*)string_buf1);
                caption_split_by_code(0x97B1, &cap,0);
				len = strlen((char*)cap.form);
				if (len > CAP_FORM_LENGTH) {
					len = CAP_FORM_LENGTH;
					cap.form[CAP_FORM_LENGTH-1] = 0;
				}
				strcpy(string_buf1, cap.form);
#ifdef USE_KOREA_CL5200
				x= (LCD_Y_MARGIN-display_font_get_width()*len)/2;
				disp_p1 = point(0, x);
#else
				disp_p1 = point(0, 0);
#endif
//				sprintf((char*)string_buf2, "CAll");
//				sprintf((char*)string_buf3, "1577.5578");			// 차후 캡션으로 변경 요망
				get_global_sparam(GLOBAL_CTS_MSG, (INT8U *)cap.form, 22);
				cap.form[22] = 0;
				len = strlen((char*)cap.form);
				if (len > CAP_FORM_LENGTH) {
					len = CAP_FORM_LENGTH;
					cap.form[CAP_FORM_LENGTH-1] = 0;
				}
				strcpy(string_buf2, cap.form);
				Dsp_Fill_Buffer(0);
				state++;
				break;
			case 1:// 0.6초 동안 메시지 표시, 키 입력 X
			case 3:
			case 6:
#ifdef USE_KOREA_CL5200
				Dsp_Write_String(disp_p1, (char*)string_buf1);
				//display_string(DISPLAY_UNITPRICE, (INT8U*)string_buf2);
				display_string_pos(DISPLAY_START_POS_PRICE, (INT8U*)string_buf2);
#else
				Dsp_Write_String(disp_p1, (char*)string_buf2);
				//display_string(DISPLAY_UNITPRICE, (INT8U*)string_buf2);
				display_string_pos(DISPLAY_START_POS_WEIGHT, (INT8U*)string_buf1);
#endif
				Dsp_Diffuse();
				VFD7_Diffuse();
				Delay_100ms_w_net(6, OFF, ON);
				state++;
				break;
			case 2:// 0.4초 동안 메시지X, 부저 2회 동작, 키 입력 X
			case 4:
			case 7:
				display_lcd_clear();
				display_alloff_primary_indication();
				Dsp_Diffuse();
				VFD7_Diffuse();
				BuzOnAdd(2);
				Delay_100ms_w_net(4, OFF, ON);
				state++;
				break;
			case 5:// 2초 동안 메시지 표시, 키 입력 X
#ifdef USE_KOREA_CL5200
				Dsp_Write_String(disp_p1, (char*)string_buf1);
				//display_string(DISPLAY_UNITPRICE, (INT8U*)string_buf2);
				display_string_pos(DISPLAY_START_POS_PRICE, (INT8U*)string_buf2);
#else
				Dsp_Write_String(disp_p1, (char*)string_buf2);
				//display_string(DISPLAY_UNITPRICE, (INT8U*)string_buf2);
				display_string_pos(DISPLAY_START_POS_WEIGHT, (INT8U*)string_buf1);
#endif
				Dsp_Diffuse();
				VFD7_Diffuse();
				Delay_100ms_w_net(20, OFF, ON);
				state++;
				break;
			case 8:// 0.6초 동안 메시지 표시, 키 입력 받아 캔슬 가능 
			case 15:
			case 11:
			case 13:
#ifdef USE_KOREA_CL5200
				Dsp_Write_String(disp_p1, (char*)string_buf1);
				//display_string(DISPLAY_UNITPRICE, (INT8U*)string_buf2);
				display_string_pos(DISPLAY_START_POS_PRICE, (INT8U*)string_buf2);
#else
				Dsp_Write_String(disp_p1, (char*)string_buf2);
				//display_string(DISPLAY_UNITPRICE, (INT8U*)string_buf2);
				display_string_pos(DISPLAY_START_POS_WEIGHT, (INT8U*)string_buf1);
#endif
				Dsp_Diffuse();
				VFD7_Diffuse();
				if (Delay_100ms_w_net(6, key_mode, ON)) state = 0xFF;
				else state++;
				break;
			case 9:// 0.4초 동안 메시지 X, 부저 2회 동작, 키 입력 받아 캔슬 가능
			case 12:
			case 14:
				display_lcd_clear();
				display_alloff_primary_indication();
				BuzOnAdd(2);
				if (Delay_100ms_w_net(4, key_mode, ON)) state = 0xFF;
				else state++;
				break;
			case 10:// 2초 동안 메시지 표시, 키 입력받아 캔슬 가능
#ifdef USE_KOREA_CL5200
				Dsp_Write_String(disp_p1, (char*)string_buf1);
				//display_string(DISPLAY_UNITPRICE, (INT8U*)string_buf2);
				display_string_pos(DISPLAY_START_POS_PRICE, (INT8U*)string_buf2);
#else
				Dsp_Write_String(disp_p1, (char*)string_buf2);
				//display_string(DISPLAY_UNITPRICE, (INT8U*)string_buf2);
				display_string_pos(DISPLAY_START_POS_WEIGHT, (INT8U*)string_buf1);
#endif
				Dsp_Diffuse();
				VFD7_Diffuse();
 				if (Delay_100ms_w_net(20, key_mode, ON)) state = 0xFF;
				else state++;
				break;
			default:
				state++;
				break;
		}
	}
	display_lcd_clear();
	display_alloff_primary_indication();
	Dsp_SetPage(old_page);
	DspLoadFont1(old_font);
	UseSaleModeEditFlag = old_mode;
	Dsp_Diffuse();
	VFD7_Diffuse();
	
	sale_display_update(UPDATE_ALL);
}
#endif

extern INT8U FTL_Check_Lifetime(void);
extern void print_memory_LifeTime_Err(INT8U fontid);
void sflash_test(void);
void nvram_test(void);
#define DHCP_CHECKTIME_100MS	300		// 30 sec
void init_main(void)
{
	char string_buf[10];
	char msg_buf[34];
	INT8U i;
	INT8U  start_mode;
	INT8U  adm_err,cc;
	INT8U  useStateDelay;
	INT16U startTime;
	INT8U mac[6];
	INT8U version[17];
	INT8U wtDec;
#ifdef USE_CTS_FUNCTION
	INT32U addr;
	INT16U hid;
	CAL_HISTORY hist;
#endif
#ifdef USE_CHN_IO_BOARD
	INT8U ret;
#endif
#ifdef USE_WHITESCREEEN_DEBUG
	INT32U bootCntForWhiteScreen;
#endif

	CommBufInit(&CommBuf,COMM_TYPE_HOST,0); // initial시 시리얼로 디버깅 정보를 받기 위해 앞으로 이동
	SYS_Initialize(NULL); // PIC32MZ initialize
    
// 	CommBufInit(&CommBuf,COMM_TYPE_HOST,0);
	CommBufInit(&CommBufAD,COMM_TYPE_AD,0);
#ifndef CPU_PIC32MZ
	CommBufInit(&CommBufKey,COMM_TYPE_KEY,0);
	CommBufInit(&CommBufUsb,COMM_TYPE_USB,0);
#endif
	CommBufInit(&CommBufPrt,COMM_TYPE_PRT,0);
	CommBufInit(&CommBufEth,COMM_TYPE_ETH,0);
	for(i=0; i<ETH_BUF_MAX_NUM; i++) {
		CommBufInit(&(CommBufEthData[i]),COMM_TYPE_ETHDATA,i);	 //Ethernet
	}
#ifdef USE_CHN_IO_BOARD
	CommBufInit(&CommBufRFID,COMM_TYPE_RFID,0);
	CommBufInit(&CommBufGPRS,COMM_TYPE_GPRS,0);
	CommBufInit(&CommBufUsbMem,COMM_TYPE_USBMEM,0);
#endif
    
	Uart0Init(BAUD_RATE_115200);	// PC
	Uart1Init(BAUD_RATE_57600); 	// ADM

	WDT_Timer_Enable();
	PrtDrvStruct.PrtState = PRT_STATE0I; // Default PrtState 
	PrtDrvStruct.SysTimerNum10ms = 8; // Default speed
	Buzzer.Status = 0;
	Buzzer.Times = 0;
	Timer0Start();	// Watchdog timer clear를 위한 timer start
	SQI_Flash_Setup();

	Flash_Init_Cache(FLASH_CACHE);
	Flash_Init_Cache(NVRAM_CACHE);
	Flash_Init_Cache(INGREDIENT_CACHE);

	Init_Ftl(); //!!!!!!!!주의!!!!! Flash_Init_Cache를 한 뒤에 실행해야함.
	InitFtlChkFlag = 1;
	MemLifetimeState = FTL_Check_Lifetime();
#ifdef CL5200J_SFLASH_TEST
sflash_test();
#endif
	// Init Variables
	memset((char *)&Startup,0,sizeof(Startup));
	Startup.ethernet_notready=1;
	memset((char *)&mPackDate,0,sizeof(PACKDATE_STRUCT));
	netClerkLockInit();

	// Init Pritner
	SensorInit();
   	PrtInit();
   	PrtDrvInit();
  	PrtDrvStruct.SysTimer = 0;
    PrtDrvStart(PRT_STATE0I);//Initial prtdrv state
	Timer0Start(); // PrtDrvStruct.SysTimerNum10ms 값이 로드(설정) 된 후 Timer0가 시작되어야함

	printer_modeset(9);
	
	SysBootTime = SysTimer100ms;
#ifdef USE_USB_DATA_ROM_UPDATE
	CheckUsbDataRomUpdate = ON;
#endif
	
	Check_Peel_Head_Sensor();
	if (PrtDrvStruct.HangingFormFlag)
	{
		PrtDrvStart(PRT_STATE_RW);//Initial prtdrv state
	}

	FLASH_PLU_AREA_PNT = FLASH_PLU_AREA+DFLASH_BASE;
	FLASH_ING_EXT_AREA_PNT  = PLU_EXTER_AREA_QTY;
	FLASH_ING_EXT_AREA_PNT *= MAX_PLU_RECORDSIZE_DEFAULT;
	FLASH_ING_EXT_AREA_PNT += EXT_MEMCARD_ADDR_X;
	FLASH_ING_NOR_AREA_PNT  = DFLASH_BASE + FLASH_INGREDIENT_AREA;
#ifdef USE_DIRECT_INGREDIENT
	FLASH_INGREDIENT_TEXT_AREA = INGREDIENT_BASIC_AREA_SIZE;
	FLASH_INGREDIENT_TEXT_AREA += FLASH_ING_NOR_AREA_PNT;
	FLASH_INGREDIENT_TEXT_EXT_AREA = INGREDIENT_EXTER_AREA_SIZE;
	FLASH_INGREDIENT_TEXT_EXT_AREA += FLASH_ING_EXT_AREA_PNT;
#endif
#ifdef USE_9999_INDIRECT_INGREDIENT
	FLASH_INGREDIENT_TABLE_EXT_AREA = INGREDIENT_EXTER_AREA_QTY;
	FLASH_INGREDIENT_TABLE_EXT_AREA *= DEFAULT_INGREDIENT_SIZE;
	FLASH_INGREDIENT_TABLE_EXT_AREA += FLASH_ING_EXT_AREA_PNT;
#endif

	FLASH_PLU_STRUCT_PNT  = DFLASH_BASE + FLASH_PLU_STRUCT_AREA + PLU_STRUCT_POINT;
	NVRAM_CLEARK_LASTSALE_PNT = NVRAM_CLEARK_LASTSALE - NVRAM_TRANS_DATA;
	Startup.start_flag = get_global_wparam(GLOBAL_START_FLAG);

	start_mode = Startup.start_flag&0x07;
	useStateDelay = ON;
	//MsgOut("Dsp_InitHardware();\r\n");
	Dsp_InitHardware();
	//MsgOut("init_flash_check();\r\n");
	init_flash_check();
	init_factory_nvram(CLEAR_BOOT);	// Moved by CJK 20060510
#ifdef USE_USB_DATA_ROM_UPDATE
	dataRomStatusLoad();
#endif
	Uart0Baudrate();
//	Uart0Init(BAUD_RATE_115200);	// for Debugging
	CommBufInit(&CommBuf,COMM_TYPE_HOST,0);////HYP 20041222

//	Dsp_InitHardware();

  	BuzOnAdd(1);

	if (CAL) {
		debug_message("+");
		display_allon_primary_indication();
		Dsp_Fill_Buffer(0xff);
		VFD7_Diffuse();
	}
	//LCD_Init();

#if defined(USE_TRACE_STANDALONE)
	//FirmwareTypeCheck(); //SG070614	// 20161018 KSY 기존 국내 독립형 개발 시 독립형에는 독립형 펌웨어만 설치 가능하도록 계발되었으며, 5200J에서는 임시로 막음
#endif
	
#ifdef USE_CTS_FUNCTION
	Flash_sread(DFLASH_BASE + FLASH_CTS_STRUCT, (INT8U*)&CtsStruct, CTS_STRUCT_SIZE);
    Flash_sread(DFLASH_BASE + FLASH_CTS_STRUCT2, (INT8U*)&CtsStruct2, CTS_STRUCT_SIZE2);
	if (CtsStruct.Initialized != CTS_INIT_COMPLETED || CtsStruct.crc16 != crc_16((INT8U*)&CtsStruct, CTS_STRUCT_SIZE - 2))
	{
		CTS_InitCtsData();
	}
	if (CtsStruct2.crc16 != crc_16((INT8U*)&CtsStruct2, CTS_STRUCT_SIZE2 - 2))
	{
		CtsStruct.CtsVersion = 0;
		CtsStruct.crc16 = crc_16((INT8U*)&CtsStruct, CTS_STRUCT_SIZE - 2);
		Flash_swrite(DFLASH_BASE + FLASH_CTS_STRUCT, (INT8U*)&CtsStruct, CTS_STRUCT_SIZE);
	}
    if (CtsStruct.CtsVersion != CTS_VERSION)
	{
		CTS_UpdateCtsData();
	}
    
    hid=get_base_wparam(FLASH_CALLOG_COUNT);
	hid&=0x7FFF;		// 0x8000으로 History qty update 되없음을 판단
	addr =(INT32U)hid;
	addr%=HISTORY_QTY;
	addr*=HISTORY_STRUCT_SIZE;
	addr+=DFLASH_BASE;
	addr+=FLASH_CALLOG_DATA;
	Flash_sread(addr,(HUGEDATA INT8U *)&hist,sizeof(hist));
	CtsEncryptBufStruct.zero = hist.ad_zero;
	CtsEncryptBufStruct.span = hist.ad_span;
	CtsEncryptBufStruct.midup = hist.ad_midup;
	CtsEncryptBufStruct.middn = hist.ad_middn;
	CtsEncryptBufStruct.time = CtsStruct.CtsCalYear * 0x1000000;
	CtsEncryptBufStruct.time += CtsStruct.CtsCalMonth * 0x10000;
	CtsEncryptBufStruct.time += CtsStruct.CtsCalDate * 0x100;
    
#ifdef USE_PERSIAN_CALENDAR
	RTC_Convert_Persian_Read();
#else
	RTC_CLK_Burst_Read();
#endif

#ifdef USE_MORNITORING_CNT
	rtc_load_operated_time();
    rtc_set_pre_minute();
	PrtLoadTphUseData();
#endif

	if (!CTS_CheckCalibrationDate()  && (CAL))
	{
		if ((UsbDataRomUpdate == DATAROMUPDATE_COMPLETE) || (UsbDataRomUpdate == DATAROMUPDATE_READY))
			CtsAlarm(OFF);
	}
#endif

	//MsgOut("KEY_Init();\r\n");
	KEY_Init();
	keyin_Init();
	check_delay_time(0);
	VertScrollMsg_Init(DSP_MENU_FONT_ID);
	Scrl_Msg_UseLogo = ON;
	// 9999999999999999999999999999999999999999999999999999
	if (CAL) {
		display_use_allon_digit('9');
	}
	//MsgOut("9999999999\r\n");
	init_lcdchess();
	debug_message("9");
	serial_init();
#ifdef USE_CHN_IO_BOARD
	UseI2CComm = OFF;
	ret = i2c_readVersionOfBoard(msg_buf);
	if (ret)
	{
		UseI2CComm = ON;
	}
#endif
#ifdef USE_PB_BATTERY
	for (i = 0; i < 10; i++)
	{
		CheckBattery();
		Delay_10ms(2);
	}
#endif
//	rom_status = get_base_bparam(FLASH_ROM_STATUS);
#ifdef USE_ADM_AUTO_RESTORE
	check_ADM_Err_count(); // caption init 발생시 ADM_INT 수행하기 때문에 그 전에 init count 체크
#endif
	if (!caption_init()) {
		ADM_Init();
		caption_error(0);
	}
	if (CAL) {
		if (useStateDelay) check_delay_time(8);
	}
	// 8888888888888888888888888888888888888888888888888888888
	display_use_allon_digit('8');
	//MsgOut("8888888888\r\n");
	debug_message("8");
	if (start_mode==0) {
		init_lcdchess();
	} else {
#if defined(USE_PIC32MZ_EFG) && defined(CL3000_PROJECT)
		Dsp_Fill_Buffer(0xff);
#else
		Dsp_Fill_Buffer(0x00);
#endif
#ifdef USE_WHITESCREEEN_DEBUG
		UseBootCountModeForDebug = get_nvram_bparam(NVRAM_BOOT_COUNT_MODE_FOR_DEBUG);
		if (UseBootCountModeForDebug > 1)
		{
			UseBootCountModeForDebug = 0;
			set_nvram_bparam(NVRAM_BOOT_COUNT_MODE_FOR_DEBUG, UseBootCountModeForDebug);
		}
#endif
		init_displayLogo(1, ON);
	}
	adm_err=ADM_Init();
	if (!adm_err) {	// ADM Init & Check
		caption_get_name(0xfd08,msg_buf); // .Init AD Module OK..
		cc=get_base_bparam(FLASH_ADC_WUNIT);
		wtDec = get_weight_decimal(ADM_Status.Capa, ADM_Status.Dual, cc);
		change_weight_decimal(wtDec);
		save_weight_decimal(wtDec);
	} else {
		caption_get_name(0xfe08,msg_buf); // .Init AD Module Error..
	}
	if (start_mode==1) VertScrollMsg_Add(msg_buf);
	if (CAL) {
		if (useStateDelay) check_delay_time(8);
	}
	boot_log(); // boot count 되기 전에 전원이 꺼지는 현상 방어 위해 함수이동
	
#ifdef USE_WHITESCREEEN_DEBUG
	bootCntForWhiteScreen = get_nvram_wparam(NVRAM_BOOT_COUNT_FOR_DEBUG);
	bootCntForWhiteScreen++;
	set_nvram_wparam(NVRAM_BOOT_COUNT_FOR_DEBUG, bootCntForWhiteScreen);
	FtlBackupEnableFlag = ON; // Power off시 Backup이 되도록 플래그 Set
#endif
	
#ifdef USE_WHITESCREEEN_DEBUG
	if (UseBootCountModeForDebug)
	{
		bootCntForWhiteScreen = get_nvram_wparam(NVRAM_BOOT_COUNT_FOR_DEBUG);
		bootCntForWhiteScreen++;
		set_nvram_wparam(NVRAM_BOOT_COUNT_FOR_DEBUG, bootCntForWhiteScreen);
		FtlBackupEnableFlag = ON; // Power off시 Backup이 되도록 플래그 Set
	}
#endif

	// 777777777777777777777777777777777777777777777777
	if (CAL) {
		display_use_allon_digit('7');
#ifdef USE_PIC32MZ_EFG
		display_string_pos(DISPLAY_START_POS_VERSION, DISPLAY_MODELNAME_FOR_EFG_VERSION);
		VFD7_Diffuse();
#endif
	}
	//MsgOut("7777777777\r\n");
	if (start_mode==0) init_lcdchess();
	if (init_map()) {	// 2.1. Data Initialize
		sprintf(string_buf,"7");
		caption_get_name(0xfd07,msg_buf); // .Check Memory Table OK..
	} else {
		sprintf(string_buf,"G");
		caption_get_name(0xfe07,msg_buf); // .Check Memory Table Error..
	}
	debug_message(string_buf);
	if (start_mode==1) VertScrollMsg_Add(msg_buf);
	network_param_init();
	global_load_parameter(0);	// 2.2. Load Global Parameter

	if (MemLifetimeState) {
		display_alloff_primary_indication();
		ClearDisp7();
		sprintf(msg_buf,"%s","MEMORY LIFETIME ERR");
		display_string_pos(DISPLAY_START_POS_WEIGHT, (INT8U*)msg_buf);
		Dsp_Diffuse();
		if (MemLifetimeState == FTL_MEM_LIFETIME_1_MONTH) {
			print_memory_LifeTime_Err(3);
		}
		BuzOn(4);
		Key_DelaySec(100, 0);
		BuzOn(1);
		display_string_pos_clear(DISPLAY_START_POS_WEIGHT,20);
	}

#ifdef USE_MANAGE_LICENSE
	for (i = 0; i < LIC_FUNCTION_NUMBER; i++)
	{
		Lic.funcEnable[i] = OFF;
	}
	if (lic_check_validation_scaleHWID()) // License 체크를 위한 Scale H/W ID가 설정되어 있는 경우
	{
		INT32U addr;
		INT8U funcCode_E[LIC_FUNCTION_CODE_SIZE + 1];

		for (i = 0; i < LIC_FUNCTION_NUMBER; i++)
		{
			addr = DFLASH_BASE + FLASH_LICENSE + LIC_SCALE_HW_ID_SIZE + i * LIC_FUNCTION_CODE_SIZE;
			Flash_sread(addr, funcCode_E, LIC_FUNCTION_CODE_SIZE);
			Lic.funcEnable[i] = lic_check_function_license(i, funcCode_E); // Floating Clerk
		}
	}
#endif
	Startup.global_com =get_global_bparam(GLOBAL_AREA_COMMUNICATION);
	network_status.service_flag  =get_net_wparam(NETWORK_SERVICE_FLAG);
	network_status.service_bflag2=get_net_bparam(NETWORK_SERVICE_FLAG2);	// 2005.11.05
	network_status.allow_sale=0;
	network_status.remote_plu=0;
	network_status.master_check = 0;
	network_status.time_check = 0;
	network_status.connection = 0;
	if (network_status.service_flag&0x10) network_status.allow_sale=1;
	if (network_status.service_flag&0x40) network_status.remote_plu=1;
	if ((Startup.global_com&0x02) == 0x02) CheckingWirelessLAN = ON; // 무선랜 사용시 모듈체크(MAC 인식작업) 수행
	if (CAL) {
		if (useStateDelay) check_delay_time(8);
	}
	// 6666666666666666666666666666666666666666666666666
	if (CAL) {
		display_use_allon_digit('6');
#ifdef USE_PIC32MZ_EFG
		display_string_pos(DISPLAY_START_POS_VERSION, DISPLAY_MODELNAME_FOR_EFG_VERSION);
		VFD7_Diffuse();
#endif
	}
	//MsgOut("6666666666\r\n");
	if (start_mode==0) init_lcdchess();
	caption_get_name(0xfd06,msg_buf);	// .Load Parameter
	if (start_mode==1) VertScrollMsg_Add(msg_buf);
	debug_message("6");
	if (!CAL) {
		BuzOnAdd(1);
#if defined(USE_CTS_FUNCTION)
		if (CtsStruct.CtsEnable)
		{
			status_run.run_mode = RUN_WAITING_CAL;
		}
		else
		{
			status_run.run_mode = RUN_CALIBRATION;
		}
#else
		status_run.run_mode = RUN_CALIBRATION;
#endif
		EthVariableInit();
		if (Startup.global_com&0x01) {
			if(!EthInit())
			{
				Startup.global_com &= ~0x01;
			}
		}
#if defined(USE_CTS_FUNCTION)
		if (status_run.run_mode == RUN_WAITING_CAL) mode_waiting_cal();
#endif
		mode_calibration();
		//ETHERNET_Reset();
		_SOFTWARE_RESET;
		//goto REPT_CAL;
	}
	if (adm_err) init_adm_error(adm_err);
	if (CAL) {
		if (useStateDelay) check_delay_time(8);
	}

	// 5555555555555555555555555555555555555555555
	display_use_allon_digit('5');
#ifdef USE_PIC32MZ_EFG
    display_string_pos(DISPLAY_START_POS_VERSION, DISPLAY_CPUTYPE_FOR_EFG_VERSION);
	VFD7_Diffuse();
#endif
	//MsgOut("5555555555\r\n");
	if (start_mode==0) init_lcdchess();
	debug_message("5");
	if (useStateDelay) check_delay_time(8);

	// 44444444444444444444444444444444444444
	display_use_allon_digit('4');
#ifdef USE_PIC32MZ_EFG
    display_string_pos(DISPLAY_START_POS_VERSION, DISPLAY_CPUTYPE_FOR_EFG_VERSION);
	VFD7_Diffuse();
#endif
	//MsgOut("4444444444\r\n");
	if (start_mode==0) init_lcdchess();
	status_scale.departmentid   = get_net_wparam(NETWORK_DEPARTMENT_ID);
	status_scale.clerkid        = get_net_wparam(NETWORK_CLERK_ID     );
	status_scale.scaleid        = get_net_wparam(NETWORK_SCALE_ID     );
	status_scale.restrict	    = get_global_wparam(GLOBAL_ACTION_ENABLE);
	EthVariableInit();
	if (Startup.global_com&0x01) {
		Startup.global_com&=0xfe;

		if (EthInit()) {
			caption_get_name(0xfd04,msg_buf); //.Network Check OK
			sprintf(string_buf,"4");
			if (!EthComparefromFlash()) {
				//HYP 20041222
				sprintf(string_buf,"D");
				//caption_get_name(0xfd04,msg_buf); //.Network Check OK
				EthReConfig();
			}
			Startup.global_com|=0x01;
			CommBufInit(&CommBufEth,COMM_TYPE_ETH,0);//HYP 20041222
			for(i=0; i<ETH_BUF_MAX_NUM; i++) {
				CommBufInit(&(CommBufEthData[i]),COMM_TYPE_ETHDATA,i);	 //Ethernet
				EthDataProc(&(CommBufEthData[i]));
			}
//			dtime=2;
		} else {
//			dtime=6;
			sprintf(string_buf,"d");
			caption_get_name(0xfe04,msg_buf); //.Network Check Error
		}
		///////////////////////////////////////////////////////////////////
		Startup.global_com|=0x01;//HYP 20071218 for test new lancard
		
	} else {
//		dtime=8;
		sprintf(string_buf,"4");
		msg_buf[0] = 0;
		//caption_get_name(0xfe14,msg_buf); //.No Network Mode
	}
	
	
	debug_message(string_buf);
	if (start_mode==1) VertScrollMsg_Add(msg_buf);
	if (useStateDelay) check_delay_time(8);

	// 33333333333333333333333333333333333333333333333333333
	display_use_allon_digit('3');
#ifdef USE_PIC32MZ_EFG
	display_string_pos(DISPLAY_START_POS_VERSION, DISPLAY_MODELNAME_FOR_EFG_VERSION);
	VFD7_Diffuse();
#endif
	//MsgOut("3333333333\r\n");
	if (start_mode==0) init_lcdchess();
	if (!KEY_ErrorChk(10000)) {
		caption_get_name(0xfe02,msg_buf); //.Key Pressed or Error
		BuzOn(2);
	} else {
		//caption_get_name(0xfd02,msg_buf); //.Key Check
		msg_buf[0] = 0;
	}
	debug_message("3");
	if (start_mode==1) VertScrollMsg_Add(msg_buf);
	if (useStateDelay) check_delay_time(8);

	// 22222222222222222222222222222222222222222222222222222
	display_use_allon_digit('2');
#ifdef USE_PIC32MZ_EFG
	display_string_pos(DISPLAY_START_POS_VERSION, DISPLAY_MODELNAME_FOR_EFG_VERSION);
	VFD7_Diffuse();
#endif
	//MsgOut("2222222222\r\n");
	if (start_mode==0) init_lcdchess();
	if (plu_init(0)) {
		sprintf(string_buf,"2");
	} else {
		sprintf(string_buf,"B");
		plu_struct_set_base();
		plu_init(0);
	}
	debug_message(string_buf);


	if (useStateDelay) check_delay_time(8);

	// 111111111111111111111111111111111111111111111
	display_use_allon_digit('1');
#ifdef USE_PIC32MZ_EFG
	display_string_pos(DISPLAY_START_POS_VERSION, DISPLAY_CPUTYPE_FOR_EFG_VERSION);
	VFD7_Diffuse();
#endif
	//MsgOut("1111111111\r\n");
	if (start_mode==0) init_lcdchess();
	debug_message("1");

	Startup.wireless_type = WLTYPE_NONE;
	Startup.wireless_model = WLMODEL_NONE;
	if ((Startup.global_com&0x03) == 0x03) 	// Ethernet 사용 & Wireless Bridge 사용
	{
		//wait_cnt = 0;
		//while(wait_cnt++ < 3)
		{
			if (wlan_CheckVer_Bridge(Startup.wireless_type, version, 1))
			{
				if (version[0] == 'T')	// TOBNET
				{
					Startup.wireless_type = WLTYPE_TOBNET;
					Startup.wireless_model = WLMODEL_T_VERSION;
					if (version[1] == 'A') // Hi-Flying
					{
						Startup.wireless_model = WLMODEL_HIFLYING;
					}
				}
				else
				{
					Startup.wireless_type = WLTYPE_LANTRONIX;
					Startup.wireless_model = WLMODEL_LANTRONIX;
				}
				//break;
			}
		} 
		/*			
		//무선 브릿지 사용시 브릿지 인식을 위해 자기 ip로 connection 시도
		ip_val = EthCardStruct.IP[3];	ip_val <<= 8;
		ip_val |= EthCardStruct.IP[2];	ip_val <<= 8;
		ip_val |= EthCardStruct.IP[1];	ip_val <<= 8;
		ip_val |= EthCardStruct.IP[0];
		change_dest_ip_val(ip_val);
		Ethernet_Server(CLIENT_OPEN, 1, CLIENT_NORMAL);
		Ethernet_Server(CLIENT_CLOSE, 0, CLIENT_NORMAL);
		restore_dest_ip();
		*/
	}
	caption_get_name(0xfd01,msg_buf); //.Ready
	if (start_mode==1) VertScrollMsg_Add(msg_buf);
	if (Startup.start_flag&0x60) {
		init_show_lasttime();
	}
	if (!label_defaultload()) {
		debug_message("^");
	}
	if (useStateDelay) check_delay_time(8);
	// 0000000000000000000000000000000000000000000000000
	display_use_allon_digit('0');
#ifdef USE_PIC32MZ_EFG
	display_string_pos(DISPLAY_START_POS_VERSION, DISPLAY_CPUTYPE_FOR_EFG_VERSION);
	VFD7_Diffuse();
#endif
	//MsgOut("0000000000\r\n");
	if (start_mode==0) init_lcdchess();
	debug_message("0");
	if (useStateDelay) check_delay_time(8);
	if (Startup.start_flag&0x80) {
		Dsp_Fill_Buffer(0);
		Dsp_Diffuse();
		display_alloff_primary_indication();
		// system_password();
		VFD7_Diffuse();
		debug_message("Pass");
		while(1) {
//			set_nvram_bparam(NVRAM_LASTCLERKNUMBER,0);
			debug_message("[");
			if (permission_check(PMS_DEALER,0))	break;
			debug_message("]");
		}
	}
	// Deleted by CJK 20060510 : it was moved up.
	//init_factory_nvram(CLEAR_BOOT);
//	boot_log();
	//CheckAdInitStatus();
	network_timeout_init();
	network_search_slave();
	/////////////////////////////////////////////////
	Startup.ethernet_notready=0;
	display_alloff_primary_indication();
	VFD7_Diffuse();

	ReportUseDailyReportBackUp();	//Added by JJANG 20080519, PLU REPORT BACK UP
	scroll_PLU_init();

	if (Startup.global_com&0x01)
	{
		if (EthCardStruct.DHCPEnable && !EthCardStruct.IP[0])
		{
			if (start_mode == 1)
			{
				sprintf(msg_buf, "Check DHCP");
				VertScrollMsg_Add(msg_buf);
			}
			else
			{
				display_alloff_primary_indication();
				sprintf(msg_buf, "Check");
				display_string(0, (INT8U *)msg_buf);
				sprintf(msg_buf, "DHCP ");
				display_string(1, (INT8U *)msg_buf);
				VFD7_Diffuse();
			}
			while (!network_check_wireless_bridge());	// wait for searching wlan bridge until 1 min

			startTime = SysTimer100ms;
			while (!EthCardStruct.IP[0]) 
			{
				TcpIp_Task();
				EthLocalCmdSend(ETH_REQ_IP, 0);
				EthLocalCmdSend(ETH_REQ_GATEWAY, 0);
				EthLocalCmdSend(ETH_REQ_SUBNET_MASK, 0);

				if ((INT16U)(SysTimer100ms - startTime) > DHCP_CHECKTIME_100MS)
				{
					BuzOn(2);
	//				Startup.global_com &= (~0x01);
					if (start_mode == 1)
					{
						sprintf(msg_buf, "Check DHCP Fail");
						VertScrollMsg_Add(msg_buf);
					}
					else
					{
						sprintf(msg_buf, "Fail ");
						display_string(2, (INT8U *)msg_buf);
						VFD7_Diffuse();
					}
					while (1) // add 3 sec for display
					{
						if ((INT16U)(SysTimer100ms - startTime) < DHCP_CHECKTIME_100MS + 30) break;
					}
					break;
				}
			}
			BuzOn(1);
			ethernet_list.localip[0] = EthCardStruct.IP[3];
			ethernet_list.localip[1] = EthCardStruct.IP[2];
			ethernet_list.localip[2] = EthCardStruct.IP[1];
			ethernet_list.localip[3] = EthCardStruct.IP[0];
			ethernet_list.remoteip[0]= EthCardStruct.TCPDestIP[3];
			ethernet_list.remoteip[1]= EthCardStruct.TCPDestIP[2];
			ethernet_list.remoteip[2]= EthCardStruct.TCPDestIP[1];
			ethernet_list.remoteip[3]= EthCardStruct.TCPDestIP[0];
		}
	}

}


#ifdef CL5200J_SFLASH_TEST
extern void SFlash_program_data(INT8U cs, INT32U start_addr, HUGEDATA INT8U *data, INT32U byte_count);
extern void SFlash_sread(INT8U cs, INT32U start_addr,HUGEDATA INT8U *str,INT16U leng);
void sflash_test(void)
{
	INT32U ii, jj;
	INT32U err_cnt;
	INT32U size, offset;
	INT8U b,a;

	size = 1;
	offset = 1;

	sprintf(MsgBuf, "start sflash test\r\n");
	MsgOut(MsgBuf);

	err_cnt = 0;
	jj = 0;
	for (ii = 0; ii < 4*1024*1024; ii += size)
	{
		a = ii+jj;
		SFlash_program_data(FLASH0_CS, ii+offset, (INT8U *)&a, size);
		//Flash_swrite_c2(ii+offset, (INT8U *)&a, size);
		SFlash_program_data(FLASH1_CS, ii+offset, (INT8U *)&a, size);
		if ((ii % 256) == 255) jj++;
	}
	sprintf(MsgBuf, "write complete\r\n");
	MsgOut(MsgBuf);

//	jj = 0;
//	for (ii = 0; ii < 4*1024*1024; ii += size)
//	{
//		a = ii+jj;
//		SFlash_sread(FLASH0_CS, ii+offset, (INT8U *)&b, size);
//		//Flash_sread_c2(ii+offset, (INT8U *)&b, size);
//		if (b != a)
//		{
//			err_cnt++;
//			if (err_cnt < 10)
//			{
//				sprintf(MsgBuf, "[%06x](%X,%X) ", ii+offset, a, b);
//				MsgOut(MsgBuf);
//			}
//		}
//		if ((ii % 256) == 255) jj++;
//	}
//
//	sprintf(MsgBuf, "err_cnt = %d\r\n", err_cnt);
//	MsgOut(MsgBuf);



	err_cnt = 0;

	jj = 0;
	for (ii = 0; ii < 4*1024*1024; ii += size)
	{
		a = ii+jj;
		SFlash_sread(FLASH1_CS, ii+offset, (INT8U *)&b, size);
		if (b != a)
		{
			err_cnt++;
			if (err_cnt < 10)
			{
				sprintf(MsgBuf, "[%06x](%X,%X) ", ii+offset, a, b);
				MsgOut(MsgBuf);
			}
		}
		if ((ii % 256) == 255) jj++;
	}

	sprintf(MsgBuf, "err_cnt = %d\r\n", err_cnt);
	MsgOut(MsgBuf);

	while(1);
}
#endif


#ifdef CL5200J_SFLASH_NVRAM_TEST
void nvram_test(void)
{
	INT32U ii;
	INT32U err_cnt;
	INT32U size, offset, max_kb, add;
	INT8U b,a;
	INT16U prev_time;
	INT8U test_buf[300], rcv_buf[300];
	INT32U i;
	INT16U p;

	size = 24;
	offset = 0;
	max_kb = 1024;
	add = 0;
	
	for (i = 0; i < size; i++)
	{
		test_buf[i] = i;
	}

	sprintf(MsgBuf, "start nvram test\r\n");
	MsgOut(MsgBuf);
	
	

	err_cnt = 0;
	prev_time = SysTimer1_25ms;
	for (ii = 0; ii < max_kb*1024 - offset; ii += size)
	{
		set_nvram_sparam(ii+offset, test_buf, (INT16S)size);
	}
	prev_time = SysTimer1_25ms - prev_time;
	prev_time = (prev_time*1.25);
	sprintf(MsgBuf, "write complete(%d)\r\n", prev_time);
	MsgOut(MsgBuf);



	err_cnt = 0;
	for (ii = 0; ii < max_kb*1024 - offset; ii += size)
	{
		get_nvram_sparam(ii+offset, rcv_buf, size);
		//if (b != a)
		//if (ii+offset > 0xffd)
		if ((p=memcmp(test_buf, rcv_buf, size)) != 0)
		{
			err_cnt++;
			if (err_cnt < 10)
			{
				//sprintf(MsgBuf, "[%06x](%X,%X) ", ii+offset, a, b);
				sprintf(MsgBuf, "[%06x](%X:%X,%X) ", ii+offset, p, test_buf[p], rcv_buf[p]);
				MsgOut(MsgBuf);
			}
		}
	}

	sprintf(MsgBuf, "err_cnt = %d\r\n", err_cnt);
	MsgOut(MsgBuf);

	while(1){}
}
#endif //#ifdef CL5200J_SFLASH_NVRAM_TEST

#ifdef USE_ADM_AUTO_RESTORE
void check_ADM_Err_count(void)
{
	INT8U init_flag;

	init_flag = Flash_bread(DFLASH_BASE + FLASH_ADM_INIT_COMPLETE);
	if(init_flag != 0x73)
	{
		Flash_bwrite(DFLASH_BASE + FLASH_ADM_INIT_ERROR_COUNT, 0);
		Flash_bwrite(DFLASH_BASE + FLASH_ADM_EEP_ERROR_COUNT, 0);
		Flash_bwrite(DFLASH_BASE + FLASH_ADM_INIT_COMPLETE, 0x73);
	}
	return;
}
#endif

#ifdef USE_INIT_MASK
/**
 * @brief		초기화 방어 관련 연결 리스트 더미 노드 얻어오는 함수
 * @param		void	
 * @return 	더미 노드 주소	
 * @remark	
 */
struct INIT_MASK_LINKED_LIST* init_mask_get_dummy_pointer(void)
{
	return &g_init_mask;
}

/**
 * @brief		초기화 방어 관련 연결 리스트 관리를 위한 메타 노드 얻어오는 함수
 * @param		void	
 * @return 	메타 변수 주소	
 * @remark	
 */
struct INIT_MASK_LINKED_LIST_META* init_mask_get_global_meta_pointer(void)
{
	return &g_init_mask_meta;
}
#endif

/**
 * @brief		네트워크 세팅 초기화 위한 함수	
 * @param		void	
 * @return 	void	
 * @remark	
 * 				- 기존 init_flash_check()에서 수행했던 네트워크 초기화 부분 하나의 함수로 구성
 * 				- Menu1914, 1915, 1920(Send Mode) 추가	
 */
void init_network(void)
{
	char data[32] = {0};

	sprintf(data,"NET   ");
	display_string(2,(INT8U *)data);
	VFD7_Diffuse();
	global_net_init();
	memset(data,0,32);
#if defined(USE_GSMART_TRACE_DEFAULT)	// 양산 적용버전인 GS일반버전은 네트웍 디폴트 미적용(생산라인 테스트 고려)
	strcpy(data,"GS11111");
	set_net_bparam(NETWORK_WLAN_USEWEP,3);
#elif defined(USE_LOTTEMART_DEFAULT)
	strcpy(data,"SLMART");
	set_net_bparam(NETWORK_WLAN_USEWEP,5);
#else
	strcpy(data,"CAS_WEP");
	set_net_bparam(NETWORK_WLAN_USEWEP,0);
#endif
	set_net_sparam(NETWORK_WLAN_SSID,(INT8U *)data,32);
	set_net_bparam(NETWORK_WLAN_WEPID ,1);

#if defined(USE_GSMART_TRACE_DEFAULT)	// 양산 적용버전인 GS일반버전은 네트웍 디폴트 미적용(생산라인 테스트 고려)
	memset(data, 0, 32);
	strcpy(data,"123456712345671234567abcde");
	set_net_sparam(NETWORK_WLAN_WEP ,(INT8U *)data, 32);
	set_net_sparam(NETWORK_MACHINE_REMOTE_IP, remote_ip, 4);	//10, 56, 27, 1
#elif defined(USE_LOTTEMART_DEFAULT)
	memset(data, 0, 32);
	strcpy(data,"Ltmt@_%&");
	set_net_sparam(NETWORK_WLAN_WEP ,(INT8U *)data, 32);
#else
	memset(data,0,32);
	strcpy(data,"0000000000000");
	set_net_sparam(NETWORK_WLAN_WEP ,(INT8U *)data,13);
#endif

	// 1914(Remote IP)
	INT8U str[4] = {0};
	set_net_sparam(NETWORK_MACHINE_REMOTE_IP, str, 4);  // remote ip
	set_net_wparam(NETWORK_MACHINE_REMOTE_TCPPORT,0);
	set_net_wparam(NETWORK_MACHINE_REMOTE_UDPPORT,0);

	// 1915(RS232C)
	set_global_lparam(GLOBAL_AREA_RS232C_BAUD, BAUD_RATE_57600);

	// 1920 - Send Mode
	// init_flash_check()에서 FLASH_CLEAR_AREA까지 0으로 지우므로 기본 값이 0으로 설정됨.
	set_net_bparam(NETWORK_SERVICE_FLAG2, 0);	// Modified by CJK 20060519

	return;
}