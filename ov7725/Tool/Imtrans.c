#include "Imtrans.h"
#include "picture.h"
#include "config.h"
#include "camera/SCCB.h"
#include "arm_math.h"
#include "LCD/LCD.h"
#include "usart.h"

#define   RGB565_MASK_RED         0xF800 
#define   RGB565_MASK_GREEN     0x07E0 
#define   RGB565_MASK_BLUE       0x001F 

/*
量化补偿，说明:
 16bit RGB565 -> 24bit RGB888 的转换
 16bit RGB656 R4 R3 R2 R1 R0 G5 G4 G3 G2 G1 G0 B4 B3 B2 B1 B0
 24ibt RGB888 R4 R3 R2 R1 R0 0 0 0 G5 G4 G3 G2 G1 G0 0 0 B4 B3 B2 B1 B0 0 0 0
 24ibt RGB888 R4 R3 R2 R1 R0 R2 R1 R0 G5 G4 G3 G2 G1 G0 G1 G0 B4 B3 B2 B1 B0 B2 B1 B0


第二行： 24bit RGB888 数据为转换后，未进行补偿的数据，在精度上会有损失
第三行： 24bit RGB888 数据为经过量化补偿的数据，对低位做了量化补偿，是一种合理的线性补偿*/

u16 chuliline=0;
q31_t coloRRR[320];
q31_t coloGGG[320];
q31_t coloBBB[320];
extern u16 canuse;
extern u16 start;
extern u16 replaceline;
extern __IO uint16_t  g_ColorData16t[40][320];
u16 qumo=0;


void RGB565TORGB888(void)
{
//	static uint8_t i=0;
	 uint16_t jj=0;	
//	qumo=(canuse%40);
//	if( qumo==1)
//	{chuliline=39;}
//	else if(qumo==0)
//	{chuliline=38;}
//	else
//	{chuliline=qumo-2;}
//	
//	
////	if(replaceline==0)
//		for(jj=0;jj<320;jj++)
//		{
//			coloRRR[jj]=((g_ColorData16t[chuliline][jj]>>11)<<3)|((g_ColorData16t[chuliline][jj]>>11)&0x07);
//			coloGGG[jj]=(((g_ColorData16t[chuliline][jj]>>5)&0x3f)<<2)|(((g_ColorData16t[chuliline][jj]>>5)&0x3f)&0x03);
//			coloBBB[jj]=((g_ColorData16t[chuliline][jj]&0x1f)<<3)|(g_ColorData16t[chuliline][jj]&0x07);			
//		}
	qumo=(canuse%40);
    if(qumo==1)
	{chuliline=39;}		
	else if(qumo==0)
	{chuliline=38;}
	else
	{chuliline=qumo-2;}
	
		for(int jj=0;jj<320;jj++)
		{
			//最后空闲位重复
			coloRRR[jj]=((g_ColorData16t[chuliline][jj]>>11)<<3)|((g_ColorData16t[chuliline][jj]>>11)&0x07);
			coloGGG[jj]=(((g_ColorData16t[chuliline][jj]>>5)&0x3f)<<2)|(((g_ColorData16t[chuliline][jj]>>5)&0x3f)&0x03);
			coloBBB[jj]=((g_ColorData16t[chuliline][jj]&0x1f)<<3)|(g_ColorData16t[chuliline][jj]&0x07);			
		}
}
