# RCC

RCC主要涉及復位以及系統時間的分配



# 時鐘樹

### 時鐘源1(外部)
* *OSC_OUT* 和 *OSC_IN* 為外部高速晶振
### 時鐘源2(內部)
* 左上角的 *8MHz HSI RC*

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0121/%E6%99%82%E9%90%98%E6%A8%B9.png)

<br>

# 各種方式產生主頻

1. 直接連接到主頻，只要選擇HSE將會以外部高速晶振輸出至主頻
2. 直接連接到主頻，只要選擇HSI將把8MHz的內部振盪器輸出至主頻
3. 將外部的高速時鐘頻率透過兩個選擇器輸送到**倍頻器**(輸入前也可以先將頻率除於2)，控制倍頻器的係數調整再將頻率輸出至主頻(PLLCLK)
4. 與第三種方法相同，只是時鐘源使用內部震盪器，並且會在進入倍頻器前除於2，因此8MHz會以4MHz進入倍頻器

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0121/%E5%90%84%E8%BC%B8%E5%87%BA%E4%B8%BB%E9%A0%BB%E6%96%B9%E5%BC%8F.png)


# 系統時間初始化

`ErrorStatus HSEStartUpStatus;`定義晶振狀態枚舉<br>
`RCC_DeInit();`將RCC相關的暫存器全部設為初始值<br>
`RCC_HSEConfig(RCC_HSE_ON);`開啟使用外部高速晶振<br>
`HSEStartUpStatus = RCC_WaitForHSEStartUp();`等待外部高速晶振啟動<br>
`if(HSEStartUpStatus == SUCCESS)`判斷是否成功啟動外部高速晶振<br>

***如果成功啟動外部高速晶振，執行下列程序***<br>

> 時鐘源與倍頻器的設置
 
`RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);`設置PLL時鐘源與倍頻係數

    RCC_PLLMul_x：倍頻係數(x可為2~16)
    RCC_PLLSource_HSE_Div1：外部高速晶振，進入倍頻器前沒有除於2
    RCC_PLLSource_HSE_Div2：外部高速晶振，進入倍頻器前有除於2
    RCC_PLLSource_HSI_Div2：內部晶振，進入倍頻器前有除於2(只有這選項)
    
> AHB與APB的分頻器設置
  
`RCC_HCLKConfig(RCC_SYSCLK_Div1);`設置AHB分頻器

    RCC_SYSCLK_Divx：AHB分頻器係數(1、2、4、8、16、64、128、256、512)

`RCC_PCLK1Config(RCC_HCLK_Div2);`設置低速AHB時鐘(PCLK1)

`RCC_PCLK2Config(RCC_HCLK_Div1);`設置高速AHB時鐘(PCLK2)

    低速與高速AHB時鐘分頻為(1、2、4、8、16)
    分頻頻率設置根據總線上的相關功能需求的最大頻率，再加上用戶的綜合考慮而設置的

> 設置Flash

`FLASH_SetLatency(FLASH_Latency_2);`設置Flash延時時鐘週期

    根據不同的系統主頻調整不同的時鐘週期參數
    0~24MHz，FLASH_Latency_0
    24~48MHz，FLASH_Latency_1
    48~72MHz，FLASH_Latency_2

`FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);`選擇Flash預取緩存模式

    事先把要用到的資料先讀出來存在SRAM，否則只會在需要用的那一瞬間再去將資料從Flash讀出來

`RCC_RLLCmd(ENABLE);`啟動PLL

`while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);`等待，直到PLL穩定工作

`RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);`選擇SYSCLK為時鐘源

    HSI -> RCC_SYSCLKSource_HSI
    HSE -> RCC_SYSCLKSource_HSE
    PLLCLK -> RCC_SYSCLKSource_PLLCLK

`while(RCC_GetSYSCLKSource() != 0x08);`等待時鐘源切換為PLL

    RCC_GetSYSCLKSource();取得現在時鐘源的模式
    0x00 -> HSI
    0x04 -> HSE
    0x08 -> PLL





