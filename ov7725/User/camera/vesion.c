#include "version.h"
typedef struct 
{
	__IO uint16_t  rgb[40][320]={0};//DCMI接口采集原始数据内存
}RGB565_t;
typedef struct
{
	//单通道数值
	single[5][320];
}SingleCh;
typedef struct 
{
	SingleCh R;
	SingleCh G;
	SingleCh B;
}RGB888_t;
typedef struct 
{
	RGB565_t RGB565;
	RGB888_t RGB888;
}version;