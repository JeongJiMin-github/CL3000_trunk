/*****************************************************************************
|*			
|*  Copyright		:	(c) 2002 CAS
|*  Filename		:	prt_line.c
|*  Version		:	0.1
|*  Programmer(s)	:	Hong Yi-Phyo (HYP)
|*  Created		:	2002/12/10
|*  Description		:	    
|*				
****************************************************************************/

#include <stdio.h>
#include "globals.h"
#include "commbuf.h"
#include "prt_common.h"
#include "prt_render.h"
#include "prtdrv.h"

//HYP 20040602
extern short CheckBoundaryErrX(short sx,short ex);
extern short CheckBoundaryErrY(short sy,short ey);	

extern _inline INT8U Inline_Ror8(INT8U ch, INT8U count);

void PrtHLine(short x1,short x2,short y,short w,char bw)	  //short length disable!!!
{
	INT16U rx1,rx2,*prt_memory,*prt_memory2;
	short    dx1,dx2,x,i;
	short   lwidthbyte16;

	//HYP 20040723 
	if(PrtDrvStruct.PrtDirection == NON_INVERT) {
		x1 = x1 + PrtStruct.NonInvModeTphAlign;
		x2 = x2 + PrtStruct.NonInvModeTphAlign;
	}

	x1 = x1 + PrtStruct.LMargin;
	x2 = x2 + PrtStruct.LMargin;

	y = y + PrtStruct.TMargin;

	if (x1 > x2) {
		x=x2;
		x2=x1;
		x1=x;
	}

	if (w==0) {
		w=1;
	}
	// Check Boundary 
	if (CheckBoundary(x1,y,x2,y+w) == FALSE) {
//		return;
		//HYP 20040602
		x2 = x2 - CheckBoundaryErrX(x1,x2);
		w = w - CheckBoundaryErrY(y,y+w);
		if(x2 <= x1) return;
		if(w <= 0) return;
	}
	if (y>=PrtStruct.PrintableHeight) {
		y=PrtStruct.PrintableHeight-1; 
		w=1;
	}
	if (y+w>PrtStruct.PrintableHeight) {
		w=PrtStruct.PrintableHeight-y;
	}

	//
	dx1 = x1>>4;
	rx1 = x1&0x0f;
#if defined(USE_RSS_BARCODE) || defined(USE_ONLY_RSS_EXPAND_BARCODE)
	dx2 = x2>>4;
	rx2 = x2&0x0f;
#endif
	lwidthbyte16 = PrtStruct.PrintableWidthBytes>>1;
	prt_memory = (HUGEDATA INT16U *)PrtStruct.PrtMemory + (long)y*lwidthbyte16 + dx1; //you must cast!!

#if defined(USE_RSS_BARCODE) || defined(USE_ONLY_RSS_EXPAND_BARCODE)
	if (dx1 == dx2) {
		prt_memory2 = prt_memory;
		for (i=0; i<w; i++) {
			if (bw)	{
				*prt_memory2 |= (nlmask16[rx1] & lmask16[rx2]);
			} else {
				*prt_memory2 &= (lmask16[rx1] | nlmask16[rx2]);
			}
			prt_memory2+=lwidthbyte16;
		}
		return;
	}	
#endif
	if (rx1) {
		prt_memory2 = prt_memory;
		for (i=0; i<w; i++) {
			if (bw)	{
				*prt_memory2 |= nlmask16[rx1];
			} else {
				*prt_memory2 &= lmask16[rx1];
			}
			prt_memory2+=lwidthbyte16;
		}
		dx1++;
		prt_memory++;
	}

#ifdef USE_STANDARD_BARCODE
	dx2=x2>>4;
	rx2 = x2&0x0f;
#endif
	for (x=dx1; x<dx2; x++) {
		prt_memory2 = prt_memory;
		for (i=0; i<w; i++) {
			if(bw) {
				*prt_memory2 = 0xffff;  //
			} else {
				*prt_memory2 = 0x0000;  //
			}
			prt_memory2+=lwidthbyte16;
		}
		prt_memory++;
	}
	/* End Pos */
	if (rx2) {
		prt_memory2 = prt_memory;
		for (i=0; i<w; i++) {
			if (bw)	{
				*prt_memory2 |= lmask16[rx2];
			} else {
				*prt_memory2 &= nlmask16[rx2];
			}
			prt_memory2+=lwidthbyte16;
		}
	}
} 

void PrtVLine(short y1,short y2,short x,char w,char bw)	 // 0<w<=8
{
	INT8U mask,rx,dx,rw,maskl,nmaskl,maskr,nmaskr;
	long pos;
	short y,lwidthbyte;

	//HYP 20040723 
	if(PrtDrvStruct.PrtDirection == NON_INVERT) { 
		x = x + PrtStruct.NonInvModeTphAlign;
	}

	x = x + PrtStruct.LMargin;
	y1 = y1 + PrtStruct.TMargin;
	y2 = y2 + PrtStruct.TMargin;

	if (w==0) {
		w=1;
	}

	rx=x&0x07;
 	dx=x>>3;

	// Check Boundary 
	if (CheckBoundary(x,y1,x+w,y2) == FALSE) {
//		return;
		//HYP 20040602
		w = w - CheckBoundaryErrX(x,x+w);
		y2 = y2 - CheckBoundaryErrY(y1,y2);
		if(w <= 0) return;
		if(y2 <= y1) return;
	}

	lwidthbyte = PrtStruct.PrintableWidthBytes;
	pos=(long)y1*lwidthbyte+dx;

	rw = w & 0x07;
	if(rw == 0) {
		mask = 0xff;
	} else {
		mask = lmask[rw];
	}

	if (rx != 0) {
		mask = Inline_Ror8(mask,rx);
		maskr = mask & nlmask[rx];
		if(((8-rx) >= rw) && (rw != 0)) {
			if (bw)	{
				for (y=y1; y<=y2; y++) {
					PrtStruct.PrtMemory[pos] |= maskr;
					pos+=lwidthbyte;
				}
			} else {
				nmaskr = ~maskr;
				for (y=y1; y<=y2; y++) {
					PrtStruct.PrtMemory[pos] &= nmaskr;
					pos+=lwidthbyte;
				}
			}
		} else {
			maskl = mask & lmask[rx];
			if (bw)	{
				for (y=y1; y<=y2; y++) {
					PrtStruct.PrtMemory[pos] |= maskr;
					PrtStruct.PrtMemory[pos+1] |= maskl;
					pos+=lwidthbyte;
				}
			} else {
				nmaskr = ~maskr;
				nmaskl = ~maskl;
				for (y=y1; y<=y2; y++) {
					PrtStruct.PrtMemory[pos] &= nmaskr;
					PrtStruct.PrtMemory[pos+1] &= nmaskl;
					pos+=lwidthbyte;
				}
			}
		}
	} else {
		if (bw)	{
			for (y=y1; y<=y2; y++) {
				PrtStruct.PrtMemory[pos] |= mask;
				pos+=lwidthbyte;
			}
		} else {
			nmaskr = ~mask;
			for (y=y1; y<=y2; y++) {
				PrtStruct.PrtMemory[pos] &= nmaskr;
				pos+=lwidthbyte;
			}
		}
	}
}

void PrtRect(short x1,short y1,short x2,short y2,char w,char bw)
{
	//HYP 20040226
	if(w<=0) w=1;
	if(w>8) w=8;

	PrtHLine(x1,x2,y1,w,bw);
	PrtHLine(x1,x2,y2-w+1,w,bw);
	PrtVLine(y1,y2,x1,w,bw);
	PrtVLine(y1,y2,x2-w+1,w,bw);
}

void PrtRectFill(short x1,short y1,short x2,short y2,char bw)
{
	PrtHLine(x1,x2,y1,(y2-y1+1),bw);
}

void setPixel(int x, int y)
{
	int dx, rx;
	INT8U *prt_memory2;
	
	if(x >= 0) {
		dx = x >> 3;
		rx = x % 8;
	} else {
		dx = -(-x >> 3);
		rx = -(x % 8);
	}
	prt_memory2 = PrtStruct.PrtMemory + ((long)y * PrtStruct.PrintableWidthBytes)  + dx;
	if(x >= 0)
		*prt_memory2 |= (0x80 >> rx);
	else
		*prt_memory2 |= (0x1 << rx);
}

void plotEllipse(int xm, int ym, int a, int b)
{
	int x = -a, y = 0;
	long e2 = (long)b*b;
	long err = x*(2*e2+x)+e2;
	
	do {
		setPixel(xm-x, ym+y);
		setPixel(xm+x, ym+y);
		setPixel(xm+x, ym-y);
		setPixel(xm-x, ym-y);
		e2 = 2*err;
		if (e2 >= (x*2+1)*(long)b*b)
		err += (++x*2+1)*(long)b*b;
		if (e2 <= (y*2+1)*(long)a*a)
		err += (++y*2+1)*(long)a*a;
	} while (x <= 0);
	
	while (y++ < b) {
		setPixel(xm, ym+y);
		setPixel(xm, ym-y);
	}
}

void PrtEllipse(short x, short y, short width, short height, char w)
{
	int i, j;
	short rad_x;
	short rad_y;

	rad_x = (width - x + 1) / 2;
	rad_y = (height - y + 1) / 2;

	x = x + PrtStruct.LMargin;
	y = y + PrtStruct.TMargin;
	if (y >= PrtStruct.PrintableHeight) {
		y = PrtStruct.PrintableHeight - 1; 
	}
	if (w==0)
		w=1;
	
	if (CheckBoundary(x,y,x + rad_x * 2 + w, y + rad_y *2 + w) == FALSE) {
		return;
	}
	
	for(i = 0; i < w; i++){
		for(j = 0; j < w; j++) {
			plotEllipse(x + rad_x + i, y + rad_y + j, rad_x, rad_y);
		}
	}
}
