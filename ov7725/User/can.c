/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "stm32f4xx_can.h"
#include "misc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


void CamreaCanInit(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef  	   NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
		/* Connect CAN pins to AF */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); 

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* CAN register init */
	CAN_DeInit(CAN1);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;         //time triggered communication mode
	CAN_InitStructure.CAN_ABOM = DISABLE;         //automatic bus-off management
	CAN_InitStructure.CAN_AWUM = DISABLE;         //automatic wake-up mode
	CAN_InitStructure.CAN_NART = DISABLE;         //non-automatic retransmission mode
	CAN_InitStructure.CAN_RFLM = DISABLE;         //Receive FIFO Locked mode
	CAN_InitStructure.CAN_TXFP = DISABLE;         //transmit FIFO priority
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //CAN operating mode
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;   // keep CAN_SJW == 1, never change it
  CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq; //max=16
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq; //max=8
	CAN_InitStructure.CAN_Prescaler = 4;	
	CAN_Init(CAN1, &CAN_InitStructure);
	
		/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=0;	    //Filter 0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; //32 Bit
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x04A0;               //32 Bis ID 只收Ox25地址数
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
// 	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;           //32 Bit Mask
// 	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;           //activate Filter
	CAN_FilterInit(&CAN_FilterInitStructure);                        //intialize Filter

	/* Enable FIFO 0 message pending Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
}
/**
  * @brief  transmit an array 
  * @param  CANx:  CANx, where x can be 1,2.
  * @param  StdId: the StdId you want to select as StdId.
  * @param  Msg:   a pointer that point the array you want to transmit.
  * @param  len:   the length of the the array that you want to transmit.
  * @retval 1, if transmit successful. 
  * @author Calcus Lee
**/
uint8_t CAN_TxMsg(CAN_TypeDef* CANx,
				  uint32_t StdId,
				  uint8_t * Msg,
				  uint8_t len)
{
	uint8_t mbox;
	uint8_t i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId=StdId;
	TxMessage.ExtId=StdId;
	TxMessage.IDE=CAN_Id_Standard;
	TxMessage.RTR=CAN_RTR_Data;
	TxMessage.DLC=len;
	for(i=0;i<len;i++)
		TxMessage.Data[i]=Msg[i];
	mbox=CAN_Transmit(CANx, &TxMessage);
	while(CAN_TransmitStatus(CANx,mbox)!=CAN_TxStatus_Ok);
	return 1;
}


/**
  * @brief  Receive an array 
  * @param  CANx:  CANx, where x can be 1,2.
  * @param  StdId: the StdId you want to select as StdId.
  * @param  Msg:   a array you want to transmit.
  * @param  len:   the length of the the array that you want to transmit.
  * @retval 1, if receive successful
  * @author Calcus Lee
**/
uint8_t CAN_RxMsg(CAN_TypeDef* CANx,
				  uint32_t * StdId,
				  uint8_t * buf,
				  uint8_t len)
{
	uint8_t i=0;
	CanRxMsg RxMessage;
	if(CAN_MessagePending(CANx,CAN_FIFO0)!=0)
	CAN_Receive(CANx,CAN_FIFO0,&RxMessage);
	 for(i=0;i<len;i++)
    buf[i]=RxMessage.Data[i];
    *StdId= RxMessage.StdId; 
	  return 1;
}




