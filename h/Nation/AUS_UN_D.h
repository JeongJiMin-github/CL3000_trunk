 /* 
 * File:   AUS_UN_D.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 direct_ingredient를 사용하는 오스트레일리아 전용 파일
 */

#ifndef AUS_UN_D_H
#define	AUS_UN_D_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "AU"
 #define USE_AU_TICKET_DEFAULT
 #define USE_AUSTRALIA_DEFAULT			    // 호주 전용 디폴트 값 적용
//----------   국가 전용 기능  ----------//
 #define USE_STANDARD_DIRECT_INGREDIENT
 #define USE_AU_ROUND
 #define USE_AUSTRALIA_COOL_FUNCTION		// 호주 전용 CoOL(Country of Origin Label) 기능
 #define USE_AUSTRALIA_INGREDIENT		    // 호주 전용 PLU 7000ea, Direct ingredient block 7000ea
//----------   국가 전용 라벨 설정  ----------//
 // 호주는 전용 라벨 사용
 #define USE_AU_LABELFORMAT				    // 호주 인증(라벨에 프린트 되는 글자 2mm 이상)을 위한 호주 전용 라벨
//----------   국가 전용 디스플레이 설정  ----------//

//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//
		
#endif /* AUS_UN_D_H */