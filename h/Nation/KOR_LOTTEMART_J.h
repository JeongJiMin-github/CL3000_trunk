 /* 
 * File:   KOR_LOTTEMART_J.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� Lotte Mart(���� ������ + GS1 DataBar Expand) ���� ����
 */

#ifndef KOR_LOTTEMART_J_H
#define	KOR_LOTTEMART_J_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	"(J)"
 #define USE_LOTTEMART_DEFAULT
 #define USE_TRACE_MEATPART_DEFAULT	        // �⺻ ������ 11�� ���
 #define USE_TRACE_MEATGRADE_STANDARD_DEFAULT
//----------   ���� ���� ���  ----------//
 #define USE_KOREA_VERSION_DISPLAY	        // KOREA ���� ǥ��
 #define USE_IMPORT_MEAT_PROTOCOL
 #define USE_LOTTEMART_BARCODE_FUNCTION
 #define USE_TRACE_STANDALONE   	        // ��� �̷±��
 #define USE_GSMART_PLU         	        // Use GSMart PLU Structure(Kor)
 #define PLU_COMMODITY_LINE_NO 5	        // ��ǰ��5��
 #define USE_NUTRITION_FACT		            // Use Nutrition Fact
 #define USE_SELLBYDATETIME_ZERO	        // Sell by Date, Sell by Time�� 0�� �� �μ� ���ϴ� ���
 #define USE_CHECKING_HARMFUL_MEAT	        // ���Լ��� �������� ���(���ظ���Ʈ ���� ����)
 #define USE_TRACE_MEATPART			        // ������ ���
 #define USE_MULTI_TRACE_NO_TABLE	        // Use Multi Trace Number Table with saving
 #define USE_TRACE_MEATGRADE		        // ���� ���ǥ��(2011.7.1����)
 #define USE_HIDE_WIRELESS_PASSWORD	        // Wireless Security Password �Է½� '*'ǥ��
 #define USE_CHECK_OUTOFPAPER_ON_LABEL	    // Label��忡�� Paper End üũ�ϴ� ���. ��� ����
 #define USE_LOTTEMART_PROTOCOL		        // �ǽð����ۿ� �Ǹ�data�� ������� �ʰ� ���۵��� ����. ����data�� ����.
 #define USE_LOTTEMART_PIG_TRACE	        // �����̷� �� LOT 14�ڸ� �߰� 
 #define USE_LOTTEMART_ITEMCODE13           // Ext.barcode �ʵ带 13�ڸ� ITEMCODE�� ����ϴ� ���.
 #define USE_EMART_CHANGE_DATA_AT_NON_SALE
 #define USE_LOTTEMART_NETWORK
//----------   ���� ���� �� ����  ----------//

//----------   ���� ���� ���÷��� ����  ----------//

//----------   ���� ���� ���ڵ� ����  ----------//
/*		 GS1 ���ڵ� ���� (�� ���ڵ� �ߺ� ���� �ȵ�)           */
//  #define USE_RSS_BARCODE
 #define USE_ONLY_RSS_EXPAND_BARCODE
/* ���ڵ� ������ ���� (���� ������ ������ ������ ��� �ּ�ó��) */
//  #define USE_BARCODE_LEN_50
//  #define USE_BARCODE_LEN_60
 #define USE_BARCODE_LEN_70
//  #define USE_BARCODE_LEN_90
/////////////////////////////////////////////////////////////
//----------   ���� ���� Ư������ ����  ----------//

#endif  /* KOR_LOTTEMART_J_H */