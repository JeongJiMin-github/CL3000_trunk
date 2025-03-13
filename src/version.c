/*****************************************************************************
|*			
|*  Copyright		:	(c) 2002 CAS
|*  Filename		:	version.c
|*  Version			:	0.2
|*  Programmer(s)	:	Hong Yi-Phyo (HYP)
|*  Created			:	2003/05/21
|*  Description		:	CLP version string   
|*				
****************************************************************************/

#include "globals.h"
#include "revision_info.h"

////////////////////////////////////////////////////////
//Version String Description
//
// <Model>
//  CL : CL5000/CL5500
//  C3 : CL3500 (not use)
//  C4 : CL5200
//  C5 : CL5200J
//  C7 : CL7200
//	CA : CL3000
//
// <Type>
//  Num  Symbol  Scale Type        Model Name
//   1     B     Bench(Standard)   CL5500-B
//   2     P     Pole              CL5500-P/R, CL3500-P
//   3     H     Hanging           CL5500-H
//   4     S     Self-service      CL5500-S
//   5     D     Double Body       CL5500-D
//   6     B     Bench(CL5200)     CL5200-B
//   7     B     Bench(CL3000)     CL3000-B
//
// <Country> (현재 미사용)
//  KT1 : KOR 저울나라 버전
//  KS1 : KOR 이력기능버전
//  EU0 : 일반버전
////////////////////////////////////////////////////////

	//RUSSIA는 부팅시 버전 표시 고정. initial.c 에서 변경. 
//	                                   012345678901234

#define VER1	"3"
#define VER2	"05"
#define VER3	"07"

#ifdef USE_AUTO_FIND_VERSION
#define FINDING_START 	"_veRsIon"
#define FINDING_END		"_eNd"
#endif

#define VER4	"3"
#define VER5	"02"
#define VER6	"09"

#ifdef USE_PIC32MZ_EFG
	#ifdef CL3000_PROJECT
		ROMDATA  char VERSION_STRING[] = {"CA2102EU0P"VER3 VER1 VER2}; //CL3000-P
	#else
		ROMDATA  char VERSION_STRING[] = {"C52102EU0P"VER3 VER1 VER2}; //CL5200J-P
	#endif
#else
	#ifdef CL3000_PROJECT
		ROMDATA  char VERSION_STRING[] = {"CA2006EU0P"VER6 VER4 VER5}; //CL3000-P
	#else
		ROMDATA  char VERSION_STRING[] = {"C52006EU0P"VER6 VER4 VER5}; //CL5200J-P
	#endif
#endif
				//16 chars include NULL
				// 0~1  : Model
				// 2~3  : Year
				// 4~5  : Month	
				// 6~8  : Country
				// 9    : Type(Symbol)
				// 10~11: Sub-Version//Vendor	// Modified by CJK 20061121
				// 12~14: Version 

ROMDATA  char VERSION_STRING2[4] = {DEALER_VER_STRING};	// "(R)"
ROMDATA  char VERSION_COUNTRY[3] = {COUNTRY_STR};	// "AU"
#ifdef USE_AUTO_FIND_VERSION
ROMDATA  char GET_VERSION_NAME_GEN[] = FINDING_START "_" MODEL_NAME "N_" COUNTRY_STR "_" "V"VER1 VER2 VER3 "_" DEALER_VER_STRING "_" LOAD_REVISION FINDING_END;
#endif
void version_get(FW_VERSION *fw)
{
	INT16S i;
	for (i=0; i<sizeof(FW_VERSION); i++) {
		((INT8U*)fw)[i] = ((_rom unsigned char	*)VERSION_STRING)[i];
	}
}

void version2_get(FW_VERSION2 *fw)
{
	INT16S i, j;
	for (i=0; i<sizeof(VERSION_STRING2); i++) {
		((INT8U*)fw)[i] = ((_rom unsigned char	*)VERSION_STRING2)[i];	
	}
	for (j=0; j<sizeof(VERSION_COUNTRY); j++, i++) {
		((INT8U*)fw)[i] = ((_rom unsigned char	*)VERSION_COUNTRY)[j];	
	}
}
			   
