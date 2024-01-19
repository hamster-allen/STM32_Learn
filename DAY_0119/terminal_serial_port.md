# 如何在USART接收時取得{}中間的資料


## 重定向fputc
***一定要在USART庫裡加入此程式，否則電腦的串口助手無法透過串口與STM32溝通***
```
//使UASRT串口可用printf函数发送
//在usart.h文件里可更换使用printf函数的串口号

#if 1
#pragma import(__use_no_semihosting)             
            
struct __FILE {  //标准库需要的支持函数     
	int handle; 
};

FILE __stdout;       

void _sys_exit(int x){ //定义_sys_exit()以避免使用半主机模式    
	x = x; 
} 

int fputc(int ch, FILE *f){  //重定义fputc函数 
	while((USART_n->SR&0X40)==0);  //循环发送,直到发送完毕   
        USART_n->DR = (u8) ch;      
	return ch;
}
#endif 
```
## 中斷函數
```
void USART1_IRQHandler(void)
{
  u8 Res; //設定接收變數
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    Res = USART_ReceiveData(USART1); //讀取USART的資料
    if(Res == '{')
    {
      USART1_STATUS = 0x80; //當接收到大括號的開頭時，將自訂旗標狀態設為1000 0000
    }
    if(USART1_STATUS == 0x80) //表示收到大括號開頭，開始讀取內部資料
    {
      USART1_RX_BUF[USART1_i++] = Res; //將資料存到list裡面
      if(Res == '}') //收到大括號尾部表示資料結束
      {
        printf("%s", USART1_RX_BUF); //將資料印出來
        USART1_i = 0; //將全區變數的i設為原來的0
        USART1_STATUS = 0x00; //將狀態旗標設為原來的0x00
        memset(&USART1_RX_BUF,0,200); //將list清空
      }
    }
  }
}
```
