#include "camera.h"
#include "usart.h"
extern camera_t OV;
void frameSpeedOut()
{
	if(OV.frame.fpsOutFlag == 1)
	{
		OV.frame.fps = OV.frame.frameNum;
		OV.frame.frameNum = 0;
		
		OV.frame.fpsOutFlag = 0;
		
		//输出帧率
		//printf("%d",OV.frame.fps);
		//printf("%d",OV.frame.dcmiItNum);
		OV.frame.dcmiItNum = 0;
		
		//输出程序时间
		printf("%d",OV.frame.timeCount);
	}
}






