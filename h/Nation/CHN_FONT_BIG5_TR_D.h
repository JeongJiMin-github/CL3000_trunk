 /* 
 * File:   CHN_FONT_BIG5_TR_D.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� TR tranditional chinese (big5) ���� ����
 */

#ifndef CHN_FONT_BIG5_TR_D_H
#define	CHN_FONT_BIG5_TR_D_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	".TR"
 #define USE_USA_DEFAULT
//----------   ���� ���� ���  ----------//
 #define COUNTRY_USA
 #define USE_USA_UNIT_PRICE_DOT_DISP		// �̱� ȭ���ǿ� �´� $/kg, $/lb dot ǥ��
 #define USE_STANDARD_DIRECT_INGREDIENT
 #define USE_USA_INGREDIENT					// USA Direct Ingredient(PLU 4000EA, each ingredient data Max : 2100bytes)
  #ifdef USE_USA_INGREDIENT
    #define USE_DIRECT_INGREDIENT			// Use direct ingredient
    #define USE_INGREDIENT_98_PROTOCOL		// PLU ��� �� ��� Direct ingredient�� 98�� ���(protocol �Ŵ��� ����)
  #endif
 #define USE_TRACEABILITY					// Use European Traceability
 #define USE_NUTRITION_FACT					// Use Nutrition Fact
 #define USE_ADDITIONAL_NUTRIENT			// Nutrient �׸� �߰�.
 #define USE_SELLBYDATETIME_ZERO			// Sell by Date, Sell by Time�� 0�� �� �μ� ���ϴ� ���
 #define USE_TRANSFER_MENU_SETTING			// MENU Setting Up/down ��� �߰�, *���� ������ ����.
 #define USE_CHECK_OUTOFPAPER_ON_LABEL		// Label��忡�� Paper End üũ�ϴ� ���. ��� ����
 #define USE_INWEIGHTKEY_WITHOUT_INTERVAL	// IN_Weight Key ���� Weight Interval�� ������� 1g������ ��� ����
 #define USE_TEXT_EMPHASIS					// ~1 : Italic, ~2 : Bold, ~3 : Underline �����ϴ� ��� ���� ���� �����̽� �� ���Ͱ� ������ ���� ����ǥ�� �����Ѵ�
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_TAX_RATE						// Use Tax Rate
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_REPORT_TAX						// TAX Report ���
 #define USE_REPORT_PAY						// PAY/CHANGE Report ���
 #define ETH_LED_INDICATOR_DISABLE 		 	// wired lan link indicator disable for NTEP USA 		   
 #define USE_FIXEDWEIGHT_UNIT_OZ		    // Nonweight, Count PLU Fixed Weight Unit change to oz
 #define USE_JULIAN_DATE
 #undef USE_ALLOW_CAL_MODE	                // �̱��� �ش� ��� ���� �� �ϰ� ������ ����
 #define USE_SHOW_DELETE_DESCRIPTION		// Delete �޴����� ���� �׸� Graphci LCD�� ǥ�� (�޴� 1223 group, 1243 SMSG, 1253 Origin, 1263 Barcode, 1323 Nutrifact �ش�)
//----------   ���� ���� �� ����  ----------//
 //#define USE_ENG_LABELFORMAT				// ���� ������ ���(No.1~28 ���)	   // CODE 0x407F(16511bytes)
 //#define USE_KOR_LABELFORMAT				// ���� ������ ���(No.32~45 ���)    // CODE 0x2314(8980bytes)
 #define USE_USA_LABELFORMAT
 #define USE_LABEL_LENGTH_120MM		// �⺻ 80mm
//----------   ���� ���� ���÷��� ����  ----------//
  #ifdef _USE_LCD_7_26d_16x96_
    #define USE_MENU_DISPLAY_ON_GRAPHIC_AREA // �׷��� LCD �� �޴� ȭ�� ǥ��
    #define USE_LABEL_NAME_SCROLL			//M1510 �󺧸� ��ũ�� ǥ�� (**CL5200J ���÷��� �׽�Ʈ �ʿ�**)
  #endif
 #define USE_CHN_FONT
 #define USE_BIG5_CHN_FONT 					//tranditional chinese (big5)
 #define FONT_CODEPAGE CODEPAGE_0950_CHN	//tranditional chinese code page
//----------   ���� ���� ���ڵ� ����  ----------//

//----------   ���� ���� Ư������ ����  ----------//

#endif  /* CHN_FONT_BIG5_TR_D_H */