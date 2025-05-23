 /* 
 * File:   UKR_Z_D.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 direct_ingredient를 사용하는 우크라이나 전용 파일
 */

#ifndef UKR_Z_D_H
#define	UKR_Z_D_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	"(Z)"
 #define USE_RUSSIA_DEFAULT			        // default set for russia 우크라이나는 러시아 기능과 동일
 #define USE_UKRAINE_DEFAULT
//----------   국가 전용 기능  ----------//
 #define USE_NUTRITION_FACT			        // Use Nutrition Fact
 #define USE_JULIAN_DATE			        // 바코드 인쇄시 julian date 사용 parameter 529에서 설정 0: 기존, 1:DDDYY(default), 2:DDDTT, 3: YMMDD
 #define USE_SELLBYDATETIME_ZERO	        // Sell by Date, Sell by Time이 0일 때 인쇄 안하는 기능
 #define USE_SELLBYDATE_CHANGE		        // Sell by Date 자리수에 동작 변경(Ignore PARAM#532, 6자리 : YYMMDD, 4자리 : 생산일에 더함)
 #define USE_DIRECT_INGREDIENT		        // use direct ingredient. Russia 는 항상 사용
 #define USE_INGREDIENT_98_PROTOCOL
 #define USE_PROTOCOL_CL5000JR		        // for CL5000J Protocol
 #define USE_INWEIGHTKEY_WITHOUT_INTERVAL	// IN_Weight Key 사용시 Weight Interval에 상관없이 1g단위로 사용 가능
 #define USE_RUSSIA_BARCODE_FUNCTION	    // 추가 심벌 추가 (decimal Point, sell by date. packing date, produce date)
 #define USE_PERCENT_WEIGHT			        // 영공 percent Tare 기능을 러시아에서 percent Weight 로 사용
 #define USE_DMSG_2100_BYTE			        // Direct Ingredient 2100자
 #define USE_REMOTE_KEY
 #define USE_REMOTE_DISP
 #define USE_ADMIN_EMAIL
 #define USE_TRANSFER_MENU_SETTING
 #define USE_KEY_BLOCK_IN_SELFSERVICE_CL5000JR
 #define USE_SALE_MIN_WEIGHT_AT_AUTO_MODE   // 카자흐스탄 요청사항: 라벨 Auto 모드에서 Min.Weight 이하 값 판매 가능하게 설정하는 기능
//----------   국가 전용 라벨 설정  ----------//

//----------   국가 전용 디스플레이 설정  ----------//
 #define USE_PRICE_TOTAL_7DIGIT		        // Total 7자리 사용
 #define USE_PRICE_UNIT_7DIGIT	            // Unit Price 7자리 사용
 #define USE_PRICECHECK_8DIGIT	            // Price Checkdigit(8digit)
 #define FONT_CODEPAGE CODEPAGE_1251
//----------   국가 전용 바코드 설정  ----------//
  #ifdef USE_QR_BARCODE
    #define USE_QR_ADDTOTAL			        // QR바코드 이용한 ADD TOTAL 기능
  #endif
//----------   국가 전용 특정업장 설정  ----------//

#endif /* UKR_Z_D_H */