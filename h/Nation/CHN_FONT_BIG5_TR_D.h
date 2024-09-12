 /* 
 * File:   CHN_FONT_BIG5_TR_D.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 TR tranditional chinese (big5) 전용 파일
 */

#ifndef CHN_FONT_BIG5_TR_D_H
#define	CHN_FONT_BIG5_TR_D_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	".TR"
 #define USE_USA_DEFAULT
//----------   국가 전용 기능  ----------//
 #define COUNTRY_USA
 #define USE_USA_UNIT_PRICE_DOT_DISP		// 미국 화장판에 맞는 $/kg, $/lb dot 표시
 #define USE_STANDARD_DIRECT_INGREDIENT
 #define USE_USA_INGREDIENT					// USA Direct Ingredient(PLU 4000EA, each ingredient data Max : 2100bytes)
  #ifdef USE_USA_INGREDIENT
    #define USE_DIRECT_INGREDIENT			// Use direct ingredient
    #define USE_INGREDIENT_98_PROTOCOL		// PLU 통신 할 경우 Direct ingredient는 98을 사용(protocol 매뉴얼 참조)
  #endif
 #define USE_TRACEABILITY					// Use European Traceability
 #define USE_NUTRITION_FACT					// Use Nutrition Fact
 #define USE_ADDITIONAL_NUTRIENT			// Nutrient 항목 추가.
 #define USE_SELLBYDATETIME_ZERO			// Sell by Date, Sell by Time이 0일 때 인쇄 안하는 기능
 #define USE_TRANSFER_MENU_SETTING			// MENU Setting Up/down 기능 추가, *아직 영공만 가능.
 #define USE_CHECK_OUTOFPAPER_ON_LABEL		// Label모드에서 Paper End 체크하는 기능. 즉시 멈춤
 #define USE_INWEIGHTKEY_WITHOUT_INTERVAL	// IN_Weight Key 사용시 Weight Interval에 상관없이 1g단위로 사용 가능
 #define USE_TEXT_EMPHASIS					// ~1 : Italic, ~2 : Bold, ~3 : Underline 강조하는 기능 이후 부터 스페이스 나 엔터가 나오기 전에 강조표현 유지한다
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_TAX_RATE						// Use Tax Rate
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_REPORT_TAX						// TAX Report 사용
 #define USE_REPORT_PAY						// PAY/CHANGE Report 사용
 #define ETH_LED_INDICATOR_DISABLE 		 	// wired lan link indicator disable for NTEP USA 		   
 #define USE_FIXEDWEIGHT_UNIT_OZ		    // Nonweight, Count PLU Fixed Weight Unit change to oz
 #define USE_JULIAN_DATE
 #undef USE_ALLOW_CAL_MODE	                // 미국은 해당 기능 동작 안 하게 디파인 제거
 #define USE_SHOW_DELETE_DESCRIPTION		// Delete 메뉴에서 삭제 항목 Graphci LCD에 표시 (메뉴 1223 group, 1243 SMSG, 1253 Origin, 1263 Barcode, 1323 Nutrifact 해당)
//----------   국가 전용 라벨 설정  ----------//
 //#define USE_ENG_LABELFORMAT				// 영공 라벨포맷 사용(No.1~28 사용)	   // CODE 0x407F(16511bytes)
 //#define USE_KOR_LABELFORMAT				// 국내 라벨포맷 사용(No.32~45 사용)    // CODE 0x2314(8980bytes)
 #define USE_USA_LABELFORMAT
 #define USE_LABEL_LENGTH_120MM		// 기본 80mm
//----------   국가 전용 디스플레이 설정  ----------//
  #ifdef _USE_LCD_7_26d_16x96_
    #define USE_MENU_DISPLAY_ON_GRAPHIC_AREA // 그래픽 LCD 에 메뉴 화면 표시
    #define USE_LABEL_NAME_SCROLL			//M1510 라벨명 스크롤 표시 (**CL5200J 디스플레이 테스트 필요**)
  #endif
 #define USE_CHN_FONT
 #define USE_BIG5_CHN_FONT 					//tranditional chinese (big5)
 #define FONT_CODEPAGE CODEPAGE_0950_CHN	//tranditional chinese code page
//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//

#endif  /* CHN_FONT_BIG5_TR_D_H */