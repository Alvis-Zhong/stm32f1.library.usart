/*******************************************************************
	************************************************************
	*
	*@file     Bsp_usart_interrupt.c
	*@author   Bin
	*@data     2016.08
	*@brief    串口驱动文件
	*
	*************************************************************
*********************************************************************/

#include "bsp_usart_interrupt.h"

/****************************************************************************************
	*@brief		中断配置函数
	*@param		无
	*@retvel	无
*****************************************************************************************/
static void NVIC_Config(void)
{
	/* 结构体定义 */
	NVIC_InitTypeDef NVIC_InitStruct ; 
	/* 中断向量组选择 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0 ) ; 
	/* 中断通道设置为串口 */
	NVIC_InitStruct.NVIC_IRQChannel = USART_IRQ ;
	/* 中断优先级设置 */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0 ;
	/* 中断抢占式优先级设置 */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1 ;
	/* 使能串口的中断处理 */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ; 
	/* 调用中断初始化函数配置上述设置*/
	NVIC_Init( &NVIC_InitStruct ) ; 
}

/**************************************************************************
	*@brief		串口配置函数
	*@param		无
	*@retvel	无
***************************************************************************/
void usart_config(void)
{
	/* 结构体定义 */
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* 打开时钟 */
	USART_CLK_FUN( USART_CLK , ENABLE ) ;
	USART_GPIO_CLK_FUN( GPIO_CLK , ENABLE ) ;
	
	/* 发送管脚配置 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_TX ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;	//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init( GPIO_PORT , &GPIO_InitStructure ) ;
	
	/* 接收管脚配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;	//浮空输入
	GPIO_Init( GPIO_PORT , &GPIO_InitStructure ) ;
	
	/* USART1 参数配置 */
	//波特率设置
	USART_InitStructure.USART_BaudRate = USART_BAUD ;
	//数据传输字长设置为8字节
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//设置一位停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//无校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	//无硬件数据控制流
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//设置为收发模式
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//调用初始化函数将上述设置配置好
	USART_Init( USART_PORT , &USART_InitStructure ) ;
	
	/* 调用中断配置函数 */
	NVIC_Config() ;
	/* 使能usart的中断控制 */
	USART_ITConfig( USART_PORT , USART_IT_RXNE , ENABLE ) ;
	/* 使能usart */
	USART_Cmd( USART_PORT , ENABLE ) ; 
	
}

/**************************************************************************
	*@brief		重定向C库函数printf到USART
	*@param		ch 
	*@retvel	ch
***************************************************************************/
int fputc(int ch, FILE *f)
{
		/* 发送数据 */
		USART_SendData( USART_PORT , (uint8_t) ch ) ;
		
		/* 等待数据发送完成 */
		while (USART_GetFlagStatus( USART_PORT , USART_FLAG_TXE ) == RESET ) ;		
	
		return (ch);
}

/**************************************************************************
	*@brief		重定向C库函数scanf到USART
	*@param		ch 
	*@retvel	ch
***************************************************************************/
int fgetc(FILE *f)
{
		/* 等待数据接收完成 */
		while (USART_GetFlagStatus( USART_PORT, USART_FLAG_RXNE ) == RESET ) ;
		/* 返回接收到数据 */
		return (int)USART_ReceiveData(USART_PORT) ;
}


