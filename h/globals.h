/*****************************************************************************
|*			
|*  Copyright		:	(c) 2002 CAS
|*  Filename		:	globals.h
|*  Version			:	0.1
|*  Programmer(s)	:	Hong Yi-Phyo (HYP)
|*  Created			:	2002/11/11
|*  Description		:	CLP Printer global variable define    
|*				
****************************************************************************/

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "app.h"

#define	_USE_SERIAL_   //use 232 port  only CLP Printer 

//////////////////////////////////////////////////////////////////////////
// Compile Condition START (Single choice in one conditon)
//////////////////////////////////////////////////////////////////////////

/*
********************************************************************************
* 			                   Compile Condition 1
* 			                   < Model Type >
********************************************************************************
*/
#define CL3000_BP      //CL3000-BP
//#define CL5200J_BP     //CL5200J-BP
/********************************************************************************/


/*
********************************************************************************
* 			                   Compile Condition 2
* 			                        < CPU Type >
********************************************************************************
*/
#define USE_PIC32MZ_EFG
//#define USE_PIC32MZ_ECG
/********************************************************************************/


/*
********************************************************************************
* 			                   Compile Condition 3-1
* 			                   < Nation Define >
* 		< Dealer Code >
* 		Used Code : A  B  D E F G H I J K L M N   P Q R S T U V W     Z "TR" "DM" "TA" "RT" "PT" "UC"
* 		              a b c   e   g h i   k l m n   p q r s   u      y z "sp"
* 		Remained Code :     C                   O                 X Y       
* 		                    d   f       j         o         t    v w x 
********************************************************************************
*/
//------- ���� �߿��� ��� -------//
#define USE_STANDARD        					// ����

//------- ��Ÿ �߰����� ���� Ȯ�� �ʿ��� �߿��� -------//
//#define USE_ARAB							// (A) �ƶ� (��Ʈ)
//#define USE_EUROPE_WITHOUT_TICKET			// (u) Ƽ�ϱ���� ������� �ʴ� ���� ���� �߿���
//#define USE_RUSSIA						// (R) ���þ�

//------- ���� �߿��� (���ĺ�, �������� ����) -------//
//#define USE_EMART           					// (E) E-Mart
//#define USE_EMART_CL5200     					// (m) CL5200 �̸�Ʈ (�̷°������ ����)
//#define USE_GSMART          					// (G) GS ��Ʈ ����
//#define USE_GSMART_TRACE    					// (M) GS ��Ʈ ��� ����
//#define USE_HYUNDAI       					// (H) �����ȭ�� ����
//#define USE_IMPORT_MEAT     					// (I) ������ ��ü�̷� (������ ���� ��ü)
//#define USE_IMPORT_MEAT_CART    				// (s) ������ ���� ��ü(CART)(Pb Bat)
//#define USE_KOREA_CL5200     					// (k) CL5200 ���� ���� (�̷°��� ��� ����)
//#define USE_LOTTEMART       					// (J) Lotte Mart(���� ������ + GS1 DataBar Expand)
//#define USE_LOTTESUPER						// (U) Lotte Super(���� ���������� �Ļ�)
//#define USE_LOTTESUPER_8LINE 					// (W) Lotte Super ��ǰ�� 8��(����ī ���)
//#define USE_NHMART          					// (N) ���� ����
//#define USE_NHMART_SAFE     					// (n) ���� �Ƚ� �ѿ�
//#define USE_SCALE_POS       					// (T) ���ﳪ�� �̷����� => CL5200J���� ���ŵ� (���� ��û���� CL3000 ���ﳪ�� �߿��� �Ļ�)
//#define USE_SHINSEGAE_MEAT     				// (S) �ż��� ��ȭ��

//------- �ؿ� �߿��� (���ĺ� ����) -------//
//#define USE_ARAB_EMIRATES						// (a) �ƶ����̷���Ʈ
//#define USE_BELARUS_EVROOPT       			// (q) ����罺 EVROOPT SELF
//#define USE_CANADA_LABEL    					// (L) ĳ���� �� ���� ���(200mm)
//#define USE_CHINESE_FONT_BIG5					// TR tranditional chinese (big5)
//#define USE_CHINESE_FONT_GB2312 				// (B) simple chinese (gb2312)
//#define USE_CHN_STANDARDMARKET				// (c) �߱�ǥ��ȭ����
//#define USE_CHN_TRACE_STANDARD   				// (F) chinese trace standard (gb2312) <= chn E-Mart
//#define USE_GBR_HMART       					// (P) ���� H-MART ����(�ѱ���Ʈ)
//#define USE_HEBREW							// (h) ���긮�� (�̽���)
//#define USE_INDIA_DADUS						// DM �ε�(DADUS MITHAI VATIKA)
//#define USE_INDIA_TATA						// TA �ε� TATA Group ���� �߿���
//#define USE_INDONESIA_EMPORIUM				// (e) �ε��׽þ� �������� ����
//#define USE_MANAGE_INVENTORY					// (i) ���� ������ �� ���� ���.
//#define USE_MON_EMART							// (g) CL5500 ���� �̸�Ʈ
//#define USE_NINGBO_RETAIL_STANDARDMARKET		// (z) �߱��׺�ǥ��ȭ����(�Ҹű��) "/Branch/CarScale_Ningbo_Retail"
//#define USE_NINGBO_STANDARDMARKET				// (b) �߱��׺�ǥ��ȭ���� "/Branch/CarScale_Ningbo"
//#define USE_PERSIA							// (p) Persian(�̶�)
//#define USE_RUSSIA_INDIRECT  					// (Q) ���þ�(Indirect Ingredient Version)
//#define USE_RUSSIA_LOGISTICS     				// (l) ���þ� ���� ���� 
//#define USE_RUSSIA_RTT_TARE					// RT ���þ� ���� �� (RTT �������� TARE ������ �߰�)
//#define USE_RUSSIA_SPAR         				// (sp) ���þ� SPAR ��Ʈ 
//#define USE_SAUDI								// PT �ƶ���(����ƶ���)
//#define USE_SRILANKA    						// (K) ������ī
//#define USE_UKRAINE               			// (Z) ��ũ���̳�
//#define USE_UKRAINE_CLASSMART					// UC ��ũ���̳� CLASS MART ���� ��
//#define USE_VIETNAM          					// (V) ��Ʈ��

//------- ��� ���Ļ� �� �̻�� �߿��� -------//
//#define USE_BELARUS							// (r) ����罺
//#define USE_CHN_NEW_STANDARDMARKET			// (a) �߱� ǥ��ȭ����(2013������,���÷�����������) "/Branch/CarScale_RemoteDisplay/"
//#define USE_MANAGE_INVENTORY_CART				// (y) ���������� ��� �߰��� īƮ ���� ���� (2024�� ���� ���� ��� X)
/********************************************************************************/


/*
********************************************************************************
* 			                   Compile Condition 3-2
* 			                   < Country Define >
* 		< ���� ���(USE_STANDARD) ���� ���� >
* 		������ ���� ������ ��� �ּ�ó��
********************************************************************************
*/
#ifdef USE_STANDARD
/********************************************************************************/
//#define COUNTRY_AUSTRALIA						// ȣ��
//#define COUNTRY_CANADA						// ĳ����
//#define COUNTRY_CHINA   						// �߱� Heap Size : 64000
//#define COUNTRY_DOMINICA						// ���̴�ī
//#define COUNTRY_GEORGIA						// ������
//#define COUNTRY_GERMANY						// ����
//#define COUNTRY_HUNGARY						// �밡��
//#define COUNTRY_INDIA   						// �ε� Heap Size : 64000
//#define COUNTRY_INDONESIA						// �ε��׽þ� Indonesia Lotte-Mart
//#define COUNTRY_KENYA							// �ɳ�
//#define COUNTRY_MAKEDONIA						// ���ɵ��Ͼ�
//#define COUNTRY_MALAYSIA_CN					// �����̽þ� �߱� ��Ʈ MALAYSIA chinese simplyfied version(���� ĸ��: GB2312 FONT)
//#define COUNTRY_MEXICO						// �߽���
//#define COUNTRY_MONGOL						// ���� ����
//#define COUNTRY_POLAND   						// ������ Heap Size : 64000
//#define COUNTRY_SOUTHAFRICA					// ���ư�					
//#define COUNTRY_TUNISIA   					// Ƣ���� Heap Size : 64000
//#define COUNTRY_TURKEY   						// Ƣ��Ű�� Heap Size : 64000
#define COUNTRY_USA							// �̱�

#endif /* USE_STANDARD */
/********************************************************************************/


/*
********************************************************************************
*                   CL3000 ��ǰ ���� Define (Hardewrare ����)
********************************************************************************
*/
#ifdef CL3000_BP
    #define MODEL_NAME "CL3000"
	#define MODEL_NAME_SMALL "cl3000"
    #define USE_SINGLE_LINE_GRAPHIC			// Graphic LCD �� �� ���
    #define _USE_LCD_7_26d_16x96_    		// CL3000 : 7-seg LCD(26digits)
    #define _USE_LCD26d_16x96_    			// Graphic LCD : graphic part(16x96)
    #define CPU_PIC32MZ
    #define DISABLE_LIST_MENU 				// LIST Menu �̻��
    #define DISABLE_X2Z2_REPORT 			// Report x2, z2 �̻��
    #define USE_USB_DATA_ROM_UPDATE 		// USB memory�� ����� DATAROM update
    #define USE_CL3000_KEY 					// P:72+36Ű, B:72+35Ű ���
    #define _CL3000_PRINTER_MECH 			// rewind motor disable, GAP_SENSOR_MECHANICAL_OFFSET
    #define CL3000_PROJECT
    #define USE_ONLY_1EA_PRT_RENDERING_BUF 	// printer buffer 1EA�� ��� (�޸� Ȯ��)
    #define USE_CL3000_LCD_ESD_TEST
    //#define USE_NVRAM_INCREASE_LIFETIME 	// Flash(NVRAM)������ ������ �ø��� ���� �޸𸮸� ����.(Ƽ�ϱ��� 3.9�⿡�� 5.7������ �����.)
    							  			// ���þƸ� ������ ������ �ش� ��� ����Ǿ�����, ����Ʈ�� �����ϱ�� �����Ͽ� �ش�����ο� ���� ���� �κ� ����
    #define USE_DSP_USB_FW_UPDATE_VERSION	// USB FW ������Ʈ �� ���� �� ���� ǥ��
    #define USE_DSP_MESSAGE_FOR_MEM_CLEAR	// Memory clear ���� �޼��� ǥ��
   	#define CONTINEOUS_PRT_TEST				// M1864"Chess Printer Test" �� ���� ���� �׽�Ʈ ��� �߰�
	#define USE_AUTO_FIND_VERSION			// ���� ���� �ڵ����� ã�ƿ��� ���
	#define USE_CL3000N_NEW_DISP_IC			// ���� DISP IC(ST7522)���� �̽��� ���Ͽ� �ű� DISP IC(RW1087)����� ����(r3325����)
#endif
/********************************************************************************/


/*
********************************************************************************
*                   CL5200J ��ǰ ���� Define (Hardewrare ����)
********************************************************************************
*/
#ifdef CL5200J_BP
	#define MODEL_NAME "CL5200J"
	#define MODEL_NAME_SMALL "cl5200j"
    #define USE_SINGLE_LINE_GRAPHIC			// Graphic LCD �� �� ���
    #define _USE_LCD_7_37D_40DOT_     		// CL5200J : 7-seg LCD(25digits)
    #define _USE_LCD37D_40DOT_     			// CL5200J NAME : 8x5-dot LCD(12digits)
    #define CPU_PIC32MZ
    #define DISABLE_LIST_MENU 				// LIST Menu �̻��
    #define DISABLE_X2Z2_REPORT 			// Report x2, z2 �̻��
    #define USE_USB_DATA_ROM_UPDATE 		// USB memory�� ����� DATAROM update
    #define USE_CL5200_KEY 					// P:72Ű, B:54Ű ���
    //#define USE_NVRAM_INCREASE_LIFETIME 	// Flash(NVRAM)������ ������ �ø��� ���� �޸𸮸� ����.(Ƽ�ϱ��� 3.9�⿡�� 5.7������ �����.)
    							  			// ���þƸ� ������ ������ �ش� ��� ����Ǿ�����, ����Ʈ�� �����ϱ�� �����Ͽ� �ش�����ο� ���� ���� �κ� ����
    #define USE_ONLY_1EA_PRT_RENDERING_BUF 	// printer buffer 1EA�� ��� (�޸� Ȯ��)
    #define CONTINEOUS_PRT_TEST				// M1864"Chess Printer Test" �� ���� ���� �׽�Ʈ ��� �߰�
	#define USE_AUTO_FIND_VERSION					// ���� ���� �ڵ����� ã�ƿ��� ���
    #define USE_DSP_USB_FW_UPDATE_VERSION	// USB FW ������Ʈ �� ���� �� ���� ǥ��
#endif
/********************************************************************************/


/*
********************************************************************************
*                   ETC Compiler Option define
********************************************************************************
*/
//Compiler Option define
#ifndef CPU_PIC32MZ
#define _PACKED_DEF	__packed
#define __no_init   __no_init
#else
#define _PACKED_DEF	//__attribute__ ((packed))
#define __no_init
#endif //#ifndef CPU_PIC32MZ
#define PACK_2			#pragma pack(2)
#define PACK_4			#pragma pack(4)
#define PACK_CLOSE		#pragma pack()
//#define DISCOUNT_MAX_NUM_200	<= ���� ���� ó�� �ȵ�. �� �����ؾ���
/********************************************************************************/


/*
********************************************************************************
*                   < Common Definition - �� ���� ���� >
********************************************************************************
*/
//#define USE_CERT_APPLY					// ������ �߿��� ����� ����
//#define USE_CANADA_MC_SETTING				// CANADA MC ���� ���� ���. �Ķ����, �⺻ �� �� �� ���� ���� ����
#define USE_ALLOW_CAL_MODE					// 8000 + 'menu'key�� ���� CAL ��� ���� ����
											// ���������� define�� ������� ���� �Ұ�.
//#define _USE_UNDER_SELFKEY_ 				// BODY�� SELFKEY PAD ����
#define USE_CHECK_WLAN_STATUS_LANTRONIX		// LANTRONIX CHECK ���(SCAN SSID, ���� ��)
#ifndef USE_SRILANKA_ZERO
 #define USE_UNDER_LIMIT_MINUS20D			// -20d �̸� ����ǥ������ ��� (OIML2006)
#endif
//#define USE_ONLY_LABEL					// for CL5200 Label version
#define USE_CTS_FUNCTION 					// Calibration Tracebility System
//#define USE_MANAGE_LICENSE				// License ���� ���
//#define USE_X10000_PRINT 					// X 10000 �� ���� TEST�� 
#define USE_KOR_LABELFORM31 				// 31�� �� ���� ���
#define USE_FUNCTION_KEY_BLOCK 				// Ű�е� ������ ���Ű onoff ��뿩�� �����ϴ� ���
//#define USE_CHN_CERTIFICATION				// �߱� �����㰡�� ���� ���. �����߿��� ���۽� �ּ� ó��
#define DISPLAY_REVISION_INFO   			// Revision ���� ��¿��� �����ϴ� ������

#define USE_PARTIAL_MEM_INIT 				// �޸� �κ� �ʱ�ȭ
#ifdef USE_EXTEND_8M_DFLASH
	//#define USE_LABEL_FORMAT_EXT 			// �� ���� Ȯ��(4~8MB ����, 20�� �߰� Ȯ��)
#endif
#define USE_INIT_MASK 						// �޸� �ʱ�ȭ ���
#define USE_MORNITORING_CNT 				// Menu 1879 ����͸� ������ ���
#define USE_ADM_AUTO_RESTORE				// ADM error(���� 03, 04) �� �� �ڵ����� init �����ϵ��� ����(��������)
//#define USE_CONTINUOUS_PRINT_DELAY_1500ms	// EMC�׽�Ʈ �� �μ� �ð� ���� ����(1500ms)
//#define USE_WHITESCREEEN_DEBUG  			// CL3000 White Screen ����� �ڵ�. ����� �Ϸ�� ���ſ���
//#define USE_BUZZER_OFF_TEST               // ���� ���� ���Ұ�(���� �� ����� ��� �϶� �Ҹ����°� ����)

#define USE_QR_BARCODE						// QR ���ڵ�  
#define USE_BARCODE_CODE128X				// CODE128X ��� 
#define USE_STANDARD_BARCODE				// GS1 DataBar ������
/********************************************************************************/


/*
********************************************************************************
*                   <FONT(Windows codepage)>
********************************************************************************
*/
#define CODEPAGE_1250	1
#define CODEPAGE_1251	2
#define CODEPAGE_1252	3
#define CODEPAGE_1253	4
#define CODEPAGE_1254	5
#define CODEPAGE_1255	6
#define CODEPAGE_1256	7
#define CODEPAGE_1257	8
#define CODEPAGE_1258	9
#define CODEPAGE_1252_ARM   10 		// Armenia
#define CODEPAGE_1251_GEO   11 		// Georgia
#define CODEPAGE_1250_POL   12 		// POLAND
#define CODEPAGE_0936_CHN   13
#define CODEPAGE_0949_KOR   14
#define CODEPAGE_1252_IND   15
#define CODEPAGE_1251_MNG   16 		// Mongolia
#define CODEPAGE_1250_HUN   17 		// Hungary
#define CODEPAGE_1251_KAZ   18 		// Kazakhstan
#define CODEPAGE_1257_BAL   19 		// Baltic
#define CODEPAGE_1258_VNM	20 		// Vietnam
#define CODEPAGE_0874_ENG	21 		// Thailand
#define CODEPAGE_1256_IRN   22 		// Persian
#define CODEPAGE_0950_CHN	23 		// Traditional Chinese
/********************************************************************************/


/*
********************************************************************************
*                   ������� ������ ��� difine
********************************************************************************
*/
#ifdef USE_STANDARD
//----------   STANDARD_DIRECT_INGREDIENT  ----------//
	#ifdef COUNTRY_AUSTRALIA
		#include "./Nation/AUS_UN_D.h"
	#elif defined COUNTRY_USA
		#include "./Nation/USA_UN_D.h"
	#elif defined COUNTRY_DOMINICA
		#include "./Nation/DOM_UN_D.h"			
	#elif defined COUNTRY_CANADA
		#include "./Nation/CAN_UN_D.h"	
/********************************************************************************/			
	#elif defined COUNTRY_GERMANY
		#include "./Nation/DEU_UN.h"	
	#elif defined COUNTRY_INDIA
		#include "./Nation/IND_UN.h"				
	#elif defined COUNTRY_CHINA
		#include "./Nation/CHN_UN.h"			
	#elif defined COUNTRY_INDONESIA				/* Indonesia Lotte Mart */
		#include "./Nation/IDN_UN.h"			
	#elif defined COUNTRY_GEORGIA
		#include "./Nation/GEO_UN.h"	
	#elif defined COUNTRY_MALAYSIA_CN
		#include "./Nation/MYS_UN_CN.h"	
	#elif defined COUNTRY_TURKEY
		#include "./Nation/TUR_UN.h"			
	#elif defined COUNTRY_MEXICO
		#include "./Nation/MEX_UN.h"
	#elif defined COUNTRY_POLAND
		#include "./Nation/POL_UN.h"			
	#elif defined COUNTRY_MAKEDONIA
		#include "./Nation/MKD_UN.h"			
	#elif defined COUNTRY_KENYA
		#include "./Nation/KEN_UN.h"	
	#elif defined COUNTRY_SOUTHAFRICA
		#include "./Nation/ZAF_UN.h"
	#elif defined COUNTRY_TUNISIA
		#include "./Nation/TUN_UN.h"
	#elif defined COUNTRY_MONGOL
		#include "./Nation/MNG_UN.h"
	#elif defined COUNTRY_HUNGARY
		#include "./Nation/HUN_UN.h"
	#endif	// USE_STANDARD
//////////////////////////////////////////////////////////////////////////////////
/*			< ���� ������� �� Direct Ingredient, EUROPE_WITHOUT_TICKET ���� ������� >			*/
	#include "./Nation/UN_function.h"
//////////////////////////////////////////////////////////////////////////////////
#endif	// #ifdef USE_STANDARD
/********************************************************************************/


/*
********************************************************************************
*                   ���� ������ ��� difine
********************************************************************************
*/
#ifdef USE_NHMART
	#include "./Nation/KOR_NHMART_N.h"
#endif

#ifdef USE_NHMART_SAFE
	#include "./Nation/KOR_NHMART_SAFE_n.h"
#endif

#ifdef USE_GSMART_TRACE
	#include "./Nation/KOR_GSMART_TRACE_M.h"
#endif

#ifdef USE_GSMART
	#include "./Nation/KOR_GSMART_G.h"
#endif

#ifdef USE_HYUNDAI
	#include "./Nation/KOR_HYUNDAI_H.h"
#endif

#ifdef USE_SCALE_POS
	#include "./Nation/KOR_SCALE_POS_T.h"
#endif

#ifdef USE_EMART
	#include "./Nation/KOR_EMART_E.h"
#endif

#ifdef USE_RUSSIA_SPAR
	#include "./Nation/RUS_SP_D.h"
#endif

#ifdef USE_RUSSIA_INDIRECT
	#include "./Nation/RUS_Q.h"
#endif

#ifdef USE_RUSSIA_RTT_TARE
	#include "./Nation/RUS_RTT_TARE_RT_D.h"
#endif

#ifdef USE_UKRAINE
	#include "./Nation/UKR_Z_D.h"
#endif

#ifdef USE_RUSSIA_LOGISTICS
	#include "./Nation/RUS_l_D.h"
#endif

#ifdef USE_UKRAINE_CLASSMART
	#include "./Nation/UKR_CLASSMART_UC_D.h"
#endif

#ifdef USE_BELARUS
	#include "./Nation/BLR_r_D.h"
#endif

#ifdef USE_BELARUS_EVROOPT
	#include "./Nation/BLR_EVROOPT_q_D.h"
#endif

#ifdef USE_VIETNAM
	#include "./Nation/VNM_V.h"
#endif

#ifdef USE_IMPORT_MEAT
	#include "./Nation/KOR_IMPORT_MEAT_I.h"
#endif

#ifdef USE_SHINSEGAE_MEAT
	#include "./Nation/KOR_SHINSEGAE_MEAT_S.h"
#endif

#ifdef USE_IMPORT_MEAT_CART
	#include "./Nation/KOR_IMPORT_MEAT_CART_s.h"
#endif

#ifdef USE_LOTTEMART
	#include "./Nation/KOR_LOTTEMART_J.h"
#endif

#ifdef USE_LOTTESUPER
	#include "./Nation/KOR_LOTTESUPER_U.h"
#endif

#ifdef USE_LOTTESUPER_8LINE
	#include "./Nation/KOR_LOTTESUPER_8LINE_W.h"
#endif

#ifdef USE_CANADA_LABEL
	#include "./Nation/CAN_LABEL_L.h"
#endif

#ifdef USE_SRILANKA
	#include "./Nation/LKA_K.h"
#endif

#ifdef USE_CHINESE_FONT_BIG5
	#include "./Nation/CHN_FONT_BIG5_TR_D.h"
#endif

#ifdef USE_CHINESE_FONT_GB2312
	#include "./Nation/CHN_FONT_GB2312_B.h"
#endif

#ifdef USE_CHN_TRACE_STANDARD	//USE_CHN_EMART
	#include "./Nation/CHN_TRACE_STANDARD_F.h"
#endif

#ifdef USE_GBR_HMART
	#include "./Nation/GBR_HMART_P.h"
#endif

#ifdef USE_ARAB_EMIRATES
  	#include "./Nation/ARE_a.h"
#endif

#ifdef USE_PERSIA
  	#include "./Nation/IRN_p.h"
#endif

#ifdef USE_HEBREW
  	#include "./Nation/ISR_HEBREW_h.h"
#endif

#ifdef USE_SAUDI   // �ƶ������ + ���� Direct Ingredient
	#include "./Nation/SAU_PT_D.h"
#endif

#ifdef USE_CHN_STANDARDMARKET
	#include "./Nation/CHN_STANDARDMARKET_c.h"
#endif

#ifdef USE_MANAGE_INVENTORY
	#include "./Nation/MANAGE_INVENTORY_i.h"
#endif

#ifdef USE_MANAGE_INVENTORY_CART
	#include "./Nation/MANAGE_INVENTORY_CART_y.h"
#endif

#ifdef USE_KOREA_CL5200
	#include "./Nation/KOR_CL5200_k.h"
#endif

#ifdef USE_EMART_CL5200
	#include "./Nation/KOR_EMART_CL5200_m.h"
#endif

#ifdef USE_INDONESIA_EMPORIUM
	#include "./Nation/IDN_EMPORIUM_e_D.h"
#endif

#ifdef USE_MON_EMART
	#include "./Nation/MNG_EMART_g.h"
#endif

#ifdef USE_INDIA_DADUS
	#include "./Nation/IND_DADUS_DM.h"
#endif

#ifdef USE_INDIA_TATA
	#include "./Nation/IND_TATA_TA.h"
#endif

//////////////////////////////////////////////////////////////////////////////////
/*								< ���þ� �߿��� >								*/
#ifdef USE_RUSSIA
	#include "./Nation/RUS_function.h"

/*						< ���þ� ���� Ư�� ����(Country) ���� >						*/
#define COUNTRY_RUSSIA // Default
//#define COUNTRY_ARMENIA
//#define COUNTRY_KAZAKHSTAN

  #ifdef COUNTRY_RUSSIA
  	#include "./Nation/RUS_R_D.h"
  #elif defined COUNTRY_ARMENIA
  	#include "./Nation/ARM_R_D.h"
  #elif defined COUNTRY_KAZAKHSTAN
  	#include "./Nation/KAZ_R_D.h"
  #else
  	#define FONT_CODEPAGE CODEPAGE_1251
  #endif
#endif
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
/*							< �ƶ� ��Ʈ ��� �߿��� >							*/
#ifdef USE_ARAB
	#include "./Nation/ARAB_FONT_function_A.h"

/*						< �ƶ� ��Ʈ Ư�� ����(Country) ���� >						*/
//#deifne COUNTRY_LEBANON			// ���� CL3000, Cl5200J�� ���ٳ� �߿��� �Ļ� X(24�� 6�� ����)


#endif
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
/*							< ����(Ƽ�� ��� X) ��� �߿��� >							*/
#ifdef USE_EUROPE_WITHOUT_TICKET
	#include "./Nation/UN_function.h"

/*					< ����(Ƽ�� ��� X) Ư�� ����(Country) ���� >						*/
//#define COUNTRY_EASTERN_EUROPE	// ������ CodePage1250 ������������ ��� ����մϴ�(�����ڵ� "EU").
//#define COUNTRY_FRANCE			// ������
//#define COUNTRY_GEORGIA			// ������
//#define COUNTRY_GERMANY			// ����
//#define COUNTRY_GREECE			// �׸���
//#define COUNTRY_LATVIA			// ��Ʈ���
//#define COUNTRY_LITHUANIA			// �����ƴϾ�
//#define COUNTRY_SPAIN				// ������ No ticket, spain caption

	#ifdef COUNTRY_GERMANY
		#include "./Nation/DEU_EUR_u.h"
	#elif defined COUNTRY_FRANCE
		#include "./Nation/FRA_EUR_u.h"
	#elif defined COUNTRY_GEORGIA
		#include "./Nation/GEO_EUR_u.h"
	#elif defined COUNTRY_GREECE				/* for CL5200 for greece �ǸŽ� label�� ����Ʈ�� ticket���� ��°��� */
		#include "./Nation/GRC_EUR_u.h"
	#elif defined COUNTRY_LITHUANIA
		#include "./Nation/LTU_EUR_u.h"
	#elif defined COUNTRY_LATVIA
		#include "./Nation/LVA_EUR_u.h"
	#elif defined COUNTRY_SPAIN
		#include "./Nation/ESP_EUR_u.h"
	#elif defined COUNTRY_EASTERN_EUROPE
		#include "./Nation/EASTERN_EUR_u.h"
	#endif
#endif // USE_EUROPE_WITHOUT_TICKET
//////////////////////////////////////////////////////////////////////////////////


/********************************************************************************/


/*
********************************************************************************
* 			                   ��ǰ ���� ����
*               < ������ �ش� ���ǹ��� Ȱ��ȭ ���� �ʿ��� ��� ��� >
********************************************************************************
*/
#ifdef USE_CERT_APPLY
	#define USE_CHECK_CAL_SW_TO_UPDATE_FW	// ���� USE_OIML_SETTING �ڵ� ���뼺�ִ� ������ ����
	#define USE_CL3000_LCD_ESD_TEST			// ������ ESD ��� disp ���� ����(CL3000/cl5200j disp ����)
#endif //USE_CERT_APPLY
/********************************************************************************/


 /*
********************************************************************************
* 			                     ��Ʈ ����
*   < ������Ͽ� FONT_CODEPAGE�� ���ǵǾ� ���� �ʴٸ� �ش� ���ǹ��� ����Ʈ�� ���� >
********************************************************************************
*/
#ifndef FONT_CODEPAGE
	#define FONT_CODEPAGE CODEPAGE_1252
#endif
/********************************************************************************/

/////////////////////////////Compile Condition END/////////////////////////////////

//#define USE_KEY_BUZZER	// Buzzer�� Key�� ���� ������ �︮���� ��

#define ROMDATA	const
#define HUGEDATA
#define _huge
#define _rom const
#define _inline
#define NEARDATA

//#define NDEBUG //debug message to output window


#define ON		1
#define OFF		0
#define IDLE	2

#define	DISABLE	0
#define	ENABLE	1
#define YES		1
#define NO		0
#if !defined(TRUE)
  #define	TRUE	(!0)
  #define	FALSE	0  
#endif

// <Flash memory Type> //
#define EN25QH32A		0 // ���� (Eon)
#define SST26VF032B		1 // ��ü �� (Microchp)

#define NUL		0x00	
#define SOH		0x01
#define	STX		0x02
#define ETX	   	0x03
#define EOT		0x04
#define ENQ		0x05
#define ACK		0x06
#define BEL		0x07
#define BS 		0x08
#define HT 	   	0x09
#define LF 		0x0A
#define VT 		0x0B
#define FF 		0x0C
#define _CR 	0x0D
#define SO		0x0E
#define SI		0x0F
#define DLE		0x10
#define DC1		0x11
#define DC2		0x12
#define DC3		0x13
#define DC4		0x14
#define NAK		0x15
#define SYN		0x16
#define ETB		0x17

#define EM		0x19
#define SUB		0x1A
#define _ESC	0x1B   		//091207 yoo test  
#define FC 		0x1C
#define GS 		0x1D
#define __RS 	0x1E		//QR encode lib conflict
#define US 		0x1F
#define DEL 	0x7F

typedef unsigned char BOOLEAN; /* Logical data type (TRUE or FALSE) */
typedef unsigned char INT8U; /* Unsigned 8 bit value */
typedef signed char INT8S; /* Signed 8 bit value */
typedef unsigned short INT16U; /* Unsigned 16 bit value */
typedef signed short INT16S; /* Signed 16 bit value */
typedef unsigned long INT32U; /* Unsigned 32 bit value */
typedef signed long INT32S; /* Signed 32 bit value */
//typedef unsigned xxxxx INT64U; /* Unsigned 64 bit value (if available)*/
//typedef signed xxxxx INT64S; /* Signed 64 bit value (if available)*/
typedef float FP32; /* 32 bit, single prec. floating-point */
typedef double FP64; /* 64 bit, double prec. floating-point */

#endif /* _GLOBALS_H */
#ifndef _FW_VERSION_DEFINE
#define _FW_VERSION_DEFINE

#define FW_BOOT_VERSION_ROM_ADDR	0x21fff0
#define FW_VERSION_ROM_ADDR	 0x07fff0

#pragma pack(2)
typedef struct {
	char model[2];
	char year[2];
	char month[2];	// 6
	char country[3];//
	char keytype;	// 10
	char subversion[2];	//vendor[2];		// Modified by CJK 20061121
	char version;
	char reversion[2];
	char testversion;	// CJK080321
} FW_VERSION;

typedef struct {
	char dealer[4];	// include NULL
	char country[3];	// include NULL
	char calpassword[6];	// Cal Password version & init default
	char reserved[3];
} FW_VERSION2;
#pragma pack()

#ifdef USE_MORNITORING_CNT
#pragma pack(2)
typedef struct {
    INT32U TotalCnt;
    INT32U AfterCnt;
    INT32U InspectDate;
    INT32U Reserved;
} REMOTE_STRUCT;
#pragma pack()
#endif

extern void version_get(FW_VERSION *fw);
extern void version2_get(FW_VERSION2 *fw);
#endif

#ifndef _ROM_COUNTRY_DEFINE
#define _ROM_COUNTRY_DEFINE

#define ROM_STD	0
#define ROM_RUS	1

#if defined(USE_RUSSIA) || defined(USE_RUSSIA_INDIRECT) || defined(USE_BELARUS) || defined(USE_UKRAINE) || defined(USE_BELARUS_EVROOPT) || defined(USE_UKRAINE_CLASSMART) || defined(USE_RUSSIA_RTT_TARE) || defined(USE_RUSSIA_LOGISTICS) || defined(USE_RUSSIA_SPAR)
  #define ROM_COUNTRY ROM_RUS
#else
  #define ROM_COUNTRY ROM_STD
#endif
#endif

#if defined(USE_MANAGE_LICENSE)		// License ���� ���
#define USE_DES_ENCRYPTION
#endif

//#define TEST_REPORT