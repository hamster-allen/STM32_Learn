/*********************************************************************************************
模板制作：  杜洋工作室/洋桃电子
程序名：	USART串口发送程序
编写人：	杜洋	
编写时间：	2018年1月9日
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

修改日志：　　
1-	
	
							
说明：
 # 本模板加载了STM32F103内部的RCC时钟设置，并加入了利用滴答定时器的延时函数。
 # 可根据自己的需要增加或删减。

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"

#include "usart.h"


int main (void){//主程序
	u8 a;
	//初始化程序
	RCC_Configuration(); //时钟设置

	USART1_Init(115200); //串口初始化（参数是波特率）

	//主循环
	while(1){

		//查询方式接收
// 		if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != RESET){  //查询串口待处理标志位
// 			a =USART_ReceiveData(USART1);//读取接收到的数据
// 			printf("%c",a); //把收到的数据发送回电脑		  
// 		}
		 

//      delay_ms(1000); //延时
	}
}


/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
/*

【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒

*/



