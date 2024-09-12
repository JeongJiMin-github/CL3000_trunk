 /* 
 * File:   IRN_p.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� Persian(�̶�) ���� ����
 */

#ifndef IRN_p_H
#define	IRN_p_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "  "
 #define DEALER_VER_STRING  "(p)"
 #define USE_PERSIA_DEFAULT
//----------   ���� ���� ���  ----------//
 #define USE_TRACEABILITY		        // Use European Traceability
 #define USE_NUTRITION_FACT	            // Use Nutrition Fact
 #define USE_TAX_RATE			        // Use Tax Rate
 #define USE_DISCOUNTED_TAX_FOR_DISCOUNTED_TOTAL
 #define USE_SELLBYDATETIME_ZERO        // Sell by Date, Sell by Time�� 0�� �� �μ� ���ϴ� ���
 #define USE_REPORT_TAX			        // TAX Report ���
 #define USE_REPORT_PAY			        // PAY/CHANGE Report ���
 #define USE_PERSIAN_CALENDAR	        // �̶� �޷� ���
 #define USE_ARAB_CONVERT		        // �ƶ��� ��ȯ ���
 #define USE_EXTEND_PRICE_PRIMARYSIGN	// Use 5character price sign (Iran sign: Toman)
 #define USE_SEND_TICKET_DATA
 #define USE_ZERO_RANGE_MSG		        // ���� ���� �ʰ��� ���� ������ ��쿡 ��� �޽��� ���
 #define USE_PERSIA_TICKET_PRT
//----------   ���� ���� �� ����  ----------//
 #define USE_ENG_LABELFORMAT 	        // ���� ������ ���(No.1~28 ���)	  // CODE 0x407F(16511bytes) 
 #define USE_KOR_LABELFORMAT 	        // ���� ������ ���(No.32~45 ���)	  // CODE 0x2314(8980bytes)
//----------   ���� ���� ���÷��� ����  ----------//
 #define USE_PRICECHECK_8DIGIT	        // Price Checkdigit(8digit)
 #define USE_PRICE_TOTAL_7DIGIT		    // Total 7�ڸ� ���
 #define USE_PRICE_UNIT_7DIGIT	        // Unit Price 7�ڸ� ���
 #define USE_ARAB_FONT			        // �ƶ���Ʈ ���
  #ifdef USE_ARAB_FONT
    #define USE_PRINT_LTR_FOR_ARAB	// �ƶ��� ���� ������ LTR �������� �μ�
  #endif
//----------   ���� ���� ���ڵ� ����  ----------//

//----------   ���� ���� Ư������ ����  ----------//

#endif  /* IRN_p_H */