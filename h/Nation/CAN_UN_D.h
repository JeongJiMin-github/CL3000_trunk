 /* 
 * File:   CAN_UN_D.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� direct_ingredient�� ����ϴ� ĳ���� ���� ����
 */

#ifndef CAN_UN_D_H
#define	CAN_UN_D_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "CA"
 #define USE_CANADA_DEFAULT
//----------   ���� ���� ���  ----------//
 #define USE_STANDARD_DIRECT_INGREDIENT
 #define USE_CANADA_CALUNIT				        // Cal unit lb, using kg
 #define USE_CANADA_100GToggle			        // 100g/kg key change to toggle
 // USA/CANADA ���� �� ���
 #define ETH_LED_INDICATOR_DISABLE		        // wired lan link indicator disable for NTEP USA		
 #undef USE_ALLOW_CAL_MODE				        // ����/ĳ���ٴ� �ش� ��� ���� �� �ϰ� ������ ����
  #ifdef USE_CANADA_MC_SETTING
    #define USE_CANADA_CERTI_DEFAULT			// ĳ���� ���� ���� �Ķ���� �� �� ���� ���� P536, P739, P740, M1510 PLU, Misc. PLU Label ID set to 51
    #define USE_CANADA_ZERO_MARK_SETTING	    // ĳ���� ���� ���� ����. CAL8170���� Gross Zero Mark[N] / Net Zero Mark[Y], Menu���� ���� �Ұ�
  #endif
 #define USE_FIXEDWEIGHT_UNIT_OZ		        // Nonweight, Count PLU Fixed Weight Unit change to oz
 #define USE_NORTH_AMERICA_INGREDIENT	        // �̱� / ĳ���� ���� Direct Ingredient & Nutrition Fact ���� ( PLU 2,000EA, each ingredient data Max : 2100bytes, each Nutrition Fact )
 #define USE_ADDITIONAL_NUTRIENT
 #define USE_DIRECT_NUTRITION
 #define USE_JULIAN_DATE
//----------   ���� ���� �� ����  ----------//
 #define USE_USA_LABELFORMAT
 #define USE_LABEL_LENGTH_120MM		// �⺻ 80mm
//----------   ���� ���� ���÷��� ����  ----------//
 #define USE_CANADA_FONT_DEFAULT
  #ifdef USE_CANADA_FONT_DEFAULT
    #define FONT_CODEPAGE CODEPAGE_0949_KOR
    #define USE_KOR_FONT
    #define USE_FONT_ID6_12BY28				    // Font ID 6 12x28 font ���
    #define USE_FONT_ID4_Arial				    // Font ID 4 Arial font ���
  #endif
 #define USE_CANADA_UNIT_PRICE_DOT_DISP		    // ĳ���� ȭ���ǿ� �´� $/kg, $/lb dot ǥ��
  #ifdef _USE_LCD_7_26d_16x96_
    #define USE_MENU_DISPLAY_ON_GRAPHIC_AREA    // �׷��� LCD �� �޴� ȭ�� ǥ��
    #define USE_LABEL_NAME_SCROLL			    //M1510 �󺧸� ��ũ�� ǥ�� (**CL5200J ���÷��� �׽�Ʈ �ʿ�**)
  #endif
//----------   ���� ���� ���ڵ� ����  ----------//

//----------   ���� ���� Ư������ ����  ----------//
	  
#endif /* CAN_UN_D_H */