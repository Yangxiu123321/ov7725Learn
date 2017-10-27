#include "usart.h"
#include "stdarg.h"
#include "stdio.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void USART_NVIC_Config(void);

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure Open207V_USART 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART_Configuration(void)
{ 												
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_AHB1PeriphClockCmd(Open207V_USART_TX_GPIO_CLK,ENABLE);
  RCC_AHB1PeriphClockCmd(Open207V_USART_RX_GPIO_CLK,ENABLE);

  RCC_APB1PeriphClockCmd(Open207V_USART_CLK,ENABLE);


  GPIO_PinAFConfig(Open207V_USART_TX_GPIO_PORT, Open207V_USART_TX_SOURCE, Open207V_USART_TX_AF);
  GPIO_PinAFConfig(Open207V_USART_RX_GPIO_PORT, Open207V_USART_RX_SOURCE, Open207V_USART_RX_AF);

  /*
  *  Open207V_USART_TX -> PA9 , Open207V_USART_RX -PA10
  */
  GPIO_InitStructure.GPIO_Pin = Open207V_USART_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(Open207V_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = Open207V_USART_RX_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Open207V_USART_TX_GPIO_PORT, &GPIO_InitStructure);

/*
   		 USARTx configured as follow:
         - BaudRate = 115200 baud  
		 - Word Length = 8 Bits
         - One Stop Bit
         - No parity
         - Hardware flow control disabled (RTS and CTS signals)
         - Receive and transmit    
 */

  USART_InitStructure.USART_BaudRate = 921600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(Open207V_USART, &USART_InitStructure);
  /* Enable the Open207V_USART Transmit interrupt: this interrupt is generated when the 
     Open207V_USART transmit data register is empty */
		 
 	//////////   ÉèÖÃUSART3ÖÐ¶Ï       ///////////////
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
  USART_Cmd(Open207V_USART, ENABLE);
  //USART_NVIC_Config();

}
void UART3_SendChar(int32_t disp)
{

	uint16_t dispbuf[6];
	uint8_t i;
  if(disp>=0)
	{
	dispbuf[2] = disp%10 + '0';
	dispbuf[1] = disp/10%10 + '0';
	dispbuf[0] = disp/10/10%10 + '0';

	for(i=0;i<3;i++)
		 USART_SendData(USART3,dispbuf[i]);
	}
	else
	{
	dispbuf[2] = -disp%10 + '0';
	dispbuf[1] = -disp/10%10 + '0';
	dispbuf[0] = -disp/10/10%10 + '0';
	
	USART_SendData(USART3,'-');
	for(i=0;i<3;i++)
		 USART_SendData(USART3,dispbuf[i]);
	}
}

void USART_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = Open207V_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/* Use no semihosting */
#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{  
	int handle;
};
FILE __stdout;

int _sys_exit(int x)
{
	x = x;
}
#endif

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(Open207V_USART, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(Open207V_USART, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
void USART_OUT(USART_TypeDef* USARTx, const char *Data, ...)
{ 
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data != 0)				                          //????????????
	{
		if(*Data == 0x5c)									  //'\'
		{	
			switch (*++Data)
			{
				case 'r':							          //???
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //???
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}	 
		}
		else if(*Data == '%')									  //
		{
			switch (*++Data)				
			{
				case 's':										  //???
                	s = va_arg(ap, const char *);
                	for (; *s; s++) 
				    {
                    	USART_SendData(USARTx, *s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //???
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) 
				    {
                    	USART_SendData(USARTx, *s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}
/******************************************************
		??????????
        char *itoa(int value, char *string, int radix)
		radix=10 ???10??	????,?????0;  

	    ?:d=-379;
		??	itoa(d, buf, 10); ?
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    uint32_t i;
	int d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 100000000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} 