 /* 
 * File:   TUR_UN.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� Ƣ��Ű�� ���� ����
 */

#ifndef TUR_UN_H
#define	TUR_UN_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "TR"
//----------   ���� ���� ���  ----------//
 #define USE_LONGPRESS_KEY_OVERRIDE         // long key �� PLU �ܰ� ���� ���
//----------   ���� ���� �� ����  ----------//

//----------   ���� ���� ���÷��� ����  ----------//
 #define FONT_CODEPAGE CODEPAGE_1254
//----------   ���� ���� ���ڵ� ����  ----------//
  #ifdef USE_QR_BARCODE
    #define USE_QR_ADDTOTAL				          // QR���ڵ� �̿��� ADD TOTAL ��� 
  #endif
//----------   ���� ���� Ư������ ����  ----------//

#endif /* TUR_UN_H */