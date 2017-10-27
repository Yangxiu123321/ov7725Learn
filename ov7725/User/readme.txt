
//--------------------------------------------------//
此工程是测试用
  1: 25fps
  2: 高级自动白平衡
  3：每帧图像发送电脑
  while(1)
	{
		while(g_DCMI_IT_FRAME_FLAG!=0);//等待识别条到来
     g_DCMI_IT_FRAME_FLAG=2;
		
					//记录数据
			for(i=0;i<20;i++)//存数据
			{
				for(ii=0;ii<320;ii++)
				{
					g_ColorData16t_deal[i][ii]=g_ColorData16t[i][ii];
				}
			}
			
			
	    LCD_SetCursor(0,144);
			LCD_REG = 0x0022;	
			for(i=0;i<10;i++)
			{
				for(ii=0;ii<320;ii++)
				LCD_RAM =g_ColorData16t_deal[i][ii];
			}//0.8ms-O0
		}