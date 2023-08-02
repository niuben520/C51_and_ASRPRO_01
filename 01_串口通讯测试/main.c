#include "config.h"
#include "GPIO.h"
#include "UART.h"
#include "delay.h"
#include <stdio.h>

#define MOS P20

char putchar (char dat) {
	TX1_write2buff(dat);
	return dat;
}

void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
	
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//初始化
	
		GPIO_InitStructure.Pin  = GPIO_Pin_0;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);//初始化
}

void UART_config(void) {
    COMx_InitDefine		COMx_InitStructure;					//结构定义
	
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 9600ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
    COMx_InitStructure.UART_Priority  = Priority_0;			//指定中断优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4
	
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 9600ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
    COMx_InitStructure.UART_Priority  = Priority_0;			//指定中断优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    COMx_InitStructure.UART_P_SW      = UART2_SW_P10_P11;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
    UART_Configuration(UART2, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4
	
}
void on_uart1_recv() {
		// uart1收到数据了
		u8 i;
    for(i=0; i<COM1.RX_Cnt; i++)	{
        // RX1_Buffer[i]存的是接收的数据，写出用 TX1_write2buff
				TX2_write2buff(RX1_Buffer[i]); // 将1收到的数据，通过UART2发送出去
//				TX1_write2buff(RX1_Buffer[i]); // 将1收到的数据，通过UART1发送出去
				
    }
}
void on_uart2_recv(){
		// uart2收到数据了
		u8 i;
    for(i=0; i<COM2.RX_Cnt; i++)	{
				TX1_write2buff(RX2_Buffer[i]); 	// 将2收到的数据，从UART1发出去
				
    }
}

void main() {
		
    GPIO_config();

    UART_config();

    EA = 1;


    while(1) {
        delay_ms(10);

//				// 判断 UART1 是否收到数据了 5
//        if((COM1.RX_TimeOut > 0 )&& (--COM1.RX_TimeOut == 0)) {
//            //超时计数
//						if(COM1.RX_Cnt > 0) {
//								on_uart1_recv();
//						}
//						COM1.RX_Cnt = 0;
//        }
//				
				// 判断 UART2 是否收到数据了
        if(COM2.RX_TimeOut > 0) {
            //超时计数
					printf("hallo1");
            if(--COM2.RX_TimeOut == 0) {
							printf("hallo2");
									if(COM2.RX_Cnt > 0) {
										printf("hallo3");
                    on_uart2_recv();
											
											if (RX2_Buffer[0] == 1){
													MOS = 1;
											}
											else if(RX2_Buffer[0] == 0){
												MOS = 0;
											}									
									}
                COM2.RX_Cnt = 0;
            }
        }
				
    }
}