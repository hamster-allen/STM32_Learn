# 透過觸摸按鍵控制繼電器

## 事前工作

* 短接繼電器的跳線


![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/%E7%B9%BC%E9%9B%BB%E5%99%A8%E8%B7%B3%E7%B7%9A.png)

* 短接觸摸按鍵的跳線
  
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/%E8%A7%B8%E6%91%B8%E6%8C%89%E9%8D%B5%E8%B7%B3%E7%B7%9A.png)

<br>

## 原理

#### 【控制連接圖示】

* 透過STM32輸出控制繼電器的電磁鐵加以控制是否導通

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/%E6%8E%A7%E5%88%B6%E9%80%A3%E6%8E%A5%E5%9C%96%E7%A4%BA.png)

#### 【JTAG電路圖與注意事項】

***在JTAG中也使用到控制繼電器的腳位(PA13、PA14)，並且在上電時為JTAG工作模式，如果要當作IO口使用必須透過軟體設置***

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/JTAG_IC.png)

#### 【繼電器與步進馬達電路圖】

* 繼電器腳位經過P26跳線連接至ULN2003
* ULN2003是一個達靈頓電路晶片，可以使用小電流驅動大電流，透過大電流輸出給繼電器的線圈，使線圈吸合

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/%E7%B9%BC%E9%9B%BB%E5%99%A8%E8%88%87%E6%AD%A5%E9%80%B2%E9%A6%AC%E9%81%94%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

* 因為ULN2003裡有反向閘，所以輸出回反向

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/ULN2003%E6%8E%A5%E8%85%B3%E5%AE%9A%E7%BE%A9.png)

<br>

## 程序

**注意: 因為PA13、PA14上電後為JTAG功能，因此需要載RCC程序裡啟動AFIO時鐘**<br>

* 定義
  * `#define RELAYPORT GPIOA`腳位定義
  * `#define RELAY1 GPIO_Pin_14`腳位定義
  * `#define RELAY2 GPIO_Pin_13`腳位定義

#### 【主程序】
```
int main (void)
{
  RCC_Configuration();        //系統時鐘初始化 
  TOUCH_KEY_Init();           //觸摸按鍵初始化
  RELAY_Init();               //繼電器初始化
  
  while(1){
    if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))RELAY_1(1); //按鍵A被按下時，繼電器1開啟
    if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))RELAY_1(0); //按鍵B被按下時，繼電器1關閉
    if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))RELAY_2(1); //按鍵C被按下時，繼電器2開啟
    if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))RELAY_2(0); //按鍵D被按下時，繼電器2關閉
  }
}
```






#### 【繼電器初始化】

```
void RELAY_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;                            //GPIO結構體聲明
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //外設時鐘使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);             //啟用AFIO重映射功能時鐘
  GPIO_InitStructure.GPIO_Pin = RELAY1 | RELAY2;                   //初始化的腳位
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                 //腳位設定為推挽輸出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                //GPIO頻率為50MHz
  GPIO_Init(RELAYPORT, &GPIO_InitStructure);                       //初始化上述的設定
  //必须將禁用JTAG功能才能做GPIO使用
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);             //禁用JTAG功能，這樣腳位才能被用為IO端口
  GPIO_ResetBits(RELAYPORT,RELAY1 | RELAY2);                       //將腳位設置在低電平，繼電器為關斷狀態
}
```

#### 【繼電器控制1】
```
void RELAY_1(u8 c)        //c=0繼電器放開，c=1繼電器吸合
{ 
  GPIO_WriteBit(RELAYPORT,RELAY1,(BitAction)(c));
}
```


#### 【繼電器控制2】
```
void RELAY_2(u8 c)        //c=0繼電器放開，c=1繼電器吸合
{ 
  GPIO_WriteBit(RELAYPORT,RELAY2,(BitAction)(c));
}
```












