# 步進馬達程式延伸範例

* 功能
  * A -> 逆時針旋轉一圈
  * B -> 順時針旋轉一圈
  * C -> 逆時針旋轉(放開停止)
  * D -> 順時針旋轉(放開停止)

<br>

* 定義
  * `extern u8 STEP;;`全局變數(控制馬達旋轉的方向)
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

#### 【馬達單步8拍函數】

輸入參數: <br>
a->目前的步數(可以控制走到8拍中的哪一拍)<br>
speed->馬達旋轉的速度<br>

```
void STEP_MOTOR_8A (u8 a,u16 speed)
{
  switch (a){
    case 0:
      GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);  //0
      GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);                                  //1
      break;
    case 1:
      GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);                 //0
      GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);                   //1
      break;
    case 2:
      GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D);  //0
      GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);                                  //1
      break;
    case 3:
      GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);                 //0
      GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);                   //1
      break;
    case 4:
      GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D);  //0
      GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);                                  //1
      break;
    case 5:
      GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);                 //0
      GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);                   //1
      break;
    case 6:
      GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C);  //0
      GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);                                  //1
      break;
    case 7:
      GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);                 //0
      GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);                   //1
      break;
    default:
      break;
  }

  delay_ms(speed);
  STEP_MOTOR_OFF();
}
```

#### 【馬達依步數執行函數】

輸入參數:<br>
RL->順時針(1)or逆時針(0)<br>
num->旋轉幾拍，假設現在在第2拍，輸入5，馬達將會旋轉5拍，也就是到第7拍<br>
speed->馬達旋轉的速度<br>

```
void STEP_MOTOR_NUM (u8 RL,u16 num,u8 speed)
{
  u16 i;
  for(i=0;i<num;i++)
  {
    if(RL==1)    //RL=1順時針，RL=0逆時針
    {
      STEP++;
      if(STEP>7)STEP=0;
    }
    else
    {
      if(STEP==0)STEP=8;
      STEP--;
    }

    STEP_MOTOR_8A(STEP,speed);
  }
}
```

#### 【旋轉以圈執行函數】

輸入參數:<br>
RL->順時針(1)or逆時針(0)<br>
LOOP->旋轉圈數<br>
speed->馬達旋轉的速度<br>

```
void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u8 speed)
{
  STEP_MOTOR_NUM(RL,LOOP*4076,speed); 
}
```

> 注意:<br>
> `LOOP*4076`是步數與角度進行轉換，`4076`為轉換常數<br>
> 因為馬達的設計精度不同，因此需要透過實測微調<br>



































