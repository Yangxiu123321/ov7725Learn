#include "version.h"
typedef struct 
{
	__IO uint16_t  rgb[40][320]={0};//DCMI�ӿڲɼ�ԭʼ�����ڴ�
}RGB565_t;
typedef struct
{
	//��ͨ����ֵ
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