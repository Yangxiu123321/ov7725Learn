#ifndef __CAN_H
#define __CAN_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

void CamreaCanInit(void);
uint8_t CAN_RxMsg(CAN_TypeDef* CANx,
				  uint32_t * StdId,
				  uint8_t * buf,
				  uint8_t len);

uint8_t CAN_TxMsg(CAN_TypeDef* CANx,
				  uint32_t StdId,
				  uint8_t * buf,
				  uint8_t len);

#endif
