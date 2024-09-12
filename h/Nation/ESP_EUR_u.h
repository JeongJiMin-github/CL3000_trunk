 /* 
 * File:   ESP_EUR_u.h
 * Author: JiMin JEONG
 * @brief  globals.h에서 분리된 유럽 디폴트 사용 스페인 전용 파일
 */

#ifndef ESP_EUR_u_H
#define	ESP_EUR_u_H

//----------   국가 Default  ----------//
 #define COUNTRY_STR "ES"
//----------   국가 전용 기능  ----------//
 #define USE_TAX_RATE					    // Use Tax Rate
 #define USE_REPORT_TAX
//----------   국가 전용 라벨 설정  ----------//
 #undef USE_ONLY_LABEL                      // 스페인 티켓모드 사용
//----------   국가 전용 디스플레이 설정  ----------//

//----------   국가 전용 바코드 설정  ----------//

//----------   국가 전용 특정업장 설정  ----------//

#endif  /* ESP_EUR_u_H */