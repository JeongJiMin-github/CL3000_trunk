 /* 
 * File:   IND_UN.h
 * Author: JiMin JEONG
 * @brief  globals.h���� �и��� ���� �ε� ���� ����
 * Created 2024/06/05
 */

#ifndef IND_UN_H
#define	IND_UN_H

//----------   ���� Default  ----------//
 #define COUNTRY_STR "IN"
 #define USE_INDIA_DEFAULT
//----------   ���� ���� ���  ----------//
 #define USE_INDIA_FUNCTION				    // India Function(Item Code 13, Area Scale Mode)
 //#define USE_SRAM_PLU_INDIA				// NVRAM Plu
 #define USE_DISPLAY_OPT_CL5200			    // CL5200 Diplay �ӵ� ����
 #define USE_QUICK_DELETE_PLU			    // PLU Delete all �ӵ� ����
 #define USE_INDIA_PRINT_UNIT			    // �ε� ���� ȭ�� �� �ܰ� ǥ��
 #define USE_INGREDIENT_CODE_4DIGIT		    // ������ 510byte 999��, ���� index��ȣ�� 4�ڸ� (1~9999��) ��� ����
 //#define USE_9999_INDIRECT_INGREDIENT	    // ������ 510byte 999��, ���� index��ȣ�� 9999������ ��� ������
 #define USE_INDIA_TAX					    // �ε� ���� ���� (CGST, SGST, GST)
 #define USE_INDIA_TAX_REPORT_MENU		    // �ε� TAX �޴� �߰� 
 #define USE_TAX_RATE
 #define USE_CHECK_DUPLICATION_TICKET	    // duplication ticket ��� ����� ���, print �Ŀ� ���� ���θ� ����ڰ� �Ź� ����.
 #define USE_COPYKEY						// copy ticket ����� Ticket�� Copy ǥ��
 #define USE_PRINT_FILL_WEIGHT              // Fixed Weight �跮 �� �� ���� �� Gross Weight Field�� �̿��Ͽ� "Fill Weight"�� �μ�
 #define USE_QR_BARCODE_ERASE_GAP		    // QR Add Total �μ� �� QR ���ڵ尡 �μ�Ǵ� ��ġ�� �ִ��� �÷��� ���� ������ ���ּ� �μ�ǰ� �� - ������ Ƽ�� ���� �Ƴ��� ���� (�ε� ��û)
 #define USE_PRT_SCALE_NUM_IN_REPORT		// Print Scale Number in report
 #define USE_PRT_SCALE_NUM_IN_TICKET		// Ƽ�� �μ� �� Scale No ��� (�ε� ��û)
//----------   ���� ���� �� ����  ----------//
 #define USE_LABEL_LENGTH_120MM		        // �⺻ 80mm
//----------   ���� ���� ���÷��� ����  ----------//
 #define FONT_CODEPAGE CODEPAGE_1252_IND
//----------   ���� ���� ���ڵ� ����  ----------//
/*		 GS1 ���ڵ� ���� (�� ���ڵ� �ߺ� ���� �ȵ�)           */
//  #define USE_RSS_BARCODE
//  #define USE_ONLY_RSS_EXPAND_BARCODE
/* ���ڵ� ������ ���� (���� ������ ������ ������ ��� �ּ�ó��) */
 #define USE_BARCODE_LEN_50
//  #define USE_BARCODE_LEN_60
//  #define USE_BARCODE_LEN_70
//  #define USE_BARCODE_LEN_90
/////////////////////////////////////////////////////////////
  #ifdef USE_QR_BARCODE
    #define USE_QR_ADDTOTAL				    //QR���ڵ� �̿��� ADD TOTAL ��� 
  #endif
//----------   ���� ���� Ư������ ����  ----------//

#endif  /* IND_UN_H */            