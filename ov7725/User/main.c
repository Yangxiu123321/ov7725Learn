/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "usart.h"
#include "LCD/LCD.h"
#include "camera/dcmi_OV7670.h"
#include "camera/picture.h"
#include "arm_math.h"
#include "can.h"
#include "Imtrans.h"
#include "rodata.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup IOToggle
  * @{
  */ 
void LCD_CS_ResetBits(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
static __IO uint32_t TimingDelay;
__IO int16_t g_DCMI_IT_FRAME_FLAG=0;//�Ƿ񲶻��µ�һ֡ͼ���־

extern  q31_t  ROI_R,ROI_G,ROI_B;
extern  uint32_t ROI_RGB_mod;
extern __IO uint16_t  g_ColorData16t[40][320];//DCMI�ӿڲɼ�ԭʼ�����ڴ�
extern __IO uint16_t  g_ColorData16t_ROI[5][10];//DCMI�ӿڲɼ�ԭʼ�����ڴ�
extern __IO uint16_t  g_ColorData16t_deal[5][320];//DCMI�ӿڲɼ�ԭʼ�����ڴ�
extern uint16_t camer_lines_4;


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
uint16_t fps_time=0;
extern __IO u16 canuse;
extern u16 start;
extern u16 replaceline;
u16 lines=0;
u16 comelines=0;	
u16 jj1=0;

int main(void)
{	
	RCC_ClocksTypeDef SYS_Clocks;
	int16_t i=0,ii=0;
	uint16_t fps=0;
	uint16_t jj=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	static u16 yushu=0,cmline=0;

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f2xx.c file
  */
	if (SysTick_Config(SystemCoreClock / 1000))
  	{
	    /* Capture error */ 
	    while (1);
  	}
	RCC_GetClocksFreq(&SYS_Clocks);

		
	//I/O������ ���ڲ鿴��������ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		
	//�ڴ���չ���ԣ�����

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);


	/*���ô��п� 115200*/
	USART_Configuration();
	/*����LCD��ʾ��*/
	LCD_Initializtion();
	/*����ʾ����ɫ����Ϊ��ɫ*/
	LCD_Clear(Red);
		
		
  /*������Ϣ���*/
//	GUI_Text(100,200,"Camera Init..",White,Blue);

	/*��������ͷ�Ĵ���*/
	if(DCMI_OV7725_Init()!=0)
	{
		USART_OUT(USART3,"Camera Init fails!!");		
	}
//		GUI_Text(100,200,"Camera Init fails!!",White,Blue);
	else
	{
		USART_OUT(USART3,"Camera Init Ok!!");
	}
//		GUI_Text(100,200,"Camera Init Ok!!",White,Blue);
	/*Set LCD direction*/		
	LCD_WriteReg(0x0011,0x6068);//�ӽǹ�53��  һ��26��
	LCD_SetCursor(0,0);
	Prepare_Write_RAM();
																										  
	/* Start Image capture and Display on the LCD *****************************/
  /* Enable DMA transfer */
  DMA_Cmd(DMA2_Stream1, ENABLE);

  /* Enable DCMI interface */
  DCMI_Cmd(ENABLE); 

  /* Start Image capture */ 
  DCMI_CaptureCmd(ENABLE);
	Delay(500);
//	GUI_Text(100,220,"Camera AEC_Fun...",White,Blue);//240*320
	USART_OUT(USART3,"Camera AEC_Fun...");		
			
			
// 	/*�Զ��ع�����*/
// 	AEC_WHITE_Fun();
	
  //PA11 PA12CAN�ӿ�
  // CamreaCanInit();
	 Delay(100);
	 
//   SendAllReadyToCar();//���ͳ�ʼ�������Ϣ
	
  g_DCMI_IT_FRAME_FLAG=2;
	
	fps_time=0;//����֡�ʱ���
  extern u8 graypixel[PIXEL_H][PIXEL_W];
	while(1)//��DSP��������3.7ms!!!!!!!!!
	{
		static uint8_t CANSendData1[8]={2};
		static int timecount=0;
		while(g_DCMI_IT_FRAME_FLAG!=0){};//�ȴ�ʶ��������
    g_DCMI_IT_FRAME_FLAG=2;
		if(start==1)
		{
			Itera_Threshold();
			yushu=(canuse%40);
			if(yushu==1)
			{cmline=39;}		
			else if(yushu==0)
			{cmline=38;}
			else
			{cmline=yushu-2;}
			
			if(canuse==1)
			{		
				LCD_SetCursor(0,239);
				LCD_REG = 0x0022;
				for(jj=0;jj<320;jj++)
				{
					if(graypixel[39][jj]==255)
					{
						LCD_RAM=0xffff;
					}else{
						LCD_RAM=0x0000;
					}
				}
				//	LCD_RAM=g_ColorData16t[39][jj];
			}
			else
			{
					LCD_SetCursor(0,canuse-2);
					LCD_REG = 0x0022;
				  for(jj=0;jj<320;jj++)
					{
						if(graypixel[cmline][jj]==255)
						{
							LCD_RAM=0xffff;
						}else{
							LCD_RAM=0x0000;
						}
						}
					//LCD_RAM=g_ColorData16t[cmline][jj];
			}
/********************************************/			
		timecount++;
		//���Ժ���
		if(timecount>10000)
		{
			//USART_OUT(USART3,"%d\t",);
			//CAN_TxMsg(CAN1,1,CANSendData1,1);
			timecount=0;
		}	
//	SeeSmallCar();	
	
   }


// 				  
				
// // 				/*������*/
//           SendDataToPC();
	
			     //����֡��
//				   fps++;
//				   if(fps_time>1000)
//					 {
//						 GUI_Num(21*8, 104, fps,Black,Red);
//						 fps=fps_time=0;
//					 }	

		
//   					SendData();
	}
}

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0)
  {}
}

void TimingDelay_Decrement(void)
{
	if(fps_time++>5000)
	{
		fps_time=0;
	}
	
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void LCD_CS_ResetBits(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//LCD_CS PD7

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	


	GPIO_ResetBits(GPIOD , GPIO_Pin_7);		 //CS=0;
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {

  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
