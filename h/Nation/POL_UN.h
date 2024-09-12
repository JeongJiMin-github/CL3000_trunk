 /* 
 * File:   POL_UN.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 폴란드 전용 파일
 */

#ifndef POL_UN_H
#define	POL_UN_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "PL"
 #define USE_POLAND_DEFAULT
//----------   국가 전용 기능  ----------//
 #define USE_EU_NUTRITIONFACT
 //#define USE_EU_NUTRITIONFACT_ENG_CAP         // Caption in english(Dealer define)
//----------   국가 전용 라벨 설정  ----------//
  #ifdef USE_EU_NUTRITIONFACT
    #define USE_LABEL_LENGTH_120MM		        // 기본 80mm
  #endif
//----------   국가 전용 디스플레이 설정  ----------//
 #define FONT_CODEPAGE CODEPAGE_1250_POL
//----------   국가 전용 바코드 설정  ----------//
  #ifdef USE_QR_BARCODE
    #define USE_QR_ADDTOTAL				//QR바코드 이용한 ADD TOTAL 기능 
    #define USE_LABEL_PRINT_QRADD_SALE // QR Add 판매 시 개별 라벨 인쇄 사용
  #endif
//----------   국가 전용 특정업장 설정  ----------//
		
#endif  /* POL_UN_H */