#include "Imtrans.h"
#include "picture.h"
#include "config.h"
#include "camera/SCCB.h"
#include "arm_math.h"
#include "LCD/LCD.h"
#include "usart.h"
#include "rodata.h"
#include "arm_math.h"

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
/****************************************************************************
* 名    称：ExtractSignal()	
* 功    能：将RGB分为三个通道
* 入口参数：无
* 出口参数：无
* 说    明：无
* 调用方法：无 
****************************************************************************/
 uint8_t graypixel[PIXEL_H][PIXEL_W];
 extern __IO uint16_t  g_ColorData16t[40][320];
void ExtractSignal(void)
{
	uint16_t i=0, j=0;
	uint16_t R,G,B,mux,mux_later;
	float data = 0.0;
	for(i = 0; i<PIXEL_H; i++)
	{
		for(j = 0; j<PIXEL_W; j++)
		{
			R = (uint16_t)GETR_FROM_RGB16(g_ColorData16t[i][j]);
			G = (uint16_t)GETG_FROM_RGB16(g_ColorData16t[i][j]);
			B = (uint16_t)GETB_FROM_RGB16(g_ColorData16t[i][j]);
			mux = (R*30+G*59+B*11)/100;
    
			graypixel[i][j] = mux;
		}
	}
}

/****************************************************************************
* 名    称：Itera_Threshold()	
* 功    能：迭代阈值法，能找到灰度图的最佳二值化点
* 入口参数：无
* 出口参数：无
* 说    明：无
* 调用方法：无 
* 运行时间： 5ms
****************************************************************************/
uint8_t gray_itera_threshold[256];
uint16_t threshold_h[256];
void Itera_Threshold(void)
{
  uint16_t i=0,j=0,k=0,cnt=0,mux=0;
	uint8_t newthreshold=0;
	uint16_t Pmax=0,Pmin=0;
	uint32_t sum_h1=0,sum_h2=0;
	ExtractSignal();
	for( i=0; i<PIXEL_H; i++ )
	{
	  for( j=0; j<PIXEL_W; j++ )
		{
		  mux = graypixel[i][j];                     
		      
			gray_itera_threshold[mux]++;	
		}
	}
	
	Pmin = gray_itera_threshold[0];
	Pmax = gray_itera_threshold[0];
	for( cnt=0; cnt<256; cnt++ )
	{
	  if( Pmin>gray_itera_threshold[cnt] ){
		  Pmin = gray_itera_threshold[cnt];
		}
		if( Pmax<gray_itera_threshold[cnt] ){
		  Pmax = gray_itera_threshold[cnt];
		}
	}
	
	threshold_h[0] = ( Pmax + Pmin ) / 2;
	
	//Ñ°ÕÒ×î¼ÑãÐÖµ
	for( k=0; k<256; k++ )
	{
		//·Ö¸îÇ°¾°ºÍ±³¾°
		for( cnt=0; cnt<threshold_h[k]; cnt++ )
		{
			sum_h1 += gray_itera_threshold[cnt];
		}
		for( cnt=threshold_h[k]; cnt<256; cnt++ )
		{
			sum_h2 += gray_itera_threshold[cnt];
		}
		sum_h1 /= threshold_h[k];
		sum_h2 /= (256-threshold_h[k]);
		
		//¼ÆËã³öÐÂµÄãÐÖµ
		threshold_h[k+1] = ( sum_h1 + sum_h2 ) / 2; 
		
		if(__fabs(threshold_h[k]-threshold_h[k+1]) <= GRAY_BREAK_RANGE ){
			newthreshold = threshold_h[k+1];
		  break;
		}
		
		sum_h1 = 0;
		sum_h2 = 0;
	}
	for( i=0; i<PIXEL_H; i++ )
	{
	  for( j=0; j<PIXEL_W; j++ )
		{
		  mux = graypixel[i][j];                     //»ñÈ¡»Ò¶ÈÍ¼µÄÊý¾Ý
		  //Í¼Ïñ¶þÖµ»¯
			if( mux > newthreshold ){
			  graypixel[i][j] = 255;
			}
			else{
			  graypixel[i][j] = 0;
			}
		}
	}
}