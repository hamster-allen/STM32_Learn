# 定時器控制PWM


## 主程序

```
int main (void)
{
  delay_ms(500);
  RCC_Configuration();                                          //系統時鐘初始化
  RELAY_Init();                                                 //繼電器初始化
  I2C_Configuration();                                          //I2C初始化
  OLED0561_Init();                                              //OLED初始化
  OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    ");
  OLED_DISPLAY_8x16_BUFFER(3,"   SG90 TEST2   ");
  TOUCH_KEY_Init();                                             //觸摸按鍵初始化

  TIM3_PWM_Init(59999,23);
  //第一個參數：溢出值
  //第二個參數：分頻係數，增加係數等同於計時時間變慢，可以達到更長時間的計時效果
  //設置頻率為50Hz，公式為：溢出时间Tout(单位秒)=(溢出值+1)(分頻係數+1)/Tclk  ==>	 20MS = (59999+1)*(23+1)/72000000
  //Tclk為通用定時器的時鐘，如果APB1没有分頻，則就為系統時鐘，72MHZ
  //PWM時鐘頻率=72000000/(59999+1)*(23+1) = 50HZ (20ms),設置自動裝載值60000,預分頻係數24
  
  while(1)
  {
    if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
    {
      OLED_DISPLAY_8x16_BUFFER(6,"  Angle 0       ");
      TIM_SetCompare3(TIM3,1500);                               //改变比较值TIM3->CCR2达到调节占空比的效果（1500为0度）
    }
    if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
    {
      OLED_DISPLAY_8x16_BUFFER(6,"  Angle 45      ");
      TIM_SetCompare3(TIM3,3000);                               //改变比较值TIM3->CCR2达到调节占空比的效果
    }
    if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
    {
      OLED_DISPLAY_8x16_BUFFER(6,"  Angle 90     ");
      TIM_SetCompare3(TIM3,4500);                               //改变比较值TIM3->CCR2达到调节占空比的效果
    }
    if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
    {
      OLED_DISPLAY_8x16_BUFFER(6,"  Angle 180     ");
      TIM_SetCompare3(TIM3,7500);                               //改变比较值TIM3->CCR2达到调节占空比的效果
    }
  }
}
```

## TIMER3初始化設定函數

* 輸入參數
  * arr -> 重裝載值
  * psc -> 預分頻係數

```
void TIM3_PWM_Init(u16 arr,u16 psc)
{
  GPIO_InitTypeDef         GPIO_InitStrue;
  TIM_OCInitTypeDef        TIM_OCInitStrue;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStrue;

  //======時鐘設置======//
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);            //啟動TIM3相關GPIO時鐘
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);           //啟動GPIO時鐘，因為LED(PB0)與TIM3_CH3復用
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);            //啟動AFIO時鐘，因為要啟用中斷功能

  //======LED腳位設置======//
  GPIO_InitStrue.GPIO_Pin=GPIO_Pin_0;                            //TIM_CH3(PB0與TIM3_CH3復用)
  GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;                      //復用推挽
  GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitStrue);                              //GPIO端口初始化
      
  //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //映射，重映射只用於64、100、144腳單片機
  //當沒有重映射時，TIM3的四个通道CH1，CH2，CH3，CH4分别對應PA6，PA7,PB0,PB1
  //當部分重映射時，TIM3的四个通道CH1，CH2，CH3，CH4分别對應PB4，PB5,PB0,PB1 (GPIO_PartialRemap_TIM3)
  //當完全重映射時，TIM3的四个通道CH1，CH2，CH3，CH4分别對應PC6，PC7,PC8,PC9 (GPIO_FullRemap_TIM3)
  //一般不建議開啟映射功能

  //======設置TIM======//
  TIM_TimeBaseInitStrue.TIM_Period=arr;                         //設置自動重裝載值(溢出值)
  TIM_TimeBaseInitStrue.TIM_Prescaler=psc;                      //預分頻係數
  TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;     //計數器為累加計數
  TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;         //時鐘的分頻因子，只對穩定性產生影響，一般設為TIM_CKD_DIV1
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);                //TIM3初始化設置(設置PWM的周期)
      
  TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;                   //PWM模式1:CNT < CCR時輸出有效電平
  TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;           //設置有效電平為：高電平
  TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;       //使狀態透過IO進行輸出
  TIM_OC3Init(TIM3,&TIM_OCInitStrue);                           //將上述三個設置寫入TIM3的通道3
  
  TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);              //TIM3的通道3啟動預裝暫存器
      
  TIM_Cmd(TIM3,ENABLE);                                         //啟動TIM3
}
```

#### TIM_OCMode_PWM1和TIM_OCMode_PWM2比較

|PWM模式|功能|
|:---:|:---:|
|TIM_OCMode_PWM1|向上計數時，計數值(TIMx_CNT) < CCR值時為有效電平，否則為無效電平<br>下向計數時，計數值(TIMx_CNT) > CCR值時為無效電平，否則為有效電平|
|TIM_OCMode_PWM2|向上計數時，計數值(TIMx_CNT) < CCR值時為無效電平，否則為有效電平<br>下向計數時，計數值(TIMx_CNT) > CCR值時為有效電平，否則為無效電平|





  ![CCR值]()



















