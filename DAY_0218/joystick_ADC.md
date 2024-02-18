# ADC驅動原理與分析-搖桿

***基本原理與前面光敏電阻和可變電阻的相同***

<br>

## 效果呈現

* 根據搖桿XY軸不同會有不同的值顯示在OLED上
* ADC_IN6 -> X軸(右:4090、左:0)
* ADC_IN7 -> Y軸(上:4090、下:0)

  ![效果呈現]()

<br>

## 前置工作

* 連接搖桿的跳線(P17)
  
  ![搖桿跳線]()

* 斷開編碼器的跳線(P18)，因為腳位共用

  ![編碼器跳線]()

<br>

## 電路圖

* 編碼器&搖桿腳位之電路圖對應名稱

  ![電路圖1]()

* 三個腳位透過跳線(P17)連接到搖桿
* `JS_X`和`JS_Y`分別連接到搖桿內部的可變電阻
* `JS_D`連接到搖桿的按鍵

  ![電路圖2]()

<br>

## 程序

***此程序使用DAY_0218的光敏電阻&電位器進行修改，與之前不同的地方會特別標註並加以說明***

**此程序DAM將讀取後的數據自動存放到SRAM中，因此CPU只需要直接到SRAM讀取存放到SRAM中的值即可**

* 定義
  * `#define ADC1_DR_Address  ((uint32_t)0x4001244C)`定義ADC1的地址，由上方【ADC暫存器地址】得知
    > 使用到DMA，因此需要告訴DAM要從哪個地址讀取數據
  * `#define ADCPORT  GPIOA`定義ADC腳位
  * `#define ADC_CH4  GPIO_Pin_4`定義ADC腳位，可變電阻
  * `#define ADC_CH5  GPIO_Pin_5`定義ADC腳位，光敏電阻
  * `#define ADC_CH6  GPIO_Pin_6`定義ADC腳位，搖桿X軸
  * `#define ADC_CH7  GPIO_Pin_7`定義ADC腳位，搖桿Y軸

* 變量
  * `extern vu16 ADC_DMA_IN[2];`用於儲存ADC最終轉化的數據
    > 因為要讀取的數據有兩個

#### 【DMA初始化】
```
void ADC_DMA_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;                                           //聲明DMA初始化結構體
  DMA_DeInit(DMA1_Channel1);                                                   //使DAM1通道復位，表示即將使用DAM通道傳送數據
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;                  //設置DMA通道外設地址=ADC1_DR_Address，也就是在告訴DMA要從哪裡讀取數據
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_DMA_IN;                     //設置DAM將讀出的數據存放到指定的變量(可直接讀此變量即是ADC值)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //指定外設的源地址
  DMA_InitStructure.DMA_BufferSize = 2;                                        //緩衝區大小，目前只有光敏電阻，因此設置1即可(根據ADC採集通道数量修改)
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //當前外設暫存器地址不變：Disable不變，Enable遞增(用於多通道採集)
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //當前暫存器地址：Disable不變，Enable遞增(用於多通道採集)，目前只採集一個數據，因此不需要偏移
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //設置DAM讀取的數據寬度，設置為16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;          //設置DMA存放地址的數據寬度，設置為16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                              //DMA通道操作模式位環形緩衝模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA通道優先級高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //禁止DMA通道儲存器到儲存器傳輸
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //初始化DMA通道1
  DMA_Cmd(DMA1_Channel1, ENABLE);                                              //使能DMA通道1
}
```


#### 【GPIO初始化】
```
void ADC_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;                                                            //聲明GPIO初始化結構體
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);   //開啟GPIO相關時鐘
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                                               //開啟DMA相關時鐘(因為要使用DAM傳送ADC的數據)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);                                             //開啟ADC相關時鐘

  |--------------------------------------------------------------------------------------------------------------------------------|
  |  GPIO_InitStructure.GPIO_Pin = ADC_CH6 | ADC_CH7;                                              //選擇ADC腳位                    |
  |                                                                                                * 更改通道端口為6和7，搖桿的XY軸  |
  |--------------------------------------------------------------------------------------------------------------------------------|
                
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                                    //選擇輸入的工作模式     
  GPIO_Init(ADCPORT, &GPIO_InitStructure);			
}
```


#### 【ADC初始化】

```
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;                                           //聲明ADC初始化結構體
  ADC_GPIO_Init();                                                             //GPIO初始化函數
  ADC_DMA_Init();                                                              //DMA初始化函數
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                           //設置ADC1、ADC2為獨立工作模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                 //開啟ADC掃描模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                           //開啟連續轉換工作模式，也就是不斷循環採集ADC通道上的值
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;          //轉換由軟體控制轉換
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                       //ADC數據向右邊對齊
  ADC_InitStructure.ADC_NbrOfChannel = 2;                                      //ADC的通道數
  ADC_Init(ADC1, &ADC_InitStructure);                                          //將上述的設置進行初始化

  |----------------------------------------------------------------------------------------------------------------|
  |  //設置指定ADC的規則組通道，設置他的轉化順序與採樣時間                                                             |
  |  //ADC1,ADC通道x,規則採樣顺序值為y,採樣時間為28周期                                                               |
  |  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_28Cycles5);          * 更改採集通道為6         |
  |  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_28Cycles5);          * 更改採集通道為7         |
  |----------------------------------------------------------------------------------------------------------------|
  
  ADC_DMACmd(ADC1, ENABLE);                                                    //啟用ADC的DMA支持，也就是允許DMA讀取ADC數據
  ADC_Cmd(ADC1, ENABLE);                                                       //開啟ADC1
  ADC_ResetCalibration(ADC1);                                                  //重置指定ADC的校準暫存器
  while(ADC_GetResetCalibrationStatus(ADC1));                                  //獲取ADC重置校準暫存器的狀態，並等待完成
  ADC_StartCalibration(ADC1);                                                  //開始指定ADC校準狀態
  while(ADC_GetCalibrationStatus(ADC1));                                       //獲取指定ADC的校準程序，並等待完成
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                      //開啟ADC軟體開始轉化，執行後，ADC1將會開始把模擬量轉化成數據，再通過DMA功能存放到指定的變量中
}
```

#### 【搖桿初始化】
**此初始化只有搖桿的微動開關，不包含ADC和DMA的初始化設置**
```
void JoyStick_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;                                                             //聲明GPIO初始化結構體
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);    //啟用GPIO相關時鐘
  GPIO_InitStructure.GPIO_Pin = JoyStick_KEY;                                                       //選擇要初始化的GPIO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                                     //設置GPIO為上拉電阻      
  GPIO_Init(JoyStickPORT,&GPIO_InitStructure);			
}
```
















































































