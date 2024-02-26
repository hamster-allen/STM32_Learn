# 看門狗

- [x] 看門狗是一個計數器
- [x] 啟動後開始倒數計時
- [x] 每過一段時間CPU要重新寫入計數值(餵狗)
- [x] CPU能重寫計數值，表示程序運行正常
- [x] 如果程序運行出錯或死機，則不能重寫計數值
- [x] 當計數值減到0時，看們狗會讓整個系統復位


## 獨立看門狗

* 基於一個12位的遞減計數器 & 一個8位的預分頻器
* 內部獨立的40KHz的RC震盪器提供時鐘
* 可在當機和待機模式下運行
* 自由定時器
* 隨時都可以餵狗

![獨立看門狗示意圖]()

## 窗口看門狗

* 7位的遞減計數器
* 時鐘源使用主時鐘，並不是獨立的
* 可以產生中斷信號和復位信號
* 在調試模式下可以被凍結
* 只能在指定的範圍餵狗，如果不在指定的範圍餵狗會發出復位信號

![窗口看門狗示意圖]()

![窗口看門狗園理圖]()



## 主程序-獨立看門狗

```
int main (void)
{
  delay_ms(500);
  RCC_Configuration();                                //系統時鐘初始化 
  RELAY_Init();                                       //繼電器初始化
  LED_Init();                                         //LED初始化
  KEY_Init();                                         //KEY觸摸按鍵
  
  I2C_Configuration();                                //I2C初始化
  
  OLED0561_Init();                                    //OLED初始化
  OLED_DISPLAY_8x16_BUFFER(0,"   IWDG TEST    ");
  OLED_DISPLAY_8x16_BUFFER(4,"    RESET!      ");
  delay_ms(800);
  OLED_DISPLAY_8x16_BUFFER(4,"                ");
  
  IWDG_Init();                                        //初始化並啟動看門狗
  
  while(1)
  {
    IWDG_Feed();                                      //喂狗
      
    if(!GPIO_ReadInputDataBit(KEYPORT,KEY1))
    {
      delay_s(2);                                     //延遲2秒，使程序不能喂狗而導致復位
    }
  }
}
```

## 獨立看門狗程序

* 看門狗定時時間計算公式：<br>Tout(單位:ms) = (預分頻值*重裝載值)/40

#### 【定義】

`#define  pre  IWDG_Prescaler_64`預分頻器的取值範圍：4,8,16,32,64,128,256<br>
`#define  rlr  625`重裝載值範圍：0～0xFFF（4095）


#### 【獨立看門狗初始化函數】
```
void IWDG_Init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);     //啟動對暫存器IWDG_PR和IWDG_RLR的寫操作
  IWDG_SetPrescaler(pre);                           //設置IWDG預分頻器
  IWDG_SetReload(rlr);                              //設置IWDG重裝載值
  IWDG_ReloadCounter();                             //將重裝載值寫入到計數器
  IWDG_Enable();                                    //啟動IWDG
}
```
#### 【獨立看門狗餵狗函數】
```
void IWDG_Feed(void)
{
  IWDG_ReloadCounter();                             //將重裝載值寫入到計數器
}
```

<br>
<br>


## 主程序-窗口看門狗

```
int main (void)
{
  delay_ms(500);
  RCC_Configuration();                                //系統時鐘初始化 
  RELAY_Init();                                       //繼電器初始化
  LED_Init();                                         //LED初始化
  KEY_Init();                                         //KEY觸摸按鍵
  
  I2C_Configuration();                                //I2C初始化
  
  OLED0561_Init();                                    //OLED初始化
  OLED_DISPLAY_8x16_BUFFER(0,"   IWDG TEST    ");
  OLED_DISPLAY_8x16_BUFFER(4,"    RESET!      ");
  delay_ms(800);
  OLED_DISPLAY_8x16_BUFFER(4,"                ");
  
  WWDG_Init();                                        //初始化並啟動看門狗
  
  while(1)
  {
    delay_ms(54);                                     //使程序可以在上窗口和下窗口之間餵狗，否則會直接重啟
    WWDG_Feed();                                      //喂狗
      
    if(!GPIO_ReadInputDataBit(KEYPORT,KEY1))
    {
      delay_s(2);                                     //延遲2秒，使程序不能喂狗而導致復位
    }
  }
}
```

## 窗口看門狗程序

* 上窗口超時時間(us) = 4096*預分頻值*(計數器初始值-窗口值)/APB1時鐘頻率(MHz)
* 下窗口超時時間(us) = 4096*預分頻值*(計數器初始值-0x40)/APB1時鐘頻率(MHz)

#### 【定義】

`#define  WWDG_CNT  0x7F`計數器初始值(重裝載值)，範圍：0x40～0x7F<br>
`#define  wr  0x50`窗口值(上窗口邊界)，範圍：0x40～0x7F，下窗口邊界為固定的<br>
`#define  fprer  WWDG_Prescaler_8`預分頻值，取值：1,2,4,8

> 根據上述參數計算：<br>
> 上窗口時間:48ms，下窗口時間:64ms

#### 【窗口看門狗初始化函數】
```
void WWDG_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);   //APB1打開WWDG的相關時鐘
  WWDG_SetPrescaler(fprer);                              //設置預分頻值
  WWDG_SetWindowValue(wr);                               //設置窗口值
  WWDG_Enable(WWDG_CNT);                                 //啟動看门狗,設置計數器
  WWDG_ClearFlag();                                      //清空提前喚醒中斷旗標(當計數值到達0x40的中斷)
  WWDG_NVIC_Init();                                      //初始化窗口看門狗 NVIC
  WWDG_EnableIT();                                       //啟動窗口看門狗中斷
}
```
#### 【窗口看門狗中斷初始化函數】
```
void WWDG_NVIC_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;            //設置中斷頻道為 WWDG 中斷
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //設置搶占優先級為2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //設置子優先級為3
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;              //啟動中斷
  NVIC_Init(&NVIC_InitStructure);                            //NVIC 初始化
}
```

#### 【窗口看門狗餵狗函數】
```
void WWDG_Feed(void)
{
  WWDG_SetCounter(WWDG_CNT);                             //設置計數器寫入初始值
}
```

#### 【窗口看門狗中斷函數】
```
void WWDG_IRQHandler(void)
{
  WWDG_ClearFlag();                                     //清除提前喚醒中斷旗標

	//此處可以加入在復位前需要處理的工作或保存的數據
}
```





















