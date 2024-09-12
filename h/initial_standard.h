/*****************************************************************************
|*			
|*  Copyright		:	(c) 2010 CAS
|*  Filename		:	initial_standard.h
|*  Version			:	1.5 (FLASH_STRUCT_VERSION)
|*  Programmer(s)	:	Chung Jongkwon (CJK)
|*  Created			:	2010/11/24
|*  Description		:	CL5000 Memory Definition (Standard)
|*				
*****************************************************************************/
#ifndef FLASH_STRUCT_VERSION_DEF
	#define FLASH_STRUCT_VERSION_DEF
	//Direct Ingredient
	#ifdef USE_DIRECT_INGREDIENT
		#ifdef USE_USA_INGREDIENT
			#define FLASH_STRUCT_VERSION "20"	// US Direct Ingredient
		#elif defined (USE_EMPORIUM_INGREDIENT)
			#define FLASH_STRUCT_VERSION "24"	// Indonesia Emporium Market Ingredient
		#elif defined (USE_AUSTRALIA_INGREDIENT)
			#define FLASH_STRUCT_VERSION "25"	// Australia Ingredient
		#elif defined (USE_SAUDI_INGREDIENT)
			#define FLASH_STRUCT_VERSION "32"	// Saudi Ingredient
		#elif defined (USE_NORTH_AMERICA_INGREDIENT)
			#define FLASH_STRUCT_VERSION "35"	// North America Ingredient ( Direct Nutrition Fact ���, PLU 2,000��, CL3000 �̱�/ĳ���� ��� ���� )
		#else	
			#define FLASH_STRUCT_VERSION "19"	// Standard Direct Ingredient
		#endif
	//Standard ingredient
	#else
		#ifdef USE_9999_INDIRECT_INGREDIENT
			#define FLASH_STRUCT_VERSION "25"	// ������ 510byte 999��, ���� index��ȣ�� 9999������ ��� ������
		#else
		#define FLASH_STRUCT_VERSION "15"	// Standard
		#endif
	#endif
	#define NVRAM_STRUCT_VERSION "103"
#endif

#ifndef	_MEMORY_MAP_DEFINE
#define	_MEMORY_MAP_DEFINE
#define FLASH_STR_VERSION	0
#define FLASH_CREATE_DATE	2
#define FLASH_ROM_STATUS	8
#define FLASH_MEMORY_ADDRESS	16
////////////////////////////////////////////
// �߿� : ��ġ����Ұ�
//  CTS Structure (Enable, Period, Serial No)
#define FLASH_CTS_STRUCT	48
// �߿� : FW UPDATE MODE ������ġ�� BOOT���� ����ϹǷ� ����Ǹ� �ȵ�
// 80 = FLASH_FWMODE_SET
#define FLASH_FWMODE_SET	80
////////////////////////////////////////////
#define FLASH_FW_UPDATE_VER	81
#define FLASH_START_PASSWORD	82
#define FLASH_START_COUNT_HIGH	90
#define FLASH_START_COUNT	92
#define FLASH_START_LOG	94
////////////////////////////////////////////
// �߿� : ��ġ����Ұ�
#define FLASH_CALLOG_COUNT	124
#define FLASH_CALLOG_DATA	126
// �߿� : ��ġ����Ұ�
//  CTS Structure2 (Product No)
#define FLASH_CTS_STRUCT2	246
////////////////////////////////////////////
#define FLASH_ADC_AREA	276
#define FLASH_ADC_WUNIT	404
#define FLASH_ADC_FILTER_LEVEL	405
#define FLASH_ADC_USE_FULLTARE	406
#define FLASH_ADC_TARE_CONFIG	407
#ifdef USE_ADM_AUTO_RESTORE
#define FLASH_ADM_INIT_ERROR_COUNT	408
#define FLASH_ADM_EEP_ERROR_COUNT	409
#define FLASH_ADM_INIT_COMPLETE     410
#define FLASH_ADC_RESERVED2	411
#else
#define FLASH_ADC_RESERVED2	408
#endif
#define FLASH_ETHERNET_MAC	412
////////////////////////////////////////////
// �߿� : ��ġ����Ұ�
//  BOOT���� ����� MAC, IP, Version ������ Printer ������ ���� 48byte�� ������
#define FLASH_PRINTER_AREA	418
///////////////////////////////////////////
#define FLASH_GLOBAL_AREA	674
#define FLASH_BASIC_SYMBOL_AREA	806
#define FLASH_DEALER_AREA	1016
#define FLASH_BASIC_SYMBOL_AREA_END	1074
#define FLASH_BASIC_PARAMETER	1086
#define FLASH_NET_AREA	1416
#define FLASH_FONT_AREA	1598
#define FLASH_CAPTION_AREA	263796
#define FLASH_LABEL_CAPTION_AREA	335800
#define FLASH_PCS_UNIT_AREA	342200
#define FLASH_PLU_STRUCT_CAPTION_AREA	342344
#define FLASH_ORIGIN_AREA	346440
#define FLASH_PLU_STRUCT_AREA	363440
#define FLASH_KEY_AREA	365518
#define FLASH_KEY_SHIFT_AREA	365774
#define FLASH_KEY_PGM_AREA	366030
#define FLASH_KEY_PGM_SHIFT_AREA	366286
#define FLASH_KEY_PGM2_AREA	366542
#define FLASH_KEY_PGM2_SHIFT_AREA	366798
#define FLASH_PSKEY_AREA	367054
#define FLASH_PSKEY_SHIFT_AREA	367374
#define FLASH_PSKEY_PGM_AREA	367694
#define FLASH_PSKEY_PGM_SHIFT_AREA	368014
#define FLASH_PSKEY_PGM2_AREA	368334
#define FLASH_PSKEY_PGM2_SHIFT_AREA	368654
#define FLASH_KEY_CDTC_AREA	368974
#define FLASH_KEY_CUSTOMER_AREA	369230
#define FLASH_KEY_SPEED_AREA	369486
#define FLASH_LABEL_AREA	372686
#define FLASH_BITMAP_AREA	455086
#define FLASH_CURRENCY_AREA	569858
#define FLASH_SCROLL_AREA	569918
#define FLASH_PRT_ADVE_AREA	570928
#define FLASH_NUMBER_PLU	574128
#define FLASH_NUMBER_BARCODE	574132
#define FLASH_NUMBER_ORIGIN	574134
#define FLASH_NUMBER_TARE	574136
#define FLASH_NUMBER_NUTRIFACT	574138
#define FLASH_NUMBER_INGRIDIENT_SIZE	574140
#define FLASH_NUMBER_INGRIDIENT	574142
#define FLASH_NUMBER_SHOP	574144
#define FLASH_NUMBER_LABELADVERTISE	574146
#define FLASH_NUMBER_DEPARTMENT	574148
#define FLASH_NUMBER_GROUP	574150
#define FLASH_NUMBER_COUNTRY	574152
#define FLASH_NUMBER_SLAUGHTER	574154
#define FLASH_NUMBER_CUTTINGHALL	574156
#define FLASH_NUMBER_TRACEBILITY	574158
#define FLASH_NUMBER_CLERK	574160
#define FLASH_NUMBER_CUSTOMER	574162
#define FLASH_NUMBER_DISCOUNT	574164
#define FLASH_NUMBER_BONUS	574166
#define FLASH_NUMBER_TAX	574168
#define FLASH_NUMBER_SCALE	574170
#define FLASH_NUMBER_PAID	574172
#define FLASH_NUMBER_REPORT_PERIOD	574173
#define FLASH_NUMBER_TRANSACTION	574174
#define FLASH_NUMBER_RESERVED1	574176
#define FLASH_VERIFY_DATE	574180	// Reserved for Korea Function(Check Verification Timer)	// CJK080219
#define FLASH_TAX_AREA	574184
#define FLASH_BARCODE_FORMAT_AREA	574264
#define FLASH_SHOPNAME_AREA	579664
#define FLASH_DEPARTMENT_AREA	580944
#define FLASH_SUB_DEPARTMENT_AREA	584144
#define FLASH_TARE_AREA	587344
#define FLASH_CLERK_AREA	589744
#define FLASH_CLERK_PERMISSION	594544
#define FLASH_CUSTOMER_AREA	594624	//594604
#define FLASH_CUSTOMER_RESERVED_AREA	614424	// Added by CJK 20060608
#define FLASH_LICENSE		614796
#define FLASH_REMOTE_IP2	614876
#define FLASH_SCANNER_FORMAT	614882
#define FLASH_SLAVE_INFO	615304	// CJK080517
#define FLASH_EXT_PRT_LABEL_NAME	615464 //external printer(BT-DT-4), Label form name
#define FLASH_DISCOUNT_GLOBAL_WEIGHT	615604
#define FLASH_DISCOUNT_GLOBAL_COUNT	615652
#define FLASH_DISCOUNT_GLOBAL_NONFOOD	615700
#define FLASH_DISCOUNT_AREA	615748
#if defined(USE_USA_INGREDIENT) || defined(USE_NORTH_AMERICA_INGREDIENT)
	#define FLASH_INGREDIENT_AREA	620548
//	#define FLASH_INGREDIENT_TEXT_AREA	 620548
	#define FLASH_INGREDIENT_RESERVED_AREA	 1122148
	#define FLASH_PLU_AREA	1134548
#else
	#define FLASH_INGREDIENT_AREA	620548
//	#define FLASH_INGREDIENT_TEXT_AREA	 1132548	   //SG070725.ADD
	#define FLASH_INGREDIENT_RESERVED_AREA	 1132548   //SG070725.ADD
	#define FLASH_PLU_AREA	1134548
#endif
#ifdef USE_NORTH_AMERICA_INGREDIENT
#define FLASH_NUTRIFACT_AREA	1549548
#else
#define FLASH_NUTRIFACT_AREA	1902548
#endif  // USE_NORTH_AMERICA_INGREDIENT
#define FLASH_COUNTRY_AREA	1949548

#ifdef USE_TRACEABILITY
	#define FLASH_COUNTRY_AREA	1949548
	#define FLASH_SLAUGHTERHOUSE_AREA	1969548
	#define FLASH_CUTTINGHALL_AREA	1991548
	#define FLASH_TRACEABILITY_AREA	2013548
#endif

#define FLASH_JOBBATCH_AREA 		2041548	 //SG061220:ADD
#define FLASH_SCANNER_TABLE_AREA 	2041708	 //SG070124:ADD
#define FLASH_LABEL_KEY_TABLE_AREA	2085740  //SG070222:ADD
#define FLASH_TICKET_HEAD_MESSAGE_AREA	2085788	//320 = 64*5 bytes
#define FLASH_TICKET_TAIL_MESSAGE_AREA	2086108	//320 = 64*5 bytes
#ifdef USE_LOT_NUMBER_FUNCTION
#define FLASH_LOT_NUMBER	2086428
#endif

// Menu 1891 �޴� ���� �÷��� �ּ� (2 byte)
#ifdef USE_INIT_MASK
#define FLASH_KEEP_INIT_SETTINGS 2086448	
#endif

#define FLASH_END_POINT 	2092140  //SG070222:MODD

// �� Ȯ�� ����(4~8MB) �ּ� ����� ���� define
#define LABEL_CUSTOM_2MB_NUM			20 		// ���� 2MB �������� ����� ���� �� �� ����
#define LABEL_HEADER_SIZE				24 		// �� ��� ũ��
#define FLASH_LABEL_IMAGE_AREA_ADDR		(FLASH_LABEL_AREA + LABEL_CUSTOM_2MB_NUM * LABEL_HEADER_SIZE) // �� �̹��� �ּ�

#ifdef USE_EXTEND_8M_DFLASH
// Ȯ��޸� ����
// base : EXT_MEMCARD2_ADDR_X(0x400000)
// ����ϴ� �κп��� "EXT_MEMCARD_ADDR_X + FLASH_USB_DATA_ROM_UPDATE" �� ���� ����Ͽ� ���� �ҽ���
// �����ϱ� ���� Define�� �޸� �ּҸ� "2MB + 2MB + �ּ�" �� ���� �Ͽ���.
#define FLASH_REMOTE_DATA_ROM_UPDATE_FLAG     EXT_MEMCARD_ADDR_X + 4194254
#define FLASH_USB_DATA_ROM_UPDATE       EXT_MEMCARD_ADDR_X + 4194255
#define FLASH_FTL_ERR_MANAGEMENT_AREA	EXT_MEMCARD_ADDR_X + 4194256

#ifdef USE_LABEL_FORMAT_EXT
	#define LABEL_CUSTOM_EXT_NUM		20  // ~ 8MB Ȯ�� �������� ����� ���� �� �� ����
#else
	#define LABEL_CUSTOM_EXT_NUM	 	0  // ~ 8MB Ȯ�� �������� ����� ���� �� �� ����
#endif

// �� Ȯ�� ����(4~8MB) �ּ� ����
#define FLASH_LABEL_FORMAT_EXT_OFFSET			4070654 // �� Ȯ�� ���� ������

// �� Ȯ�� ���� ��� �ּ�
// = 2MB + 2MB + �� Ȯ�� ���� ������
#define FLASH_LABEL_FORMAT_EXT_HEADER_AREA	\
	(EXT_MEMCARD_ADDR + EXT_MEMCARD_ADDR + FLASH_LABEL_FORMAT_EXT_OFFSET)	

// �� Ȯ�� ���� �̹��� �ּ�
// = �� Ȯ�� ���� ��� �ּ� + �� ��� ������ * �� Ȯ�� ���������� ����
#define FLASH_LABEL_FORMAT_EXT_IMAGE_AREA	\
	(FLASH_LABEL_FORMAT_EXT_HEADER_AREA + LABEL_HEADER_SIZE * LABEL_CUSTOM_EXT_NUM) 		

#else
// Ȯ��޸� ����
// base : EXT_MEMCARD_ADDR_X(0x200000)
#define FLASH_USB_DATA_ROM_UPDATE	2097103	 // CL5200J
#define FLASH_FTL_ERR_MANAGEMENT_AREA	2097104	 // CL5200J
#define LABEL_CUSTOM_EXT_NUM	 	0  // ~ 8MB Ȯ�� �������� ����� ���� �� �� ����
#endif

#define LABEL_CUSTOM_TOTAL_NUM \
	(LABEL_CUSTOM_2MB_NUM + LABEL_CUSTOM_EXT_NUM)	// �� �� ����

#endif // of #ifndef _MEMORY_MAP_DEFINE

#define MAX_PLU_RECORDSIZE_DEFAULT	256
#ifdef USE_USA_INGREDIENT
	#define MAX_PLU_ADDRESS 4000
	#define PLU_BASIC_AREA_QTY	3000l
	#define PLU_EXTER_AREA_QTY	1000l
#elif defined (USE_AUSTRALIA_INGREDIENT)
	#define MAX_PLU_ADDRESS 7000
	#define PLU_BASIC_AREA_QTY	3000l
	#define PLU_EXTER_AREA_QTY	4000l
#elif defined (USE_SAUDI_INGREDIENT)
	#define MAX_PLU_ADDRESS 3000
	#define PLU_BASIC_AREA_QTY	3000l
	#define PLU_EXTER_AREA_QTY	0
#elif defined (USE_NORTH_AMERICA_INGREDIENT)
	#define MAX_PLU_ADDRESS 2000
	#define PLU_BASIC_AREA_QTY	1000l
	#define PLU_EXTER_AREA_QTY	1000l
#else
	#define MAX_PLU_ADDRESS 10000
	#define PLU_BASIC_AREA_QTY	3000l
	#define PLU_EXTER_AREA_QTY	7000l
#endif

#define DEFAULT_INGREDIENT_SIZE 		512
#ifdef USE_DIRECT_INGREDIENT
 #ifdef USE_USA_INGREDIENT
	#define DIRECT_INGREDIENT_BLOCK_NUM		14	// Direct Ingredient Block ��(�� PLU�� ���� �� �ִ� �ִ� Block ��)
	#define INGREDIENT_BASIC_AREA_QTY		0l
	#define INGREDIENT_EXTER_AREA_QTY		0l
	#define INGREDIENT_TEXT_BASIC_AREA_QTY	3300l
	#define INGREDIENT_TEXT_EXTER_AREA_QTY	12000l
	#define PLU_REPORT_QTY_1				4000	//para 625 : 0, PLU ����(3000+1000)�� PLU report ����(3000+6000) �� ���� ��
	#define PLU_REPORT_QTY_2				4000	//para 625 : 1, PLU ����(3000+1000)�� PLU report ����(3000+4500) �� ���� ��
 #elif defined (USE_EMPORIUM_INGREDIENT)
	#define DIRECT_INGREDIENT_BLOCK_NUM		7		// Direct Ingredient Block ��(�� PLU�� ���� �� �ִ� �ִ� Block ��)
	#define INGREDIENT_BASIC_AREA_QTY		1000l
	#define INGREDIENT_EXTER_AREA_QTY		0l
	#define INGREDIENT_TEXT_BASIC_AREA_QTY	0l
	#define INGREDIENT_TEXT_EXTER_AREA_QTY	2000l
	#define PLU_REPORT_QTY_1				9000	//para 625 : 0, PLU ����(3000+7000)�� PLU report ����(3000+6000) �� ���� ��
	#define PLU_REPORT_QTY_2				7500	//para 625 : 1, PLU ����(3000+7000)�� PLU report ����(3000+4500) �� ���� ��
 #elif defined (USE_AUSTRALIA_INGREDIENT)
	#define DIRECT_INGREDIENT_BLOCK_NUM		1		// Direct Ingredient Block ��(�� PLU�� ���� �� �ִ� �ִ� Block ��)
	#define INGREDIENT_BASIC_AREA_QTY		1000l
	#define INGREDIENT_EXTER_AREA_QTY		0l
	#define INGREDIENT_TEXT_BASIC_AREA_QTY	0l
	#define INGREDIENT_TEXT_EXTER_AREA_QTY	7000l
	#define PLU_REPORT_QTY_1				7000	//para 625 : 0, PLU ����(3000+7000)�� PLU report ����(3000+6000) �� ���� ��
	#define PLU_REPORT_QTY_2				7000	//para 625 : 1, PLU ����(3000+7000)�� PLU report ����(3000+4500) �� ���� ��
 #elif defined (USE_SAUDI_INGREDIENT)
	#define DIRECT_INGREDIENT_BLOCK_NUM		4		// Direct Ingredient Block ��(�� PLU�� ���� �� �ִ� �ִ� Block ��)
	#define INGREDIENT_BASIC_AREA_QTY		1000l
	#define INGREDIENT_EXTER_AREA_QTY		0l
	#define INGREDIENT_TEXT_BASIC_AREA_QTY	0l
	#define INGREDIENT_TEXT_EXTER_AREA_QTY	12000l	// PLU 3000�� + Direct Ingredient 4Block / 4 Block x 3000 EA = 12000 EA
	#define PLU_REPORT_QTY_1				3000
	#define PLU_REPORT_QTY_2				3000	
#elif defined (USE_NORTH_AMERICA_INGREDIENT)
	#define DIRECT_INGREDIENT_BLOCK_NUM 	14		// Direct Ingredient Block ��(�� PLU�� ���� �� �ִ� �ִ� Block �� - 14 Blocks: 2,100��)
	#define INGREDIENT_BASIC_AREA_QTY		0l
	#define INGREDIENT_EXTER_AREA_QTY		0l
	#define INGREDIENT_TEXT_BASIC_AREA_QTY	3300l	// �̱��� ������ �޸� ���� ��� 
	#define INGREDIENT_TEXT_EXTER_AREA_QTY	12000l	// �̱��� ������ �޸� ���� ��� 
	#define PLU_REPORT_QTY_1				2000	//para 625 : 0, PLU ����(1000+1000)�� PLU report ����(3000+6000) �� ���� ��
	#define PLU_REPORT_QTY_2				2000	//para 625 : 1, PLU ����(1000+1000)�� PLU report ����(3000+4500) �� ���� ��
 #else // Standard Direct Ingredient
	#define DIRECT_INGREDIENT_BLOCK_NUM		7		// Direct Ingredient Block ��(�� PLU�� ���� �� �ִ� �ִ� Block ��)	
	#define INGREDIENT_BASIC_AREA_QTY		1000l
	#define INGREDIENT_EXTER_AREA_QTY		0l
	#define INGREDIENT_TEXT_BASIC_AREA_QTY	0l
	#define INGREDIENT_TEXT_EXTER_AREA_QTY	2000l
	#define PLU_REPORT_QTY_1				9000	//para 625 : 0, PLU ����(3000+7000)�� PLU report ����(3000+6000) �� ���� ��
	#define PLU_REPORT_QTY_2				7500    //para 625 : 1, PLU ����(3000+7000)�� PLU report ����(3000+4500) �� ���� ��
 #endif
	#define DEFAULT_INGREDIENT_TEXT_SIZE	152 
	#define DEFAULT_INGREDIENT_TEXT_ALL_SIZE  ((DEFAULT_INGREDIENT_TEXT_SIZE - 2) * DIRECT_INGREDIENT_BLOCK_NUM)  // ingredient ��ü����
#else	// Static indexed ingredient
	#ifdef USE_9999_INDIRECT_INGREDIENT		// ������ 510byte 999��, ���� index��ȣ�� 1~9999������ ��� ������
		#define MAX_INGREDIENT_INDEX_NUM		9999l		// Ingredient index Max �� 
		#define INGREDIENT_BASIC_AREA_QTY		1000l
		#define INGREDIENT_EXTER_AREA_QTY		0l
		#define INGREDIENT_TEXT_BASIC_AREA_QTY	0l
		#define INGREDIENT_TEXT_EXTER_AREA_QTY	0l
		#define PLU_REPORT_QTY_1				9000	//para 625 : 0, PLU ����(3000+7000)�� PLU report ����(3000+6000) �� ���� ��
		#define PLU_REPORT_QTY_2				7500	//para 625 : 1, PLU ����(3000+7000)�� PLU report ����(3000+4500) �� ���� ��
	#else
        #ifdef USE_INGREDIENT_CODE_4DIGIT
		#define MAX_INGREDIENT_INDEX_NUM		9999l		// Ingredient index Max �� 
		#endif
		#define INGREDIENT_BASIC_AREA_QTY		1000l
		#define INGREDIENT_EXTER_AREA_QTY		0l
		#define INGREDIENT_TEXT_BASIC_AREA_QTY	0l
		#define INGREDIENT_TEXT_EXTER_AREA_QTY	0l
		#ifdef USE_INDIA_FUNCTION
		#define PLU_REPORT_QTY_1				3000	//para 625 : 0, PLU ����(3000+7000)�� PLU report ����(3000 + 0) �� ���� ��
		#define PLU_REPORT_QTY_2				7500    //para 625 : 1, PLU ����(3000+7000)�� PLU report ����(3000+4500) �� ���� ��
		#else
		#define PLU_REPORT_QTY_1				9000	//para 625 : 0, PLU ����(3000+7000)�� PLU report ����(3000+6000) �� ���� ��
		#define PLU_REPORT_QTY_2				7500	//para 625 : 1, PLU ����(3000+7000)�� PLU report ����(3000+4500) �� ���� ��
		#endif
	#endif
	#define DEFAULT_INGREDIENT_TEXT_SIZE	304
#endif

