/*****************************************************************************
|*  Copyright		:	(c) 2003 CAS
|*  Filename		:	prt_cmd.c
|*  Version		:	0.1
|*  Modified		:	2004/08/20
****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "initial.h"
#include "commbuf.h"
#include "prt_common.h"
#include "mode_sale.h"
#include "common.h"
#include "prt_interpreter.h"
#include "barcode_file.h"
///#include "bar_common.h"
///#include "prt_line.h"
///#include "prt_font.h"
///#include "prt_cmdtest.h"
///#include "main.h"
///#include "mode_main.h"
///#include "mode_commun.h"
///#include "initial.h"
///#include "flash.h"
///#include "flash_app.h"
///#include "caption_app.h"
///#include "common_sunday.h"
///#include "plu_manager.h"
///#include "rtc.h"
///#include "prt_font.h"
///#include "plu_auxcall.h"
///
///#include "plu_manager.h"
///#include "tax.h"
///#include "rtc.h"
///#include "rtc_app.h"
///#include "common_sunday.h"
///#include "origin.h"
///#include "prt_advertise.h"
///#include "shopname.h"
///#include "clerk_app.h"
///#include "customer_app.h"
///#include "ingredient.h"
///#include "tare_file.h"
///#include "pcs_unit.h"
///#include "department.h"
///#include "pcs_unit.h"
///#include "traceability.h"
///#include "workplace.h"
///#include "label_caption.h"
///#include "adm_app.h"
///#include "nutrition_fact.h"
///#include "menu.h"
///#include "plu_device.h"

#include "prtdrv.h"
#include "prt_cmd.h"
#include "prt_app.h"
#include "mode_main.h"


#include "comm_app.h"
#include "main.h"
#include "dsp_common.h"
#include "flash_app.h"
#include "plu_device.h"


INT16U LabelCurY;
INT8U PrtFontType;
//!INT8U Rcpt_WidthMode;
INT8U prev_font_type = 0xff, prev_inchsp=0xff, prev_just=0xff;

void LabCmd_SetStrField(INT16U x, INT16U y, INT16U h, INT16U w) 
{
	INT16U	i;
	char string_buf[50];
	CWD cwd;

	i = 0;
	string_buf[i++] = _ESC;
    string_buf[i++] = 'S';
	string_buf[i++] = 'X';
	cwd.word = x;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	string_buf[i++] = ',';
	string_buf[i++] = 'Y';
	cwd.word = y;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	string_buf[i++] = ',';
	string_buf[i++] = 'H';
	cwd.word = h;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	string_buf[i++] = ',';
	string_buf[i++] = 'W';
	cwd.word = w;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	PutDataRxBuf(&CommBufPrt, string_buf, i);
}

void LabCmd_MakeBarField(INT16U x, INT16U y, INT16U fh, INT16U bh, INT16U ch, INT8U w) 
{
	INT16U	i;
	char string_buf[50];
	CWD cwd;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'B';
	string_buf[i++] = 'X';
	cwd.word = x;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	string_buf[i++] = ',';
	string_buf[i++] = 'Y';
	cwd.word = y;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	string_buf[i++] = ',';
	string_buf[i++] = 'H';
	cwd.word = fh;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	cwd.word = bh;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	cwd.word = ch;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	string_buf[i++] = ',';
	string_buf[i++] = 'W';
	string_buf[i++] = w;
	PutDataRxBuf(&CommBufPrt, string_buf, i);
}

void LabCmd_CharSize(INT8U m, INT8U n)
// m=width,n=height
{
	char string_buf[10];
	INT8U i;
	i = 0;
	//sprintf(string_buf, "%cM%c%cN%c", ESC, m,ESC, n);
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = 'M';
	string_buf[i++] = m;
	string_buf[i++] = ',';
	string_buf[i++] = 'N';
	string_buf[i++] = n;

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_PrintStart(INT8U num)
{
	char string_buf[10];

    string_buf[0] = _ESC;
	string_buf[1] = '!';
	string_buf[2] = num;
	PutDataRxBuf(&CommBufPrt,string_buf,3);
}

void LabCmd_ReverseFeed(void)
{
	char string_buf[5];

	string_buf[0] = BS;
	PutDataRxBuf(&CommBufPrt,string_buf,1);
}

void LabCmd_SetTextField(INT8U field_num,char *str)
{
	char string_buf[MAX_INGREDIENT_SIZE + 20];
	INT16U	i,j;

	i=0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '{';
	string_buf[i++] = field_num;
	string_buf[i++] = '\0';
	string_buf[i++] = ',';
	string_buf[i++] = 'G';

	string_buf[i++] = PrtFontType;//1=double, 0=one byte font

	string_buf[i++] = ',';
	string_buf[i++] = '[';
	j=0;
	while(str[j] != '\0') {
		string_buf[i++] = str[j++];
	}
	string_buf[i++] = '\0';

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_SetTextField_Direct(INT8U field_num, char *str, INT16U m_size)
{
	char string_buf[500];
	INT16U	i, len;

	i=0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '{';
	string_buf[i++] = field_num;
	string_buf[i++] = '\0';
	string_buf[i++] = ',';
	string_buf[i++] = 'G';

	string_buf[i++] = PrtFontType;//1=double, 0=one byte font

	string_buf[i++] = ',';
	string_buf[i++] = '[';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
//	len = strlen(str); //SG060309
	for(len=0; len<m_size; len++){
		if(str[len]=='\0') break;
	}
	if(len > m_size) len = m_size;
	PutDataRxBuf(&CommBufPrt,(HUGEDATA char*)str,len);
	string_buf[0] = '\0';
	PutDataRxBuf(&CommBufPrt,string_buf,1);
}

#ifdef USE_DIRECT_INGREDIENT

void LabCmd_SetTextField_DirectIngredient(INT8U field_num, INT8U *ingred_text, int len)
{
	INT16U	i;
	INT8U string_buf[15];

	i=0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '{';
	string_buf[i++] = field_num;
	string_buf[i++] = '\0';
	string_buf[i++] = ',';
	string_buf[i++] = 'G';

	string_buf[i++] = PrtFontType;//1=double, 0=one byte font

	string_buf[i++] = ',';
	string_buf[i++] = '[';
	PutDataRxBuf(&CommBufPrt,string_buf,i);

	PutDataRxBuf(&CommBufPrt,ingred_text,len);

	string_buf[0] = '\0';
	PutDataRxBuf(&CommBufPrt,string_buf,1);
}
#endif

//void LabCmd_SetBarField(INT8U field_num,HUGEDATA char *Str) // 

void LabCmd_DrawBarInField(INT8U field_num,INT8U bar_type,HUGEDATA char *str) // 
{
#ifdef USE_QR_ADDTOTAL
	char string_buf[1000];
#else
	char string_buf[100];
#endif
	INT16U	i,j;

#ifdef USE_QR_BARCODE
	memset (string_buf, 0, sizeof(string_buf)); //20140917
#endif
	i=0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'B';
	string_buf[i++] = '{';
	string_buf[i++] = field_num;
	string_buf[i++] = 0;
	string_buf[i++] = ',';
	string_buf[i++] = 'T';
	string_buf[i++] = bar_type;
	switch (bar_type) {
		case CODE128C:	// Deleted by CJK 20041213//Recovered by HYP 20050528
		case CODE11  :
		case I2OF5   :
		case S2OF5   :
		case M2OF5   :
		case MSI     :
			string_buf[i++] = ',';
			string_buf[i++] = 'B';
			string_buf[i++] = 1;
			break;
#ifdef USE_BARCODE_CODE128X
		case CODE128X:
			break;
#endif
#ifdef USE_QR_BARCODE // 20140916
		case QRCODE  :
			break;
#endif
	}
	string_buf[i++] = ',';
	string_buf[i++] = '[';
	j=0;
	while(str[j] != '\0') {
		string_buf[i++] = str[j++];
	}

#ifdef USE_QR_ADDTOTAL
	if ((bar_type == QRCODE) && (TotalQRFlag == 1)) {
		if (QrCfg_DelLastSuffix) // Not Use Last Suffix
		{
			string_buf[i-2] = '\0';
			string_buf[i-1] = '\0';
		}
	}
#endif
	string_buf[i++] = '\0';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_SetBarField(INT8U field_num) 
{
	char string_buf[10];

    string_buf[0] = _ESC;
	string_buf[1] = 'B';
	string_buf[2] = 'F';
	string_buf[3] = field_num;
	PutDataRxBuf(&CommBufPrt,string_buf,4);
}

void LabCmd_StrStart(void)
{
	char string_buf[10];
	INT16U	i;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '[';

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_StrEnd(void)
{
	char string_buf[10];

	string_buf[0] = 0;
	PutDataRxBuf(&CommBufPrt,string_buf,1);
}

void LabCmd_ClearLabelForm(void)
{
	char string_buf[10];
	INT16U	i;

	i = 0;
	string_buf[i++] = GS;
	string_buf[i++] = 'C';

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_ClearTphImage(void)
{
	char string_buf[10];
	INT16U	i;

	i = 0;
	string_buf[i++] = GS;
	string_buf[i++] = 'Z';

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_InvertPrt(INT8U mode)
{
	char string_buf[10];
	INT16U	i;

	i = 0;
	string_buf[i++] = GS;
	string_buf[i++] = 'I';
	string_buf[i++] = mode;

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_FormFeed(void)		// L, R
{
	char string_buf[10];

	string_buf[0] = FF;
	PutDataRxBuf(&CommBufPrt,string_buf,1);
}

void LabCmd_RewindOnOff(INT8U on)
{
	char string_buf[10];

	sprintf(string_buf, "%cM", GS);
	PutDataRxBuf(&CommBufPrt,string_buf,2);
	string_buf[0] = on;
	PutDataRxBuf(&CommBufPrt,string_buf,1);
}

void LabCmd_AlignmentFeed(void)
{
	INT8U backup_rewind;

	backup_rewind = PrtDrvStruct.RewindEnable;

	LabCmd_RewindOnOff(OFF);
	LabCmd_FormFeed();
	LabCmd_FormFeed();
	LabCmd_RewindOnOff(backup_rewind);

	PrintListWaitEnd();
}
/*//HYP 20060407 Erase non-use function
void LabCmd_RotateStr(INT8U rot)
{
	char string_buf[10];
	INT16U	i;

	i = 0;
	string_buf[i++] = ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = 'R';
	string_buf[i++] = rot;

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_RotateBar(INT8U rot)
{
	char string_buf[10];
	INT16U	i;

	i = 0;
	string_buf[i++] = ESC;
	string_buf[i++] = 'B';
	string_buf[i++] = 'R';
	string_buf[i++] = rot;

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void LabCmd_PrtStrLine(INT8U x, INT8U mw, INT8U mh, char *str, INT8U linefeed)
{
	INT16U calc_y;
	INT8U fontHeight;
	INT16U margin;

	margin = 32;
	fontHeight = FontStruct.FHbits1 * mh;
	calc_y = LabelCurY + fontHeight;

////sprintf(MsgBuf, "FH:%d, MY:%d, TH:%d, LH:%d CY:%d\r\n", 
////	FontStruct.FHbits1, mh, fontHeight, PrtStruct.LabelHeight, calc_y);
////MsgOut(MsgBuf);
	if(calc_y > PrtStruct.LabelHeight-margin) {
		LabCmd_PrintStart(1);
		PrintListWaitEnd();
		LabelCurY = 0;
		calc_y = LabelCurY + fontHeight;
	}
	LabCmd_CharSize(mw, mh);
	LabCmd_SetStrField(margin+x, margin+LabelCurY, fontHeight, PrtStruct.LabelWidth-margin*2-x);

	LabCmd_StrStart();
	PutDataRxBuf(&CommBufPrt,(HUGEDATA char *)str,strlen(str));
	LabCmd_StrEnd();

	if(linefeed) {
		LabelCurY = calc_y + StrStruct.LineSp;
	}
}

void RcptCmd_PrtStrLine(INT8U x, INT8U mw, INT8U mh, char *str, INT8U linefeed)
{
	INT16U calc_x, calc_xmargin;
	INT8U len, i;
	INT16U margin;
	char string_buf[50];

	margin = 0;

	calc_x = x / (14*mw);
	calc_xmargin = margin / (14*mw);

////	LabCmd_CharSize(mw, mh);
////	LabCmd_SetStrField(margin+x, margin+LabelCurY, fontHeight, PrtStruct.LabelWidth-margin*2-x);
////
////	LabCmd_StrStart();
////	PutDataRxBuf(&CommBufPrt,(HUGEDATA char *)str,strlen(str));
////	LabCmd_StrEnd();

	RcptCmd_CharSize(mw, mh);

	for(i = 0; i < calc_x+calc_xmargin; i++) {
		string_buf[i] = 0x20;
	}
	sprintf(string_buf+i, "%s", str);
	len = strlen(string_buf);
//sprintf(MsgBuf, "len:%d, calc_x:%d, calc_xmar:%d, i:%d\r\n", len, calc_x, calc_xmargin, i);
//MsgOut(MsgBuf);
	if(linefeed) {
		string_buf[len++] = LF;
		string_buf[len++] = 0;
		PutDataRxBuf(&CommBufPrt, string_buf, len);
	} else {
		string_buf[len++] = CR;
		string_buf[len++] = 0;
		PutDataRxBuf(&CommBufPrt, string_buf, len);
	}
		
}

void LabCmd_SkipLine(INT8U line)
{
	INT16U calc_y;
	INT8U fontHeight;
	INT16U margin;

	margin = 32;
	fontHeight = FontStruct.FHbits1 * line;
	calc_y = LabelCurY + fontHeight;

////sprintf(MsgBuf, "FH:%d, MY:%d, TH:%d, LH:%d CY:%d\r\n", 
////	FontStruct.FHbits1, mh, fontHeight, PrtStruct.LabelHeight, calc_y);
////MsgOut(MsgBuf);
	if(calc_y > PrtStruct.LabelHeight-margin) {
		LabCmd_PrintStart(1);
		PrintListWaitEnd();
		LabelCurY = 0;
		calc_y = LabelCurY + fontHeight;
	}
	LabelCurY = calc_y + StrStruct.LineSp;
}

void LabCmd_PrtBarcode(INT16U fh, INT16U bh, INT16U ch, INT8U w) 
{
	
	INT16U calc_y;
//	INT8U fontHeight;
	INT16U margin;

	margin = 32;
	calc_y = LabelCurY + bh + ch;

	if(calc_y > PrtStruct.LabelHeight-margin) {
		LabCmd_PrintStart(1);
		PrintListWaitEnd();
		LabelCurY = 0;
		calc_y = LabelCurY + bh + ch;
	}
	LabCmd_MakeBarField(margin, margin+LabelCurY, fh, bh, ch, w);
	LabelCurY = calc_y + StrStruct.LineSp;
}
*/

/***************************************************************
//void print_set_substitutebar(INT8U btype,HUGEDATA char *Str)
{
	char string_buf[100];
	INT16U	i;

	string_buf[0] = 0x1b;
	string_buf[1] = 'B';
	string_buf[2] = 'T';
	string_buf[3] = btype;
	PutDataRxBuf(&CommBufPrt,string_buf,4);

	string_buf[0] = 0x1b;
	string_buf[1] = 'B';
	string_buf[2] = '[';
	i = 0;
	while(Str[i] != '\0') {
		string_buf[3+i] = Str[i];
		i++;
	}
	string_buf[3+i] = '\0';
	PutDataRxBuf(&CommBufPrt,string_buf,4+i);
}
*/

void LabCmd_ClearLabel(void)
{
	LabCmd_ClearLabelForm();
	LabCmd_ClearTphImage();
	status_scale.cur_labelid = 0;
}

/////////////////////////////////////////////////////////////////////
void RcptCmd_SetFontType(INT8U type)
{
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = 'G';
	string_buf[i++] = type;
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void RcptCmd_CharSize(INT8U m, INT8U n)
{
	char string_buf[10];
	INT8U i;
	i = 0;
	//sprintf(string_buf, "%cM%c%cN%c", ESC, m,ESC, n);
	string_buf[i++] = _ESC;
	string_buf[i++] = 'M';
	string_buf[i++] = m;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'N';
	string_buf[i++] = n;

	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void RcptCmd_IntercharSP(INT8U inchsp)
{
//	char string_buf[10];
//	sprintf(string_buf, "%cSP", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
//	string_buf[0] = m;
//	PutDataRxBuf(&CommBufPrt,string_buf,1);
	char string_buf[10];
	INT8U i;

	if(prev_inchsp != inchsp || StrStruct.InterChSp != inchsp) {
		i = 0;
		string_buf[i++] = _ESC;
		string_buf[i++] = 'S';
		string_buf[i++] = 'P';
		string_buf[i++] = inchsp;
		PutDataRxBuf(&CommBufPrt,string_buf,i);
	}
	prev_inchsp = inchsp;
}

void RcptCmd_LineSP(INT8U m)
{
//	char string_buf[10];
//	sprintf(string_buf, "%cSU", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
//	string_buf[0] = m;
//	PutDataRxBuf(&CommBufPrt,string_buf,1);
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = 'U';
	string_buf[i++] = m;
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void RcptCmd_ChRot(INT8U m)
{
	char string_buf[4];
	INT8U i;
	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = 'O';
	string_buf[i++] = m;
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}
void RcptCmd_StrMirx(INT8U m)
{
	char string_buf[4];
	INT8U i;
	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = 'E';
	string_buf[i++] = m;
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}
void RcptCmd_SetFontId(INT8U m)
{
	char string_buf[10];
	INT8U i;
//#	static INT8U prev_fontid = 0xff;
//#
//#	if(m == prev_fontid) return;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = 'A';
	string_buf[i++] = m;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = 'B';
	string_buf[i++] = m;
	PutDataRxBuf(&CommBufPrt,string_buf,i);
//#	prev_fontid = m;
}


void RcptCmd_Start(void)
{
	char string_buf[2];
    string_buf[0] = _ESC;
	string_buf[1] = '!';
	PutDataRxBuf(&CommBufPrt,string_buf,2);
}

void RcptCmd_StrJust(INT8U just)
{
//	char string_buf[5];
//	sprintf(string_buf, "%cSJ%c", ESC, just);
//	PutDataRxBuf(&CommBufPrt,string_buf,4);
	char string_buf[10];
	INT8U i;

	if(prev_just != just) {
		i = 0;
		string_buf[i++] = _ESC;
		string_buf[i++] = 'S';
		string_buf[i++] = 'J';
		string_buf[i++] = just;
		PutDataRxBuf(&CommBufPrt,string_buf,i);
	}
	prev_just = just;
}

void RcptCmd_TypeBold(void)
{
//	char string_buf[5];
//	sprintf(string_buf, "%cS*", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '*';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}
/* HYP 20060407 Erase non-use function
void RcptCmd_TypeItalic(void)
{
//	char string_buf[5];
//	sprintf(string_buf, "%cS/", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '/';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void RcptCmd_TypeDLine(void)
{
//	char string_buf[5];
//	sprintf(string_buf, "%cS/", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '=';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}
*/
void RcptCmd_TypeUnderline(void)
{
//	char string_buf[5];
//	sprintf(string_buf, "%cS/", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '_';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void RcptCmd_TypeThroughLine(void)
{
//	char string_buf[5];
//	sprintf(string_buf, "%cS/", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '-';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

void RcptCmd_TypeNone(void)
{
//	char string_buf[5];
//	sprintf(string_buf, "%cS@", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'S';
	string_buf[i++] = '@';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}

INT8U Rcpt_CurNumOfTab;

void RcptCmd_CharTab(INT8U no)
{
	char ch;

	Rcpt_CurNumOfTab += no;
	while(no--) {
		ch = '\t';
		PutDataRxBuf(&CommBufPrt,&ch,1);
	}
}

void RcptCmd_CharLF(INT8U no)
{
	char ch;

	Rcpt_CurNumOfTab = 0;
	while(no--) {
		ch = '\n';
		PutDataRxBuf(&CommBufPrt,&ch,1);
	}
}

void RcptCmd_ReceiptFeed(void)
{
	char string_buf[10];
	INT8U i;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'F';
	PutDataRxBuf(&CommBufPrt,string_buf,i);
}
/////////////////////////////////////////////////////////////////////

void RcptCmd_PrintBarcode(INT8U bar_type, HUGEDATA char* bar_str)
{
#ifdef USE_QR_ADDTOTAL
	char string_buf[1000];  // 출력할 QR Code를 저장하는 버퍼(사이즈 조정)
#else
	char string_buf[100];
#endif
	INT16U	i,j;

#ifdef USE_QR_BARCODE
	memset (string_buf, 0, sizeof(string_buf)); //20140917
#endif

	i=0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'B';
//	string_buf[i++] = '{';
//	string_buf[i++] = field_num;
//	string_buf[i++] = 0;
//	string_buf[i++] = ',';
	string_buf[i++] = 'T';
	string_buf[i++] = bar_type;
	switch (bar_type) {
		case CODE128C:	// Deleted by CJK 20041213//Recovered by HYP 20050528
		case CODE11  :
		case I2OF5   :
		case S2OF5   :
		case M2OF5   :
		case MSI     :
	        string_buf[i++] = _ESC;
			string_buf[i++] = 'B';
			string_buf[i++] = 'B';
			string_buf[i++] = 1;
			break;
#ifdef USE_BARCODE_CODE128X
		case CODE128X:
			break;
#endif
#ifdef USE_QR_BARCODE // 20140916
		case QRCODE  :
			break;
#endif
	}
	string_buf[i++] = _ESC;
	string_buf[i++] = 'B';
	string_buf[i++] = '[';
	j=0;
	while(bar_str[j] != '\0') {
		string_buf[i++] = bar_str[j++];
	}
#ifdef USE_QR_ADDTOTAL
	if ((bar_type == QRCODE) && (TotalQRFlag == 1)) {
		if (QrCfg_DelLastSuffix) // Not Use Last Suffix
		{
			string_buf[i-2] = '\0';
			string_buf[i-1] = '\0';
		}
	}
#endif
	string_buf[i++] = '\0';
	PutDataRxBuf(&CommBufPrt,string_buf,i);


//	char string_buf[30];
//
//	sprintf(string_buf, "%cBT", ESC);
//	PutDataRxBuf(&CommBufPrt,string_buf,3);
//	string_buf[0] = bar_type;
//	PutDataRxBuf(&CommBufPrt,string_buf,1);
//
//	sprintf(string_buf, "%cB[%s", ESC, bar_str);
//	PutDataRxBuf(&CommBufPrt,string_buf,strlen(string_buf)+1); 
}

extern void PrintListWaitEnd(void);
void RcptCmd_ChangeMode(INT8U s_mode, INT8U is_backup)		// TO_RECEIPT, TO_RESTORE
{
	char string_buf[10];
	static INT8U backup_prt_mode, backup_gap_mode;

	if(s_mode == TO_RESTORE || s_mode == TO_RESTORE_FORCE) {	//TO_RESTORE
		if(PrtStruct.Mode != backup_prt_mode || s_mode == TO_RESTORE_FORCE) {
			sprintf(string_buf, "%cO", GS);
			PutDataRxBuf(&CommBufPrt,string_buf,2);
			string_buf[0] = backup_prt_mode;
			PutDataRxBuf(&CommBufPrt,string_buf,1);

			status_scale.cur_labelid = 0;	// Reload Label : Added by CJK 20050224
		}
		if(PrtDrvStruct.LabelGapMode != backup_gap_mode || s_mode == TO_RESTORE_FORCE) {
			sprintf(string_buf, "%cJ", GS);
			PutDataRxBuf(&CommBufPrt,string_buf,2);
			string_buf[0] = backup_gap_mode;
			PutDataRxBuf(&CommBufPrt,string_buf,1);
		}
		PrintListWaitEnd();
		return;
	}

	if(is_backup) {
		backup_prt_mode = PrtStruct.Mode;
		backup_gap_mode = PrtDrvStruct.LabelGapMode;
	}

	if(s_mode == TO_BACKUP) {
		return;
	} else if(s_mode == TO_RECEIPT) {
		if(PrtStruct.Mode != RECEIPT) {
			if(PrtFlashStruct.RewindEnable) {
				Operation.rewindReady = ON;
			}

			sprintf(string_buf, "%cO", GS);
			PutDataRxBuf(&CommBufPrt,string_buf,2);
			string_buf[0] = RECEIPT;
			PutDataRxBuf(&CommBufPrt,string_buf,1);
			PrintListWaitEnd();
		}
	} else if(s_mode == TO_GAPLABEL) {
		if(PrtStruct.Mode != LABEL) {
			sprintf(string_buf, "%cO", GS);
			PutDataRxBuf(&CommBufPrt,string_buf,2);
			string_buf[0] = LABEL;
			PutDataRxBuf(&CommBufPrt,string_buf,1);
		}
		sprintf(string_buf, "%cJ", GS);
		PutDataRxBuf(&CommBufPrt,string_buf,2);
		string_buf[0] = GAP;
		PutDataRxBuf(&CommBufPrt,string_buf,1);
		PrintListWaitEnd();
	} else if(s_mode == TO_NOGAPLABEL) {
		if(PrtStruct.Mode != LABEL) {
			sprintf(string_buf, "%cO", GS);
			PutDataRxBuf(&CommBufPrt,string_buf,2);
			string_buf[0] = LABEL;
			PutDataRxBuf(&CommBufPrt,string_buf,1);
		}
		sprintf(string_buf, "%cJ", GS);
		PutDataRxBuf(&CommBufPrt,string_buf,2);
		string_buf[0] = NO_GAP;
		PutDataRxBuf(&CommBufPrt,string_buf,1);
		PrintListWaitEnd();
	}
}

//extern HUGEDATA FONT1_STRUCT Font1Struct[MAX_FONT1_NUM];


void RcptCmd_BarHeight(INT16U fh, INT16U bh, INT16U ch) 
{
	INT16U	i;
	char string_buf[50];
	CWD cwd;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'B';
	string_buf[i++] = 'H';
	cwd.word = fh;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	cwd.word = bh;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;
	cwd.word = ch;
	string_buf[i++] = cwd.byte.left;
	string_buf[i++] = cwd.byte.right;

	PutDataRxBuf(&CommBufPrt, string_buf, i);
}


void RcptCmd_BarWidth(INT8U w)
{
	INT16U	i;
	char string_buf[50];

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'B';
	string_buf[i++] = 'W';
	string_buf[i++] = w;

	PutDataRxBuf(&CommBufPrt, string_buf, i);
}



void RcptCmd_BarGuard(INT8U guard) 
{
	INT16U	i;
	char string_buf[50];
//	CWD cwd;

	i = 0;
	string_buf[i++] = _ESC;
	string_buf[i++] = 'B';
	string_buf[i++] = 'G';
	string_buf[i++] = guard;

	PutDataRxBuf(&CommBufPrt, string_buf, i);
}

INT8U rcpt_tab_pt[4];
INT8U rcpt_tab_sz[4];
INT8U Rcpt_MaxWidthChar = 0;

//INT8U rcpt_tab_gap_big, rcpt_tab_gap;
			//  0	1   2	3  num
//INT8U tab_sz_1[5][5] = { {  7,  9, 10,  2,  3},		//TKT_LAY_0	// Max:30  40mm(30/36 char) / 60mm(50/56 char)
//		         {  8,  8, 12,  2,  3},		//TKT_LAY_1
//		         { 14, 14,  0,  2,  2},		//TKT_LAY_2
//		         { 28,  0,  0,  0,  1},		//TKT_LAY_3
//		         { 10,  9,  9,  2,  3} };	//TKT_LAY_4

//INT8U tab_sz_2[3][5] = { {  7,  8,  0,  0,  2},	//TKT_LAY_0	// Max:15  40mm(15/18 char) / 60mm(25/28 char)
//		         {  6,  9,  0,  0,  1},		//TKT_LAY_1
//		         { 14,  0,  0,  0,  1},		//TKT_LAY_2

//SG070503
			//  0	1   2	3  num
INT8U tab_sz_1[7][5] = { {  7,  9, 10,  2,  3},		//TKT_LAY_0	// Max:30  40mm(30/36 char) / 60mm(50/56 char)
		         {  8,  8, 12,  2,  3},		//TKT_LAY_1
		         { 14, 14,  0,  2,  2},		//TKT_LAY_2
		         { 28,  0,  0,  0,  1},		//TKT_LAY_3
		         { 10,  9,  9,  2,  3}, 	//TKT_LAY_4
			 { 12,  18, 0,  0,  1}, 	//TKT_LAY_5
			 {  7,  9, 10,  2,  3} };	//TKT_LAY_6
//SG070503 
INT8U tab_sz_2[7][5] = { {  6,  8,  8,  2,  3},		//TKT_LAY_0	// Max:15  40mm(15/18 char) / 60mm(25/28 char)
		         {  0,  0,  0,  0,  0},		//TKT_LAY_1
		         { 14,  0,  0,  0,  1},		//TKT_LAY_2
		         {  6,  7,  8,  2,  3}, 	//TKT_LAY_3
			 {  0,  0,  0,  0,  0}, 	//TKT_LAY_4
			 {  6,  9,  0,  0,  1}, 	//TKT_LAY_5
			 {  3,  3,  9,  2,  3} };	//TKT_LAY_6

//extern INT8U num_chars;

void RcptCmd_SetFontStyle(INT8U fontid, INT8U font_type, INT8U layer_type, INT8U inchsp, INT8U just)
{
//	INT8U num_chars;
	INT16U i;		
	INT8U ftype, tab_gap, diff, last_pt;


	RcptCmd_SetFontId(fontid);
	i = Font1Struct[fontid].Wbits1;
	//SG if(prev_font_type != font_type) {
		switch(font_type) {
			case RPT_FONT_SMALL :
				RcptCmd_CharSize(1, 1);
				//i *= 1;
				break;
			case RPT_FONT_NORMAL :
				RcptCmd_CharSize(1, 2);
				//i *= 1;
				break;
			case RPT_FONT_SUBTITLE :
				RcptCmd_CharSize(2, 2);
				i *= 2;
				break;
			case RPT_FONT_TITLE  :
				RcptCmd_CharSize(2, 3);
				i *= 2;
				break;
		}

	//SG }
	//SG prev_font_type = font_type;

	RcptCmd_IntercharSP(inchsp);
	RcptCmd_StrJust(just);

	i += inchsp;

	Rcpt_MaxWidthChar = PrtStruct.ReceiptWidth/i - 1;

	ftype = font_type/2;
	if(ftype == 0) {
		if((Rcpt_MaxWidthChar%2)) Rcpt_MaxWidthChar--;
		//Rcpt_MaxWidthChar -= 6;	// DEL070124
		if(layer_type == 0xff) return;
		if(layer_type > 6) layer_type = 0;

		last_pt = tab_sz_1[layer_type][4];

		tab_gap = 0;
		//for(i = 0; i < last_pt; i++) {
		for(i = 0; i < 3; i++) {
			tab_gap += tab_sz_1[layer_type][i];
		}
		if((Rcpt_MaxWidthChar-6) < tab_gap) tab_gap = 0;
		else {
			diff = (Rcpt_MaxWidthChar-6) - tab_gap;	// Margin (left:4char, right:2char) 포함
			tab_gap = diff / last_pt;
		}

		rcpt_tab_pt[0] = 0;
		for(i = 0; i < last_pt; i++) {
			rcpt_tab_sz[i] = tab_sz_1[layer_type][i] + tab_gap;
			if(diff%last_pt) {
				rcpt_tab_sz[i]++;
				diff--;
			}
			rcpt_tab_pt[i+1] = rcpt_tab_pt[i] + rcpt_tab_sz[i];
		}
		rcpt_tab_sz[i] = tab_sz_1[layer_type][i];
		i++;
		for(;i < 4; i++) {
			rcpt_tab_pt[i] = rcpt_tab_pt[i-1] + rcpt_tab_sz[i-1];
			rcpt_tab_sz[i] = tab_sz_1[layer_type][i];
		}
	} else {
		//if((Rcpt_MaxWidthChar%2)) Rcpt_MaxWidthChar--;
		//Rcpt_MaxWidthChar -= 3;	// DEL070124
		if(layer_type == 0xff) return;
		//SG if(layer_type > 2) layer_type = 0;
		if(layer_type > 6) layer_type = 0;

		last_pt = tab_sz_2[layer_type][4];

		tab_gap = 0;
		//for(i = 0; i < last_pt; i++) {
		for(i = 0; i < 3; i++) {
			tab_gap += tab_sz_2[layer_type][i];
		}
		if((Rcpt_MaxWidthChar-3) < tab_gap) tab_gap = 0;
		else {
			diff = (Rcpt_MaxWidthChar-3) - tab_gap;	// Margin (left:2char, right:1char) 포함
			tab_gap = diff / last_pt;
		}

		rcpt_tab_pt[0] = 0;
		for(i = 0; i < last_pt; i++) {
			rcpt_tab_sz[i] = tab_sz_2[layer_type][i] + tab_gap;
			//SG if(diff%tab_gap) {
			//SG	rcpt_tab_sz[i]++;
			//SG	diff--;
			//SG }
			rcpt_tab_pt[i+1] = rcpt_tab_pt[i] + rcpt_tab_sz[i];
		}
		rcpt_tab_sz[i] = tab_sz_2[layer_type][i];
		i++;
		for(;i < 4; i++) {
			rcpt_tab_pt[i] = rcpt_tab_pt[i-1] + rcpt_tab_sz[i-1];
			rcpt_tab_sz[i] = tab_sz_2[layer_type][i];
		}
	}
	
	//Rcpt_MaxWidthChar = rcpt_tab_pt[last_pt-1] + rcpt_tab_sz[last_pt-1];

//SGTEST
//sprintf(MsgBuf, "[%d][%d]nch=%3d, diff=%d, sz0=%2d, sz1=%2d, sz2=%2d, sz3=%2d, max=%2d, gap=%2d,layer=%2d\r\n"
//		, fontid, font_type, PrtStruct.ReceiptWidth, diff, rcpt_tab_sz[0], rcpt_tab_sz[1], rcpt_tab_sz[2], rcpt_tab_sz[3], Rcpt_MaxWidthChar, tab_gap,layer_type);
//MsgOut(MsgBuf);

}

//extern INT8U prev_font_type, prev_inchsp, prev_just;
void RcptCmd_DefaultSet(void)
{
	prev_font_type = 0xff;
	prev_inchsp = 0xff;
	prev_just = 0xff;

	RcptCmd_SetFontType(PrtFontType);//0=one byte font, 1=two byte font
	RcptCmd_SetFontId(3);
	RcptCmd_IntercharSP(0);
	//RcptCmd_LineSP(4);
	RcptCmd_LineSP(0);
	RcptCmd_TypeNone();
	RcptCmd_ChRot(0);		//이전 설정값 초기화
	RcptCmd_StrMirx(0);		//이전 설정값 초기화
}

void RcptCmd_PutStringInit(char *dst)
{
	INT8U i, size;

	size = Rcpt_MaxWidthChar;
	if(size > sizeof(dst)) size = sizeof(dst) - 1;
	for(i = 0; i < size; i++) {
		dst[i] = ' ';
	}
	dst[size] = 0;
}


void RcptCmd_PutString(INT8U position, char *dst, char *src, INT8U align)
{
	INT8U x, max_len, len, i, start_x;
	char align_t[3] = {'-', '+', '-'};
	char form[64];
	//char backup_ch;
	char temp_src[64];

	x       = rcpt_tab_pt[position];
	max_len = rcpt_tab_sz[position];

//	_memcpy((INT8U*)temp_src, (INT8U*)src, 64);
	memcpy((INT8U*)temp_src, (INT8U*)src, 64);
	temp_src[63] = 0;
	start_x = 0;
	len = strlen(temp_src);

//	backup_ch = 0;
	if(len > max_len) {
		//backup_ch = src[max_len];
		//if(position == 0 || len > x+max_len) {
		if(len > x+max_len) {
			temp_src[max_len] = 0;
		} else {
			x = x - (len - max_len);
			max_len = len;
		}
	} else {
		if(align == CENTER) {
			start_x = max_len - len;
			start_x++;	// 홀수 일때, 한칸 오른쪽으로 이동
			start_x /= 2;
			for(i=0;i<start_x;i++) form[i] = ' ';
		}
	}
	sprintf(form+start_x, "%%%c%ds", align_t[align], max_len-start_x);
	sprintf(dst+x, form, temp_src);
	//if(backup_ch) src[max_len] = backup_ch;
}


INT8U RcptCmd_PrintMultiLine(char *src, INT16U max_size, INT8U num_tab)
{
	INT16U len, prt_len;
	INT8U *addr;
	INT8U temp_tab, num_tab_ch;
	INT8U line_no;
	INT16U i;
	INT8U kor_flag;

	line_no = 0;
	addr = (INT8U*)src;
	if(num_tab >= Rcpt_CurNumOfTab) temp_tab = num_tab - Rcpt_CurNumOfTab;
	else temp_tab = num_tab;
	len = strlen((char *)addr);
	if(len > max_size) len = max_size;
	if(len == 0) {
		RcptCmd_CharLF(1);
		line_no++;
	}
	while(len > 0) {
		//prt_len = (INT16U)num_chars;
		prt_len = (INT16U)Rcpt_MaxWidthChar;// + 4;
		RcptCmd_CharTab(temp_tab);
		if(Rcpt_CurNumOfTab > 0) {
			num_tab_ch = PrtFlashStruct.HTabCh[Rcpt_CurNumOfTab-1];
			prt_len -= (INT16U)num_tab_ch;
		}
//sprintf(MsgBuf, "[3]num_ch=%d, prt_len=%d, len=%d, temp_tab=%d, cur_tab=%d, #_t_ch=%d, #tab=%d\r\n", num_chars, prt_len, len, temp_tab, Rcpt_CurNumOfTab, num_tab_ch, StrStruct.HTabCh[3]);
//MsgOut(MsgBuf);
//sprintf(MsgBuf, "prt_len=%d, len=%d, char1=%02x, char2=%02x\r\n", prt_len, len, addr[prt_len-1], addr[len-1]);
//MsgOut(MsgBuf);
		if(prt_len > len) prt_len = len;
//		if(addr[prt_len-1]&0x80) prt_len--;	// 한글일 경우
		kor_flag = 0;
		for(i = 0; i < prt_len; i++) {
			if(DspStruct.StrType == TWO_BYTE_FONT_STR) {	// Added by CJK 20060223
				if(kor_flag == 0) {
					if(addr[i] >= 0x80) kor_flag = 1;
				} else {
					kor_flag = 0;
				}
			}
			if(addr[i] == LF) {
				prt_len = i;
				break;
			}
		}
		if(DspStruct.StrType == TWO_BYTE_FONT_STR) {	// Added by CJK 20060223
			if(kor_flag == 1) prt_len--;
		}
		PutDataRxBuf(&CommBufPrt,(HUGEDATA char *)addr,prt_len);
		RcptCmd_CharLF(1);
		len -= prt_len;
		addr += prt_len;
		temp_tab = num_tab;
		line_no++;
		if(*addr == LF) {
			addr++;
			len--;
		}
	}
	return line_no;
}


void RcptCmd_PrintLine(INT8U line_no, INT8U width)
{
	char string_buf[100];
	char line_ch[2] = {'-', '='};
	INT8U size;

	if(width) size = width;
	else      size = Rcpt_MaxWidthChar - 2;// L_Margin:2char	// + 4;	// 왼쪽 TAB(4char)

	line_no--;
	line_no %= 2;
	//RcptCmd_SetFontStyle(3, RPT_FONT_NORMAL, TKT_LAY_N, 0, LEFT);
	memset((INT8U *)string_buf, line_ch[line_no], size);
	string_buf[size] = 0;
	PutDataRxBuf(&CommBufPrt, string_buf, size);
	RcptCmd_CharLF(1);
}


void RcptCmd_PrintChar(char ch)
{
	PutDataRxBuf(&CommBufPrt, &ch, 1);
}


void RcptCmd_PrintCharLine(char ch, INT8U length)
{
	char string_buf[100];
	INT8U size;

	if(length) size = length;
	else      size = Rcpt_MaxWidthChar - 2;// L_Margin:2char	// + 4;	// 왼쪽 TAB(4char)

	memset((INT8U *)string_buf, ch, size);
	string_buf[size] = 0;
	PutDataRxBuf(&CommBufPrt, string_buf, size);
}

#ifdef USE_ARAB_CONVERT
void RcptCmd_PrintArabAllign(char *str, INT8U maxCapSize)
{
	INT8U i, len;
	
	len = strlen(str);
	
	if(len < maxCapSize)
	{
		for(i=0;i<maxCapSize-len;i++) 
		{
			RcptCmd_PrintChar(' ');
		}
	}
	RcptCmd_CharTab(1);
	for(i=0;i<15;i++) RcptCmd_PrintChar(' ');
}
#endif