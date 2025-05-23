 /* 
 * File:   KOR_EMART_E.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 한국 이마트 전용 파일
 */

#ifndef KOR_EMART_E_H
#define	KOR_EMART_E_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	"(E)"
 #define USE_EMART_DEFAULT
 #define USE_TRACE_MEATGRADE_EMART_DEFAULT
//----------   국가 전용 기능  ----------//
 #define USE_KOREA_VERSION_DISPLAY	    // KOREA 버전 표시
 #define USE_TRACE_STANDALONE   	    // 축산 이력기능
 #define USE_GSMART_PLU         	    // Use GSMart PLU Structure(Kor)
 #define PLU_COMMODITY_LINE_NO 5	    // 상품명5단
 //#define USE_TRACEABILITY       	    // Use European Traceability
 //#define USE_TAX_RATE				    // Use Tax Rate
 #define USE_SELLBYDATETIME_ZERO	    // Sell by Date, Sell by Time이 0일 때 인쇄 안하는 기능
 #define USE_EMART_NETWORK      	    // E-Mart Network(실시간 판매 업로드, 매가변경 업로드)
 //#define USE_REPORT_TAX         	    // TAX Report 사용
 //#define USE_REPORT_PAY         	    // PAY/CHANGE Report 사용
 #define USE_DBG_EMART_NETWORK	        // Use Debug Network(without print)
 #define USE_KEEPING_CONNETION  	    // 전송데이터가 계속 있을 때 Remote와 Connection 유지
 #define USE_MULTI_TRACE_NO_TABLE	    // Use Multi Trace Number Table with saving
 #define USE_BARCODE_UPRICE_OVER	    // Unit Price Barcode가 자리수 초과할 경우 'U'를 0으로 인쇄
 #define USE_KOR_SLAUGHT_CODE_4DIGIT	// 도축장 코드 4자리 사용(이마트 전용)   
 //#define USE_TRACE_FLAG_0_CHECK		// 개체관리플래그 0일때 개체선택 유무를 체크
 #define USE_CHECKING_HARMFUL_MEAT	    // 수입쇠고기 위해차단 기능(위해리스트 저울 저장)
 //#define USE_EXT_CHECKING_HARMFULNESS	// 위해리스트 서버 저장
 #define USE_FIXED_PLUTYPE			    // Use Fixed PLU Type(중량혼용, 수량혼용, 중량, 수량)
 #define USE_TRACE_MEATGRADE		    // 국내 등급표시(2011.7.1시행)
 #define USE_TRACE_SCANNER_FOR_GRADE	// 바코드 스캐너를 이용해 등급을 호출하는 기능
 #define USE_EMART_MEATGRADE_PRINT		// 이마트 등급표시 인쇄 사용
 #define USE_TRACE_CALL_MEATGRADE_KEY	// 등급변경 키 사용. (PLU 등급 미사용)
 //#define USE_CHECK_CHANGE_REASON	    // 단가 변경시 사유 코드 입력 기능.
 #define USE_CHECK_OUTOFPAPER_ON_LABEL	// Label모드에서 Paper End 체크하는 기능. 즉시 멈춤
 #define USE_EMART_BACK_LABEL		    // 이마트 후방라벨 기능 사용. Protocol 90->100 bytes
 //#define USE_NUTRITION_FACT		    // Use Nutrition Fact, USE_CONTROL_CHANGING_PRICE 과 동시 사용 불가. 
 #define USE_CONTROL_CHANGING_PRICE	    // 이마트(2012) 매가 변경 통제 기능, USE_S_PRICE_AS_BASE_PRICE와 같이 사용해야 함.
 #define USE_DIVIDING_FUNCTION		    // 쪼개기 기능 사용
 #define USE_EMART_PIG_TRACE
//----------   국가 전용 라벨 설정  ----------//
 //#define USE_ENG_LABELFORMAT		    // 영공 라벨포맷 사용
 //#define USE_KOR_LABELFORMAT		    // 국내 라벨포맷 사용(No.32~45 사용)
//----------   국가 전용 디스플레이 설정  ----------//

//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//

#endif  /* KOR_EMART_E_H */