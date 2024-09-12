 /* 
 * File:   CAN_UN_D.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 direct_ingredient를 사용하는 캐나다 전용 파일
 */

#ifndef CAN_UN_D_H
#define	CAN_UN_D_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "CA"
 #define USE_CANADA_DEFAULT
//----------   국가 전용 기능  ----------//
 #define USE_STANDARD_DIRECT_INGREDIENT
 #define USE_CANADA_CALUNIT				        // Cal unit lb, using kg
 #define USE_CANADA_100GToggle			        // 100g/kg key change to toggle
 // USA/CANADA 전용 라벨 사용
 #define ETH_LED_INDICATOR_DISABLE		        // wired lan link indicator disable for NTEP USA		
 #undef USE_ALLOW_CAL_MODE				        // 미주/캐나다는 해당 기능 동작 안 하게 디파인 제거
  #ifdef USE_CANADA_MC_SETTING
    #define USE_CANADA_CERTI_DEFAULT			// 캐나다 인증 관련 파라미터 및 라벨 설정 적용 P536, P739, P740, M1510 PLU, Misc. PLU Label ID set to 51
    #define USE_CANADA_ZERO_MARK_SETTING	    // 캐나다 인증 법규 적용. CAL8170에서 Gross Zero Mark[N] / Net Zero Mark[Y], Menu에서 변경 불가
  #endif
 #define USE_FIXEDWEIGHT_UNIT_OZ		        // Nonweight, Count PLU Fixed Weight Unit change to oz
 #define USE_NORTH_AMERICA_INGREDIENT	        // 미국 / 캐나다 전용 Direct Ingredient & Nutrition Fact 구조 ( PLU 2,000EA, each ingredient data Max : 2100bytes, each Nutrition Fact )
 #define USE_ADDITIONAL_NUTRIENT
 #define USE_DIRECT_NUTRITION
 #define USE_JULIAN_DATE
//----------   국가 전용 라벨 설정  ----------//
 #define USE_USA_LABELFORMAT
 #define USE_LABEL_LENGTH_120MM		// 기본 80mm
//----------   국가 전용 디스플레이 설정  ----------//
 #define USE_CANADA_FONT_DEFAULT
  #ifdef USE_CANADA_FONT_DEFAULT
    #define FONT_CODEPAGE CODEPAGE_0949_KOR
    #define USE_KOR_FONT
    #define USE_FONT_ID6_12BY28				    // Font ID 6 12x28 font 사용
    #define USE_FONT_ID4_Arial				    // Font ID 4 Arial font 사용
  #endif
 #define USE_CANADA_UNIT_PRICE_DOT_DISP		    // 캐나다 화장판에 맞는 $/kg, $/lb dot 표시
  #ifdef _USE_LCD_7_26d_16x96_
    #define USE_MENU_DISPLAY_ON_GRAPHIC_AREA    // 그래픽 LCD 에 메뉴 화면 표시
    #define USE_LABEL_NAME_SCROLL			    //M1510 라벨명 스크롤 표시 (**CL5200J 디스플레이 테스트 필요**)
  #endif
//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//
	  
#endif /* CAN_UN_D_H */