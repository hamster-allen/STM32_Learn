# 外部中斷原理與驅動

## 特點

* STM32F1支持將所有GPIO設置為中斷輸入
* 可由上沿、下沿、高低電平轉換三種方式觸發
* 可選擇中斷或事件觸發


## 中斷與事件比較

* 中斷模式

  * **觸發後需要CPU參與**

![中斷模式_解釋]()


* 事件模式

  * **觸發後不需要CPU參與，但需要提前設置**

![事件模式_解釋]()


## 中斷處理函數

***EXTI5到EXTI9都是使用同一個中斷處理函數***<br>
***EXTI10到EXTI15都是使用同一個中斷處理函數***

|GPIO|中斷旗標|中斷處理函數|
|:---:|:---:|:---:|
|PA0~PG0|EXTI0|EXTI0_IRQHandler|
|PA1~PG1|EXTI1|EXTI1_IRQHandler|
|PA2~PG2|EXTI2|EXTI2_IRQHandler|
|PA3~PG3|EXTI3|EXTI3_IRQHandler|
|PA4~PG4|EXTI4|EXTI4_IRQHandler|
|PA5~PG5|EXTI5|EXTI9_5_IRQHandler|
|PA6~PG6|EXTI6|、、|
|PA7~PG7|EXTI7|、、|
|PA8~PG8|EXTI8|、、|
|PA9~PG9|EXTI9|、、|
|PA10~PG10|EXTI10|EXTI15_10_IRQHandler|
|PA11~PG11|EXTI11|、、|
|PA12~PG12|EXTI12|、、|
|PA13~PG13|EXTI13|、、|
|PA14~PG14|EXTI14|、、|
|PA15~PG15|EXTI15|、、|
||EXTI16|PVD輸出|
||EXTI17|RTC鬧鐘事件|
||EXTI18|USB喚醒事件|

## 中斷與嵌套向量中斷控制器(NVIC)的關係

所有的中斷都將傳至嵌套向量中斷控制器中，會依照優先級進行排隊處理<br>
優先級越高越優先傳送中斷請求給CPU處理

![中斷_嵌套向量中斷控制器_關係說明圖]()

> 在程序中，如果要使用外部IO中斷，不只要開啟外部IO中斷，也要開啟NVIC<br>
> 確保中斷可以順利進入NVIC，並傳送給CPU處理


## 主程序

```
int main (void)
{
  u8 s;
  delay_ms(500);
  RCC_Configuration();                                     //系统時鐘初始化 
  RELAY_Init();                                            //繼電器初始化
  
  I2C_Configuration();                                     //I2C初始化
  OLED0561_Init();                                         //OLED初始化
  OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    ");
  OLED_DISPLAY_8x16_BUFFER(3," KEYPAD4x4 TEST ");
  
  INT_MARK = 0;                                            //中斷旗標(NVIC.h的全局變量)
  
  NVIC_Configuration();                                    //設置中斷優先级
  KEYPAD4x4_Init();                                        //陣列鍵盤初始化
  KEYPAD4x4_INT_INIT();                                    //陣列鍵盤的中斷初始化
  
  while(1)
  {
    //其他程序内容
    
    if(INT_MARK)                                          //中斷旗標為1表示有按鍵中斷
    { 
      INT_MARK=0;                                         //中斷旗標清0
      s=KEYPAD4x4_Read();                                 //讀出按鍵值
      if(s!=0)                                            //如果按鍵不是0，表示有按鍵操作
      {
        //-------------------------"----------------"
        OLED_DISPLAY_8x16_BUFFER(6," KEY NO.        ");
        OLED_DISPLAY_8x16(6,8*8,s/10+0x30);
        OLED_DISPLAY_8x16(6,9*8,s%10+0x30);
      }
    }
  }
}
```

## NVIC設置函數

#### 【定義】

* `extern u8 INT_MARK;`中斷旗標

#### 【陣列鍵盤中斷初始化函數】

```
void KEYPAD4x4_INT_INIT (void)
{
  NVIC_InitTypeDef  NVIC_InitStruct;                           //聲明結構體
  EXTI_InitTypeDef  EXTI_InitStruct;                           //聲明結構體
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);         //啟動GPIO時鐘(需要與復用時鐘同一個)   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);        //啟動復用時鐘，才能使用外部中斷
  
  //======第1个中斷======//
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);  //定義將GPIO組設置到指定的中斷通道
  	
  EXTI_InitStruct.EXTI_Line=EXTI_Line4;                        //定義中斷通道
  EXTI_InitStruct.EXTI_LineCmd=ENABLE;                         //啟動中斷
  EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;               //中斷模式設為中斷
  EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;           //中斷觸發為下降沿
  	
  EXTI_Init(& EXTI_InitStruct);                                //寫入以上EXTI參數
  	
  NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;                  //定義中斷入口
  NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;                   //啟動中斷，讓外部中斷可以進入NVIC
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;         //設置搶占優先級為2
  NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;                //設置子優先級為2
  NVIC_Init(& NVIC_InitStruct);                                //寫入以上NVIC參數
	
  //======第2个中斷======//	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
  	
  EXTI_InitStruct.EXTI_Line=EXTI_Line5;
  EXTI_InitStruct.EXTI_LineCmd=ENABLE;
  EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
  	
  EXTI_Init(& EXTI_InitStruct);
  	
  NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;                //因為EXIT9到EXIT5使用同一個處理函數
  NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
  NVIC_Init(& NVIC_InitStruct);
  
  //======第3个中斷======//
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
  
  EXTI_InitStruct.EXTI_Line=EXTI_Line6;
  EXTI_InitStruct.EXTI_LineCmd=ENABLE;
  EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
  	
  EXTI_Init(& EXTI_InitStruct);
  	
  NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;                //因為EXIT9到EXIT5使用同一個處理函數
  NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
  NVIC_Init(& NVIC_InitStruct);

  //======第4个中斷======//
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
  	
  EXTI_InitStruct.EXTI_Line=EXTI_Line7;
  EXTI_InitStruct.EXTI_LineCmd=ENABLE;
  EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
  	
  EXTI_Init(& EXTI_InitStruct);
  	
  NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;                //因為EXIT9到EXIT5使用同一個處理函數
  NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
  NVIC_Init(& NVIC_InitStruct);
}
```

#### 【中斷處理函數】

```
void  EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4)!=RESET){ 
    INT_MARK=1;                              //中斷旗標設為1，表示有中斷
    EXTI_ClearITPendingBit(EXTI_Line4);      //清除EXTI LINE上的中斷旗標
  }     
}

void  EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
  {
    INT_MARK=2;                              //中斷旗標設為2，表示有中斷
    EXTI_ClearITPendingBit(EXTI_Line5);      //清除EXTI LINE上的中斷旗標
  }     
  if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
  {
    INT_MARK=3;                              //中斷旗標設為3，表示有中斷
    EXTI_ClearITPendingBit(EXTI_Line6);      //清除EXTI LINE上的中斷旗標
  }     
  if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
  {
    INT_MARK=4;                              //中斷旗標設為4，表示有中斷
    EXTI_ClearITPendingBit(EXTI_Line7);      //清除EXTI LINE上的中斷旗標
  }     
}
```






















