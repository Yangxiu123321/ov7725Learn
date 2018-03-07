#ifndef __CAMERA_H

#include "stm32f4xx.h"

#define __CAMREA_H
typedef struct
{
	uint16_t timeCount;
	uint16_t timeCountOut;
	
	uint16_t dcmiItNum;
  uint16_t fps;
	uint8_t fpsOutFlag;
	uint16_t frameNum;
}frame_t;

typedef struct
{
	frame_t frame;
}camera_t;




void frameSpeedOut(void);
#endif

