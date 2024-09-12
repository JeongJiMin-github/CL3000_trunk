/**
********************************************************************************
* Copyright (c) 2015 CAS
* @brief   Network Setting Source File\n
* @file    Network_set.c
* @version 1.1
* @date    2015/03/30
* @author  Young Hoon Lee (LYH)
********************************************************************************
* @remark  [Version History]\n
*          v1.0 2003/12/15 created\n
*          v1.1 2015/03/30 modified by LYH \n
********************************************************************************
*/

/*
********************************************************************************
*                       INCLUDES
********************************************************************************
*/
/* Insert include files here */
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "network_set.h"
#include "caption_app.h"
#include "flash.h"
#include "flash_app.h"
#include "initial.h"
#include "menu.h"
#include "common.h"
#include "prt_font.h"
#include "dsp_common.h"
#include "commbuf.h"
#include "ethernet.h"
#include "main.h"
#include "pgm_app.h"
#include "serial.h"
#include "key.h"
#include "key_typedef.h"
#include "key_app.h"
#include "key_function.h"
#include "network_command.h"
#include "ethernet_comm.h"
#include "plu_edit.h"
#include "mode_main.h"
#include "dsp_app.h"
#include "dsp_vfd7.h"
#include "plu_device.h"
#include "initial_glb.h"
#include "mode_commun.h"
#include "mode_error.h"
#include "wlan_set_lantronix.h"

/*
********************************************************************************
*                       GLOBAL(EXPORTED) VARIABLES & TABLES
********************************************************************************
*/
/* Insert global variables & tables here */
extern  NETWORK_STATUS network_status;
extern char	gmsg[];
extern PLU_ADDR plu_table_addr[];
extern INT16U recv_counter;

#ifdef USE_CHECK_WLAN_STATUS_LANTRONIX
extern void Ethernet_RestoreNetwork(void);
#endif

extern INT16S FontSizeChange(INT16S fontSize);
extern INT16U SysBootTime;
extern INT8U CheckingWirelessLAN;

extern void commun_send_block_addr(HUGEDATA COMM_BUF *CBuf,INT32U startaddr,INT8U flag,INT16U size);
extern INT8U commun_send_plu_from_address(HUGEDATA COMM_BUF *CBuf,INT16U address,INT32U pluno,INT8U xmode);

extern INT16S CheckRxBufCountInt(HUGEDATA COMM_BUF *CBuf);
/*
********************************************************************************
*                       LOCAL DEFINITIONS & MACROS
********************************************************************************
*/
/* Insert #define here */
#define M_ID_SSID	1
#define M_ID_USEWEP	2
#define M_ID_WEPID	3
#define M_ID_WEPKEY	4

#define M_ID_SLAVE_NO	1
#define M_ID_SLAVE_IP	2
#define M_ID_SLAVE_PORT	3
#define M_ID_SLAVE_USE	4

/*
********************************************************************************
*                       LOCAL DATA TYPES & STRUCTURES
********************************************************************************
*/
/* Insert local typedef & structure here */
INT8U global_one2one_send;
INT16U global_one2one_count;

INT8U netset_servicetype_x_pos;
INT8U netset_servicetype_y_pos;

#ifdef USE_CHECK_WLAN_STATUS_LANTRONIX
INT8U wl_networkstatus_RSSI[] = {"RSSI:"};
INT8U wl_networkstatus_SSID[] = {"SSID:"};
INT8U wl_networkstatus_Chan[] = {"Chan:"};
INT8U wl_networkstatus_0_Prompt[] = {"0>"};
#endif

INT8U syncStatusSymPLU[4] = {	' ',	// 0x00 : none
				'P',	// 0x01 : plu memory full
				'U',	// 0x02 : etc. unknown.
				'P'	// 0x02 : plu memory full & etc. unknown.
};
INT8U syncStatusSymDC[4] = {	' ',	// 0x00 : none
				'D',	// 0x01 : d/c memory full
				'U',	// 0x02 : etc. unknown.
				'D'	// 0x03 : d/c memory full & etc. unknown.
};

/*
********************************************************************************
*                       GLOBAL(FILE SCOPE) VARIABLES & TABLES
********************************************************************************
*/
/* Insert file scope variable & tables here */


/*
********************************************************************************
*                       LOCAL FUNCTION PROTOTYPES
********************************************************************************
*/
/* Insert static function prototypes here */ 
static INT8U netset_disnable(void);
static BOOLEAN netset_str2ip(HUGEDATA INT8U *str, HUGEDATA INT8U *ip);
#ifndef CPU_PIC32MZ
static void netset_MAC_set(void);
#endif
static void netset_servicetype_callback(long num);
static void netset_rs232c_baud_callback(long num);
static void netset_usewep_callback(long val);
static void netset_select_wepid_callback(long val);

static void netset_wl_setting_rs232(void);

#ifdef USE_CHECK_WLAN_STATUS_LANTRONIX
static void netset_wl_get_status(void);
static void netset_wl_scan_ssid(void);
static void netset_wirless_module_get_status(void);
#endif

static void callback_security_type(long num);
static INT8U network_block_copy(INT32U from,INT32U to,INT8U plu,HUGEDATA COMM_BUF *pCommBuf);
static void netset_slave_dataUpdate(INT32U num);
static INT32U netset_reset_confirm(INT8U flag);
/*
********************************************************************************
*                       GLOBAL(EXPORTED) FUNCTIONS
********************************************************************************
*/
/* Insert global functions here */

// mode : 1 = 전체 체크(0.0.0.0 허용안함)
//        0 = 0.0.0.0 허용
BOOLEAN netset_ip_valid_check(HUGEDATA INT8U *ip, INT8U mode)
{
	BOOLEAN result;
	result = TRUE;

	if((ip[0] == 0 && mode) || (ip[0] == 127) || (ip[0]>=224)){
		result = FALSE;
	} else {
		switch(ip[0]) {
#ifndef USE_CHINA_DEFAULT			// PARAMETER SETTING
			case 128: 
				if(ip[1] == 0) result = FALSE;	//128.0.x.x
				break;
#endif
			case 191: 
				if(ip[1] == 255) result = FALSE;	// 191.255.x.x
				break;
			case 192: 
				if(ip[1] == 0 && ip[2] == 0) result = FALSE;	// 192.0.0.x	// 192.0.1.x -> 192.0.0.x by CJK
				break;
			case 223: 
				if(ip[1] == 255 && ip[2] == 255) result = FALSE;	// 223.255.255.x
				break;
		}
	}
	return result;
}


void netset_enable_interface(void)
{
	INT32U set_val[4];
	INT8U result;
	CAPTION_STRUCT cap;
	INT8U v8_1;

	v8_1 = get_global_bparam(GLOBAL_AREA_COMMUNICATION);
	set_val[0] = (INT32U)(v8_1&0x01);
	
	Menu_Init();

	caption_split_by_code(0x8410, &cap,0);//"1. Enable Interface"
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x9410, &cap,0);//"Ethernet(TCP/IP) : [ ]"
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		v8_1 &= ~0x01;
		v8_1 |= set_val[0]&0x01;
		set_global_bparam(GLOBAL_AREA_COMMUNICATION,v8_1);
	}
}

void netset_lock(void)
{
	INT32U value;
	CAPTION_STRUCT cap;
	INT8U result;

	Menu_Init();
	caption_split_by_code(0x1930, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	value=0;
	caption_split_by_code(0x1931, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &value, NULL, NULL, NULL);
	result = Menu_InputDisplay();

	if(result == MENU_RET_SAVE) {
		status_scale.scale_lock=0;
		if (value) {
			status_scale.scale_lock=1;
		}
		set_nvram_bparam(NVRAM_SCALE_LOCK,status_scale.scale_lock);
	}
}

void netset_send_speedkey(void)
{
	INT32U value;
	CAPTION_STRUCT cap;
	INT8U result;

	Menu_Init();
	caption_split_by_code(0x1924, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	value=0;
	caption_split_by_code(0x3161, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &value, NULL, NULL, NULL);
	result = Menu_InputDisplay();

	if(result == MENU_RET_SAVE) {
	}
}

void netset_plu_delete(void)	// 0x1940
{
	INT16U ret;
	INT16U  deptno;

	if (Startup.disable_dept) return;
	Dsp_Fill_Buffer(0);						// LCD Buffer clear
	Dsp_Diffuse(); 
	deptno=1;

//PAGE_KI_IN:
	display_title_page_change(0x1148,1,1);

	ret = plu_input_department_only(&deptno, 0);
	if (ret==2)   goto RET_ESC;
	if (deptno<1) goto RET_ESC;
RET_ESC:	
	Dsp_Fill_Buffer(0);						// LCD Buffer clear
	Dsp_Diffuse(); 
	return;	

}


void netset_list(void)
{
	POINT	disp_p;
	char	buffer[32];
	INT8U   w_version[17];

	INT8U ip_l[4] = {10,10,4,33};
    INT8U gw[4] = {10,10,0,1};
    INT8U sn[4] = {255,0,0,0};

	INT8U global_com;
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
	char	strbuf[12];
#endif
	if (netset_disnable()) return;
RET_PG1:
	Dsp_Fill_Buffer(0);
	display_title_page_change(0x1940,1,1);

	//caption_split_by_code(0x1A15, &cap,0);
	sprintf(buffer,"MAC    %02x.%02x.%02x.%02x.%02x.%02x",
		EthCardStruct.MAC[5],EthCardStruct.MAC[4],EthCardStruct.MAC[3],EthCardStruct.MAC[2],EthCardStruct.MAC[1],EthCardStruct.MAC[0]);
	display_string_pos(5, (INT8U*)buffer);
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
	disp_p = point(0,0);
	sprintf(strbuf,"MAC address");
	Dsp_Write_String(disp_p,(HUGEDATA char *)strbuf);
#endif
	goto RET_EXT;

RET_PG2:

	global_com = get_global_bparam(GLOBAL_AREA_COMMUNICATION);

	if (!(global_com & 0x02)) { /* Parameter 731(Wireless Bridge) Check */
		/* if Parameter 731 : N */
		BuzOn(2);
		goto RET_EXT;
	}

	/* if Parameter 731 : Y */
	Dsp_Fill_Buffer(0);
	disp_p = point(0,0);

	Dsp_Write_String(disp_p,(HUGEDATA char *)"WLAN wait...");
	Dsp_Diffuse();

//	if (Startup.wireless_type == WLTYPE_TOBNET)
//	{
		wlan_SetNetwork(ip_l, gw, sn);  // change local IP for w/l setting
		if (wlan_CheckVer_Bridge(Startup.wireless_type, w_version, 0))
		{
			Dsp_Fill_Buffer(0);
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
			Dsp_Write_String(disp_p,(HUGEDATA char *)"WLAN MAC");
#endif
			sprintf(buffer,"WLAN   %02x.%02x.%02x.%02x.%02x.%02x",
				w_version[8],w_version[9],w_version[10],w_version[11],w_version[12],w_version[13]);

			display_string_pos(5, (INT8U*)buffer);
			VFD7_Diffuse();
			BuzOn(1);
		}
		else /* 실패 시 */
		{
			Dsp_Fill_Buffer(0);
			Dsp_Write_String(disp_p,(HUGEDATA char *)"WLAN MAC err");

			display_string_pos_clear(5, 20);
			VFD7_Diffuse();
			BuzOn(2);
		}
		Ethernet_RestoreNetwork();  // 무선랜 MAC 확인 후 IP 복구 안 시키는 오류 수정 (다른 모델도 동일, CN1 r3378 Merge)
//	}

	////if (wait) {
RET_EXT:
		Dsp_Diffuse();
		while (1) {
			while (!KEY_Read());
			BuzOn(1);
			if ((KeyDrv.CnvCode == KP_PAGE_DN) || (KeyDrv.CnvCode == KP_ARROW_DN)) goto RET_PG2;
			if ((KeyDrv.CnvCode == KP_PAGE_UP) || (KeyDrv.CnvCode == KP_ARROW_UP)) goto RET_PG1;
#ifndef CPU_PIC32MZ
				if (KeyDrv.CnvCode == KP_TEST)
				{
					netset_MAC_set();
				}
#endif
			/* 
			 * 현재 작업 중 CL5200N과 코드의 구성을 맞추기 위해서 up/down 키 2개로만 구분하기로 결정 
			 * 따라서 Menu 1144 등과 같이 인덱스 범위 밖의 서브 메뉴 진입 시 부저 등 처리 X
			 * 또한, 3개 이상의 서브 메뉴 구성이 필요한 경우에는 로직 변경이 필요함
			 */
			if ((KeyDrv.CnvCode == KP_PAGE_DN) || (KeyDrv.CnvCode == KP_ARROW_DN)) wlan_macaddr_proc();
			if ((KeyDrv.CnvCode == KP_PAGE_UP) || (KeyDrv.CnvCode == KP_ARROW_UP)) eth_macaddr_proc();
			if(KeyDrv.CnvCode == KP_ESC) break;
		}
	////}
	Dsp_Fill_Buffer(0);
	VFD7_Fill_Buffer(0);
	Dsp_Diffuse();
	return;
}


void netset_servicetype(void)
{
	INT32U set_val[2];
	INT8U result;
	CAPTION_STRUCT cap;
	INT8U v8_1,v8_old;

	v8_1 = get_net_bparam(NETWORK_SERVICE_TYPE);
	v8_old = v8_1;
	set_val[0] =(INT32U)get_net_wparam(NETWORK_SCALE_ID);
	set_val[1] =(INT32U)v8_1;
	
	Menu_Init();

	caption_split_by_code(0x1911, &cap,0);//"1. Service Type"
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x3918, &cap,0);	// Scale ID
	if(cap.input_max > MAX_SCALE - 1) cap.input_max = MAX_SCALE - 1;
#ifdef USE_MAX_SCALE_NO_255
	cap.input_max = 255;
	cap.input_dlength = 3;
	cap.input_length = 3;
#endif
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);

	caption_split_by_code(0x3919, &cap,0);//"Service Type (0-5) : [ ]"
	caption_adjust(&cap,NULL);
	netset_servicetype_y_pos = Startup.menu_type+1;
	netset_servicetype_x_pos=strlen((char *)cap.form)+1;
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[1], NULL, NULL, netset_servicetype_callback);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		set_net_wparam(NETWORK_SCALE_ID     ,(INT16U)set_val[0]);
		v8_1 = (INT8U)set_val[1];
		set_net_bparam(NETWORK_SERVICE_TYPE, v8_1);
		status_scale.scaleid = (INT16U)set_val[0];
		if (v8_1!=v8_old) {
			_SOFTWARE_RESET;
		}
	}

}


void netset_dhcp_set(void)
{
	INT32U set_val,old_val;
	INT8U result;
	CAPTION_STRUCT cap;
	
	if (netset_disnable()) return;
	set_val=get_net_bparam(NETWORK_LOCAL_ETHERNET_FLAG);
	old_val=set_val;
	Menu_Init();

	caption_split_by_code(0x1912, &cap,0);//"2. DHCP Set"
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x1a20, &cap,0);//""
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val, NULL, NULL, NULL);
	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		if(set_val== 1) {	// Enable DHCP
			EthCardStruct.DHCPEnable = 1;
		} else {	     	// Disable DHCP
			EthCardStruct.DHCPEnable = 0;
		}
		result=set_val;
		set_net_bparam(NETWORK_LOCAL_ETHERNET_FLAG,result);
		EthLocalCmdSend(ETH_SET_DHCP     ,0);
		EthLocalCmdSend(ETH_SET_SOFTRESET,0);
		if (old_val!=set_val) {
			_SOFTWARE_RESET;
		}
	}
}

void netset_local_ip_set(void)
{
	INT32U set_val[2];
	INT8U set_str[3][16];
	INT8U form[16];
	INT8U ip_old[4];
	INT8U result;
	CAPTION_STRUCT cap;
	INT8U ip[4];

	if (netset_disnable()) return;

	if(EthCardStruct.DHCPEnable == 1) 
	{
		sprintf((char*)set_str[0], "%03d.%03d.%03d.%03d", EthCardStruct.IP[3], EthCardStruct.IP[2], EthCardStruct.IP[1], EthCardStruct.IP[0]);
		get_net_sparam(NETWORK_LOCAL_GATEWAY,	 ip,4);
		sprintf((char*)set_str[1], "%03d.%03d.%03d.%03d", EthCardStruct.Gateway[3], EthCardStruct.Gateway[2], EthCardStruct.Gateway[1], EthCardStruct.Gateway[0]);
		get_net_sparam(NETWORK_LOCAL_SUBNETMASK,ip,4);
		sprintf((char*)set_str[2], "%03d.%03d.%03d.%03d", EthCardStruct.SubnetMask[3], EthCardStruct.SubnetMask[2], EthCardStruct.SubnetMask[1], EthCardStruct.SubnetMask[0]);
	}
	else
	{
		get_net_sparam(NETWORK_LOCAL_IP,	 ip,4);
		memcpy(ip_old,ip,4);
		sprintf((char*)set_str[0], "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);
		get_net_sparam(NETWORK_LOCAL_GATEWAY,	 ip,4);
		sprintf((char*)set_str[1], "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);
		get_net_sparam(NETWORK_LOCAL_SUBNETMASK,ip,4);
		sprintf((char*)set_str[2], "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);
	}

	set_val[0]=get_net_wparam(NETWORK_LOCAL_TCP_PORT);
	set_val[1]=set_val[0];
	
	sprintf((char*)form, "999.999.999.999");
	
	Menu_Init();

	caption_split_by_code(0x1913, &cap,0);//"3. IP Setting"
	Menu_HeadCreate((HUGEDATA char *)cap.form);
 
	caption_split_by_code(0x1a21, &cap,0);//"Scale IP Addr : [   .   .   .   ]"
	caption_adjust(&cap,NULL);
#if defined(_USE_LCD_7_37D_40DOT_)
	cap.input_x = 13; // MENU_TYPE_IP일 때, IP를 한 줄에 볼 수있도록 강제로 좌표 지정
#elif defined(_USE_LCD_7_26d_16x96_)
	cap.input_x = 12;
#endif
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_IP, cap.input_x, cap.input_dlength, cap.input_length,
					cap.input_max, cap.input_min, 0, NULL, set_str[0], form, NULL);

	caption_split_by_code(0x1a22, &cap,0);//"Gateway       : [   .   .   .   ]"
	caption_adjust(&cap,NULL);
#if defined(_USE_LCD_7_37D_40DOT_)
	cap.input_x = 13; // MENU_TYPE_IP일 때, IP를 한 줄에 볼 수있도록 강제로 좌표 지정
#elif defined(_USE_LCD_7_26d_16x96_)
	cap.input_x = 12;
#endif
	Menu_InputCreate(2, (HUGEDATA char *)cap.form, MENU_TYPE_IP, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, set_str[1], form, NULL);

	caption_split_by_code(0x1a23, &cap,0);//"Subnet Mask   : [   .   .   .   ]"
	caption_adjust(&cap,NULL);
#if defined(_USE_LCD_7_37D_40DOT_)
	cap.input_x = 13; // MENU_TYPE_IP일 때, IP를 한 줄에 볼 수있도록 강제로 좌표 지정
#elif defined(_USE_LCD_7_26d_16x96_)
	cap.input_x = 12;
#endif
	Menu_InputCreate(3, (HUGEDATA char *)cap.form, MENU_TYPE_IP, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, set_str[2], form, NULL);

	if(EthCardStruct.DHCPEnable == 1) {
		Menu_SetDisable(1, ON);
		Menu_SetDisable(2, ON);
		Menu_SetDisable(3, ON);
	}

	if(Startup.country != COUNTRY_US) // USA : Port Menu disable
	{
		caption_split_by_code(0x1a24, &cap,0);//"TCP Port      : [     ]"
		caption_adjust(&cap,NULL);
		Menu_InputCreate(4, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);
	}
	
	while(1) {
		result = Menu_InputDisplay();
		if(result == MENU_RET_SAVE) {
			if(EthCardStruct.DHCPEnable == 0) {
				netset_str2ip(set_str[0], ip);
				if(!netset_ip_valid_check(ip, 1)) {
					BuzOn(2);
					error_display_caption(0x1a29);
					continue;
				}
				set_net_sparam(NETWORK_LOCAL_IP,ip,4);
			}

			netset_str2ip(set_str[1], ip);
			set_net_sparam(NETWORK_LOCAL_GATEWAY,ip,4);
			netset_str2ip(set_str[2], ip);
			set_net_sparam(NETWORK_LOCAL_SUBNETMASK,ip,4);
			set_net_wparam(NETWORK_LOCAL_TCP_PORT,(INT16U)set_val[0]);
			set_net_wparam(NETWORK_LOCAL_UDP_PORT,(INT16U)set_val[0]);
			ethernet_set_flash2card();
#ifndef CPU_PIC32MZ
			SaveEthernetDataForBoot(); // fw ethernet update에 사용할 ip,mac 등을 저장
#endif
			netset_str2ip(set_str[0], ip);
			if (memcmp(&ip,&ip_old,4)!=0) {
				_SOFTWARE_RESET;
			} else {
				EthInit();
			}

			break;
		} else {
			break;
		}
	}
}



void netset_remote_ip_set(void)
{
	INT32U set_val[2];
	INT8U set_str[1][16];
	INT8U form[16];
	INT8U result;
	CAPTION_STRUCT cap;
	INT8U ip[4];
#ifdef USE_TRACE_STANDALONE
	INT8U set_str2[16];
	INT32U set_val2;
#endif

	if (netset_disnable()) return;

	get_net_sparam(NETWORK_MACHINE_REMOTE_IP, ip, 4);
	sprintf((char*)set_str[0], "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);
	set_val[0]=get_net_wparam(NETWORK_MACHINE_REMOTE_TCPPORT);
	set_val[1]=get_net_wparam(NETWORK_MACHINE_REMOTE_UDPPORT);

#ifdef USE_TRACE_STANDALONE
	get_base_sparam(FLASH_REMOTE_IP2, ip, 4);
	sprintf((char*)set_str2, "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);
	set_val2 = get_base_wparam(FLASH_REMOTE_IP2+4);
#endif

	sprintf((char*)form, "999.999.999.999");
	
	Menu_Init();

	caption_split_by_code(0x1914, &cap,0);//"4. Remote Machine Setting"
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x1a26, &cap,0);//"Remote IP    : [   .   .   .   ]"
	caption_adjust(&cap,NULL);
#if defined(_USE_LCD_7_37D_40DOT_)
	cap.input_x = 13; // MENU_TYPE_IP일 때, IP를 한 줄에 볼 수있도록 강제로 좌표 지정
#elif defined(_USE_LCD_7_26d_16x96_)
	cap.input_x = 12;
#endif
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_IP, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, set_str[0], form, NULL);

	caption_split_by_code(0x1a24, &cap,0);//"TCP Port     : [     ]"
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);

#ifdef USE_TRACE_STANDALONE
	caption_split_by_code(0x1a26, &cap,0);//"Remote IP    : [   .   .   .   ]"
	strcat(cap.form, "2");
	caption_adjust(&cap,NULL);
#if defined(_USE_LCD_7_37D_40DOT_)
	cap.input_x = 13; // MENU_TYPE_IP일 때, IP를 한 줄에 볼 수있도록 강제로 좌표 지정
#elif defined(_USE_LCD_7_26d_16x96_)
	cap.input_x = 12;
#endif
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_IP, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, set_str2, form, NULL);

	caption_split_by_code(0x1a24, &cap,0);//"TCP Port     : [     ]"
	strcat(cap.form, "2");
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val2, NULL, NULL, NULL);
#endif

	while(1) {
		result = Menu_InputDisplay();
		if(result == MENU_RET_SAVE) {
			netset_str2ip(set_str[0], ip);
			if(!netset_ip_valid_check(ip, 0)) {
				BuzOn(2);
				error_display_caption(0x1a29);
				continue;
			}
			set_val[1]=set_val[0];
			set_net_sparam(NETWORK_MACHINE_REMOTE_IP,ip,4);
			set_net_wparam(NETWORK_MACHINE_REMOTE_TCPPORT,(INT16U)set_val[0]);
			set_net_wparam(NETWORK_MACHINE_REMOTE_UDPPORT,(INT16U)set_val[1]);
			EthCardStruct.TCPDestIP[3]=ip[0];
			EthCardStruct.TCPDestIP[2]=ip[1];
			EthCardStruct.TCPDestIP[1]=ip[2];
			EthCardStruct.TCPDestIP[0]=ip[3];
			EthCardStruct.TCPDestPort =(INT16U)set_val[0];
			EthCardStruct.UDPDestPort =(INT16U)set_val[0];
			ethernet_list.remoteip[0]=ip[0];
			ethernet_list.remoteip[1]=ip[1];
			ethernet_list.remoteip[2]=ip[2];
			ethernet_list.remoteip[3]=ip[3];
			EthLocalCmdSend(ETH_SET_DEST_IP ,0);
			EthLocalCmdSend(ETH_SET_DEST_TCP_PORT ,0);
			EthLocalCmdSend(ETH_SET_DEST_UDP_PORT ,0);
			EthLocalCmdSend(ETH_SET_SOFTRESET,0);

#ifdef USE_TRACE_STANDALONE
			netset_str2ip(set_str2, ip);
			if(!netset_ip_valid_check(ip, 0)) {
				BuzOn(2);
				error_display_caption(0x1a29);
				continue;
			}
			set_base_sparam(FLASH_REMOTE_IP2, ip, 4);	// remote ip2
			set_base_wparam(FLASH_REMOTE_IP2+4, set_val2);	// remote tcp port2
#endif

			//ethernet_set_flash2card();
			break;
		} else {
			break;
		}
	}
}



void netset_wlan_config(void)
{
	if (netset_disnable()) return;
	BuzOn(2);
	return;
}

void netset_access_point(void)
{
	INT8U set_val1[34],set_val2[65];
	INT8U result;
	CAPTION_STRUCT cap;
	INT32U  use_wep;
	INT32U  wep_keyid;
	INT32U  set_val;
	INT8U   v_8,mleng,opmode;
	// LANTRONIX Matchport b/g
	// default bridge ip
	INT8U ip[4] = {10,10,4,254};
	INT8U ip_l[4] = {10,10,4,33};
	INT8U gw[4] = {10,10,0,1};
	INT8U sn[4] = {255,0,0,0};
	INT8U wait_cnt;
	INT8U version[17];
	INT8U old_page, old_font;
	POINT disp_p;
	INT8U str[64];
	INT8U error;

	if (netset_disnable()) return;

	if (!(Startup.global_com&0x02))	// not use wireless bridge 
	{
		BuzOn(2);
		return;
	}

	get_net_sparam(NETWORK_WLAN_SSID    ,(INT8U *)set_val1,32);
	use_wep=(INT32U)get_net_bparam(NETWORK_WLAN_USEWEP);	// 0:none, 1:wep64, 2:wep128, 3:wpa(tkip), 4:wpa2(tkip), 5:WPA2(AES)
	wep_keyid = (INT32U)get_net_bparam(NETWORK_WLAN_WEPID);
	set_val1[32]=0;
	set_val =1;
	if (use_wep > 5) use_wep = 0;
	if (use_wep == 3 || use_wep == 4 || use_wep == 5)
	{
		wep_keyid = 1;
		get_net_sparam(NETWORK_WLAN_WEP, (INT8U *)set_val2, 64);
		set_val2[64] = 0;
	}
	else
	{
		if (wep_keyid > 4) wep_keyid = 0;
		if ((wep_keyid >= 1) && (wep_keyid<=4)) {
			get_net_sparam(NETWORK_WLAN_WEP+16*(wep_keyid-1),(INT8U *)set_val2,16);
			set_val2[16]=0;
		} else {
			memset(set_val2,0,16);
		}
	}
        
	Menu_Init();

	caption_split_by_code(0x1916, &cap, 0);// Wireless LAN Setting
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x3961, &cap, 0);// SSID
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_SSID, (HUGEDATA char *)cap.form, MENU_TYPE_STR, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min,0,NULL, set_val1, NULL,NULL);
	Menu_AddKeyFunction(M_ID_SSID, KP_ZERO, netset_wl_setting_rs232);	// [ZERO] : wireless brideg setting(rs232)
#ifdef USE_CHECK_WLAN_STATUS_LANTRONIX
	if(Startup.wireless_type == WLTYPE_TOBNET) {
		Menu_AddKeyFunction(M_ID_SSID, KP_TEST, netset_wirless_module_get_status);
	} else {
		Menu_AddKeyFunction(M_ID_SSID, KP_TEST, netset_wl_get_status);
	}
	Menu_AddKeyFunction(M_ID_SSID, KP_TARE, netset_wl_scan_ssid);	
#endif
	Menu_SetProperties(M_ID_SSID, MENU_P_ENGLISH_MODE);


	caption_split_by_code(0x3962, &cap, 0); // Security Setting :
	caption_adjust(&cap,NULL);
	cap.input_max = 5;
	Menu_InputCreate(M_ID_USEWEP, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min,0,&use_wep,NULL, NULL, callback_security_type);
	Menu_AddEndFunction(M_ID_USEWEP, netset_usewep_callback);


	caption_split_by_code(0x3963, &cap, 0);// WEP Key ID
	caption_adjust(&cap,NULL);
	Menu_InputCreate(M_ID_WEPID, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min,0,&wep_keyid,NULL, NULL,NULL);
	Menu_AddEndFunction(M_ID_WEPID, netset_select_wepid_callback);

	caption_split_by_code(0x3964, &cap, 0);// WEPKey
	caption_adjust(&cap,NULL);
#ifdef USE_HIDE_WIRELESS_PASSWORD
	Menu_InputCreate(M_ID_WEPKEY, (HUGEDATA char *)cap.form, MENU_TYPE_PASSWORD, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min,0,NULL, set_val2, NULL,NULL);
#else
	Menu_InputCreate(M_ID_WEPKEY, (HUGEDATA char *)cap.form, MENU_TYPE_STR, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min,0,NULL, set_val2, NULL,NULL);
#endif
	Menu_SetProperties(M_ID_WEPKEY, MENU_P_ENGLISH_MODE);

	caption_split_by_code(0x3965, &cap, 0);// OP mode
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min,0,&set_val,NULL, NULL,NULL);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		// write to flash
		set_net_sparam(NETWORK_WLAN_SSID    ,(INT8U *)set_val1,32);
		v_8=(INT8U)use_wep;
		set_net_bparam(NETWORK_WLAN_USEWEP  ,v_8);
		if (use_wep == 3 || use_wep == 4 || use_wep == 5)
		{
			wep_keyid = 1;
			set_net_sparam(NETWORK_WLAN_WEP, (INT8U *)set_val2, 64);
		}
		else
		{
			if ((wep_keyid>=1) && (wep_keyid<=4)) {
				set_net_sparam(NETWORK_WLAN_WEP+16*(wep_keyid-1),(INT8U *)set_val2,16);
			}
		}
		v_8=(INT8U)wep_keyid;
		set_net_bparam(NETWORK_WLAN_WEPID,v_8);

		strcpy((char *)EthCardStruct.SSID,(char *)set_val1);
		EthLocalCmdSend(ETH_SET_SSID     ,0);

		mleng=0;
		EthCardStruct.WEPLength=0;
		switch (use_wep) {
			default:
			case 0: mleng=0;  break;
			case 1:	mleng= 5; EthCardStruct.WEPLength=0;  break;
			case 2: mleng=13; EthCardStruct.WEPLength=1;  break;
			case 3: 
			case 4: 
			case 5:
				mleng=strlen((char*)set_val2);
				if (mleng > 64) mleng = 64;
				set_val2[64] = 0;
				break;
		}
		// WEP
		if (use_wep == 1 || use_wep == 2) {
		     EthCardStruct.WEPEnable=1;
		     EthLocalCmdSend(ETH_SET_WEP_ENABLE ,0);

		     if (use_wep==1) EthCardStruct.WEPLength=0;
		     else            EthCardStruct.WEPLength=1;
		     EthLocalCmdSend(ETH_SET_WEP_LENGTH ,0);

		     EthCardStruct.WEPID = (INT8U)wep_keyid;
		     switch (EthCardStruct.WEPID) {
			case 1:	memcpy(EthCardStruct.WEPKey1,set_val2,mleng);
			        EthLocalCmdSend(ETH_SET_WEP_KEY1 ,0);
				break;
			case 2:	memcpy(EthCardStruct.WEPKey2,set_val2,mleng);
			        EthLocalCmdSend(ETH_SET_WEP_KEY2 ,0);
				break;
			case 3:	memcpy(EthCardStruct.WEPKey3,set_val2,mleng);
			        EthLocalCmdSend(ETH_SET_WEP_KEY3 ,0);
				break;
			case 4:	memcpy(EthCardStruct.WEPKey4,set_val2,mleng);
			        EthLocalCmdSend(ETH_SET_WEP_KEY4 ,0);
				break;
		    }
		} else {
		     EthCardStruct.WEPEnable=0;
		     EthLocalCmdSend(ETH_SET_WEP_ENABLE ,0);
		}
		
		opmode =(INT8U)set_val;		
		
		// Reset		
		EthLocalCmdSend(ETH_SET_SOFTRESET,0);

		old_page = DspStruct.Page;
		old_font = DspStruct.Id1;
		Dsp_SetPage(DSP_ERROR_PAGE);
		FontSizeChange(FONT_MEDIUM);

		Dsp_Fill_Buffer(0);

		// LANTRONIX Matchport b/g
		wlan_SetNetwork(ip_l, gw, sn);	// change local IP for w/l setting	
		if (wlan_CheckVer_Bridge(Startup.wireless_type, version, 3)) // wait 3sec
		{
			sprintf((char*)str, "Wait:%s", version);
			if (version[0] == 'T')	// TOBNET
			{
				Startup.wireless_type = WLTYPE_TOBNET;
				Startup.wireless_model = WLMODEL_T_VERSION;
				if (version[1] == 'A') // Hi-Flying
				{
					Startup.wireless_model = WLMODEL_HIFLYING;
				}

				disp_p = point(Startup.menu_type*Startup.menu_ygap, 0);
				Dsp_Write_String(disp_p,(HUGEDATA char*)str);
				Dsp_Diffuse();

				error = 0;
				if (!wlan_SetIP_Tobnet(ip, gw, sn))	// Fix (ip:10.10.0.254, gw:10.10.0.1, sn:255.255.255.0)
				{
					error = 1;
				}
				if (!error && !wlan_SetWiFiSetting_Tobnet(EthCardStruct.SSID, (INT8U)use_wep, (INT8U)wep_keyid, mleng, set_val2, opmode))
				{
					error = 2;
				}
				if (error)
				{
					BuzOn(2);
					Dsp_Fill_Buffer(0);
					sprintf((char*)str, "Set Err(%d)", error);//change to ERROR msg
					Dsp_Write_String(disp_p,(HUGEDATA char*)str);
					Dsp_Diffuse();
				}
				
				wlan_RebootModule(WLTYPE_TOBNET);
				Delay_100ms(50);
				wait_cnt = 0;
				while(!wlan_CheckVer_Bridge(Startup.wireless_type, version, 0) && wait_cnt++ < 100);	// check rebooting, 200ms*3*100=1 min//(500ms*3(retry)*40) = 1 min
				if (wait_cnt >= 100) BuzOn(4);
				BuzOnAdd(1);
			}
			else	// LANTRONIX
			{
				Startup.wireless_type = WLTYPE_LANTRONIX;
				Startup.wireless_model = WLMODEL_LANTRONIX;

				disp_p = point( Startup.menu_type*Startup.menu_ygap, 0);
				//strcat((char*)str, ":L");
				Dsp_Write_String(disp_p,(HUGEDATA char*)str);
				Dsp_Diffuse();

				error = 0;
				if (!wlan_SetIP_Lantronix(ip, gw, sn))	// Fix (ip:10.10.0.254, gw:10.10.0.1, sn:255.255.255.0)
				{
					error = 1;
				}
				if (!error && !wlan_SetWiFiSetting_Lantronix(EthCardStruct.SSID, (INT8U)use_wep, (INT8U)wep_keyid, mleng, opmode))
				{
					error = 2;
				}
				if (!error && !wlan_SetCountry_Lantronix())
				{
					error = 3;
				}
				if (!error && !wlan_SetEncriptionKey_Lantronix(set_val2))
				{
					error = 4;
				}
				if (error)
				{
					BuzOn(2);
					Dsp_Fill_Buffer(0);
					sprintf((char*)str, "Set Err(%d)", error);//change to ERROR msg
					Dsp_Write_String(disp_p,(HUGEDATA char*)str);
					Dsp_Diffuse();
				}
				
				wlan_RebootModule(WLTYPE_LANTRONIX);
				wait_cnt = 0;
				while(!wlan_CheckVer_Bridge(Startup.wireless_type, version, 0) && wait_cnt++ < 15);	// check rebooting
				if (wait_cnt >= 15) BuzOn(4);
				BuzOnAdd(1);
			}

		}
		else 
		{
			BuzOn(4);
			Startup.wireless_type = WLTYPE_NONE;
			Startup.wireless_model = WLMODEL_NONE;
			disp_p = point( Startup.menu_type*Startup.menu_ygap, 0);
			sprintf((char*)str, "Set Err");//change to ERROR msg
			Dsp_Write_String(disp_p,(HUGEDATA char*)str);
			Dsp_Diffuse();
			Key_DelaySec(30, 0);
		}				

		Ethernet_RestoreNetwork();
		
		Dsp_SetPage(old_page);
		DspLoadFont1(old_font);
		Dsp_Diffuse();
		if (Startup.wireless_type == WLTYPE_TOBNET || Startup.wireless_type == WLTYPE_LANTRONIX)	// bridge type
		{
			// bridge type일 경우 저울을 bridge에 인식시키기 위해 임의데이터(버전요청)를 전송
			SysBootTime = SysTimer100ms;
			CheckingWirelessLAN = ON;
		}
	}
}


void netset_rs232c_set(void)
{
	INT32U set_val[1];
	INT8U result;
	CAPTION_STRUCT cap;

	set_val[0] = (INT32U)get_global_bparam(GLOBAL_AREA_RS232C_BAUD);
	Menu_Init();

	caption_split_by_code(0x1915, &cap,0);//"5. RS232C Setting"
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x1a30, &cap,0);//"Baud Rate (0-4): [ ]"
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, netset_rs232c_baud_callback);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		set_global_bparam(GLOBAL_AREA_RS232C_BAUD, (INT8U)set_val[0]);
		Uart0Init((INT8U)set_val[0]);
		CommBufInit(&CommBuf,COMM_TYPE_HOST,0);//HYP 20041222
	}
}


void network_one2one_copy(void)
{
	HUGEDATA COMM_BUF *pCommbuf;
	INT8U ret;
	INT8U connected, nretry;
	INT32U flag=0;
	INT8U commMethod=0; //0=RS232 or 1=TCP/IP
	CAPTION_STRUCT 	cap;
	INT16U  result;

	Menu_Init();
	caption_split_by_code(0x1950, &cap,0);
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x3951, &cap,0);
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, &flag, NULL, NULL, NULL);

	result = Menu_InputDisplay();
	if(result == MENU_RET_SAVE) {
		commMethod = flag & 0x01;
// 저장 불필요
//		uctemp= (uctemp&(0xfe))|commMethod;
//		set_net_bparam(NETWORK_SERVICE_FLAG3, uctemp);	
	}
	else   return;

	if(commMethod==1) //TCP/IP
	{
		connected=0;
		nretry=0;
		while (!connected) {
			connected=Ethernet_Server(CLIENT_OPEN, 0, CLIENT_BACKUP_TO_SCALE);
			nretry++;
			if (nretry>3) break;
		}
		if(!connected){
			BuzOn(4);	//se-hyung 20070803
			network_error(MSG_21_NET_ERR);
			return;
		}
		pCommbuf = &(CommBufEthData[0]);
	}
	else  //RS232
	{
		pCommbuf = &CommBuf;
	}


	VertScrollMsg_Init(DSP_MENU_FONT_ID);

	// Origin,PLU Structure
	VertScrollMsg_Add("Backup");
	VertScrollMsg_Add("ORG,P Struct");
	ret=network_block_copy(FLASH_ORIGIN_AREA+DFLASH_BASE,FLASH_KEY_AREA+DFLASH_BASE,0,pCommbuf);
	if (ret==2) goto END;

	Ethernet_Server(CLIENT_OPEN, 0, CLIENT_NORMAL);
	// Currency Table, Scroll Message, Print Advertise
	VertScrollMsg_Add("Curr,Message");
	ret=network_block_copy(FLASH_CURRENCY_AREA+DFLASH_BASE,FLASH_NUMBER_PLU+DFLASH_BASE,0,pCommbuf);
	if (ret==2) goto END;

	// TAX Table,Barcode Format,Shopname,Department,Group,Tare,Clerk
	VertScrollMsg_Add("Barcode,TBL.");
	ret=network_block_copy(FLASH_TAX_AREA+DFLASH_BASE,FLASH_SCANNER_FORMAT+DFLASH_BASE,0,pCommbuf);
	if (ret==2) goto END;

	// Scanner Format,Discount
	VertScrollMsg_Add("Scanner,Disc");
	ret=network_block_copy(FLASH_SCANNER_FORMAT+DFLASH_BASE,FLASH_INGREDIENT_AREA+DFLASH_BASE,0,pCommbuf);
	if (ret==2) goto END;

	// Ingredient
	VertScrollMsg_Add("Ingredient");
	ret=network_block_copy(FLASH_INGREDIENT_AREA+DFLASH_BASE,FLASH_PLU_AREA+DFLASH_BASE,0,pCommbuf);
	if (ret==2) goto END;

#ifdef USE_NUTRITION_FACT
	// Nutrition Fact,Country,Slaughter,CuttingHall,Traceability
	VertScrollMsg_Add("Nutrition..");
	ret=network_block_copy(FLASH_NUTRIFACT_AREA+DFLASH_BASE,FLASH_END_POINT+DFLASH_BASE,0,pCommbuf);
	if (ret==2) goto END;
#endif

	// PLU PTYPE Reset
	VertScrollMsg_Add("Ptype Reset");
	ret=network_block_copy(0L,0L,3,pCommbuf);
	if (ret==2) goto END;

	// PLU Copy
	VertScrollMsg_Add("PLU Copy");
	ret=network_block_copy(0L,0L,1,pCommbuf);
	if (ret==2) goto END;
END:
	Ethernet_Server(CLIENT_OPEN, 0, CLIENT_BACKUP_TO_SCALE);
	commun_send_block_addr(pCommbuf,0l,9,(INT16U)0);	// Reset

	Delay_100ms(20);
	
	if(commMethod==1){
		if (connected) {
			Ethernet_Server(CLIENT_CLOSE, 0, CLIENT_BACKUP_TO_SCALE);
		}	
	}

	sprintf(gmsg,"-END-");
	display_string(DISPLAY_UNITPRICE,(INT8U *)gmsg);
	display_clear(DISPLAY_PRICE);
	VFD7_Diffuse();
	display_clear(DISPLAY_UNITPRICE);
	display_clear(DISPLAY_PRICE);
	VFD7_Diffuse();
}

	   
void netset_slave_create(void)
{
	ETHERNET_SLAVE_SCALE  slave_str;
	CAPTION_STRUCT 	  cap;	
 	INT16U	str_size; 
	INT16U	result;
	INT8U	input_max;
	INT8U	ip[4];
		 
	INT32U	addr_start; 
	INT32U	addr_flash;
	INT32U  set_val[3];
	INT8U	set_str[16];
	INT8U 	form[16];
	INT8U	loop;


	set_val[0] = 1;

	sprintf((char*)form, "999.999.999.999");

	str_size = sizeof(slave_str);
	addr_start = DFLASH_BASE + FLASH_SLAVE_INFO;
	loop = ON;
	while (loop)
	{
		addr_flash = addr_start + (str_size * (set_val[0] - 1));
		Flash_sread(addr_flash, (HUGEDATA INT8U *)&slave_str, str_size); 

		if (slave_str.code != set_val[0])
		{
			memset((INT8U*)&slave_str, 0x00, str_size);
			slave_str.tcp_port = 20304;
		}
	       	slave_str.code = set_val[0];
		set_val[0] = slave_str.code;
		memcpy((HUGEDATA char*)ip, (HUGEDATA char*)slave_str.ip, 4);
		sprintf((char*)set_str, "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);
		set_val[1] = slave_str.tcp_port;
		set_val[2] = slave_str.use;   

		Menu_Init();
		caption_split_by_code(0x1961, &cap,0);
		Menu_HeadCreate((HUGEDATA char *)cap.form);


		caption_split_by_code(0x1a27,&cap,0);
		if (cap.input_max > MAX_SLAVE_SCALE) cap.input_max = MAX_SLAVE_SCALE;
		input_max = cap.input_max;
		caption_adjust(&cap,NULL);
		Menu_InputCreate(M_ID_SLAVE_NO, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
							cap.input_max, cap.input_min, 0, &set_val[0], NULL, NULL, NULL);
 		Menu_AddEndFunction(M_ID_SLAVE_NO, netset_slave_dataUpdate);

		caption_split_by_code(0x1a21, &cap,0);//"Scale IP     : [   .   .   .   ]"
		caption_adjust(&cap,NULL);
#if defined(_USE_LCD_7_37D_40DOT_)
		cap.input_x = 13; // MENU_TYPE_IP일 때, IP를 한 줄에 볼 수있도록 강제로 좌표 지정
#elif defined(_USE_LCD_7_26d_16x96_)
		cap.input_x = 12;
#endif
		Menu_InputCreate(M_ID_SLAVE_IP, (HUGEDATA char *)cap.form, MENU_TYPE_IP, cap.input_x, cap.input_dlength, cap.input_length,
							cap.input_max, cap.input_min, 0, NULL, set_str, form, NULL);
		//Menu_AddKeyFunction(M_ID_SLAVE_IP, KP_TEST, netset_scan_slave);

		caption_split_by_code(0x1a24, &cap,0);//"TCP Port     : [     ]" 
		caption_adjust(&cap,NULL);
		Menu_InputCreate(M_ID_SLAVE_PORT, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
							cap.input_max, cap.input_min, 0, &set_val[1], NULL, NULL, NULL);

		caption_split_by_code(0x1a28, &cap,0);//"Use     : [ ]" 
		caption_adjust(&cap,NULL);
		Menu_InputCreate(M_ID_SLAVE_USE, (HUGEDATA char *)cap.form, MENU_TYPE_YN, cap.input_x, cap.input_dlength, cap.input_length,
							cap.input_max, cap.input_min, 0, &set_val[2], NULL, NULL, NULL);

		while (1) 
		{
			result = Menu_InputDisplay();
			if (result == MENU_RET_SAVE) 
			{
				netset_str2ip(set_str, ip);
				if (!netset_ip_valid_check(ip, 0)) 
				{
					BuzOn(2);
					error_display_caption(0x1a29);
					continue;
				}

				addr_flash = addr_start + (str_size * (set_val[0] - 1));
				memset((INT8U*)&slave_str, 0x00, str_size);
				
				slave_str.code = set_val[0];
				memcpy((HUGEDATA char*)slave_str.ip, (HUGEDATA char*)ip, 4);
				slave_str.tcp_port = set_val[1];
				slave_str.use = set_val[2];
				Flash_swrite(addr_flash, (HUGEDATA INT8U *)&slave_str, str_size);

				if (set_val[0] < input_max) set_val[0]++;
				else set_val[0] = input_max; 

				break;
			}
			else if(result ==MENU_RET_ESC)
			{
				loop = OFF;
				break;
			}
		}
	}
	network_search_slave();
}

void netset_slave_list(void)
{		
	CAPTION_STRUCT 	cap;
	POINT 		disp_p;
	ETHERNET_SLAVE_SCALE  slave_str;
	char   	string_buf[50], end_flag = 0;
	INT16S	start_num, end_num, cur_id, str_size, reverse_num;
	INT16S	j = 0;
	INT16S	line_ct, lcd_size, start_id;
	INT8U	read_code;
	INT32U	addr, addr_flash;
	INT32U  async, tsync;
	INT8U   k;
	INT16U  cur_time;
	INT8U	percent[10];
	INT8U	scale_id;

	str_size = sizeof(slave_str);

	caption_split_by_code(0x1a27, &cap,0);
	start_num = cap.input_min;
	end_num   = cap.input_max; 
	
	lcd_size   = Dsp_GetChars();
	line_ct    = Startup.line_max; // (Startup.menu_type?3:2);
	addr_flash = DFLASH_BASE + FLASH_SLAVE_INFO;
	cur_id     = start_num;
	start_id   = start_num;

READ_START:
	Dsp_Fill_Buffer(0);
	if (Startup.menu_type)
	{
		caption_split_by_code(0x71FF, &cap,0);
		disp_p = point(0,0);
		Dsp_Write_String(disp_p,(HUGEDATA char*)cap.form);
	}

	j = 0;
	tsync = MAX_PLU_NO + (MAX_DISCOUNT_NO - 1) + (MAX_INGREDIENT_NO) + MAX_SCROLL_NO;
	while (cur_id <= end_num)
	{
		addr =  addr_flash + (long)str_size * (cur_id-1);
		read_code = Flash_bread(addr);
		if(read_code == cur_id)  
		{
			async = 0;
			for (k = 0; k < 4; k++)
			{
				async += commun_net_mask(k, NULL, cur_id, 14);	// 0:plu, 1:d/c, 2:ingredient, 3:scroll msg
			}
			j++;
			Flash_sread(addr, (HUGEDATA INT8U*)&slave_str, str_size); 
			percent[0] = 0;
			if (slave_str.use)
			{
				sprintf((char*)percent, "%3ld%%", ((tsync-async)*100/tsync));
			}
			sprintf(string_buf, "IP%2d    %3d.%3d.%3d.%3d", slave_str.code, slave_str.ip[0], slave_str.ip[1], slave_str.ip[2], slave_str.ip[3]);
			display_string_pos(5, (INT8U*)string_buf);
			
		 	sprintf(string_buf, "%c %3s %c%c",
		 			(slave_str.use?'Y':'N'), percent, 
		 			syncStatusSymPLU[SyncStatus[cur_id - 1]&0x03], syncStatusSymDC[(SyncStatus[cur_id - 1]>>2)&0x03]);
			
			string_buf[lcd_size] = 0;	       		
			
			disp_p = point(((j-1)+Startup.menu_type)*Startup.menu_ygap, 0);	       		 
			Dsp_Write_String(disp_p, (HUGEDATA char *)string_buf);
			if (j == 1)
				start_id = cur_id;	
			if (j == line_ct) break;
		}
		
		cur_id++;	
	}
	Dsp_Diffuse();
	if (cur_id >= end_num && j == 0) 
	{
		error_display_caption(0xE00C);
		goto RET_ESC;
	}
	cur_id = start_id;
	
	cur_time = SysTimer100ms;
	while(!KEY_Read())
	{
		if ((INT16U)(SysTimer100ms - cur_time) > 50) goto READ_START;
	}
	BuzOn(1);
	
	switch(KeyDrv.CnvCode) 
	{
	case KP_ESC:
		goto RET_ESC;
	case KP_ARROW_UP:
		while(1)  
		{
			if(cur_id <= start_num) 
			{
				BuzOn(1);
				cur_id = start_num;
				goto READ_START;
			}
			cur_id--;
			addr =  addr_flash + (long)str_size * (cur_id-1);			
			read_code = Flash_bread(addr);
			if(read_code == cur_id /*>= min_num && read_code <= max_num*/ )  
				goto READ_START;
		}
	case KP_PAGE_UP :
		reverse_num = 0;
		while(1)  
		{
			if(cur_id <= start_num) 
			{
				BuzOn(1);
				cur_id = start_num;
				goto READ_START;
			}
			cur_id--;
			addr =  addr_flash + (long)str_size * (cur_id-1);			
			read_code = Flash_bread(addr);
			if(read_code == cur_id /*>= min_num && read_code <= max_num*/ )  
			{
			       	reverse_num++;
				if(reverse_num == line_ct) 
					goto READ_START;
			}
		}
	case KP_PAGE_DN :
		reverse_num = 0;
		while(1)  
		{
			if(cur_id >= end_num) 
			{
				BuzOn(1);
				error_display_caption(0xE00E);
				if (reverse_num < line_ct)
					cur_id = start_id;
				else
					cur_id = end_num;
				goto READ_START;
			}
			else
				cur_id++;
			addr =  addr_flash + (long)str_size * (cur_id-1);			
			read_code = Flash_bread(addr);
			if(read_code == cur_id /*>= min_num && read_code <= max_num*/ )  
			{
			       	reverse_num++;
				if(reverse_num == line_ct) 
					goto READ_START;
			}
		}
	case KP_ENTER:		
	case KP_ARROW_DN:
		reverse_num = cur_id;
		while(reverse_num <= end_num)  
		{
			reverse_num++;
			addr =  addr_flash + (long)str_size * (reverse_num-1);			
			read_code = Flash_bread(addr);
			if(read_code == reverse_num /*&& (read_code >= min_num && read_code <= max_num)*/ )  
			{
			       	cur_id = reverse_num;
				goto READ_START;
			}
		}
		BuzOn(1);
		error_display_caption(0xE00E);
		goto READ_START;
	case KP_ZERO:
		scale_id = (INT8U)netset_reset_confirm(ON);
		if (scale_id)
		{
			commun_net_mask(0, NULL, scale_id, 5);	// sync plu
			commun_net_mask(1, NULL, scale_id, 5);	// sync d/c
			commun_net_mask(2, NULL, scale_id, 5);	// sync ingredient
			commun_net_mask(3, NULL, scale_id, 5);	// sync scroll msg
			SyncStatus[scale_id] = 0;		// initial
		}
		goto READ_START;	
	case KP_TEST:
		scale_id = (INT8U)netset_reset_confirm(OFF);
		if (scale_id)
		{
			commun_net_mask(0, NULL, scale_id, 4);	// sync plu
			commun_net_mask(1, NULL, scale_id, 4);	// sync d/c
			commun_net_mask(2, NULL, scale_id, 4);	// sync ingredient
			commun_net_mask(3, NULL, scale_id, 4);	// sync scroll msg
			SyncStatus[scale_id] = 0;		// initial
		}
		goto READ_START;	
	case KP_FEED:
		keyapp_feed(OFF);
		goto READ_START;	
	default :	
		BuzOn(1);
		goto READ_START;
	} //switch(KeyDrv.CnvCode)						

RET_ESC:
	display_string_pos_clear(5, 20);
	return;
}


void netset_slave_delete(void)
{
	CAPTION_STRUCT	cap;	
	POINT 		disp_p;
	ETHERNET_SLAVE_SCALE	slave_str;

	INT32U	Flash_addr;
	long 	ret_long;
	int  	result;
	INT16U	str_size;

	str_size = sizeof(slave_str);
	ret_long = 1;
	Flash_addr = 0;

	Dsp_Fill_Buffer(0);	

	menu_displaytitle(0x1963,0);
	menu_writepage(1, 1);

	caption_split_by_code(0x1A27, &cap,0);
	caption_adjust(&cap,NULL);
	display_string(DISPLAY_UNITPRICE, (INT8U*)cap.form);
	VFD7_Diffuse();

REN1:	DspStruct.DirectDraw = 1;   
#if defined(USE_ARAB_CONVERT) || defined(HEBREW_FONT)
	result=keyin_ulong(KI_GRAPHIC, KI_FORM_ILNN,	0,(long*)&ret_long, cap.input_length, 
							Startup.menu_type*Startup.menu_ygap,  cap.input_x*Startup.menu_xgap, 0,KI_NOPASS );   
#else
	result=keyin_ulong(KI_DEFAULT, KI_FORM_NORMAL,	0,(long*)&ret_long, cap.input_length, 
			   				Startup.menu_type*Startup.menu_ygap,  cap.input_x*Startup.menu_xgap, 0,KI_NOPASS );   
#endif
	switch (result) 
	{			
		case KP_ENTER:	if ((ret_long>cap.input_max) || (ret_long<cap.input_min)) 
				{	
					BuzOn(2);	
					goto REN1;	
				}
				break;
		case KP_ESC:	goto RET_ESC; 
	     	default:	BuzOn(2);	
				goto REN1;		     	 	
	}

	memset((INT8U*)&slave_str, 0x00, str_size);
	Flash_addr =  DFLASH_BASE + FLASH_SLAVE_INFO;;
	Flash_addr += str_size * (ret_long - 1);
	Flash_swrite(Flash_addr, (HUGEDATA INT8U *)&slave_str, str_size);

	commun_net_mask(0, NULL, ret_long, 5);	// sync plu
	commun_net_mask(1, NULL, ret_long, 5);	// sync d/c
	commun_net_mask(2, NULL, ret_long, 5);	// sync ingredient
	commun_net_mask(3, NULL, ret_long, 5);	// sync scroll msg
	SyncStatus[ret_long] = 0;		// initial

	network_search_slave();
RET_ESC:
	Dsp_Fill_Buffer(0);
	Dsp_Diffuse();
 	return;
}

INT8U netset_disnable(void)
{
	if (Startup.global_com&0x01) return FALSE;
	return TRUE;
}

BOOLEAN netset_str2ip(HUGEDATA INT8U *str, HUGEDATA INT8U *ip)
{
	if(strlen((char*)str) != 15) return FALSE;
	ip[0] = str2byte_n((char*)str, 3);
	ip[1] = str2byte_n((char*)str+4, 3);
	ip[2] = str2byte_n((char*)str+8, 3);
	ip[3] = str2byte_n((char*)str+12, 3);	
	return TRUE;
}

#ifndef CPU_PIC32MZ
void netset_MAC_set(void)
{
	INT8U mac[6];
	INT8U set_str[20];
	CAPTION_STRUCT cap;
	INT8U form[20];
	INT8U result;

	Dsp_SetPage(DSP_ERROR_PAGE);

	sprintf((char*)form, "AA.AA.AA.AA.AA.AA");

	get_base_sparam(FLASH_ETHERNET_MAC, mac, 6);

	sprintf((char *)set_str, "%02X.%02X.%02X.%02X.%02X.%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	
	Menu_Init();

	//caption_split_by_code(0x1914, &cap,0);//"4. Remote Machine Setting"
	cap.form[0] = 0;
	Menu_HeadCreate((HUGEDATA char *)cap.form);

	caption_split_by_code(0x1A15, &cap,0);//"MAC"
	cap.input_length = 17;
	cap.input_dlength = 17;
	caption_adjust(&cap,NULL);
#if defined(_USE_LCD_7_37D_40DOT_)
	cap.input_x = 13; // MENU_TYPE_IP일 때, IP를 한 줄에 볼 수있도록 강제로 좌표 지정
#elif defined(_USE_LCD_7_26d_16x96_)
	cap.input_x = 12;
#endif
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_IP, cap.input_x, cap.input_dlength, cap.input_length,
						cap.input_max, cap.input_min, 0, NULL, set_str, form, NULL);

	result = Menu_InputDisplay();
	if (result == MENU_RET_SAVE)
	{
		mac[0] = hexstrtobyte((char *)&set_str[0]);
		mac[1] = hexstrtobyte((char *)&set_str[3]);
		mac[2] = hexstrtobyte((char *)&set_str[6]);
		mac[3] = hexstrtobyte((char *)&set_str[9]);
		mac[4] = hexstrtobyte((char *)&set_str[12]);
		mac[5] = hexstrtobyte((char *)&set_str[15]);

		set_base_sparam(FLASH_ETHERNET_MAC, mac, 6);
		SaveEthernetDataForBoot(); // fw ethernet update에 사용할 ip,mac 등을 저장

		_SOFTWARE_RESET;
	}

	Dsp_SetPage(DSP_DEFAULT_PAGE);
	Dsp_Diffuse();
	
}
#endif

void netset_servicetype_callback(long num)
{
 	CAPTION_STRUCT cap;
	POINT   disp_p;
	char	buffer[MAX_MENU_CHAR_BUF];
	INT16U 	w;
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
    if(menuDispOn) return;
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

	caption_split_by_code(0x3919, &cap,0);
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
		switch(num) {
			case 0 : // 0.Disable Ethernet
				caption_split_by_code(0x1a11, &cap,0);
				break;
			case 1 : // 1.Slave
				caption_split_by_code(0x1a12, &cap,0);
				break;
			case 2 : // 2.Master
				caption_split_by_code(0x1a13, &cap,0);
				break;
			case 3 : // 3.Netwrok
				caption_split_by_code(0x1a14, &cap,0);
				break;
			case 4 : // 4.Local
				caption_split_by_code(0x1a16, &cap,0);
				break;
			default :// Invalid Value
				strcpy(cap.form,global_message[MSG_INVALID_VALUE]);
				break;
		}
		DspStruct.Reverse = 1;
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)&cap.form[2], strlen(&cap.form[2]), 0, 0); 
		hebrew_codetemp[strlen(&cap.form[2])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)&cap.form[2], strlen(&cap.form[2]), 0, 0, 0);
		arab_codetemp[strlen(&cap.form[2])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
	#ifdef USE_USA_DEFAULT
		Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)&form_graphic);
	#else
		Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)&cap.form[2]);
	#endif
  #else
		Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form[2]);	      //se-hyung 20071130 글씨 짤리는 현상 수정
  #endif
#endif
		DspStruct.Reverse = 0;
	}
}

void netset_rs232c_baud_callback(long num)
{
 	CAPTION_STRUCT cap;
	POINT 	disp_p;
	char	buffer[MAX_MENU_CHAR_BUF];
	INT16U 	w;
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
    if(menuDispOn) return;
#endif	
#ifdef USE_CL5200_DISPLAY_DEFAULT
	CallbackEditPosition_y = 1;
	CallbackEditPosition_x = 8;
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

	caption_split_by_code(0x1a30, &cap,0);
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
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
		Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
  #else
		Dsp_Write_String(disp_p,(HUGEDATA char *)global_message[MSG_INVALID_VALUE]);
  #endif
#endif
	} else {
		caption_split_by_code(0x1a31+num, &cap,0);
		DspStruct.Reverse = 1;
#ifdef HEBREW_FONT
		Convert_HebrewPreview((INT8U *)&cap.form[2], strlen(&cap.form[2]), 0, 0); 
		hebrew_codetemp[strlen(&cap.form[2])] = 0;
		Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
		Convert_ArabPreview((INT8U *)&cap.form[2], strlen(&cap.form[2]), 0, 0, 0);
		arab_codetemp[strlen(&cap.form[2])] = 0;
		Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
		Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)&cap.form[2]);
  #else
        Dsp_Write_String(disp_p,(HUGEDATA char *)&cap.form[2]);
  #endif
#endif
		DspStruct.Reverse = 0;
	}
}

void netset_usewep_callback(long val)
{
	INT8U wepid_id, wepkey_id;
	CAPTION_STRUCT cap;

	wepid_id  = Menu_GetMenuId(M_ID_WEPID);
	wepkey_id = Menu_GetMenuId(M_ID_WEPKEY);
	if(wepid_id == 0xff || wepkey_id == 0xff) return;

	switch(val) {
		case 0 : // Disable
			Menu[wepid_id].inhibit = ON;	// WEP Key ID
			Menu[wepkey_id].inhibit = ON;	// WEP Key1
			break;
		case 1 : // 64
			Menu[wepid_id].inhibit = OFF;	// WEP Key ID
			Menu[wepkey_id].inhibit = OFF;	// WEP Key1
			Menu[wepkey_id].length = 5;
			caption_split_by_code(0x3964, &cap, 0);
			cap.input_length = Menu[wepkey_id].length;
			//cap.input_dlength = Menu[wepkey_id].length;
			caption_adjust(&cap, NULL);
			Menu[wepkey_id].w_length = cap.input_dlength;
			strcpy(Menu[wepkey_id].str, cap.form);
			break;
		case 2 : // 128
			Menu[wepid_id].inhibit = OFF;	// WEP Key ID
			Menu[wepkey_id].inhibit = OFF;	// WEP Key1
			Menu[wepkey_id].length = 13;
			caption_split_by_code(0x3964, &cap, 0);
			cap.input_length = Menu[wepkey_id].length;
			//cap.input_dlength = Menu[wepkey_id].length;
			caption_adjust(&cap, NULL);
			Menu[wepkey_id].w_length = cap.input_dlength;
			strcpy(Menu[wepkey_id].str, cap.form);
			break;
		case 3 : // WPA(tkip)
		case 4 : // WPA2(tkip)
		case 5 : // WPA2(AES)
			Menu[wepid_id].inhibit = ON;	// WEP Key ID
			Menu[wepkey_id].inhibit = OFF;	// WEP Key1
			Menu[wepkey_id].length = 64;
			caption_split_by_code(0x3964, &cap, 0);
			cap.input_length = Menu[wepkey_id].length;
			//cap.input_dlength = Menu[wepkey_id].length;
			caption_adjust(&cap, NULL);
			Menu[wepkey_id].w_length = cap.input_dlength;
			strcpy(Menu[wepkey_id].str, cap.form);
			break;
		default :// Invalid Value
			return;
			
	}

	MenuRefresh = ON;
}

void netset_select_wepid_callback(long val)
{
	INT8U wepkey_id, usewep_id;

	wepkey_id = Menu_GetMenuId(M_ID_WEPKEY);
	if(wepkey_id == 0xff) return;
	usewep_id = Menu_GetMenuId(M_ID_USEWEP);
	if(usewep_id == 0xff) return;

	if (*Menu[usewep_id].pval >= 3)	// 3:WPA(TKIP), 4:WPA2(TKIP), 5:WPA2(AES)
	{
		get_net_sparam(NETWORK_WLAN_WEP,(INT8U *)Menu[wepkey_id].pstr,64);
		Menu[wepkey_id].pstr[64] = 0;
	}
	else
	{
		if ((val>=1) && (val<=4)) {
			get_net_sparam(NETWORK_WLAN_WEP+16*(val-1),(INT8U *)Menu[wepkey_id].pstr,16);
		} else {
			memset(Menu[wepkey_id].pstr,0,16);
		}
		Menu[wepkey_id].pstr[16] = 0;
	}

	MenuRefresh = ON;
}

void netset_wl_setting_rs232(void)
{
	POINT disp_p;
	INT8U old_page, old_font;
	char string_buf[36];

	old_page = DspStruct.Page;
	old_font = DspStruct.Id1;
	Dsp_SetPage(DSP_ERROR_PAGE);
	DspLoadFont1(DSP_MEDIUM_FONT_ID);

	Dsp_Fill_Buffer(0);

	disp_p = point(0, 0);
#ifdef USE_PERSIA_DEFAULT
	sprintf((char*)string_buf, "渴無?局??흡悳?");
#else
	sprintf((char*)string_buf, "Wireless Bridge Setting");
#endif
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
	Dsp_Write_String(disp_p,(HUGEDATA char*)string_buf);
#endif
	Dsp_Diffuse();

	EthTelnetToBridge();

	Dsp_SetPage(old_page);
	DspLoadFont1(old_font);
	Dsp_Diffuse();
}

#ifdef USE_CHECK_WLAN_STATUS_LANTRONIX
void netset_wirless_module_get_status(void)
{
	POINT disp_p;
	INT8U old_page;
	INT8U old_font;
	INT16U prev;
	char string_buf[32];
	ap_info *recv_ap_info;
	INT8U status = 1;
	
	old_page = DspStruct.Page;
	old_font = DspStruct.Id1;
	Dsp_SetPage(DSP_ERROR_PAGE);
	DspLoadFont1(DSP_SMALL_FONT_ID);

	Dsp_Fill_Buffer(0);

	while(status)
	{
		if((INT16U)(SysTimer100ms-prev) >= 50)
		{
			prev = SysTimer100ms;
			recv_ap_info = wlan_Check_ConnInfo_Bridge();
			Dsp_Fill_Buffer(0);
			VFD7_Fill_Buffer(0);
			Dsp_Diffuse();
			VFD7_Diffuse();
			switch (status)
			{
				case 1:
					if(recv_ap_info->fwversion[0] != 0)
					{
						sprintf(string_buf, "Module Ver");
						disp_p = point(0, 0);
						Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf);
						sprintf(string_buf, "%s", recv_ap_info->fwversion);
						display_string_pos(8, (INT8U*)&string_buf);
					}
					else
						goto END;
					break;
				case 2:
					if(recv_ap_info->fwversion[0] != 0)
					{
						sprintf(string_buf, "Module MAC");
						disp_p = point(0, 0);
						Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf);
						sprintf(string_buf, "%02X.%02X.%02X.%02X.%02X.%02X" , recv_ap_info->rsimac[0]
																			, recv_ap_info->rsimac[1]
																			, recv_ap_info->rsimac[2]
																			, recv_ap_info->rsimac[3]
																			, recv_ap_info->rsimac[4]
																			, recv_ap_info->rsimac[5]);
						display_string_pos(12, (INT8U*)&string_buf);
					}
					else
						goto END;
					break;
				case 3:
					if(recv_ap_info->connState == 1)
					{
//						sprintf(string_buf, "SSID");
						sprintf(string_buf, "%s", recv_ap_info->ssid);
						disp_p = point(0, 0);
						Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf);
					}
					else
						status = 7;
					break;
				case 4:
					if(recv_ap_info->connState == 1)
					{
						sprintf(string_buf, "AP MAC");
						disp_p = point(0, 0);
						Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf);
						sprintf(string_buf, "%02X.%02X.%02X.%02X.%02X.%02X", recv_ap_info->bssid[0]
																			, recv_ap_info->bssid[1]
																			, recv_ap_info->bssid[2]
																			, recv_ap_info->bssid[3]
																			, recv_ap_info->bssid[4]
																			, recv_ap_info->bssid[5]);
						display_string_pos(12, (INT8U*)&string_buf);
					}
					else
						status = 7;
					break;
				case 5:
					if(recv_ap_info->connState == 1)
					{
						sprintf(string_buf, "CH=%d", recv_ap_info->channel);
						disp_p = point(0, 0); 
						Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf);
					}
					else
						status = 7;
					break;
				case 6:
					if(recv_ap_info->connState == 1)
					{
						sprintf(string_buf, "RSSI=-%ddBm", recv_ap_info->rssiVal);
						disp_p = point(0, 0); 
						Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf);
					}
					else
						status = 7;
					break;
				case 7:
					if(recv_ap_info->connState == 0)
					{
						disp_p = point(0, 0); 
						sprintf(string_buf, "Disconnected");
						Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf);
						status = 1;
					}
					break;
			}
			Dsp_Diffuse();
			VFD7_Diffuse();
		}
		if (KEY_Read()) {
			prev = 0;
			switch(KeyDrv.CnvCode) {
				case KP_ARROW_DN :
				case KP_PAGE_DN :
					if (status < 6) {
						status++;
						BuzOn(1);
					}
					else
						BuzOn(2);
					break;
				case KP_ARROW_UP :
				case KP_PAGE_UP :
					if (status > 1) {
						status--;
						BuzOn(1);
					}
					else
						BuzOn(2);
					break;
				case KP_ESC : 		// CANCEL
					BuzOn(1);
					status = 0;
					break;
				default :
					BuzOn(2);
					break;
			}
		}
	}
END:
	KEY_Read();
	Dsp_SetPage(old_page);
	DspLoadFont1(old_font);
	Dsp_Diffuse();
	VFD7_Diffuse();
}

void netset_wl_get_status(void)
{
	POINT disp_p;
	INT8U old_page, old_font;
	char string_buf[4][36];
	char string_buf1[32];
	
	// default bridge ip
	INT8U ip[4] = {10,10,4,254};
	INT8U ip_l[4] = {10,10,4,253};
	INT8U gw[4] = {10,10,0,1};
	INT8U sn[4] = {255,0,0,0};
	
	INT8U i, j;
	INT8U r_data;
	INT8U state;
	//INT8U displayFlag;
	INT8U completeFlag;
	INT8U sendFlag;
	INT16U prev;
	INT8U result;
	INT8U line;
	
	if (Startup.wireless_type != WLTYPE_LANTRONIX)
	{
		BuzOn(2);
		return;
	}

	old_page = DspStruct.Page;
	old_font = DspStruct.Id1;
	Dsp_SetPage(DSP_ERROR_PAGE);
	DspLoadFont1(DSP_SMALL_FONT_ID);

	Dsp_Fill_Buffer(0);

	disp_p = point(0, 0);
#ifdef USE_PERSIA_DEFAULT
	sprintf((char*)string_buf1, "膏蔬 局??흡悳?");
#else
	sprintf((char*)string_buf1, "Wireless Bridge Status");
#endif
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)string_buf1, strlen(string_buf1), 0, 0); 
	hebrew_codetemp[strlen(string_buf1)] = 0;
	disp_p.x = 202 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)string_buf1, strlen(string_buf1), 0, 0, 0); 
	arab_codetemp[strlen(string_buf1)] = 0;
	disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
	Dsp_Write_String(disp_p,(HUGEDATA char*)string_buf1);
#endif
	Dsp_Diffuse();

	
	

	wlan_SetNetwork(ip_l, gw, sn);	// change local IP for w/l setting
	EthCardStruct.TCPDestIP[3] = 10;
	EthCardStruct.TCPDestIP[2] = 10;
	EthCardStruct.TCPDestIP[1] = 4;
	EthCardStruct.TCPDestIP[0] = 254;
	EthCardStruct.TCPDestPort = 9999;
	result = Ethernet_Server(CLIENT_OPEN, 50, CLIENT_NORMAL);

	sendFlag = OFF;
	if (result)
	{
		sprintf(string_buf1, "MMMMMMMMMM");
		commun_outleng(&(CommBufEthData[0]), string_buf1, 10);
		
		prev = SysTimer100ms - 10;
		while (1)
		{
			network_tx_proc();
			if (KEY_IsKey()) break;
			
			if (!sendFlag && (INT16U)(SysTimer100ms-prev) >= 10)
			{
				prev = SysTimer100ms;
				sprintf(string_buf1, "NS\r\n");
				commun_outleng(&(CommBufEthData[0]), string_buf1, 4);
				completeFlag = OFF;
				sendFlag = ON;
				state = 0;
				line = 0;
			}
			else if ((INT16U)(SysTimer100ms-prev) >= 100)
			{
				sendFlag = OFF;
			}
				 
			
			while(CheckRxBuf(&(CommBufEthData[0])))
			{
				r_data = GetCharRxBuf(&(CommBufEthData[0]));
				switch (state)
				{
					case 0:
						if (r_data == _CR) state = 1;
						break;
					case 1:
						if (r_data == 0) state = 2;
						else state = 0;
						break;
					case 2:
						if (r_data == LF) state = 3;
						else state = 0;
						i = 0;
						break;
					case 3:
						if (r_data == '0') state = 10;	// 0>(END)
						else if (r_data == 'R') state = 20;	// RSSI
						else if (r_data == 'S') state = 30;	// SSID
						else if (r_data == 'C') state = 40;	// Chan
						else 
						{
							state = 0;
							break;
						}
						string_buf[line][i++] = r_data;
						break;
					case 4:
						if (r_data == _CR)
						{
							state = 1;
							//displayFlag = ON;
							string_buf[line][i++] = 0;
							line++;
							if (line > 3) line = 3;
						}
						else
						{
							string_buf[line][i++] = r_data;
						}
						break;
					case 10:
						if (r_data == wl_networkstatus_0_Prompt[i]) 
						{
							string_buf[line][i] = r_data;
							i++;
							if (strlen((char*)wl_networkstatus_0_Prompt) == i)
							{
								completeFlag = ON;
								state = 0;
							}
						}
						else
						{
							state = 0;
						}
						break;
					case 20:
						if (r_data == wl_networkstatus_RSSI[i]) 
						{
							string_buf[line][i] = r_data;
							i++;
							if (strlen((char*)wl_networkstatus_RSSI) == i)
								state = 4;
						}
						else
						{
							state = 0;
						}
						break;
					case 30:
						if (r_data == wl_networkstatus_SSID[i]) 
						{
							string_buf[line][i] = r_data;
							i++;
							if (strlen((char*)wl_networkstatus_SSID) == i)
								state = 4;
						}
						else
						{
							state = 0;
						}
						break;
					case 40:
						if (r_data == wl_networkstatus_Chan[i]) 
						{
							string_buf[line][i] = r_data;
							i++;
							if (strlen((char*)wl_networkstatus_Chan) == i)
								state = 4;
						}
						else
						{
							state = 0;
						}
						break;
						
				}
			}
			if (completeFlag)
			{
				sendFlag = OFF;
				completeFlag = OFF;
				Dsp_Fill_Buffer(0);
				for (j = 0; j < line; j++)
				{
					disp_p = point((j)*8, 0);	
#ifdef HEBREW_FONT
					Convert_HebrewPreview((INT8U *)&string_buf[j][0], strlen(&string_buf[j][0]), 0, 0); 
					hebrew_codetemp[strlen(&string_buf[j][0])] = 0;
					disp_p.x = 202 - disp_p.x;
					Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
					Convert_ArabPreview((INT8U *)&string_buf[j][0], strlen(&string_buf[j][0]), 0, 0, 0); 
					arab_codetemp[strlen(&string_buf[j][0])] = 0;
					disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
					Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
					Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf[j][0]);
#endif
					string_buf[j][0] = 0;	// clear
				}
				Dsp_Diffuse();
			}
		}
						
		KEY_Read();
		
		sprintf(string_buf1, "QU\r\n");
		commun_outleng(&(CommBufEthData[0]), string_buf1, 4);
		
		Ethernet_Server(CLIENT_CLOSE, 0, CLIENT_NORMAL);
	}

	Ethernet_RestoreNetwork();// restore local IP
	
	Dsp_SetPage(old_page);
	DspLoadFont1(old_font);
	Dsp_Diffuse();
}

void netset_wl_scan_ssid(void)
{
	POINT disp_p;
	INT8U old_page, old_font;
	char string_buf[5][100];
	char string_buf1[32];
	
	// default bridge ip
	INT8U ip[4] = {10,10,4,254};
	INT8U ip_l[4] = {10,10,4,253};
	INT8U gw[4] = {10,10,0,1};
	INT8U sn[4] = {255,0,0,0};
	
	INT8U i, j;
	INT8U r_data;
	INT8U state;
	INT8U completeFlag;
	INT16U prev;
	INT8U line;
	INT8U result;
	
	if (Startup.wireless_type != WLTYPE_LANTRONIX)
	{
		BuzOn(2);
		return;
	}	

	old_page = DspStruct.Page;
	old_font = DspStruct.Id1;
	Dsp_SetPage(DSP_ERROR_PAGE);
	DspLoadFont1(DSP_SMALL_FONT_ID);

	Dsp_Fill_Buffer(0);

	disp_p = point(0, 0);
#ifdef USE_PERSIA_DEFAULT
	sprintf((char*)string_buf1, "膏蔬 局??흡悳?");
#else
	sprintf((char*)string_buf, "Wireless Bridge Status");
#endif
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)&string_buf[0], strlen(string_buf[0]), 0, 0); 
	hebrew_codetemp[strlen(string_buf[0])] = 0;
	disp_p.x = 202 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)&string_buf[0], strlen(string_buf[0]), 0, 0, 0); 
	arab_codetemp[strlen(string_buf[0])] = 0;
	disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
	Dsp_Write_String(disp_p,(HUGEDATA char*)string_buf);
#endif
	Dsp_Diffuse();

	
	

	wlan_SetNetwork(ip_l, gw, sn);	// change local IP for w/l setting
	EthCardStruct.TCPDestIP[3] = 10;
	EthCardStruct.TCPDestIP[2] = 10;
	EthCardStruct.TCPDestIP[1] = 4;
	EthCardStruct.TCPDestIP[0] = 254;
	EthCardStruct.TCPDestPort = 9999;
	result = Ethernet_Server(CLIENT_OPEN, 50, CLIENT_NORMAL);
	
	if (result)
	{
		sprintf(string_buf1, "MMMMMMMMMM");
		commun_outleng(&(CommBufEthData[0]), string_buf1, 10);
	
		prev = SysTimer100ms - 50;
		while (1)
		{
			network_tx_proc();
			if (KEY_IsKey()) break;
			
			if ((INT16U)(SysTimer100ms-prev) >= 50)
			{
				prev = SysTimer100ms;
				sprintf(string_buf1, "SA\r\n");
				commun_outleng(&(CommBufEthData[0]), string_buf1, 4);
				completeFlag = OFF;
				state = 0;
				line = 0;
			}
			
			
			while(CheckRxBuf(&(CommBufEthData[0])))
			{
				r_data = GetCharRxBuf(&(CommBufEthData[0]));
				switch (state)
				{
					case 0:
						if (!completeFlag)
						{
							if (r_data == _CR) state = 1;
						}
						break;
					case 1:
						if (r_data == 0) state = 2;
						else state = 0;
						break;
					case 2:
						if (r_data == LF) state = 3;
						else state = 0;
						i = 0;
						line = 0;
						break;
					case 3:
						if (i == 0 && r_data == '0')
						{
							state = 10;
							//break;
						}
						if (r_data == _CR) 
						{
							state = 4;
							string_buf[line][i++] = 0;
							break;
						}
						string_buf[line][i++] = r_data;
						break;
					case 4:
						if (r_data == 0) 
						{
							state = 5;
						}
						else
						{
							state = 0;
						}
						break;
					case 5:
						if (r_data == LF) 
						{
							line++;
							i = 0;
							state = 3;
							if (line >= 5) 
							{
								completeFlag = ON;
								state = 0;
							}
						}
						else
						{
							state = 0;
						}
						break;
					case 10:
						if (r_data == '>')
						{
							completeFlag = ON;
							state = 0;
						}
						else
						{
							string_buf[line][i++] = r_data;
							state = 3;
						}
						break;
						
				}
			}
			if (completeFlag)
			{
				completeFlag = OFF;
				Dsp_Fill_Buffer(0);
				for (j = 0; j < line; j++)
				{
					disp_p = point((j)*8, 0);	
#ifdef HEBREW_FONT
					Convert_HebrewPreview((INT8U *)&string_buf[j][19], strlen(&string_buf[j][19]), 0, 0); 
					hebrew_codetemp[strlen(&string_buf[j][19])] = 0;
					disp_p.x = 202 - disp_p.x;
					Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
					Convert_ArabPreview((INT8U *)&string_buf[j][19], strlen(&string_buf[j][19]), 0, 0, 0); 
					arab_codetemp[strlen(&string_buf[j][19])] = 0;
					disp_p.x = LCD_Y_MARGIN_REAL - DspStruct.Hbits1 - disp_p.x;
					Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
					Dsp_Write_String(disp_p,(HUGEDATA char*)&string_buf[j][19]);
#endif
					string_buf[j][0] = 0;	// clear
				}
				Dsp_Diffuse();
			}
		}
						
		KEY_Read();
	
		
		sprintf(string_buf[0], "QU\r\n");
		commun_outleng(&(CommBufEthData[0]), string_buf[0], 4);
		
		Ethernet_Server(CLIENT_CLOSE, 0, CLIENT_NORMAL);
	}

	Ethernet_RestoreNetwork();// restore local IP

	Dsp_SetPage(old_page);
	DspLoadFont1(old_font);
	Dsp_Diffuse();

}
#endif //#ifdef USE_CHECK_WLAN_STATUS_LANTRONIX

void callback_security_type(long num)
{
	POINT disp_p;
	char	buffer[MAX_MENU_CHAR_BUF];
	INT16U  w;
#ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
	if(menuDispOn) return;
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
	Dsp_Write_String(disp_p, buffer);
  #endif
#endif

	switch(num)
	{
		case 0 :
			sprintf(MsgBuf, "NONE");
			break;
		case 1 :
			sprintf(MsgBuf, "WEP,64");
			break;
		case 2 :
			sprintf(MsgBuf, "WEP,128");
			break;
		case 3 :
			sprintf(MsgBuf, "WPA,TKIP");
			break;
		case 4 :
			sprintf(MsgBuf, "WPA2,TKIP");
			break;
		case 5 :
			sprintf(MsgBuf, "WPA2,AES");
			break;
		default : 
			sprintf(MsgBuf, global_message[MSG_INVALID_VALUE]);
			break;
	}
#ifdef HEBREW_FONT
	Convert_HebrewPreview((INT8U *)MsgBuf, strlen(MsgBuf), 0, 0); 
	hebrew_codetemp[strlen(MsgBuf)] = 0;
	Dsp_Write_1stringLong(disp_p, hebrew_codetemp);
#elif defined(USE_ARAB_CONVERT)
	Convert_ArabPreview((INT8U *)MsgBuf, strlen(MsgBuf), 0, 0, 0); 
	arab_codetemp[strlen(MsgBuf)] = 0;
	Dsp_Write_1stringLong(disp_p, arab_codetemp);
#else
  #ifdef USE_MENU_DISPLAY_ON_GRAPHIC_AREA
	Dsp_Write_Reverse_String(disp_p,(HUGEDATA char *)MsgBuf);
  #else
	Dsp_Write_String(disp_p, MsgBuf);
  #endif
#endif
}


INT8U network_block_copy(INT32U from,INT32U to,INT8U plu,HUGEDATA COMM_BUF *pCommBuf)
{
	INT8U  deptno;
	INT32U addr_start,send_size;
	INT16U plu_addr;
	INT16S    nth,i,rem;
	INT8U  end;
	INT16U	wait;

	nth = 0;
	rem = 0;
	end = 0;
	addr_start=0l;
	send_size =0l;
	switch (plu) {
	    case  3: break;
	    case  1: nth = (INT16S)plu_rem_number_get(); break;
	    default: addr_start = from;
		     send_size  = to - from;
		     nth        = send_size / 512l;
		     rem        = send_size % 512l;
		     break;
	}
	display_clear(DISPLAY_PRICE    );
	display_clear(DISPLAY_UNITPRICE);
	memset(gmsg,0,30);
	if (plu==3) {
		sprintf(gmsg,"P-SET");
	} else if (plu==1) {
		sprintf(gmsg,"%05d",nth);
		display_string(DISPLAY_PRICE,(INT8U *)gmsg);
		sprintf(gmsg,"S-PLU");
	} else {
		sprintf(gmsg,"%05d",nth);
		display_string(DISPLAY_PRICE,(INT8U *)gmsg);
	}
	display_string(DISPLAY_UNITPRICE,(INT8U *)gmsg);
	VFD7_Diffuse();
	if (plu>1) goto NEXT;
	if (rem) nth++;
	if((plu == 1)&&(nth == 0))
	{
		end=2;
		goto NEXT;
	}
	for (i=0; i<nth; i++) {
TRYAGAIN1:
		switch (plu) {
		   case 1:
			network_status.backup_plu = 0;
			send_size  = plu_table_addr[i].pluno;
			deptno     = (INT8U)(send_size>>24); 
			send_size &= 0x00ffffff;
			plu_table_search((INT8U)deptno,(long)send_size,(INT16U *)&plu_addr,0);
			recv_counter=0;
			if (plu_addr) {
				end=1;
				commun_send_plu_from_address(pCommBuf,plu_addr,0,1);
			}
			sprintf(gmsg,"P%05d",plu_addr);
			break;
		   default:
			global_one2one_send = 0;
			if (i==nth-1) {
				end=1;
				if (rem) {
					commun_send_block_addr(pCommBuf,addr_start,2,(INT16U)rem);

				} else {
					commun_send_block_addr(pCommBuf,addr_start,2,512);
				}
			} else {
				commun_send_block_addr(pCommBuf,addr_start,1,512);
			}
			sprintf(gmsg,"N%05d",i);
			break;
		}
		display_string(DISPLAY_UNITPRICE,(INT8U *)gmsg);
		VFD7_Diffuse();
		
		wait = SysTimer100ms;
		while (1) 
		{
			network_common_trans();
			if (plu==1) {
				if (network_status.backup_plu) break;
			} else {
				if (global_one2one_send==9) {
					end=2;
					goto END2;
				}
				if (global_one2one_send) break;
			}
			if (KEY_Read()) {
				BuzOn(1);
				if (KeyDrv.CnvCode==KS_CLEAR) {
					end=2;
					goto NEXT;
				}
				if (KeyDrv.CnvCode==KP_ESC) {
					end=3;
					goto NEXT;
				}
			}
			if((INT16U)(SysTimer100ms-wait) > 30) 
			{
				Ethernet_Server(CLIENT_OPEN, 0, CLIENT_BACKUP_TO_SCALE);
				goto TRYAGAIN1;
			}

		}

		if (!plu) addr_start+=512;
	}
NEXT:	
	if (end==0) {
TRYAGAIN2:
		global_one2one_send=0;
		if (plu) {
		    if (plu!=1) {
			commun_send_block_addr(pCommBuf,addr_start,plu,(INT16U)0);
		    }
		} else {
			commun_send_block_addr(pCommBuf,addr_start,2,(INT16U)0);
		}
		
		wait = SysTimer100ms;
		while (1) {
			network_common_trans();
			if (global_one2one_send) {
				break;
			}
			if (KEY_Read()) {
				BuzOn(1);
				if (KeyDrv.CnvCode==KS_CLEAR) {
					end=2;
					goto NEXT;
				}
				if (KeyDrv.CnvCode==KP_ESC) {
					end=3;
					goto NEXT;
				}
			}
			if((INT16U)(SysTimer100ms-wait) > 30) goto TRYAGAIN2;
		}
	}
END2:
	return end;
}

void netset_slave_dataUpdate(INT32U num)
{
	ETHERNET_SLAVE_SCALE  slave_str;
	INT16U  strSize;
	INT32U	addr;
	INT8U   slaveInfoIndex;
	INT8U	ip[4];

	strSize = sizeof(slave_str);		      
	addr = DFLASH_BASE;
	addr += FLASH_SLAVE_INFO;
	addr += ((num - 1) * strSize);
	slaveInfoIndex = Flash_bread(addr);
	if(slaveInfoIndex == num){
		Flash_sread(addr, (HUGEDATA INT8U *)&slave_str, strSize);
	} else {
		memset(&slave_str.code, 0, strSize);
		slave_str.tcp_port = 20304;
		slave_str.code = num;		
	}
	*Menu[0].pval = slave_str.code;
	memcpy(ip, (HUGEDATA char*)slave_str.ip, 4);
	sprintf((char*)Menu[1].pstr, "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);
	*Menu[2].pval = slave_str.tcp_port;
	*Menu[3].pval = slave_str.use;

      	MenuRefresh = ON;
}

//flag on  : zero key, reset
//flag off : test key, complete
INT32U netset_reset_confirm(INT8U flag)	 	
{
	CAPTION_STRUCT 		cap;
	INT8U   result,old_font,key_mode;
	INT8U	ret;
	INT8U 	old_page;
	INT32U 	scale_id;
	char	string_buf[50];

	scale_id = 0;
	ret = OFF;
	old_page = DspStruct.Page;	

	Dsp_SetPage(DSP_ERROR_PAGE);
	Dsp_Fill_Buffer(0);
	display_string_pos_clear(5, 20);

	key_mode=KEY_GetMode();
	KEY_SetMode(1);
	old_font=DspStruct.Id1;
	DspLoadFont1(DSP_MENU_FONT_ID);

	Menu_Init();
	
	if (flag) sprintf(string_buf, "   Init");
	else sprintf(string_buf, "   Finish");
	Menu_HeadCreate((HUGEDATA char *)string_buf);

	caption_split_by_code(0x3918, &cap,0);
	if (cap.input_max > MAX_SLAVE_SCALE) cap.input_max = MAX_SLAVE_SCALE;
	caption_adjust(&cap,NULL);
	Menu_InputCreate(1, (HUGEDATA char *)cap.form, MENU_TYPE_NUM, cap.input_x, cap.input_dlength, cap.input_length,
				cap.input_max, cap.input_min, 0, &scale_id, NULL, NULL, NULL);

	result = Menu_InputDisplay();

	if (result == MENU_RET_SAVE) 
	{
		if (Menu_ConfirmDisplay(0)) ret = (INT8U)scale_id;
	}
	else if (result == MENU_RET_ESC) ret = OFF;

	Dsp_Fill_Buffer(0);
	Dsp_SetPage(old_page);
	Dsp_Diffuse();
	DspLoadFont1(old_font);
	KEY_SetMode(key_mode);
	sale_display_update(0x0fff);

	return ret;
}
