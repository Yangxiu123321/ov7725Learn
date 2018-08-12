/**
  ******************************************************************************
  * @file    main.c 
  * @author  Yangxiu
  * @version V1.0.0
  * @date    18-April-2018
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
#include "can.h"
#include "misc.h"
#include "timer.h"
#include "usart.h"
#include "camera.h"
#include "rodata.h"
#include "LCD/LCD.h"
#include "arm_math.h"
#include "Imtrans.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dcmi.h"
#include "stm32f4xx_gpio.h"
#include "camera/dcmi_OV7725.h"
#include "camera/picture.h"




/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup IOToggle
  * @{
  */ 
void LCD_CS_ResetBits(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
static __IO uint32_t TimingDelay;
__IO int16_t g_DCMI_IT_FRAME_FLAG=0;//是否捕获新的一帧图像标志

extern  q31_t  ROI_R,ROI_G,ROI_B;
extern  uint32_t ROI_RGB_mod;
extern __IO uint16_t  g_ColorData16t[40][320];//DCMI接口采集原始数据内存
extern __IO uint16_t  g_ColorData16t_ROI[5][10];//DCMI接口采集原始数据内存
extern __IO uint16_t  g_ColorData16t_deal[5][320];//DCMI接口采集原始数据内存
extern uint16_t camer_lines_4;


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
camera_t OV;
	
uint16_t fps_time=0;
extern __IO uint16_t canuse;
extern uint16_t start;
extern uint16_t replaceline;
uint16_t lines=0;
uint16_t comelines=0;	
uint16_t jj1=0;

int main(void)
{	
	RCC_ClocksTypeDef SYS_Clocks;
	GPIO_InitTypeDef GPIO_InitStructure;

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

		
	//I/O口设置 用于查看代码运行时间
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		
	//内存扩展尝试！！！

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);


	/*设置串行口 115200*/
	USART_Configuration();
	/*设置LCD显示屏*/
	LCD_Initializtion();
	/*将显示屏底色设置为红色*/
	LCD_Clear(Red);
	/*设置1s定时器用于计算帧率*/
	TIM_Init(TIM2,10000-1,8400-1,1,1);
	/*设置1ms定时器用于计算程序周期*/
	TIM_Init(TIM3,1000-1,84-1,1,1);

	/*配置摄像头寄存器*/
	if(DCMI_OV7725_Init()!=0)
	{
		USART_OUT(USART3,"Camera Init fails!!");		
	}
//		GUI_Text(100,200,"Camera Init fails!!",White,Blue);
	else
	{
		USART_OUT(USART3,"Camera Init Ok!!\r\n");
	}
//		GUI_Text(100,200,"Camera Init Ok!!",White,Blue);
	/*Set LCD direction*/		
	LCD_WriteReg(0x0011,0x6068);//视角共53度  一面26度
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
	USART_OUT(USART3,"Camera AEC_Fun...\n");		
			

 	/*自动曝光设置*/
 	//AEC_WHITE_Fun();
	
  //PA11 PA12CAN接口
  // CamreaCanInit();
	 Delay(100);
	 
	// SendAllReadyToCar();//发送初始化完成信息
	
  g_DCMI_IT_FRAME_FLAG=2;
	
	fps_time=0;//计算帧率变量
  extern uint8_t graypixel[PIXEL_H][PIXEL_W];
	while(1)//加DSP后总运行3.7ms!!!!!!!!!
	{
		// 输出相关的相机速度变量
		frameSpeedOut();
		OV.frame.timeCount = 0;	
		if(OV.frame.tansFinishFlag == 1)
		{
			//USART_OUT(USART3,"Camera AEC_Fun...");
			//ExtractSignal();	
			//Itera_Threshold();
			//showSrcImgOnLCD();
			getBallColor();
		}
		OV.frame.timeCountOut = OV.frame.timeCount;
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
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
