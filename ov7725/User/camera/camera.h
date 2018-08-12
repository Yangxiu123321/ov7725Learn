#ifndef __CAMERA_H

#include "stm32f4xx.h"
#include "EasyTracer.h"


#define __CAMREA_H

// 原始图像尺寸
#define SRCIMG_WIDTH 320
#define SRCIMG_HEIGHT 240
// 原始图像中心
#define SRCIMG_WIDTH_CENTER 160
#define SRCIMG_HEIGHT_CENTER 120

// 像素的宽度
#define IMG_WIDTH 320
// 像素的高度
#define IMG_HEIGHT 40
// ROI
#define IMG_ROI_WIDTH 80
#define IMG_ROI_HEIGHT 80


typedef struct
{
  COLOR_RGB rgb;
	COLOR_HSL hsl;
}colorSpace_t;

typedef struct
{
  uint32_t noBallNum;
	uint32_t WhiteBallNum;
	uint32_t BlackBallNum;
	uint8_t ballColor;
}ballColor_t;


typedef struct
{
	// 接受完一帧的标志
	uint8_t tansFinishFlag;
	// 接收完一行的标志
	uint8_t transLineFinishFlag;
	// 球颜色
	ballColor_t ballColor;
	
	uint16_t timeCount;
	uint16_t timeCountOut;
	
	uint16_t dcmiItNum;
  uint16_t fps;
	uint8_t fpsOutFlag;
	// FRAME
	uint16_t frameNum;
	// DCMI_IT_VSYNC
	uint16_t VSYNCNum;
	// rows
	uint8_t rows;
	uint8_t rowsOnROI;
	uint16_t cols;
	// 颜色空间
	colorSpace_t colorSpace;
}frame_t;

typedef struct
{
  uint8_t usartFlag;
}debug_t;

typedef struct
{
	__IO  uint16_t  gImageRGB[IMG_HEIGHT][IMG_WIDTH];
	// ROI
	//__IO uint16_t  gImageRoiRGB[IMG_ROI_HEIGHT][IMG_ROI_WIDTH];
	
	frame_t frame;
	debug_t debug;
}camera_t;


/*********function*********/

void frameSpeedOut(void);
void showSrcImgOnLCD(void);
int getBallColor(void);
#endif

