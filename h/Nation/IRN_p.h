 /* 
 * File:   IRN_p.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 Persian(이란) 전용 파일
 */

#ifndef IRN_p_H
#define	IRN_p_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING  "(p)"
 #define USE_PERSIA_DEFAULT
//----------   국가 전용 기능  ----------//
 #define USE_TRACEABILITY		        // Use European Traceability
 #define USE_NUTRITION_FACT	            // Use Nutrition Fact
 #define USE_TAX_RATE			        // Use Tax Rate
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_SELLBYDATETIME_ZERO        // Sell by Date, Sell by Time이 0일 때 인쇄 안하는 기능
 #define USE_REPORT_TAX			        // TAX Report 사용
 #define USE_REPORT_PAY			        // PAY/CHANGE Report 사용
 #define USE_PERSIAN_CALENDAR	        // 이란 달력 사용
 #define USE_ARAB_CONVERT		        // 아랍어 변환 사용
 #define USE_EXTEND_PRICE_PRIMARYSIGN	// Use 5character price sign (Iran sign: Toman)
 #define USE_SEND_TICKET_DATA
 #define USE_ZERO_RANGE_MSG		        // 영점 범위 초과로 무게 안잡힐 경우에 경고 메시지 사용
 #define USE_PERSIA_TICKET_PRT
//----------   국가 전용 라벨 설정  ----------//
 #define USE_ENG_LABELFORMAT 	        // 영공 라벨포맷 사용(No.1~28 사용)	  // CODE 0x407F(16511bytes) 
 #define USE_KOR_LABELFORMAT 	        // 국내 라벨포맷 사용(No.32~45 사용)	  // CODE 0x2314(8980bytes)
//----------   국가 전용 디스플레이 설정  ----------//
 #define USE_PRICECHECK_8DIGIT	        // Price Checkdigit(8digit)
 #define USE_PRICE_TOTAL_7DIGIT		    // Total 7자리 사용
 #define USE_PRICE_UNIT_7DIGIT	        // Unit Price 7자리 사용
 #define USE_ARAB_FONT			        // 아랍폰트 사용
  #ifdef USE_ARAB_FONT
    #define USE_PRINT_LTR_FOR_ARAB	// 아랍어 없는 데이터 LTR 방향으로 인쇄
  #endif
//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//

#endif  /* IRN_p_H */