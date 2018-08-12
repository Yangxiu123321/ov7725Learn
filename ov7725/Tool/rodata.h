#ifndef __RODATA_H
#define __RODATA_H
#endif
//未加入修饰
#define GETR_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )RGB565) >>11)<<3))		  			//·µ»Ø8Î» R
#define GETG_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x7ff)) >>5)<<2)) 	//·µ»Ø8Î» G
#define GETB_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x1f))<<3)))       	//·µ»Ø8Î» B
////加入修饰
//#define GETR_FROM_RGB16(RGB565) ((RGB565>>11)<<3)|((RGB565>>11)&0x07)
//#define GETG_FROM_RGB16(RGB565) (((RGB565 >> 5) & 0x3f) << 2) | (((RGB565 >> 5) & 0x3f) & 0x03);
//#define GETB_FROM_RGB16(RGB565) ((RGB565&0x1f)<<3)|(RGB565&0x07)
//原始数据高度
 #define PIXEL_H 40
//定义原始数据宽度
#define PIXEL_W 320
//阈值允许参数
#define GRAY_BREAK_RANGE 1 

