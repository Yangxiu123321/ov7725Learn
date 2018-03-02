/**
  ******************************************************************************
  * @file    I2C/EEPROM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "usart.h"
#include "LCD/LCD.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dcmi.h"
#include "stm32f4xx_usart.h"
#include "camera/picture.h"


#define USARTx_IRQHANDLER   USART1_IRQHandler
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_EEPROM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}
extern __IO int16_t g_DCMI_IT_FRAME_FLAG;
extern uint32_t	fps;
extern uint32_t	disfps;
extern void TimingDelay_Decrement(void);
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
void USARTx_IRQHANDLER(void)
{
  if(USART_GetITStatus(Open207V_USART, USART_IT_RXNE) != RESET)
  {
  		//USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		printf("\n\rUSART Hyperterminal Interrupts Receive a word: %c\n\r",USART_ReceiveData(Open207V_USART));
  }
}

extern __IO uint16_t  g_ColorData16t[40][320];//DCMI接口采集原始数据内存
extern __IO uint16_t  g_ColorData16t_ROI[10][10];//DCMI接口采集原始数据内存
extern __IO uint16_t  g_ColorData16t_deal[10][320];//DCMI接口采集原始数据内存
uint16_t camer_lines=0,camer_lines_4=0;//上电时默认看远方  在拐弯时改变此值 在中断里
uint8_t cameralineschange='b';


__IO uint16_t canuse=0;
uint16_t start=0;
uint16_t replaceline=0;
uint16_t shuzu[320]={0}; 
void DCMI_IRQHandler(void)
{
	static uint16_t j=0,i=0;
	if (DCMI_GetITStatus(DCMI_IT_VSYNC) != RESET) 
	{
//		if(cameralineschange==0)
//		{
//			;
//		}
//		else if(cameralineschange=='b')//近景
//		{
//			cameralineschange=0;
//			camer_lines=126;
//			camer_lines_4=0;
//		}
//		else if(cameralineschange=='c')//远景
//		{
//			cameralineschange=0;
//			camer_lines=131-25;//远景
//			camer_lines_4=15 ;
//		}
// 		j=0;
		//new frame start
		canuse=0;
		start=1;
		DCMI_ClearITPendingBit(DCMI_IT_VSYNC); 
	}
	
	if(DCMI_GetITStatus(DCMI_IT_LINE) != RESET) 
	{
//		j++;
//		if(j==camer_lines)//第22行结束后开始取第10~20行图像数据  值越小，越靠前  远100(33cm)   近220(20cm)
//		{
//			g_DCMI_IT_FRAME_FLAG=0;		
//	  }
		if(start==1)
		{
			canuse++;replaceline=canuse-1;
		}
		shuzu[i++]=canuse;
	  if(i>319)
		{
			i=0;
		}
		g_DCMI_IT_FRAME_FLAG=0;	
		DCMI_ClearITPendingBit(DCMI_IT_LINE); 			  
	}
	
	if (DCMI_GetITStatus(DCMI_IT_FRAME) != RESET) 
	{
		DCMI_ClearITPendingBit(DCMI_IT_FRAME);
	}
	if (DCMI_GetITStatus(DCMI_IT_ERR) != RESET) 
	{
		DCMI_ClearITPendingBit(DCMI_IT_ERR);
	}
}
//串口接收终端
void USART3_IRQHandler(void)
{
	 uint8_t buffer=0;
	 if(USART_GetITStatus(USART3, USART_IT_RXNE)==SET)   
	 {
			USART_ClearITPendingBit( USART3,USART_IT_RXNE);
		  buffer=USART_ReceiveData(USART3);
	 }
}



//CAN接受中断 和主控通信
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;

	if(CAN_MessagePending(CAN1,CAN_FIFO0)==0)return;		//if there is no data, get out of this function
	

		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);				//reveive data	
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		
// 	  if(RxMessage.StdId==0x25)//主控地址
// 		{
// 			if(RxMessage.Data[0]=='b')//主控发来消息看近景
// 			{
// 				cameralineschange='b';
// 			}
// 			
// 			if(RxMessage.Data[0]=='c')		
// 			{
// 				cameralineschange='c';
// 			}


// 		}
	

	
}

void DMA2_Stream1_IRQHandler(void)
{

	  if(DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1))
    {
			

        DMA_ClearFlag(DMA2_Stream1, DMA_IT_TCIF1);
        DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);	
    }
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
