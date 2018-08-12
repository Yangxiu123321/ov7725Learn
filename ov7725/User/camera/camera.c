#include "camera.h"
#include "usart.h"
#include "rodata.h"
#include "LCD/LCD.h"
#include "EasyTracer.h"
extern camera_t OV;

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void frameSpeedOut()
{
	if(OV.frame.fpsOutFlag == 1)
	{
		OV.frame.fps = OV.frame.frameNum;
		OV.frame.frameNum = 0;
		
		OV.frame.fpsOutFlag = 0;
		
		//printf("f:%dv:%d\n",OV.frame.fps,OV.frame.VSYNCNum);
		//printf("%d",OV.frame.dcmiItNum);
		OV.frame.VSYNCNum = 0;
		OV.frame.dcmiItNum = 0;
		
		//printf("%d",OV.frame.timeCount);
	}
}

/**
  * @brief  show sourceImg on LCD
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void showSrcImgOnLCD(void)
{
	static uint16_t yushu=0;
  yushu=(OV.frame.rows % 40);
	if(yushu==1) 
	{
		OV.frame.rowsOnROI = 39;
	}		
	else if(yushu==0)
	{
		OV.frame.rowsOnROI = 38;
	}
	else
	{
		OV.frame.rowsOnROI = yushu-2;
	}
	
	if(OV.frame.rows == 1)
	{		
		LCD_SetCursor(0,239);
		LCD_REG = 0x0022;
		for(int j=0;j<320;j++)
		{
			LCD_RAM=OV.gImageRGB[39][j];
		}
	}
	else
	{
		LCD_SetCursor(0,OV.frame.rows - 2);
		LCD_REG = 0x0022;
		for(int j=0;j<320;j++)
		{
			LCD_RAM=OV.gImageRGB[OV.frame.rowsOnROI][j];
		}	
	}
		
}
/**
  * @brief   Identifying the ball
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
// 没球
#define NO_BALL    0
// 白球
#define WHITE_BALL 1
// 黑球
#define BLACK_BALL 2
// 不识别的区域
#define REJECT_AREA 3
uint16_t startRows = (SRCIMG_HEIGHT_CENTER - 40);
uint16_t endRows = (SRCIMG_HEIGHT_CENTER + 40);
int getBallColor(void)
{
	static uint16_t yushu=0;
	static int adjustTime = 0;
	if(OV.frame.rows < startRows || OV.frame.rows > (endRows))
	{
		// 次区域不识别
	  return REJECT_AREA;
	}
	

  yushu=(OV.frame.rows % 40);
	if(yushu==1) 
	{
		OV.frame.rowsOnROI = 39;
	}		
	else if(yushu==0)
	{
		OV.frame.rowsOnROI = 38;
	}
	else
	{
		OV.frame.rowsOnROI = yushu-2;
	}
	
		
  LCD_REG = 0x0022;
	LCD_SetCursor(120,OV.frame.rows - 2);
	for(int j=120;j<200;j++)
	{
		// show
		LCD_RAM=OV.gImageRGB[OV.frame.rowsOnROI][j];
		// process
		OV.frame.colorSpace.rgb.red = (unsigned char)(((OV.gImageRGB[OV.frame.rowsOnROI][j]) & 0xf800)>>8);
		OV.frame.colorSpace.rgb.green = (unsigned char)(( (OV.gImageRGB[OV.frame.rowsOnROI][j]) & 0x07e0)>>3);
		OV.frame.colorSpace.rgb.blue = (unsigned char)(( (OV.gImageRGB[OV.frame.rowsOnROI][j]) &0x001f));
		//RGBtoHSL(&OV.frame.colorSpace.rgb,&OV.frame.colorSpace.hsl);
		
		if(OV.frame.colorSpace.rgb.red > 200)
		{
		   if(OV.frame.colorSpace.rgb.green > 200)
			 {
				 //白球
				 OV.frame.ballColor.WhiteBallNum ++;
			 }else
			 {
				 //没球
			   OV.frame.ballColor.noBallNum ++;
			 }
		}else{
		   if(OV.frame.colorSpace.rgb.green < 100)
			 {
				 //黑球
			    OV.frame.ballColor.BlackBallNum ++;
			 }else
			 {
			   OV.frame.ballColor.noBallNum ++;
			 }
		}
					
	}	
	// 完成一帧之后，比较哪一种状态出现的次数最多
	adjustTime ++;
	if(adjustTime > 80)
	{
		adjustTime = 0;
	  if((OV.frame.ballColor.WhiteBallNum > OV.frame.ballColor.BlackBallNum) && (OV.frame.ballColor.WhiteBallNum > OV.frame.ballColor.noBallNum))
		{
		  OV.frame.ballColor.ballColor = 1;
		}else if((OV.frame.ballColor.BlackBallNum > OV.frame.ballColor.WhiteBallNum) && (OV.frame.ballColor.BlackBallNum > OV.frame.ballColor.noBallNum))
		{
		  OV.frame.ballColor.ballColor = 2;
		}else
		{
		  OV.frame.ballColor.ballColor = 0;
		}
		OV.frame.ballColor.WhiteBallNum = 0;
		OV.frame.ballColor.BlackBallNum = 0;
		OV.frame.ballColor.noBallNum = 0;
		printf("AT%d\r\n",OV.frame.ballColor.ballColor);
	}
	if(OV.debug.usartFlag == 1)
	{
		OV.debug.usartFlag = 0;
		//printf("AT%d\r\n",OV.frame.ballColor.ballColor);
		//printf("%d\n",OV.gImageRGB[OV.frame.rowsOnROI][j]);
		//printf("%d:%d:%d\n",OV.frame.colorSpace.rgb.red,OV.frame.colorSpace.rgb.green,OV.frame.colorSpace.rgb.blue);
		//printf("%d:%d:%d\n",OV.frame.colorSpace.hsl.hue,OV.frame.colorSpace.hsl.luminance,OV.frame.colorSpace.hsl.saturation);
	}	
	return 0;
}







