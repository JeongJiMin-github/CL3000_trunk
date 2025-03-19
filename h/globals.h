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
//------- 영공 펌웨어 사용 -------//
#define USE_STANDARD        					// 영공

//------- 기타 추가적인 국가 확인 필요한 펌웨어 -------//
//#define USE_ARAB							// (A) 아랍 (폰트)
//#define USE_EUROPE_WITHOUT_TICKET			// (u) 티켓기능을 사용하지 않는 유럽 전용 펌웨어
//#define USE_RUSSIA						// (R) 러시아

//------- 국내 펌웨어 (알파벳, 오름차순 정렬) -------//
//#define USE_EMART           					// (E) E-Mart
//#define USE_EMART_CL5200     					// (m) CL5200 이마트 (이력관리기능 적용)
//#define USE_GSMART          					// (G) GS 마트 설정
//#define USE_GSMART_TRACE    					// (M) GS 마트 축산 설정
//#define USE_HYUNDAI       					// (H) 현대백화점 설정
//#define USE_IMPORT_MEAT     					// (I) 독립형 개체이력 (수입육 위해 개체)
//#define USE_IMPORT_MEAT_CART    				// (s) 수입육 위해 개체(CART)(Pb Bat)
//#define USE_KOREA_CL5200     					// (k) CL5200 내수 버젼 (이력관리 기능 삭제)
//#define USE_LOTTEMART       					// (J) Lotte Mart(국내 수입육 + GS1 DataBar Expand)
//#define USE_LOTTESUPER						// (U) Lotte Super(국내 수입육에서 파생)
//#define USE_LOTTESUPER_8LINE 					// (W) Lotte Super 상품명 8단(델리카 사용)
//#define USE_NHMART          					// (N) 농협 설정
//#define USE_NHMART_SAFE     					// (n) 농협 안심 한우
//#define USE_SCALE_POS       					// (T) 저울나라 이력저울 => CL5200J에서 제거됨 (국내 요청으로 CL3000 저울나라 펌웨어 파생)
//#define USE_SHINSEGAE_MEAT     				// (S) 신세계 백화점

//------- 해외 펌웨어 (알파벳 정렬) -------//
//#define USE_ARAB_EMIRATES						// (a) 아랍에미레이트
//#define USE_BELARUS_EVROOPT       			// (q) 벨라루스 EVROOPT SELF
//#define USE_CANADA_LABEL    					// (L) 캐나다 라벨 길이 사용(200mm)
//#define USE_CHINESE_FONT_BIG5					// TR tranditional chinese (big5)
//#define USE_CHINESE_FONT_GB2312 				// (B) simple chinese (gb2312)
//#define USE_CHN_STANDARDMARKET				// (c) 중국표준화시장
//#define USE_CHN_TRACE_STANDARD   				// (F) chinese trace standard (gb2312) <= chn E-Mart
//#define USE_GBR_HMART       					// (P) 영국 H-MART 설정(한글폰트)
//#define USE_HEBREW							// (h) 히브리어 (이스라엘)
//#define USE_INDIA_DADUS						// DM 인도(DADUS MITHAI VATIKA)
//#define USE_INDIA_TATA						// TA 인도 TATA Group 전용 펌웨어
//#define USE_INDONESIA_EMPORIUM				// (e) 인도네시아 엠포리움 마켓
//#define USE_MANAGE_INVENTORY					// (i) 자재 재고관리 라벨 발행 기능.
//#define USE_MON_EMART							// (g) CL5500 몽골 이마트
//#define USE_NINGBO_RETAIL_STANDARDMARKET		// (z) 중국닝보표준화시장(소매기능) "/Branch/CarScale_Ningbo_Retail"
//#define USE_NINGBO_STANDARDMARKET				// (b) 중국닝보표준화시장 "/Branch/CarScale_Ningbo"
//#define USE_PERSIA							// (p) Persian(이란)
//#define USE_RUSSIA_INDIRECT  					// (Q) 러시아(Indirect Ingredient Version)
//#define USE_RUSSIA_LOGISTICS     				// (l) 러시아 물류 버전 
//#define USE_RUSSIA_RTT_TARE					// RT 러시아 공용 롬 (RTT 프로토콜 TARE 데이터 추가)
//#define USE_RUSSIA_SPAR         				// (sp) 러시아 SPAR 마트 
//#define USE_SAUDI								// PT 아랍어(사우디아라비아)
//#define USE_SRILANKA    						// (K) 스리랑카
//#define USE_UKRAINE               			// (Z) 우크라이나
//#define USE_UKRAINE_CLASSMART					// UC 우크라이나 CLASS MART 전용 롬
//#define USE_VIETNAM          					// (V) 베트남

//------- 장기 미파생 및 미사용 펌웨어 -------//
//#define USE_BELARUS							// (r) 벨라루스
//#define USE_CHN_NEW_STANDARDMARKET			// (a) 중국 표준화시장(2013입찰용,디스플레이프로토콜) "/Branch/CarScale_RemoteDisplay/"
//#define USE_MANAGE_INVENTORY_CART				// (y) 자재재고관리 기능 추가된 카트 저울 샘플 (2024년 기준 현재 사용 X)
/********************************************************************************/


/*
********************************************************************************
* 			                   Compile Condition 3-2
* 			                   < Country Define >
* 		< 영공 기반(USE_STANDARD) 국가 선택 >
* 		영공은 국가 디파인 모두 주석처리
********************************************************************************
*/
#ifdef USE_STANDARD
/********************************************************************************/
//#define COUNTRY_AUSTRALIA						// 호주
//#define COUNTRY_CANADA						// 캐나다
//#define COUNTRY_CHINA   						// 중국 Heap Size : 64000
//#define COUNTRY_DOMINICA						// 도미니카
//#define COUNTRY_GEORGIA						// 조지아
//#define COUNTRY_GERMANY						// 독일
//#define COUNTRY_HUNGARY						// 헝가리
//#define COUNTRY_INDIA   						// 인도 Heap Size : 64000
//#define COUNTRY_INDONESIA						// 인도네시아 Indonesia Lotte-Mart
//#define COUNTRY_KENYA							// 케냐
//#define COUNTRY_MAKEDONIA						// 마케도니아
//#define COUNTRY_MALAYSIA_CN					// 말레이시아 중국 폰트 MALAYSIA chinese simplyfied version(영공 캡션: GB2312 FONT)
//#define COUNTRY_MEXICO						// 멕시코
//#define COUNTRY_MONGOL						// 몽골 공용
//#define COUNTRY_POLAND   						// 폴란드 Heap Size : 64000
//#define COUNTRY_SOUTHAFRICA					// 남아공					
//#define COUNTRY_TUNISIA   					// 튀니지 Heap Size : 64000
//#define COUNTRY_TURKEY   						// 튀르키예 Heap Size : 64000
#define COUNTRY_USA							// 미국

#endif /* USE_STANDARD */
/********************************************************************************/


/*
********************************************************************************
*                   CL3000 제품 전용 Define (Hardewrare 종속)
********************************************************************************
*/
#ifdef CL3000_BP
    #define MODEL_NAME "CL3000"
	#define MODEL_NAME_SMALL "cl3000"
    #define USE_SINGLE_LINE_GRAPHIC			// Graphic LCD 한 줄 사용
    #define _USE_LCD_7_26d_16x96_    		// CL3000 : 7-seg LCD(26digits)
    #define _USE_LCD26d_16x96_    			// Graphic LCD : graphic part(16x96)
    #define CPU_PIC32MZ
    #define DISABLE_LIST_MENU 				// LIST Menu 미사용
    #define DISABLE_X2Z2_REPORT 			// Report x2, z2 미사용
    #define USE_USB_DATA_ROM_UPDATE 		// USB memory를 사용한 DATAROM update
    #define USE_CL3000_KEY 					// P:72+36키, B:72+35키 사용
    #define _CL3000_PRINTER_MECH 			// rewind motor disable, GAP_SENSOR_MECHANICAL_OFFSET
    #define CL3000_PROJECT
    #define USE_ONLY_1EA_PRT_RENDERING_BUF 	// printer buffer 1EA만 사용 (메모리 확보)
    #define USE_CL3000_LCD_ESD_TEST
    //#define USE_NVRAM_INCREASE_LIFETIME 	// Flash(NVRAM)영역의 수명을 늘리기 위한 메모리맵 셋팅.(티켓기준 3.9년에서 5.7년으로 상향됨.)
    							  			// 러시아를 제외한 국가에 해당 기능 적용되었으며, 디폴트로 적용하기로 결정하여 해당디파인에 의해 묶인 부분 삭제
    #define USE_DSP_USB_FW_UPDATE_VERSION	// USB FW 업데이트 시 문구 및 버전 표시
    #define USE_DSP_MESSAGE_FOR_MEM_CLEAR	// Memory clear 동안 메세지 표시
   	#define CONTINEOUS_PRT_TEST				// M1864"Chess Printer Test" 시 연속 발행 테스트 기능 추가
	#define USE_AUTO_FIND_VERSION			// 버전 정보 자동으로 찾아오는 기능
	#define USE_CL3000N_NEW_DISP_IC			// 기존 DISP IC(ST7522)단종 이슈로 인하여 신규 DISP IC(RW1087)사용을 위함(r3325기준)
#endif
/********************************************************************************/


/*
********************************************************************************
*                   CL5200J 제품 전용 Define (Hardewrare 종속)
********************************************************************************
*/
#ifdef CL5200J_BP
	#define MODEL_NAME "CL5200J"
	#define MODEL_NAME_SMALL "cl5200j"
    #define USE_SINGLE_LINE_GRAPHIC			// Graphic LCD 한 줄 사용
    #define _USE_LCD_7_37D_40DOT_     		// CL5200J : 7-seg LCD(25digits)
    #define _USE_LCD37D_40DOT_     			// CL5200J NAME : 8x5-dot LCD(12digits)
    #define CPU_PIC32MZ
    #define DISABLE_LIST_MENU 				// LIST Menu 미사용
    #define DISABLE_X2Z2_REPORT 			// Report x2, z2 미사용
    #define USE_USB_DATA_ROM_UPDATE 		// USB memory를 사용한 DATAROM update
    #define USE_CL5200_KEY 					// P:72키, B:54키 사용
    //#define USE_NVRAM_INCREASE_LIFETIME 	// Flash(NVRAM)영역의 수명을 늘리기 위한 메모리맵 셋팅.(티켓기준 3.9년에서 5.7년으로 상향됨.)
    							  			// 러시아를 제외한 국가에 해당 기능 적용되었으며, 디폴트로 적용하기로 결정하여 해당디파인에 의해 묶인 부분 삭제
    #define USE_ONLY_1EA_PRT_RENDERING_BUF 	// printer buffer 1EA만 사용 (메모리 확보)
    #define CONTINEOUS_PRT_TEST				// M1864"Chess Printer Test" 시 연속 발행 테스트 기능 추가
	#define USE_AUTO_FIND_VERSION					// 버전 정보 자동으로 찾아오는 기능
    #define USE_DSP_USB_FW_UPDATE_VERSION	// USB FW 업데이트 시 문구 및 버전 표시
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
//#define DISCOUNT_MAX_NUM_200	<= 현재 정상 처리 안됨. 꼭 수정해야함
/********************************************************************************/


/*
********************************************************************************
*                   < Common Definition - 전 국가 공통 >
********************************************************************************
*/
//#define USE_CERT_APPLY					// 인증용 펌웨어 디버깅 적용
//#define USE_CANADA_MC_SETTING				// CANADA MC 인증 관련 기능. 파라미터, 기본 라벨 폼 등 인증 사항 적용
#define USE_ALLOW_CAL_MODE					// 8000 + 'menu'key를 통해 CAL 모드 진입 가능
											// 내수버전은 define과 상관없이 진입 불가.
//#define _USE_UNDER_SELFKEY_ 				// BODY에 SELFKEY PAD 사용시
#define USE_CHECK_WLAN_STATUS_LANTRONIX		// LANTRONIX CHECK 기능(SCAN SSID, 감도 등)
#ifndef USE_SRILANKA_ZERO
 #define USE_UNDER_LIMIT_MINUS20D			// -20d 미만 무게표시제한 기능 (OIML2006)
#endif
//#define USE_ONLY_LABEL					// for CL5200 Label version
#define USE_CTS_FUNCTION 					// Calibration Tracebility System
//#define USE_MANAGE_LICENSE				// License 관리 기능
//#define USE_X10000_PRINT 					// X 10000 라벨 가능 TEST용 
#define USE_KOR_LABELFORM31 				// 31번 라벨 포맷 사용
#define USE_FUNCTION_KEY_BLOCK 				// 키패드 오른편 기능키 onoff 사용여부 선택하는 기능
//#define USE_CHN_CERTIFICATION				// 중국 생산허가를 위한 기능. 생산펌웨어 제작시 주석 처리
#define DISPLAY_REVISION_INFO   			// Revision 정보 출력여부 선택하는 디파인

#define USE_PARTIAL_MEM_INIT 				// 메모리 부분 초기화
#ifdef USE_EXTEND_8M_DFLASH
	//#define USE_LABEL_FORMAT_EXT 			// 라벨 개수 확장(4~8MB 영역, 20개 추가 확장)
#endif
#define USE_INIT_MASK 						// 메모리 초기화 방어
#define USE_MORNITORING_CNT 				// Menu 1879 모니터링 데이터 기능
#define USE_ADM_AUTO_RESTORE				// ADM error(에러 03, 04) 일 때 자동으로 init 수행하도록 수정(공통적용)
//#define USE_CONTINUOUS_PRINT_DELAY_1500ms	// EMC테스트 시 인쇄 시간 간격 조정(1500ms)
//#define USE_WHITESCREEEN_DEBUG  			// CL3000 White Screen 디버깅 코드. 디버깅 완료시 제거예정
//#define USE_BUZZER_OFF_TEST               // 저울 부저 음소거(개발 시 디버깅 모드 일때 소리나는거 방지)

#define USE_QR_BARCODE						// QR 바코드  
#define USE_BARCODE_CODE128X				// CODE128X 사용 
#define USE_STANDARD_BARCODE				// GS1 DataBar 미포함
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
*                   영공기반 국가별 기능 difine
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
/*			< 영공 공통사항 및 Direct Ingredient, EUROPE_WITHOUT_TICKET 설정 헤더파일 >			*/
	#include "./Nation/UN_function.h"
//////////////////////////////////////////////////////////////////////////////////
#endif	// #ifdef USE_STANDARD
/********************************************************************************/


/*
********************************************************************************
*                   전용 국가별 기능 difine
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

#ifdef USE_SAUDI   // 아랍공용롬 + 전용 Direct Ingredient
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
/*								< 러시아 펌웨어 >								*/
#ifdef USE_RUSSIA
	#include "./Nation/RUS_function.h"

/*						< 러시아 버전 특정 국가(Country) 선택 >						*/
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
/*							< 아랍 폰트 사용 펌웨어 >							*/
#ifdef USE_ARAB
	#include "./Nation/ARAB_FONT_function_A.h"

/*						< 아랍 폰트 특정 국가(Country) 선택 >						*/
//#deifne COUNTRY_LEBANON			// 현재 CL3000, Cl5200J는 레바논에 펌웨어 파생 X(24년 6월 기준)


#endif
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
/*							< 유럽(티켓 기능 X) 사용 펌웨어 >							*/
#ifdef USE_EUROPE_WITHOUT_TICKET
	#include "./Nation/UN_function.h"

/*					< 유럽(티켓 기능 X) 특정 국가(Country) 선택 >						*/
//#define COUNTRY_EASTERN_EUROPE	// 동유럽 CodePage1250 동유럽공용일 경우 사용합니다(국가코드 "EU").
//#define COUNTRY_FRANCE			// 프랑스
//#define COUNTRY_GEORGIA			// 조지아
//#define COUNTRY_GERMANY			// 독일
//#define COUNTRY_GREECE			// 그리스
//#define COUNTRY_LATVIA			// 라트비아
//#define COUNTRY_LITHUANIA			// 리투아니아
//#define COUNTRY_SPAIN				// 스페인 No ticket, spain caption

	#ifdef COUNTRY_GERMANY
		#include "./Nation/DEU_EUR_u.h"
	#elif defined COUNTRY_FRANCE
		#include "./Nation/FRA_EUR_u.h"
	#elif defined COUNTRY_GEORGIA
		#include "./Nation/GEO_EUR_u.h"
	#elif defined COUNTRY_GREECE				/* for CL5200 for greece 판매시 label만 리포트는 ticket으로 출력가능 */
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
* 			                   제품 인증 설정
*               < 인증시 해당 정의문을 활성화 시켜 필요한 기능 사용 >
********************************************************************************
*/
#ifdef USE_CERT_APPLY
	#define USE_CHECK_CAL_SW_TO_UPDATE_FW	// 기존 USE_OIML_SETTING 코드 범용성있는 문구로 수정
	#define USE_CL3000_LCD_ESD_TEST			// 인증용 ESD 방어 disp 연속 갱신(CL3000/cl5200j disp 전용)
#endif //USE_CERT_APPLY
/********************************************************************************/


 /*
********************************************************************************
* 			                     폰트 설정
*   < 헤더파일에 FONT_CODEPAGE가 정의되어 있지 않다면 해당 정의문이 디폴트로 설정 >
********************************************************************************
*/
#ifndef FONT_CODEPAGE
	#define FONT_CODEPAGE CODEPAGE_1252
#endif
/********************************************************************************/

/////////////////////////////Compile Condition END/////////////////////////////////

//#define USE_KEY_BUZZER	// Buzzer가 Key가 눌릴 때마다 울리도록 함

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
#define EN25QH32A		0 // 구모델 (Eon)
#define SST26VF032B		1 // 대체 모델 (Microchp)

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

#if defined(USE_MANAGE_LICENSE)		// License 관리 기능
#define USE_DES_ENCRYPTION
#endif

//#define TEST_REPORT