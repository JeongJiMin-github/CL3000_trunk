 /* 
 * File:   LKA_K.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 스리랑카 전용 파일
 */

#ifndef LKA_K_H
#define	LKA_K_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	"(K)"	        // '(R)' -> '(K)' : CJK20100831
//----------   국가 전용 기능  ----------//
 #define USE_SRILANKA_ZERO			        // 스리랑카 인증
 //#define USE_TRACE_STANDALONE   	        // 축산 이력기능
 //#define USE_GSMART_PLU         	        // Use GSMart PLU Structure(Kor)
 //#define PLU_COMMODITY_LINE_NO 5	        // 상품명5단
 #define USE_TRACEABILITY       	        // Use European Traceability
 #define USE_NUTRITION_FACT		            // Use Nutrition Fact
 #define USE_SELLBYDATETIME_ZERO	        // Sell by Date, Sell by Time이 0일 때 인쇄 안하는 기능
 //#define USE_EMART_NETWORK      	        // E-Mart Network(실시간 판매 업로드, 매가변경 업로드)
 #define USE_TAX_RATE			            // Use Tax Rate 
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_REPORT_TAX 		            // TAX Report 사용  
 #define USE_REPORT_PAY         	        // PAY/CHANGE Report 사용
 #define USE_CONTINUE_INIT_ZERO_ERROR	    // SriLanka initial zero range error 표시(에러 상태 유지) 
 #define USE_HIDE_DATETIME_MENU		        // 메뉴 1840 CAL 로 이동 및 숨김.
 #define USE_SRILANKA_CERTI			        // 스리랑카 인증 요청사항 적용
//----------   국가 전용 라벨 설정  ----------//
 #define USE_ENG_LABELFORMAT		        // 영공 라벨포맷 사용
 //#define USE_KOR_LABELFORMAT		        // 국내 라벨포맷 사용(No.32~45 사용)
//----------   국가 전용 디스플레이 설정  ----------//

//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//

#endif /* LKA_K_H */