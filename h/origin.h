#ifndef _ORIGIN_H_1
#define _ORIGIN_H_1


//#define		ORIGIN_STRUCT_SIZE			64
#define		ORIGIN_STRUCT_SIZE			34
#pragma pack(2)
typedef struct
{
	INT16U		origin_code;
	INT8U		origin_text[32];
//	INT16U		origin_reserve;
}	ORIGIN_STRUCT;
#pragma pack()
#endif

#ifndef _ORIGIN_H_2
#define _ORIGIN_H_2

extern 	void	origin_create(void);		// command code  0x1261
extern  void    origin_delete(void);		// command code  0x1262
extern  void 	origin_list(void);			// command code  0x1263

#endif 


 