 /* 
 * File:   IND_TATA_TA.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� TA �ε� TATA Group ���� �߿��� ���� ����
 */

#ifndef IND_TATA_TA_H
#define	IND_TATA_TA_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING  ".TA"
 #define USE_INDIA_TATA_DEFAULT					// �ε� TATA Group ���� �⺻�� ����
//----------   ���� ���� ���  ----------//
 #define USE_NUTRITION_FACT						// Use Nutrition Fact
 #define USE_SELLBYDATETIME_ZERO 				// Sell by Date, Sell by Time�� 0�� �� �μ� ���ϴ� ���
 #define USE_TRANSFER_MENU_SETTING				// MENU Setting Up/down ��� �߰�, *���� ������ ����.
 #define USE_CHECK_OUTOFPAPER_ON_LABEL			// Label��忡�� Paper End üũ�ϴ� ���. ��� ���� 
 #define USE_INWEIGHTKEY_WITHOUT_INTERVAL		// IN_Weight Key ���� Weight Interval�� ������� 1g������ ��� ����
 #define USE_TEXT_EMPHASIS
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_REMOTE_KEY
 #define USE_REMOTE_DISP
 #define USE_ADMIN_EMAIL
 #define USE_TAX_RATE							// Use Tax Rate
 #define USE_REPORT_TAX							// TAX Report ���
 #define USE_REPORT_PAY							// PAY/CHANGE Report ���
 #define USE_INDIA_FUNCTION						// India Function(Item Code 13, Area Scale Mode)
 #define USE_DISPLAY_OPT_CL5200					// CL5200 Diplay �ӵ� ����
 #define USE_QUICK_DELETE_PLU					// PLU Delete all �ӵ� ����
 #define USE_INDIA_PRINT_UNIT					// �ε� ���� ȭ�� �� �ܰ� ǥ��
 #define USE_INGREDIENT_CODE_4DIGIT				// ������ 510byte 999��, ���� index��ȣ�� 4�ڸ� (1~9999��) ��� ����
 #define USE_INDIA_TAX							// �ε� ���� ���� (CGST, SGST, GST)
 #define USE_INDIA_TAX_REPORT_MENU				// �ε� TAX �޴� �߰� 
 #define USE_TAX_RATE
 #define USE_CHECK_DUPLICATION_TICKET			// duplication ticket ��� ����� ���, print �Ŀ� ���� ���θ� ����ڰ� �Ź� ����.
 #define USE_COPYKEY							// copy ticket ����� Ticket�� Copy ǥ��
 #define USE_PRINT_FILL_WEIGHT           		// Fixed Weight �跮 �� �� ���� �� Gross Weight Field�� �̿��Ͽ� "Fill Weight"�� �μ�
//----------   ���� ���� �� ����  ----------//
 #define USE_ENG_LABELFORMAT					// ���� ������ ���(No.1~28 ���)		// CODE 0x407F(16511bytes)
 #define USE_KOR_LABELFORMAT					// ���� ������ ���(No.32~45 ���)	// CODE 0x2314(8980bytes)
 #define USE_LABEL_LENGTH_120MM					// �⺻ 80mm
//----------   ���� ���� ���÷��� ����  ----------//
 #define FONT_CODEPAGE CODEPAGE_1252_IND
//----------   ���� ���� ���ڵ� ����  ----------//
/*		 GS1 ���ڵ� ���� (�� ���ڵ� �ߺ� ���� �ȵ�)           */
//  #define USE_RSS_BARCODE
//  #define USE_ONLY_RSS_EXPAND_BARCODE
/* ���ڵ� ������ ���� (���� ������ ������ ������ ��� �ּ�ó��) */
 #define USE_BARCODE_LEN_50
//  #define USE_BARCODE_LEN_60
//  #define USE_BARCODE_LEN_70
//  #define USE_BARCODE_LEN_90
/////////////////////////////////////////////////////////////
  #ifdef USE_QR_BARCODE
    #define USE_QR_ADDTOTAL						//QR���ڵ� �̿��� ADD TOTAL ��� 
  #endif
//----------   ���� ���� Ư������ ����  ----------//

#endif  /* IND_TATA_TA_H */