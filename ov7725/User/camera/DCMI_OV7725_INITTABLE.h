#ifndef _DCMI_OV7725_INITABLE_H
#define _DCMI_OV7725_INITABLE_H

#define CHANGE_REG_NUM 89

const uint8_t change_reg[][2]=
{
	{0x12,0x46},//QVGA, RGB565, RGB @  //VGA和QVGA分辨率下看到的图像区域是相同的，只是分辨率不同。

	//BLC  BEGIN  背光补偿(黑电平矫正)
	{0x3e,0xe6},//BLC enable and set
  {0x42,0x7f},//BLC Blue Channel Target Value
	//BLC	END
	
  {0x4d,0x00},//0x09, Analog Fix Gain Amplifier

//AWB BEGIN  白平衡
	{0x63,0xc0},//AWB Control Byte 0，AWB gain enable， AWB calculate enable，WBC threshold 2///////////00
	{0x69,0x5C},//AWB Control=00  
	{0x6b,0x20},//Simple AWB

 

//	{0x02,157}, //RED   从而达到色彩还原的效果
//	{0x03,131}, //GREEN 可以根据自己的特定应用进行调整
//	{0x01,230}, //BLUE  可以通过这个来控制白平衡里不同颜色获取的比例
	
//	{0x02,150}, //RED   从而达到色彩还原的效果
//	{0x03,131}, //GREEN 可以根据自己的特定应用进行调整
//	{0x01,200}, //BLUE  可以通过这个来控制白平衡里不同颜色获取的比例	
	
	
	
	
//	{0x02,110}, //RED   从而达到色彩还原的效果  105
//	{0x03,110}, //GREEN 可以根据自己的特定应用进行调整   90
//	{0x01,230}, //BLUE  可以通过这个来控制白平衡里不同颜色获取的比例  170	
	
//	{0x02,110}, //RED   从而达到色彩还原的效果  105
//	{0x03,110}, //GREEN 可以根据自己的特定应用进行调整   90
//	{0x01,110}, //BLUE  可以通过这个来控制白平衡里不同颜色获取的比例  170		
//	
	{0x02,157}, //RED   从而达到色彩还原的效果
	{0x03,131}, //GREEN 可以根据自己的特定应用进行调整
	{0x01,230}, //BLUE  可以通过这个来控制白平衡里不同颜色获取的比例
//
//	{0x02,255}, //RED   从而达到色彩还原的效果
//	{0x03,255}, //GREEN 可以根据自己的特定应用进行调整
//	{0x01,255}, //BLUE  可以通过这个来控制白平衡里不同颜色获取的比例
	
	
	//曝光控制-------------------------------------------
	{0x08, 0xf0},//高8位 AECH0xff
//	{0x10, 0x5f},//低8位 AECL  无灯参数
	{0x10, 0x40},//低8位 AECL  亮度1参数 35 40
//	{0x10, 0x06},//低8位 AECL  亮度1参数
	{0x1F, 0x00},//LAEC
// 	{0x9E,0x5f},//自动白平衡阈值
// 	{0x9E,0x5f},//自动白平衡阈值
	
//AWB END
	
  {0x64,0x5f},//White defect auto correction ON,Black defect auto correction ON0x1f
							//Gamma function ON,Interpolation ON,Color matrix ON
  {0x65,0x00},//DSP Control Byte 2
  {0x66,0x00},//DSP color bar OFF 

//AEC, AGC
  {0x67,0x00},//AEC selection, Before gamma, YUV or RGB  

	{0x13,0xF0},//AEC Unlimited step size, Banding filter//0Xfd////////////////////////////////////////////////////
//	{0x13,0xFf},//AEC Unlimited step size, Banding filter//0Xfd////////////////////////////////////////////////////
	
  {0x0f,0x01},//Auto window setting ON
  {0x14,0x06},//Drop VSYNC output of corrupt frame, Drop HREF output of corrupt frame, 

  {0x24,0x75},//AGC/AEC - Stable Operating Region (Upper Limit)
  {0x25,0x63},//AGC/AEC - Stable Operating Region (Lower Limit)
  {0x26,0xd1},//AGC/AEC Fast Mode Operating Region

	{0x22,0x98},//Banding Filter Minimum AEC Value
  {0x23,0x03},//Banding Filter Maximum Step
//AEC,AGC

//去噪 BEGIN--------------------------------------------
	{0xac,0x60},
  {0x8e,0x00},//De-noise Threshold
	{0x91,0x00},//Auto De-noise Threshold Control
//去噪 END--------------------------------------------
	
  {0x94,0x2c},//Matrix Coefficient 1
  {0x95,0x24},//Matrix Coefficient 2
  {0x96,0x08},//Matrix Coefficient 3
  {0x97,0x14},//Matrix Coefficient 4
  {0x98,0x24},//Matrix Coefficient 5
  {0x99,0x38},//Matrix Coefficient 6

  {0x9a,0x9e},//Matrix Control, Matrix double ON

  {0x15,0x00}, //重要参数, Common Control 10, output方面的一些东西, 这个设置必须和DCMI的设置匹配!!!!!!!!!!!!!!!!!!!!

//对比度设置 BEGIN
  {0x8f,0x00},//Sharpness (Edge Enhancement) Control 0
  {0x90,0x00},//Sharpness (Edge Enhancement) Control 1  
  {0x92,0x00},//Sharpness (Edge Enhancement) Strength Upper Limit
  {0x93,0x00},//Sharpness (Edge Enhancement) Strength Lower Limit
	{0x9c,0x30},  //set contrast //对比， Contrast Gain Gain ×0x20
//对比度设置	END

//色调参数设置 BEGIN
  {0x9b,0x00},//set luma ， Brightness Control
  {0xa9,0x00},//set hue， Cosine value ×0x80
  {0xaa,0x00},//set hue， Sine value ×0x80
//色调参数设置 END


//饱和度参数设置 BEGIN
	{0xa7,0x80},//set saturation， U Component Saturation Gain Gain ×0x40
  {0xa8,0x80},//set saturation， V Component Saturation Gain Gain ×0x40
  {0x9e,0x81},//Auto UV Adjust Control 0
//饱和度参数设置 END

//特殊效果设置 BEGIN
  {0xa6,0x27},//Special Digital Effect Control
//特殊效果设置 END

//伽马校正设置 BEGIN---------------------------------------------------
  {0x7e,0x0c},//Gamma Curve 1st Segment Input End Point 0x04 Output Value
  {0x7f,0x16},//Gamma Curve 2nd Segment Input End Point 0x04 Output Value
  {0x80,0x2a},//Gamma Curve 3th Segment Input End Point 0x04 Output Value
  {0x81,0x4e},//Gamma Curve 4th Segment Input End Point 0x04 Output Value
  {0x82,0x61},//Gamma Curve 5th Segment Input End Point 0x04 Output Value
  {0x83,0x6f},//...
  {0x84,0x7b},
  {0x85,0x86},
  {0x86,0x8e},
  {0x87,0x97},
  {0x88,0xa4},
  {0x89,0xaf},
  {0x8a,0xc5},
  {0x8b,0xd7},
  {0x8c,0xe8},//Gamma Curve 15th Segment Input End Point 0x04 Output Value
  {0x8d,0x20},//Gamma Curve Highest Segment Slope - calculated as follows: SLOP[7:0] = (0x100 - GAM15[7:0]) ×4/3
//伽马校正设置 END---------------------------------------------------

//自动帧率设置	
	{0x0e,0x00},//Auto frame 方面的设置， Auto frame rate control OFF
//手动帧率设置 BEGIN---------------------------------------------
	{0x11,0x06},//Internal Clock，pre-scalar; PLL分频器, Finternal= Finput/(Bit[5:0]+1)/2   2:143帧  3:107帧  4:88帧	6:61
	{0x0d,0xc1},//PLL 倍频
	{0x2a,(0x04 & ((240 & 0x01)<<2))}, //Dummy Pixel Insert MSB，初始化 @
	{0x2b,0x00},//			Dummy Pixel Insert LSB
  {0x33,0x00},//			Dummy Row Low 8 Bits
  {0x34,0x00},//7f,  	Dummy Row High 8 Bits

// 	{0x17,0x3f}, //HSTART 26,VGA  3f,QVGA  
// 	{0x18,0x50}, //HSIZE  A0,VGA  50,QVGA 0xA0=160, 0x50=80  
// 	{0x29,0x50}, //Horizontal Data Output SizeMSBs, 50 for QVGA  
// 	{0x19,0x03}, //VSTRT  07,VGA  03,QVGA 
// 	{0x1a,0x78}, //VSIZE	F0,VGA  78,QVGA 0xF0=240, 0x78=120
// 	{0x2a,0x00}, //Dummy Pixel Insert MSB，初始化 @
//   {0x2C,0x78}, //Vertical Data Output Size MSBs 78 for QVGA
// 	{0x32,0x00}, //Image Start and Size Control,初始化 @
	{0x17,0x3f}, //HSTART 26,VGA  3f,QVGA  
	{0x18,0x50}, //HSIZE  A0,VGA  50,QVGA 0xA0=160, 0x50=80  
	{0x29,0x50}, //Horizontal Data Output SizeMSBs, 50 for QVGA  
	{0x19,0x03}, //VSTRT  07,VGA  03,QVGA 
	{0x1a,0x78}, //VSIZE	F0,VGA  78,QVGA 0xF0=240, 0x78=120
// 	{0x2a,(0x04 & ((SIMPLE_LINE_SENSOR & 0x01)<<2))}, //Dummy Pixel Insert MSB，初始化 @
//   {0x2C,SIMPLE_LINE_SENSOR>>1}, //Vertical Data Output Size MSBs 78 for QVGA

  {0x2C,240>>1}, //Vertical Data Output Size MSBs 78 for QVGA
	{0x32,0x00}, //Image Start and Size Control,初始化 @
	

//镜头设定 BEGIN---------------------------------------------------
  {0x49,0x11},//Lens Correction Coefficient
  {0x4a,0xa0},//Lens Correction Radius – radius of the circular section where no compensation applies
  {0x4b,0x60},//Lens Correction B Channel Compensation Coefficien
  {0x4c,0x60},//Lens Correction R Channel Compensation Coefficient
  {0x46,0x05},//Lens Correction Control
  {0x47,0x08},//X Coordinate of Lens Correction Center Relative to Array Center
//镜头设定 END---------------------------------------------------

  {0x0c,0x20},///重要,   这个swap是对的吗？是否意味着， RGB的顺序改变了？？？

//  {0x0c,0x60},///重要,   这个swap是对的吗？是否意味着， RGB的顺序改变了？？？

  {0x09,0x03},//Output drive capability

  //{0x0c,0x00},
  {0x00,0x00},//AGC – Gain control gain setting
  {0x00,0x00},//AGC – Gain control gain setting
  {0x00,0x00},//AGC – Gain control gain setting   
	
};

#endif
