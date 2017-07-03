#include "my_uart.h"

char uart_buffer[100] = {0};
uint16_t uart_length = 0;	 

/* �V USART3 �o�e�r�� */
void Usart3_Printf(char *string)
{
    while(*string)
		{
        USART_SendData(USART3, (unsigned short int) *string++);
 
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
}

void Usart3_ReadLine(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if(uart_length >= sizeof(uart_buffer) - 1)
		{
			memset(uart_buffer, 0, sizeof(uart_buffer));
			uart_length = 0;
		}
		
		uart_buffer[uart_length++] = (char)USART_ReceiveData(USART3);		
	}
}

void Usart3_Interrupt(void)
{
	Usart3_ReadLine();
	
	if(uart_buffer[uart_length - 1] == '\0')
	{
		if(!strcmp(uart_buffer, "ABC"))
			Usart3_Printf("This is ABC.\n");
		
		if(!strcmp(uart_buffer, "BCB"))
			Usart3_Printf("This is BCB.\n");
		
		if(!strcmp(uart_buffer, "AAC"))
			Usart3_Printf("This is AAC.\n");
		
		memset(uart_buffer, 0, sizeof(uart_buffer));
		uart_length = 0;
	}
}
 
void Usart3_Init(void)
{
		/* �ŧi USART�BGPIO ���c�� */
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
		/* �ҥ� GPIOB�BUSART3 ��RCC���� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
 
	  /* �]�w Pin10�BPin11 �s���� USART3 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
 
    /* �]�w Pin10 �� TX */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
 
    /* �]�w Pin11 ��RX */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		/* USART �򥻰ѼƳ]�w */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
    USART_Init(USART3, &USART_InitStructure);
		
		/* �ҥ� USART3 */
    USART_Cmd(USART3, ENABLE);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		
		/* �ҥ� USART3 ���_ */    
    NVIC_EnableIRQ(USART3_IRQn);
}