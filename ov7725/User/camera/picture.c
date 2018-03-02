#include "picture.h"
#include "config.h"
#include "camera/SCCB.h"
#include "arm_math.h"
#include "LCD/LCD.h"
#include "usart.h"
#include "can.h"
#include "stm32f4xx_dcmi.h"



__IO uint16_t  g_ColorData16t[40][320];
__IO uint16_t  g_ColorData16t_debug[5][320]={0};//DCMI接口采集原始数据内存
__IO uint16_t  g_ColorData16t_ROI[5][10]={0};//DCMI接口采集原始数据内存
__IO uint16_t  g_ColorData16t_deal[5][320]={0};//DCMI接口采集原始数据内存


q31_t  g_ColorDataRRR8t[SIMPLE_LINE][320]={0};//8位RGB值R分量
q31_t  g_ColorDataGGG8t[SIMPLE_LINE][320]={0};//8位RGB值G分量
q31_t  g_ColorDataBBB8t[SIMPLE_LINE][320]={0};//8位RGB值B分量

q31_t  g_ColorDataRRR8t_ROI[5][10]={0};//8位RGB值R分量
q31_t  g_ColorDataGGG8t_ROI[5][10]={0};//8位RGB值G分量
q31_t  g_ColorDataBBB8t_ROI[5][10]={0};//8位RGB值B分量  

q31_t dspq31t_buffer0[320]={0};
q31_t dspq31t_buffer1[320]={0};
q31_t dspq31t_buffer2[320]={0};

q31_t dspq31t_buffer3[320]={0};
q31_t dspq31t_buffer4[320]={0};
q31_t dspq31t_buffer5[320]={0};
//q31_t pixel[320]={0};

//q31_t find0[5][320]={0};
//q31_t find1[5][320]={0};
//q31_t find2[5][320]={0};
//q31_t find3[5][320]={0};
//q31_t find4[5][320]={0};
//q31_t find5[5][320]={0};




uint16_t red_begin,white_bein,red_end,white_end,black2_number=0,black1_number=0,white_number=0,Begin,End;
uint16_t BEGBEG=0,ENDEND=0;
q31_t ROI_R,ROI_G,ROI_B;
uint32_t ROI_RGB_mod=0;
static CanTxMsg TxMessage=
{
  0x40,/*uint32_t StdId;  !< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  0x40,/*uint32_t ExtId;  !< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  CAN_Id_Standard,/*uint8_t IDE;     !< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref CAN_identifier_type */

  CAN_RTR_Data,/*uint8_t RTR;     !< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  8,/*uint8_t DLC;     !< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8 */

  {0}/*uint8_t Data[8]; !< Contains the data to be transmitted. It ranges from 0 
                        to 0xFF. */
}
;//CAN发送消息
union Cameradata
{
	int16_t  data_16b;
	uint8_t  data_8b[2];
}CarYpos;

extern __IO uint8_t g_DCMI_IT_FRAME_FLAG;

#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))

void arm_mult_q31(
		     q31_t * pSrcA,
		     q31_t * pSrcB,
		    q31_t * pDst,
		    uint32_t blockSize)
{
		while(blockSize-->0UL)
		{	
			*pDst++ =(*pSrcA++) * (*pSrcB++);
		}
}
void arm_min_q31_action(
		     q31_t * pSrcA,
		     q31_t * pSrcB,
		    q31_t * pDst,
		    uint32_t blockSize)
{
		while(blockSize-->0UL)
		{	
      if((*pSrcA)<(*pSrcB))
			{
				*pDst++ =(*pSrcA);
      }
			else
			{
				*pDst++ =(*pSrcB);
			}
			pSrcA++;pSrcB++;
			
		}
}

void arm_max_q31_action(
		     q31_t * pSrcA,
		     q31_t * pSrcB,
		    q31_t * pDst,
		    uint32_t blockSize)
{
		while(blockSize-->0UL)
		{	
      if((*pSrcA)>(*pSrcB))
			{
				*pDst++ =(*pSrcA);
      }
			else
			{
				*pDst++ =(*pSrcB);
			}
			pSrcA++;pSrcB++;
			
		}
}

void arm_mult_q31_signle(
		     q31_t * pSrcA,
		     q31_t * pSrcB,
		    q31_t * pDst,
		    uint32_t blockSize)
{
		while(blockSize-->0UL)
		{	
			*pDst++ =(*pSrcA++) * (*pSrcB);
		}
}

void arm_div_q31_signle(
		     q31_t * pSrcA,
		     q31_t * pSrcB,
		    q31_t * pDst,
		    uint32_t blockSize)
{
		while(blockSize-->0UL)
		{	
// 			*pDst++ =(*pSrcA++) / (*pSrcB++);//UDIV
			*pDst++ =(*pSrcA++) / (*pSrcB++);//
// 			*pDst++=_QUDIV(*pSrcA++,*pSrcB++);
			
		}
}

/**    
 * @ingroup groupMath    
 */

/**    
 * @addtogroup BasicSub    
 * @{    
 */

/**    
 * @brief Q31 vector subtraction.    
 * @param[in]       *pSrcA points to the first input vector    
 * @param[in]       *pSrcB points to the second input vector    
 * @param[out]      *pDst points to the output vector    
 * @param[in]       blockSize number of samples in each vector    
 * @return none.    
 *    
 * <b>Scaling and Overflow Behavior:</b>    
 * \par    
 * The function uses saturating arithmetic.    
 * Results outside of the allowable Q31 range [0x80000000 0x7FFFFFFF] will be saturated.    
 */

void arm_sub_q31_signle(
  q31_t * pSrcA,
  q31_t * pSrcB,
  q31_t * pDst,
  uint32_t blockSize)
{
  uint32_t blkCnt;                               /* loop counter */


#ifndef ARM_MATH_CM0_FAMILY

/* Run the below code for Cortex-M4 and Cortex-M3 */
  q31_t inA1, inA2, inA3, inA4;
  q31_t inB1, inB2, inB3, inB4;

  /*loop Unrolling */
  blkCnt = blockSize >> 2u;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.    
   ** a second loop below computes the remaining 1 to 3 samples. */
  while(blkCnt > 0u)
  {
    /* C = A - B */
    /* Subtract and then store the results in the destination buffer. */
    inA1 = *pSrcA++;
    inA2 = *pSrcA++;
    inB1 = *pSrcB;
    inB2 = *pSrcB;

    inA3 = *pSrcA++;
    inA4 = *pSrcA++;
    inB3 = *pSrcB;
    inB4 = *pSrcB;

    *pDst++ = __QSUB(inA1, inB1);
    *pDst++ = __QSUB(inA2, inB2);
    *pDst++ = __QSUB(inA3, inB3);
    *pDst++ = __QSUB(inA4, inB4);

    /* Decrement the loop counter */
    blkCnt--;
  }

  /* If the blockSize is not a multiple of 4, compute any remaining output samples here.    
   ** No loop unrolling is used. */
  blkCnt = blockSize % 0x4u;

  while(blkCnt > 0u)
  {
    /* C = A - B */
    /* Subtract and then store the result in the destination buffer. */
    *pDst++ = __QSUB(*pSrcA++, *pSrcB);

    /* Decrement the loop counter */
    blkCnt--;
  }

#else

  /* Run the below code for Cortex-M0 */

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

  while(blkCnt > 0u)
  {
    /* C = A - B */
    /* Subtract and then store the result in the destination buffer. */
    *pDst++ = (q31_t) clip_q63_to_q31((q63_t) * pSrcA++ - *pSrcB);

    /* Decrement the loop counter */
    blkCnt--;
  }

#endif /* #ifndef ARM_MATH_CM0_FAMILY */

}



void SendDataToPC(void)
{
	         uint16_t i,j=0;
	         static uint8_t ii=0;
	       if(ii++==200)
				 {
						 DCMI_CaptureCmd(DISABLE);
	// 					//发送r
						printf("rrrrrr");
						printf("\n");					
						for(i=0;i<5;i++)
						{
							for(j=0;j<320;j++)
							{
								printf("%d\t",g_ColorDataRRR8t[i][j]);
							}
							printf("\n");

						}
						

						//发送g
						printf("gggggg");
						printf("\n");					
						for(i=0;i<5;i++)
						{
							for(j=0;j<320;j++)
							{
								printf("%d\t",g_ColorDataGGG8t[i][j]);
							}
							printf("\n");

						}

						//发送b
						printf("bbbbbb");
						printf("\n");					
						for(i=0;i<5;i++)
						{
							for(j=0;j<320;j++)
							{
								printf("%d\t",g_ColorDataBBB8t[i][j]);
							}
							printf("\n");
						}
						while(1);			
				}					
}
#define white 1
#define black 0
#define magenta 2
uint16_t j=0,k=0;//循环用

uint8_t state=0;///,jump=0,nextone=0
//extern uint16_t canuse;
//extern uint16_t start;
uint16_t popopo=0;
uint8_t view=0;
q31_t findwh[320]={0};
uint16_t beforeline=0;
//static uint16_t i=0;
//static q31_t mult=300;
//static uint16_t jj=0;	
uint16_t popopoold=0;
uint16_t canuseold=0;
uint16_t hangnow=0,hangold=0;

uint8_t statezhuang=0;///,jump=0,nextone=0
extern uint16_t canuse;
extern uint16_t start;
extern uint16_t replaceline;
uint16_t recordline=0;
void SeeSmallCar(void)
{
	 uint16_t i=0;
//	 q31_t mult=300;
	 uint16_t jj=0;	
//     static q31_t popopoold=0;
//	 static q31_t canuseold=0;
	if( ((replaceline>0) && ((replaceline-recordline)==5)) || (replaceline==1) )
	{
		//RGB565TORGB888();
//	qumo=(canuse%40);
//    if(qumo==1)
//	{chuliline=39;}		
//	else if(qumo==0)
//	{chuliline=38;}
//	else
//	{chuliline=qumo-2;}
//	
//		for(jj=0;jj<320;jj++)
//		{
//			coloRRR[jj]=((g_ColorData16t[chuliline][jj]>>11)<<3)|((g_ColorData16t[chuliline][jj]>>11)&0x07);
//			coloGGG[jj]=(((g_ColorData16t[chuliline][jj]>>5)&0x3f)<<2)|(((g_ColorData16t[chuliline][jj]>>5)&0x3f)&0x03);
//			coloBBB[jj]=((g_ColorData16t[chuliline][jj]&0x1f)<<3)|(g_ColorData16t[chuliline][jj]&0x07);			
//		}
		//arm_add_q31(&coloRRR[0],&coloGGG[0],dspq31t_buffer1,320);
		//arm_add_q31(dspq31t_buffer1,&coloBBB[0],dspq31t_buffer1,320);//和计算给buffer1
		
		//求S
//		arm_min_q31_action(&coloRRR[0],&coloGGG[0],dspq31t_buffer2,320);
//		arm_min_q31_action(&coloBBB[0],dspq31t_buffer2,dspq31t_buffer2,320);
//		arm_mult_q31_signle(dspq31t_buffer2,&mult,dspq31t_buffer2,320);	
//		arm_div_q31_signle(dspq31t_buffer2,dspq31t_buffer1,dspq31t_buffer2,320);	//计算后全给buffer2了

		for(jj=0;jj<320;jj++)
		{			
			if( (dspq31t_buffer1[jj]>700)   )  //&& (dspq31t_buffer2[j]>60)
			{
				findwh[jj]=White;
			}   
			else
			{
				findwh[jj]=Black;
			}
		}		
	
	 for(i=1;i<318;i++)            
	 {
		   if(findwh[i]==Black)
		   {             
			 if((findwh[i-1]==White)&&(findwh[i+1]==White))
			 { findwh[i]=White;}
			 
		   }
		   if(findwh[i]==White)
		   {                
		     if((findwh[i-1]==Black)&&(findwh[i+1]==Black))
			 {findwh[i]=Black;}
		   }
	  } 
	for(i=0;i<320;i++)
	{
		if(findwh[i]==White)
		{		
			popopo=i;view=1;
			statezhuang=9;
//				LCD_SetCursor(popopoold,canuseold-2);
//				LCD_REG = 0x0022;
//				LCD_RAM=Red;LCD_RAM=Red;LCD_RAM=Red;LCD_RAM=Red;
//				
//				LCD_SetCursor(popopo,canuse-2);
//				LCD_REG = 0x0022;
//				LCD_RAM=Blue;LCD_RAM=Blue;LCD_RAM=Blue;LCD_RAM=Blue;
			
//				LCD_SetCursor(0,canuse-2);
//				LCD_REG = 0x0022;
//			for(jj=0;jj<320;jj++)
//				LCD_RAM=g_ColorData16t[chuliline][jj];
				

			start=0;			
//			GUI_Text(200,10,"ok",White,Blue);
//			GUI_Num(100,50,popopo,White,Red);		
//			GUI_Num(50,50,canuse,White,Blue);
			popopoold=popopo;
			canuseold=canuse;
			break;
		}
	}
	
	if( (canuse==240)&&(i>319) )
	{
		view=0;statezhuang=0;
//		GUI_Text(200,10,"no",White,Blue);
	}
	recordline=replaceline;
}
	
//RGB原始采集的颜色输出
//		LCD_SetCursor(0,144);
//		LCD_REG = 0x0022;	
//		for(k=0;k<5;k++)
//		{
//			for(j=0;j<320;j++)
//			LCD_RAM =g_ColorData16t_deal[k][j];			
//		}
				
//将第一次处理的输出在屏上
//		LCD_SetCursor(0,164);
//		LCD_REG = 0x0022;	
//		for(k=0;k<5;k++)
//		{
//			for(j=0;j<320;j++)
//			LCD_RAM =g_ColorData16t_debug[k][j];
//		}
if(statezhuang==9)
	{	
		CarYpos.data_16b=popopo;	
        TxMessage.Data[0]='y';
		TxMessage.Data[1]=CarYpos.data_8b[0];
		TxMessage.Data[2]=CarYpos.data_8b[1];
//		LCD_SetCursor(CarYpos.data_16b,180);
//		LCD_REG = 0x0022;	
//		LCD_RAM =Blue;LCD_RAM =Blue;LCD_RAM =Blue;LCD_RAM =Blue;LCD_RAM =Blue;LCD_RAM =Blue;LCD_RAM =Blue;LCD_RAM =Blue;
//		GUI_Text(100,50,"ok",White,Blue);
//		GUI_Num(150,50,CarYpos.data_16b,White,Blue);		
//		GUI_Num(150,70,End-Begin,White,Blue);
		statezhuang=0;
	}
	else
	{	
//		printf("err\n");
//		GUI_Text(100,50,"no",White,Blue);
//		GUI_Num(150,50,CarYpos.data_16b,White,Blue);
		TxMessage.Data[0]='n';	
	}
	
}

void SendData(void)
{
	uint8_t mbox;
	mbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok));//等待238us
}

	

