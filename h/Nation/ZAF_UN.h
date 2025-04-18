 /* 
 * File:   ZAF_UN.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 남아프리카 공화국 전용 파일
 */

#ifndef ZAF_UN_H
#define	ZAF_UN_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "ZA"
//----------   국가 전용 기능  ----------//

//----------   국가 전용 라벨 설정  ----------//

//----------   국가 전용 디스플레이 설정  ----------//

//----------   국가 전용 바코드 설정  ----------//
  #ifdef USE_QR_BARCODE
	#define USE_QR_ADDTOTAL
  #endif
//----------   국가 전용 특정업장 설정  ----------//

#endif  /* ZAF_UN_H */