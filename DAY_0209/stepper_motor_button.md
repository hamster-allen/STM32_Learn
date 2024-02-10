# 觸摸按鍵控制步進馬達


## 原理

**在COM端輸入5V電源，想將哪個線圈導通產生磁性就給對應的接口低電平**

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0209/stepper_motor_button_picture/%E6%AD%A5%E9%80%B2%E9%A6%AC%E9%81%94%E4%BA%94%E7%B7%9A%E5%9B%9B%E7%9B%B8%E5%85%A7%E9%83%A8%E5%8E%9F%E7%90%86.png)

**驅動方式(4拍)**

* 每次旋轉90度

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0209/stepper_motor_button_picture/%E9%A9%85%E5%8B%95%E6%96%B9%E5%BC%8F4%25.png)

**驅動方式(8拍)**

* 每次旋轉45度
* 精度更高

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0209/stepper_motor_button_picture/%E9%A9%85%E5%8B%95%E6%96%B9%E5%BC%8F8%25.png)


> 重點：
> 1. 不同的步進馬達有不同的線數和相數
> 2. 不同的步進馬達也有不同的驅動電壓，使用前要先確定
> 3. 步進馬達停止旋轉時，不能長時間給線圈通電，容易導致發熱損壞
> 4. 4步和8步以外，還可以用專用的步進驅動器做最大256倍的精度驅動
> 5. 扭矩(力道)與電流相關，旋轉速度與切換時間相關

<br>

## 電路圖

**腳位經P27跳線連接至ULN2003後，再連接到步進馬達**

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0209/stepper_motor_button_picture/%E7%B9%BC%E9%9B%BB%E5%99%A8%E8%88%87%E6%AD%A5%E9%80%B2%E9%A6%AC%E9%81%94%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

> ULN2003相關資料可以參閱[DAY_0206](https://github.com/hamster-allen/STM32_Learn/tree/master/DAY_0206)

<br>

## 程序

***順時針和逆時針的旋轉函數可以搭配上方原理的驅動方式一起看更容易理解***

* 定義
  * `#define STEP_MOTOR_PORT GPIOB`IO口定義
  * `#define STEP_MOTOR_A GPIO_Pin_3`IO口定義
  * `#define STEP_MOTOR_B GPIO_Pin_4`IO口定義
  * `#define STEP_MOTOR_C GPIO_Pin_8`IO口定義
  * `#define STEP_MOTOR_D GPIO_Pin_9`IO口定義


#### 【初始化函數】
```
void STEP_MOTOR_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 	                                                   //聲明GPIO結構體
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);    //啟用IO口所在的時鐘
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                                       //啟動AFIO重映射功能時鐘
  GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D;   //設置腳位
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                           //設置為推挽輸出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                          //腳位速度設定為50MHz
  GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStructure);                                           //初始化上述設定
  //必须將禁用JTAG功能才能做GPIO使用
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);                                       //禁用JTAG功能，使腳位變為IO端口
  STEP_MOTOR_OFF();                                                                          //步進馬達起始狀態設為斷電狀態
}
```

#### 【斷電函數】
**每次只要完成動作就要回到此狀態，否則長時間通電會導致線圈損壞**
```
void STEP_MOTOR_OFF (void)
{
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);  //所有接口輸出設為0
}
```

#### 【制動(煞車)函數】
**線圈會在小於90度來回導通，因此可以達到煞車的效果**
```
void STEP_MOTOR_4S (u8 speed)
{	
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A| STEP_MOTOR_C);    //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_D);     //1
  delay_ms(speed);
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);  //將所有設為0
  delay_ms(speed);
  	
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_D);   //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C);     //1
  delay_ms(speed);
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);  //將所有設為0
  delay_ms(speed);
  	
  STEP_MOTOR_OFF();                                             //斷電
}
```

#### 【順時針轉動(4拍)】
**速度快，力道小**
```
void STEP_MOTOR_4R (u8 speed)
{
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);    //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A| STEP_MOTOR_D);   //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);    //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);    //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);    //1
  delay_ms(speed);

  STEP_MOTOR_OFF();                                             //斷電
}
```

#### 【逆時針轉動(4拍)】
**速度快，力道小**
```
void STEP_MOTOR_4L (u8 speed)
{
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);    //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);    //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);    //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);    //1
  delay_ms(speed);

  STEP_MOTOR_OFF();                                             //斷電
}
```

#### 【順時針轉動(8拍)】
**速度慢，力道大**
```
void STEP_MOTOR_8R (u8 speed)
{
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);                                  //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);                 //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);                   //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);                                  //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);                 //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);                   //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);                                  //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);                 //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);                   //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);                                  //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);                 //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);                   //1
  delay_ms(speed);

  STEP_MOTOR_OFF();
}
```

#### 【逆時針旋轉(8拍)】
**速度慢，力道大**
```
void STEP_MOTOR_8L (u8 speed)
{
  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);                                  //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);                 //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);                   //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);                                  //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);                 //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);                   //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);                                  //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);                 //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);                   //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);  //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);                                  //1
  delay_ms(speed);

  GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);                 //0
  GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);                   //1
  delay_ms(speed);

  STEP_MOTOR_OFF();
}
```












