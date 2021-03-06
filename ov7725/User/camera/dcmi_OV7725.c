/**
  ******************************************************************************
  * @file    DCMI/OV7725_Camera/dcmi_OV7725.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   This file includes the driver for OV7725 Camera module mounted on 
  *          STM322xG-EVAL board RevA and RevB.
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
#include "misc.h"
#include "LCD.h"
#include "camera.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dcmi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "camera/picture.h"
#include "camera/dcmi_OV7725.h"
#include "camera/DCMI_OV7725_INITTABLE.h"


extern camera_t OV;
/** @addtogroup DCMI_OV7725_Camera
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bits definitions ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Delay(uint32_t nTime); 
static void Delay_ms(uint32_t nTime);

/**
  * @brief  Configures the DCMI to interface with the OV7725 camera module.
  * @param  None
  * @retval None
  */
void DCMI_Config(void)
{
  DCMI_InitTypeDef DCMI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef  DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
   /* Enable DCMI GPIOs clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE); 

  /* Enable DCMI clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

  /* Connect DCMI pins to AF13 ************************************************/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);//HSYNC 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);//PCLK
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);//VSYNC
	
//RED	
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_DCMI);
	
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_DCMI);

//Blue
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_DCMI);     //�ܰ�
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);	

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);      //�ܰ�

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI);


  
  /* DCMI GPIO configuration **************************************************/

	
/*
  D0 -- PC6
  D1 -- PA10
  D2 -- PC8
  D3 -- PC9
  D4 -- PE4
  D5 -- PB6
  D6 -- PE5
  D7 -- PE6
	
  PCK - PA6
  HS -- PA4
  VS -- PB7
*/	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_10 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_4;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* PCLK(PA6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  /* DCMI configuration *******************************************************/ 
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  
  DCMI_Init(&DCMI_InitStructure);

  /* DCMI Interrupts config ***************************************************/
  DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE);
  DCMI_ITConfig(DCMI_IT_LINE, ENABLE);
  DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);
  DCMI_ITConfig(DCMI_IT_ERR, ENABLE);
      
  
	
  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
		
  /* Configures the DMA2 to transfer Data from DCMI to the LCD ****************/
  /* Enable DMA2 clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  
  /* DMA2 Stream1 Configuration */  
  DMA_DeInit(DMA2_Stream1);

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)OV.gImageRGB;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 20*320;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
     
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	
  DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);	
}

/**
  * @brief  Set PA8 Output SYSCLK/2.
  * @param  None
  * @retval None
  */
void MCO1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_ClockSecuritySystemCmd(ENABLE);

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);
		
	/* Configure MCO (PA8) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_3);
}

void DCMI_0V7670_PWDN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*PWDN*/
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	Delay_ms(10);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);	
}

/**
  * @brief  Set the VGA size(640*320).
  * @param  None
  * @retval None
  */
uint8_t DCMI_OV7725_Init(void)
{
	uint8_t i;
	SCCB_GPIO_Config();
	DCMI_Config();
	MCO1_Init();
	//DCMI_0V7725_RST_PWDN_Init();
			
  Delay_ms(0xfff);
	//Resets all registers to default valus
	if(DCMI_SingleRandomWrite(OV7725_COM7, SCCB_REG_RESET)!=0)
	{
		return 0xff;
	}
	Delay_ms(0xfff);
  
	for(i=0;i<(sizeof(change_reg)/2);i++)
	{
		if(DCMI_SingleRandomWrite(change_reg[i][0],change_reg[i][1])!=0)
		{
			return 0xff;
		}
	}
	//OV7725_config_window(0,0,320,240);// set 240*320
	Delay_ms(0xfff);
	return 0;//Init ok
}

/**
  * @brief  Read the OV7725 Manufacturer identifier.
  * @param  OV7725ID: pointer to the OV7725 Manufacturer identifier. 
  * @retval None
  */

//#define OV7725_MIDH       0x1C 
//#define OV7725_MIDL       0x1D 
// #define OV7725_PID        0x0A  
// #define OV7725_VER        0x0B  
uint8_t DCMI_OV7725_ReadID(OV7725_IDTypeDef* OV7725ID)
{
	uint8_t temp;
	if(DCMI_SingleRandomRead(OV7725_MIDH,&temp)!=0)
		return 0xff;
	OV7725ID->Manufacturer_ID1 = temp;
	if(DCMI_SingleRandomRead(OV7725_MIDL,&temp)!=0)
		return 0xff;
	OV7725ID->Manufacturer_ID2 = temp;
	if(DCMI_SingleRandomRead(OV7725_VER,&temp)!=0)
		return 0xff;
	OV7725ID->Version = temp;
	if(DCMI_SingleRandomRead(OV7725_PID,&temp)!=0)
		return 0xff;
	OV7725ID->PID = temp;

	return 0;
}
/**
  * @brief  config_OV7660_window 
  * @param  //(140,16,640,480) is good for VGA
  *			//(272,16,320,240) is good for QVGA
  * @retval None
  */
void OV7725_config_window(uint16_t startx, uint16_t starty, uint16_t width, uint16_t height)
{
	uint16_t endx=(startx+width);
	uint16_t endy=(starty+height*2);// must be "height*2"
	uint8_t temp_reg1, temp_reg2;
	uint8_t state,temp;
	
	state = state;	   //Prevent report warning

	state = DCMI_SingleRandomRead(0x03, &temp_reg1 );
	temp_reg1 &= 0xC0;
	state = DCMI_SingleRandomRead(0x32, &temp_reg2 );
	temp_reg2 &= 0xC0;
	
	// Horizontal
	temp = temp_reg2|((endx&0x7)<<3)|(startx&0x7);
	state = DCMI_SingleRandomWrite(0x32, temp );
	temp = (startx&0x7F8)>>3;
	state = DCMI_SingleRandomWrite(0x17, temp );
	temp = (endx&0x7F8)>>3;
	state = DCMI_SingleRandomWrite(0x18, temp );
	
	// Vertical
	temp = temp_reg1|((endy&0x7)<<3)|(starty&0x7);
	state = DCMI_SingleRandomWrite(0x03, temp );
	temp = (starty&0x7F8)>>3;
	state = DCMI_SingleRandomWrite(0x19, temp );
	temp = (endy&0x7F8)>>3;
	state = DCMI_SingleRandomWrite(0x1A, temp );
}
 			 			  
/**
  * @}
  */
static void Delay(uint32_t nTime)
{
  while(nTime--);
} 

static void Delay_ms(uint32_t nTime)
{
  while(nTime--)
  	{Delay(1000);}

}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
