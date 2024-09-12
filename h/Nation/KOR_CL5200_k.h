 /* 
 * File:   KOR_CL5200_k.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 CL5200 내수 버젼 (이력관리 기능 삭제) 전용 파일
 */

#ifndef KOR_CL5200_k_H
#define	KOR_CL5200_k_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	"(k)"
 #define USE_STANDARD_TRACE_DEFAULT
//----------   국가 전용 기능  ----------//
 #define USE_KOREA_VERSION_DISPLAY	    // KOREA 버전 표시
 #define USE_GSMART_PLU         	    // Use GSMart PLU Structure(Kor)
 #define PLU_COMMODITY_LINE_NO 5	    // 상품명5단
 #define USE_NUTRITION_FACT		        // Use Nutrition Fact
 #define USE_SELLBYDATETIME_ZERO	    // Sell by Date, Sell by Time이 0일 때 인쇄 안하는 기능
 #define USE_CHECK_OUTOFPAPER_ON_LABEL	// Label모드에서 Paper End 체크하는 기능. 즉시 멈춤
 //#define USE_KOREA_CL5200_STANDARD_KEY
 #define USE_KOREA_CL3000_STANDARD_KEY
 #define NOT_USE_TRACE_CL5200
//----------   국가 전용 라벨 설정  ----------//
 #define USE_KOR_LABELFORMAT		    // 국내 라벨포맷 사용(No.32~45 사용)
 //#define USE_LABEL_LENGTH_120MM		// 기본 80mm	//딜러롬(딜러코드 E(k)로 파일명 표시)
 #define USE_ENG_LABELFORMAT            // 기본 라벨 1~28번 추가
//----------   국가 전용 디스플레이 설정  ----------//
 #define FONT_CODEPAGE CODEPAGE_0949_KOR
 #define USE_KOR_FONT
//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//

#endif  /* KOR_CL5200_k_H */