# ADC驅動原理與分析-光敏電阻&可變電阻

***DAY_0218的延伸程序***

## 效果呈現

* 讀取光敏電阻的數值並顯示在OLED上
* 調整可變電阻時，OLED上的數值會跟著改變

  ![效果呈現]()


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

***程序中與之前不同的地方會特別標註並加以說明***

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
  * `|-------------- extern vu16 ADC_DMA_IN[2]; --------------|`用於儲存ADC最終轉化的數據
    > 因為現在要讀取的數據有兩個

#### 【DMA初始化】
```
void ADC_DMA_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;                                           //聲明DMA初始化結構體
  DMA_DeInit(DMA1_Channel1);                                                   //使DAM1通道復位，表示即將使用DAM通道傳送數據
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;                  //設置DMA通道外設地址=ADC1_DR_Address，也就是在告訴DMA要從哪裡讀取數據

  |--------------------------------------------------------------------------------------------------------------------------------------|
  |  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_DMA_IN;                  //設置DAM將讀出的數據存放到指定的變量(可直接讀此變量即是ADC值) |
  |                                                                            * 更改變量名稱                                             |
  |--------------------------------------------------------------------------------------------------------------------------------------|

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //指定外設的源地址

  |---------------------------------------------------------------------------------------------------------------------------------------------|
  |  DMA_InitStructure.DMA_BufferSize = 2;                                     //緩衝區大小，目前只有光敏電阻，因此設置1即可(根據ADC採集通道数量修改) |
  |                                                                            * 改緩衝區大小為2，因為要讀兩筆數據                                 |
  |---------------------------------------------------------------------------------------------------------------------------------------------|

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //當前外設暫存器地址不變：Disable不變，Enable遞增(用於多通道採集)

  |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
  |  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  //當前暫存器地址：Disable不變，Enable遞增(用於多通道採集)，目前只採集一個數據，因此不需要偏移                                     |
  |                                                                            * 將Disable改為Enable，因為現在要讀的不只一個數據，要讓儲存數據的數組自動偏移，否則光敏電阻&可變電阻的數據都會被儲存到ADC_DMA_IN[0] |
  |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

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
  |  GPIO_InitStructure.GPIO_Pin = ADC_CH4 | ADC_CH5;                                                        //選擇ADC腳位                    |
  |                                                                                                * 增加通道4，也就是可變電阻的通道  |
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

  |----------------------------------------------------------------------------------------------------------------|
  |  ADC_InitStructure.ADC_NbrOfChannel = 2;                                   //ADC的通道數                        |
  |                                                                            * 通道數改為2，因為另一組要給可變電阻  |
  |----------------------------------------------------------------------------------------------------------------|

  ADC_Init(ADC1, &ADC_InitStructure);                                          //將上述的設置進行初始化

  |----------------------------------------------------------------------------------------------------------------|
  |  //設置指定ADC的規則組通道，設置他的轉化順序與採樣時間                                                             |
  |  //ADC1,ADC通道x,規則採樣顺序值為y,採樣時間為28周期                                                               |
  |  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5);          * 新增通道4的規則組       |
  |  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_28Cycles5);          * 更改通道5的順序為2      |
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












































































