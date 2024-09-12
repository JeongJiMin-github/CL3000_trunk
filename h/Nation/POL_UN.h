 /* 
 * File:   POL_UN.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� ������ ���� ����
 */

#ifndef POL_UN_H
#define	POL_UN_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "PL"
 #define USE_POLAND_DEFAULT
//----------   ���� ���� ���  ----------//
 #define USE_EU_NUTRITIONFACT
 //#define USE_EU_NUTRITIONFACT_ENG_CAP         // Caption in english(Dealer define)
//----------   ���� ���� �� ����  ----------//
  #ifdef USE_EU_NUTRITIONFACT
    #define USE_LABEL_LENGTH_120MM		        // �⺻ 80mm
  #endif
//----------   ���� ���� ���÷��� ����  ----------//
 #define FONT_CODEPAGE CODEPAGE_1250_POL
//----------   ���� ���� ���ڵ� ����  ----------//
  #ifdef USE_QR_BARCODE
    #define USE_QR_ADDTOTAL				//QR���ڵ� �̿��� ADD TOTAL ��� 
    #define USE_LABEL_PRINT_QRADD_SALE // QR Add �Ǹ� �� ���� �� �μ� ���
  #endif
//----------   ���� ���� Ư������ ����  ----------//
		
#endif  /* POL_UN_H */