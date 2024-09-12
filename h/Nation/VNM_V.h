 /* 
 * File:   VNM_V.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� ��Ʈ�� ���� ����
 */

#ifndef VNM_V_H
#define	VNM_V_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING	"(V)"
//----------   ���� ���� ���  ----------//
 #define USE_TRACEABILITY			        // Use European Traceability
 #define USE_NUTRITION_FACT			        // Use Nutrition Fact
 #define USE_TAX_RATE				        // Use Tax Rate
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_SELLBYDATETIME_ZERO	        // Sell by Date, Sell by Time�� 0�� �� �μ� ���ϴ� ���
 #define USE_REPORT_TAX         	        // TAX Report ���
 #define USE_REPORT_PAY         	        // PAY/CHANGE Report ���
 #define USE_CHECK_OUTOFPAPER_ON_LABEL	    // Label��忡�� Paper End üũ�ϴ� ���. ��� ���� 
 #define USE_INWEIGHTKEY_WITHOUT_INTERVAL	// IN_Weight Key ���� Weight Interval�� ������� 1g������ ��� ����
 #define USE_FIXED_PERCENT_KEY		        // fixed Percent discount key 
//----------   ���� ���� �� ����  ----------//
 #define USE_ENG_LABELFORMAT		        // ���� ������ ���
 //#define USE_KOR_LABELFORMAT		        // ���� ������ ���(No.32~45 ���)
//----------   ���� ���� ���÷��� ����  ----------//
 #define USE_PRICECHECK_8DIGIT		        // Price Checkdigit(8digit)
 #define USE_VIETNAM_FONT			        // ��Ʈ�� ��Ʈ ���
 #define FONT_CODEPAGE CODEPAGE_1258_VNM
 //#define USE_PRICE_8DIGIT			        // ���� 8�ڸ� ���(scroll)
//----------   ���� ���� ���ڵ� ����  ----------//

//----------   ���� ���� Ư������ ����  ----------//

#endif	/* VNM_V_H */