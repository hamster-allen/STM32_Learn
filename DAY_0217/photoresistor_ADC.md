# ADC驅動原理與分析-光敏電阻

<br>

## 效果呈現

**當手遮住光敏電阻上的光線時，光敏電阻的阻值會變大**

  ![效果呈現](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/%E6%95%88%E6%9E%9C%E5%91%88%E7%8F%BE.png)
  
<br>

## 前置工作

* 連接ADC跳線(P8)

  ![ADC跳線](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/ADC%E8%B7%B3%E7%B7%9A.png)

<br>

## 原理

* 一般的GPIO只能讀取高電平(1)或低電平(0)，也就是邏輯電平輸入
* ADC輸入可以讀取0~3.3V之間的電壓值

  ![原理_讀取電壓](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/%E5%8E%9F%E7%90%86_%E8%AE%80%E5%8F%96%E9%9B%BB%E5%A3%93.png)
> 如果VDD是其他值的電壓，ADC讀取的最大值也會改變

<br>

## 特性

- [x] STM32F103有2個ADC
- [x] 每個ADC為12為分辨率(讀出數據的長度)，分辨率越高越精細
- [x] 2個ADC共有16個外部通道，也就是從外部可以有16個GPIO進行輸入
- [x] 2個ADC都可以使用DMA進行操作

> DMA：不需要透過軟體即可在讀取後自動存放到對應的暫存器

<br>

## ADC與DMA的關係

* ADC通道1、ADC通道2可以輸入0~3.3V的電壓值到ADC1和ADC2中
* 兩個ADC都可以透過CPU操作並加以設置，或者讀取ADC的採集電壓值
* ADC也可以透過DMA進行自動採集數據，並儲存到SRAM當中

  ![ADC與DMA的關係](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/ADC%E8%88%87DMA%E7%9A%84%E9%97%9C%E4%BF%82.png)

<br>

## 在STM32F103C8T6上的ADC腳位

* 有10個ADC通道輸入引腳
* VDDA、VSSA引腳是ADC功能的電源輸入

  ![VSSA_VDDA輸入](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/VSSA_VDDA%E8%BC%B8%E5%85%A5.png)

  ![ADC接口1](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/ADC%E6%8E%A5%E5%8F%A31.png)

  ![ADC接口2](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/ADC%E6%8E%A5%E5%8F%A32.png)

> VDDA、VSSA為獨立穩定電源輸入，可提供ADC讀取的精度和穩定性

<br>

## 電路圖

* 透過光敏電阻因偵測到不同光線所產生不同的電壓值與R8進行分壓後，經由R10連接到PA5的ADC輸入通道

  ![光敏電阻_電路圖](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/%E5%85%89%E6%95%8F%E9%9B%BB%E9%98%BB_%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

<br>

## ADC暫存器地址

[STM32F10XXX參考手冊](https://github.com/hamster-allen/STM32_Learn/blob/master/Development_Information/STM32F10XXX%E5%8F%83%E8%80%83%E6%89%8B%E5%86%8A.pdf)

* P28 -> 所有暫存器地址，由此可知ADC1的地址範圍

  ![ADC1的地址範圍](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/ADC1%E7%9A%84%E5%9C%B0%E5%9D%80%E7%AF%84%E5%9C%8D.png)
  
* P180 -> ADC的暫存器地址
* P181的`4Ch`可以看到是一個32位的暫存器，0~15位是ADC1最終轉化出來的數據，而16~31則是ADC2

  ![ADC轉換後暫存器地址](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0217/photoresistor_ADC_picture/ADC%E8%BD%89%E6%8F%9B%E5%BE%8C%E6%9A%AB%E5%AD%98%E5%99%A8%E5%9C%B0%E5%9D%80.png)

* 最後只要將起始地址加上偏移即可得出存放數據的精確位置
  * ADC -> 0x40012400 + 0x4C = 0x4001244C

<br>

## DMA設置

**此設置在`stm32f10x_dma.h`文件中**<br>

|參數|描述|設定|
|:---:|:---:|:---:|
|DMA_PeripheralBaseAddr|DMA外設地址||
|DMA_MemoryBaseAddr|DMA記憶體地址||
|DMA_DIR|數據傳輸方向|DMA_DIR_PeripheralDST -> 外社作為數據傳輸的目的地<br>DMA_DIR_PeripheralSRC -> 外社作為數據傳輸的來源|
|DMA_BufferSize|定義指定DMA通道的DMA緩衝大小，單位為數據單位||
|DMA_PeripheralInc|外設地址暫存器是否遞增|DMA_PeripheralInc_Enable -> 遞增<br>DMA_PeripheralInc_Disable -> 不變|
|DMA_MemoryInc|記憶體暫存器是否遞增|DMA_PeripheralInc_Enable -> 遞增<br>DMA_PeripheralInc_Disable -> 不變|
|DMA_PeripheralDataSize|設定外設數據寬度|DMA_PeripheralDataSize_Byte -> 8位<br>DMA_PeripheralDataSize_HalfWord -> 16位<br>DMA_PeripheralDataSize_Word -> 32位|
|DMA_MemoryDataSize|設定記憶體數據寬度|DMA_PeripheralDataSize_Byte -> 8位<br>DMA_PeripheralDataSize_HalfWord -> 16位<br>DMA_PeripheralDataSize_Word -> 32位|
|DMA_Mode|DMA的工作模式<br>注意：當DMA通道數據傳輸設置為記憶體到記憶體時，不能使用循環緩衝模式|DMA_Mode_Circular -> 循環緩衝模式<br>DMA_Mode_Normal -> 正常緩衝模式|
|DMA_Priority|DMA通道的優先級|DMA_Priority_VeryHigh -> 最高優先級<br>DMA_Priority_High  -> 高優先級<br>DMA_Priority_Medium -> 中優先級<br>DMA_Priority_Low  -> 低優先級|
|DMA_M2M|是否啟用記憶體到記憶體傳輸|DMA_M2M_Enable -> 啟用<br>DMA_M2M_Disable -> 關閉|

> DMA與ADC之設定等資料 -> [STM32F103固件函數庫用戶手冊](https://github.com/hamster-allen/STM32_Learn/blob/master/Development_Information/STM32F103%E5%9B%BA%E4%BB%B6%E5%87%BD%E6%95%B8%E5%BA%AB%E7%94%A8%E6%88%B7%E6%89%8B%E5%86%8A.pdf)

<br>

## 程序

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
  * `extern vu16 ADC_DMA_IN5;`用於儲存ADC最終轉化的數據

#### 【DMA初始化】
```
void ADC_DMA_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;                                           //聲明DMA初始化結構體
  DMA_DeInit(DMA1_Channel1);                                                   //使DAM1通道復位，表示即將使用DAM通道傳送數據
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;                  //設置DMA通道外設地址=ADC1_DR_Address，也就是在告訴DMA要從哪裡讀取數據
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_DMA_IN5;                    //設置DAM將讀出的數據存放到指定的變量(可直接讀此變量即是ADC值)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //指定外設的源地址
  DMA_InitStructure.DMA_BufferSize = 1;                                        //緩衝區大小，目前只有光敏電阻，因此設置1即可(根據ADC採集通道数量修改)
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //當前外設暫存器地址不變：Disable不變，Enable遞增(用於多通道採集)
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;                     //當前暫存器地址：Disable不變，Enable遞增(用於多通道採集)，目前只採集一個數據，因此不需要偏移
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
  GPIO_InitStructure.GPIO_Pin = ADC_CH5;                                                           //選擇ADC腳位                     
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
  ADC_InitStructure.ADC_NbrOfChannel = 1;                                      //ADC的通道數
  ADC_Init(ADC1, &ADC_InitStructure);                                          //將上述的設置進行初始化

  //設置指定ADC的規則組通道，設置他的轉化順序與採樣時間
  //ADC1,ADC通道x,規則採樣顺序值為y,採樣時間為28周期		 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_28Cycles5);
  
  ADC_DMACmd(ADC1, ENABLE);                                                    //啟用ADC的DMA支持，也就是允許DMA讀取ADC數據
  ADC_Cmd(ADC1, ENABLE);                                                       //開啟ADC1
  ADC_ResetCalibration(ADC1);                                                  //重置指定ADC的校準暫存器
  while(ADC_GetResetCalibrationStatus(ADC1));                                  //獲取ADC重置校準暫存器的狀態，並等待完成
  ADC_StartCalibration(ADC1);                                                  //開始指定ADC校準狀態
  while(ADC_GetCalibrationStatus(ADC1));                                       //獲取指定ADC的校準程序，並等待完成
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                      //開啟ADC軟體開始轉化，執行後，ADC1將會開始把模擬量轉化成數據，再通過DMA功能存放到指定的變量中
}
```

















