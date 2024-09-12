 /* 
 * File:   DOM_UN_D.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� direct_ingredient�� ����ϴ� ���̴�ī ���� ����
 */

#ifndef DOM_UN_D_H
#define	DOM_UN_D_H

//----------   ���� Default  ----------//
#define COUNTRY_STR "DO"
#define USE_DOMINICA_DEFAULT
//----------   ���� ���� ���  ----------//
 #define USE_STANDARD_DIRECT_INGREDIENT
 #define USE_FIXEDWEIGHT_UNIT_OZ		    // Nonweight, Count PLU Fixed Weight Unit change to oz
 #define USE_USA_INGREDIENT				    // USA Direct Ingredient(PLU 4000EA, each ingredient data Max : 2100bytes)
 #define ETH_LED_INDICATOR_DISABLE		    // wired lan link indicator disable for NTEP USA		
 #undef USE_ALLOW_CAL_MODE	                // �̱��� �ش� ��� ���� �� �ϰ� ������ ����
 //#define USE_DISABLE_COMM_MENU			// communication menu disable.
//----------   ���� ���� �� ����  ----------//

//----------   ���� ���� ���÷��� ����  ----------//
 #define FONT_CODEPAGE CODEPAGE_0949_KOR
 #define USE_KOR_FONT
 #define USE_USA_UNIT_PRICE_DOT_DISP		// �̱� ȭ���ǿ� �´� $/kg, $/lb dot ǥ��
//----------   ���� ���� ���ڵ� ����  ----------//

//----------   ���� ���� Ư������ ����  ----------//

#endif  /* DOM_UN_D_H*/