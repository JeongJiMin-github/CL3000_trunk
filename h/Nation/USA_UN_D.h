 /* 
 * File:   USA_UN_D.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� direct_ingredient�� ����ϴ� �̱� ���� ����
 */

#ifndef USA_UN_D_H
#define	USA_UN_D_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "US"
 #define USE_USA_DEFAULT
//----------   ���� ���� ���  ----------//
 #define USE_STANDARD_DIRECT_INGREDIENT
 #define USE_FIXEDWEIGHT_UNIT_OZ		        // Nonweight, Count PLU Fixed Weight Unit change to oz
 //#define USE_NORTH_AMERICA_INGREDIENT	        // �̱� / ĳ���� ���� Direct Ingredient & Nutrition Fact ���� ( PLU 2,000EA, each ingredient data Max : 2100bytes, each Nutrition Fact )
 //#define USE_DIRECT_NUTRITION
 #define USE_USA_INGREDIENT
 #define ETH_LED_INDICATOR_DISABLE		        // wired lan link indicator disable for NTEP USA		
 #undef USE_ALLOW_CAL_MODE	                    // �̱��� �ش� ��� ���� �� �ϰ� ������ ����
 //#define USE_DISABLE_COMM_MENU			    // communication menu disable.
 #define USE_SHOW_DELETE_DESCRIPTION		    // Delete �޴����� ���� �׸� Graphci LCD�� ǥ�� (�޴� 1223 group,  1243 SMSG, 1253 Origin, 1263 Barcode, 1323 Nutrifact �ش�)
 #define USE_ADDITIONAL_NUTRIENT			    // Nutrient �׸� �߰�.			
 #define USE_JULIAN_DATE
 #define USE_LABEL_LENGTH_120MM		            // �⺻ 80mm
 #define USE_FONT_ID6_12BY28				    // Font ID 6 12x28 font ���
 #define USE_FONT_ID4_Arial				        // Font ID 4 Arial font ���
 //#define USE_BEARER_BAR_ONE_LINE			    // ���ڵ� ��� �ϴ� �ɼǻ����� �׵θ� �ٸ� ����� �ϳ��θ� ���(�̱� ��û)
 #define USE_CANADA_ZERO_MARK_SETTING	    // ĳ���� ���� ���� �̱����� ���� CAL8170���� Gross Zero Mark[N] / Net Zero Mark[Y], Menu���� ���� �Ұ�
//----------   ���� ���� �� ����  ----------//
 // USA/CANADA ���� �� ���
 #define USE_USA_LABELFORMAT
//----------   ���� ���� ���÷��� ����  ----------//
 #define FONT_CODEPAGE CODEPAGE_0949_KOR
 #define USE_KOR_FONT
 #define USE_USA_UNIT_PRICE_DOT_DISP		    // �̱� ȭ���ǿ� �´� $/kg, $/lb dot ǥ��
  #ifdef _USE_LCD_7_26d_16x96_
    #define USE_MENU_DISPLAY_ON_GRAPHIC_AREA    // �׷��� LCD �� �޴� ȭ�� ǥ��
    #define USE_LABEL_NAME_SCROLL			    //M1510 �󺧸� ��ũ�� ǥ�� (**CL5200J ���÷��� �׽�Ʈ �ʿ�**)
  #endif
//----------   ���� ���� ���ڵ� ����  ----------//

//----------   ���� ���� Ư������ ����  ----------//
  	
#endif  /* USA_UN_D_H */            