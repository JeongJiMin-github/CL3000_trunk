 /* 
 * File:   VNM_V.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 베트남 전용 파일
 */

#ifndef VNM_V_H
#define	VNM_V_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	"(V)"
//----------   국가 전용 기능  ----------//
 #define USE_TRACEABILITY			        // Use European Traceability
 #define USE_NUTRITION_FACT			        // Use Nutrition Fact
 #define USE_TAX_RATE				        // Use Tax Rate
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_SELLBYDATETIME_ZERO	        // Sell by Date, Sell by Time이 0일 때 인쇄 안하는 기능
 #define USE_REPORT_TAX         	        // TAX Report 사용
 #define USE_REPORT_PAY         	        // PAY/CHANGE Report 사용
 #define USE_CHECK_OUTOFPAPER_ON_LABEL	    // Label모드에서 Paper End 체크하는 기능. 즉시 멈춤 
 #define USE_INWEIGHTKEY_WITHOUT_INTERVAL	// IN_Weight Key 사용시 Weight Interval에 상관없이 1g단위로 사용 가능
 #define USE_FIXED_PERCENT_KEY		        // fixed Percent discount key 
//----------   국가 전용 라벨 설정  ----------//
 #define USE_ENG_LABELFORMAT		        // 영공 라벨포맷 사용
 //#define USE_KOR_LABELFORMAT		        // 국내 라벨포맷 사용(No.32~45 사용)
//----------   국가 전용 디스플레이 설정  ----------//
 #define USE_PRICECHECK_8DIGIT		        // Price Checkdigit(8digit)
 #define USE_VIETNAM_FONT			        // 베트남 폰트 사용
 #define FONT_CODEPAGE CODEPAGE_1258_VNM
 //#define USE_PRICE_8DIGIT			        // 가격 8자리 사용(scroll)
//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//

#endif	/* VNM_V_H */