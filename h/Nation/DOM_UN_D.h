 /* 
 * File:   DOM_UN_D.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 direct_ingredient를 사용하는 도미니카 전용 파일
 */

#ifndef DOM_UN_D_H
#define	DOM_UN_D_H

//----------   국가 Default  ----------//
#define COUNTRY_STR "DO"
#define USE_DOMINICA_DEFAULT
//----------   국가 전용 기능  ----------//
 #define USE_STANDARD_DIRECT_INGREDIENT
 #define USE_FIXEDWEIGHT_UNIT_OZ		    // Nonweight, Count PLU Fixed Weight Unit change to oz
 #define USE_USA_INGREDIENT				    // USA Direct Ingredient(PLU 4000EA, each ingredient data Max : 2100bytes)
 #define ETH_LED_INDICATOR_DISABLE		    // wired lan link indicator disable for NTEP USA		
 #undef USE_ALLOW_CAL_MODE	                // 미국은 해당 기능 동작 안 하게 디파인 제거
 //#define USE_DISABLE_COMM_MENU			// communication menu disable.
//----------   국가 전용 라벨 설정  ----------//

//----------   국가 전용 디스플레이 설정  ----------//
 #define FONT_CODEPAGE CODEPAGE_0949_KOR
 #define USE_KOR_FONT
 #define USE_USA_UNIT_PRICE_DOT_DISP		// 미국 화장판에 맞는 $/kg, $/lb dot 표시
//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//

#endif  /* DOM_UN_D_H*/