#ifndef __PICTURE_H
#define __PICTURE_H
#include "stm32f4xx.h"
void GetROIRGB(void);
void RGB565TORGB888(void);
void AEC_Fun(void);
void colorseg(void);
void SendDataToPC(void);
void NEWcolorseg(void);
void RGB565TORGB888_compare(void);
void ColorMutation(void);
void SendData(void);
void SendAllReadyToCar(void);
void AEC_WHITE_Fun(void);
void SeeSmallCar(void);

typedef struct {
	uint16_t leftnumber;
	uint16_t rightnumber;
	uint16_t baixian;
}SIDE_TypeDef;

typedef struct {
	uint16_t value;
	uint16_t number;
}PIXEL_DANO;
void findwhite(void);

//typedef struct {
//	uint16_t color[320];
//	uint16_t stnum[320];
//	uint16_t width[320];
//}COLORSITU;
void findwhite(void);
#endif

