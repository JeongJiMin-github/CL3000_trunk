/*****************************************************************************
|*			
|*  Copyright		:	(c) 2002 CAS
|*  Filename		:	prt_render.h
|*  Version			:	0.1
|*  Programmer(s)	:	Hong Yi-Phyo (HYP)
|*  Created			:	2002/12/10
|*  Description		:	    
|*				
****************************************************************************/

#ifndef _RENDER_H
#define _RENDER_H

//HYP 20060427 str, bar field is ignored in pre-print area(except shop data) if pre-print mode is on
//fixed str, fixed barcode, bitmap, line is accepted
//#define USE_ONLY_SHOPNAME_PRE_PRT_AREA

extern ROMDATA INT8U nlmask[];
extern ROMDATA INT8U lmask[];
extern ROMDATA INT16U nlmask16[];
extern ROMDATA INT16U lmask16[];

extern INT8U	CheckPrePrtAreaBoundary;

//extern void RenderTest(void);
extern void PrtBarPattern(HUGEDATA INT8U *pattern,short pw);
extern void PrtPostbarPattern(HUGEDATA INT8U *pattern,short pw);
extern void PrtPattern(HUGEDATA INT8U *pattern,short dwr,short w,short h,char rot);
extern void PrtPatternHVflip(HUGEDATA INT8U *pattern,short dwr,short w,short h,char rot,char hv);
//20040209 HYP
extern void PrtPattern_Str_Area_Typeface(void);
extern void PrtTestPattern(void);
extern BOOLEAN	CheckBoundary(short sx, short sy, short ex, short ey) ;
//HYP 20050216
extern void PrtTestHPattern(void);
#endif /* _RENDER_H */
