# 七段顯示器

<br>

## 開發版事前作業

* 將印有**數碼管(P9)**的**PA11、PA12**接上
  
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0123/PA11_PA12.png)


* 將CAM總線的RX、TX斷開<br>
* 因為腳位與七段顯示器復用

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0123/CAM_RX_TX.png)

* 將LM4871與GND斷開<br>
* 否則當STM與七段顯示器進行總線通信時，會發出雜音

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0123/LM4871%E6%8F%9A%E8%81%B2%E5%99%A8.png)


## TM1640

<br>

***LED驅動專用IC，內部集成有MCU數字接口、數據鎖存器、LED驅動等電路***

* 顯示(8段*16位)
* 亮度調節(占空比8級)
* 串型接口(SCLK，DIN)
* 內置RC振盪
* 內置上電復位

<br>

### 腳位圖

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0123/TM1640%E6%8E%A5%E8%85%B3%E5%9C%96.png)

<br>

|腳位|定義|說明|
|:---:|:---:|:---:|
|DIN|數據輸入|輸入顯示資料|
|SCLK|時鐘輸入|用於時間同步|
|SEG1~SEG8|輸出(段)|控制七段顯示器上的七段管|
|GRID1-GRID11<br>GRID12-GRID16|輸出(位)|控制點亮哪一組七段顯示器|
|VDD|邏輯電源|接電源正|
|VSS|邏輯地|接系統地|

<br>

### 電路範例

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0123/TM1640%E9%9B%BB%E8%B7%AF%E7%AF%84%E4%BE%8B.png)

<br>

## 程式

### 定義解釋

* 函式庫中`#define  DEL  1`用於設定通信速率，如果發現資料丟失可以試著將通訊速率降低(從1往上加)

* 地址模式設定
  * `#define TM1640MEDO_ADD  0x40`自動加一模式，每寫入一個位置將會自動跳到下一個位置
  * `#define TM1640MEDO_ADD  0x44`固定地址模式

* 顯示亮度設定

***調節整體的亮度，不可針對單獨的進行調節***

* `#define  TM1640MEDO_DISPLAY  0x8c`亮度設定為推薦值(0x88-0x8f)<br>
* `#define  TM1640MEDO_DISPLAY  0x80`關閉亮度

### 初始化函數

`GPIO_InitTypeDef  GPIO_InitStructure;`定義枚舉
`RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);`將GPIO掛載到時鐘上
`GPIO_InitStructure.GPIO_Pin = TM1640_DIN | TM1640_SCLK;`定義要設定的腳位
`GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;`定義設定腳位的模式
`GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;`設定腳位的速度為50MHz
`GPIO_Init(TM1640_GPIOPORT, &GPIO_InitStructure);`依照上面的設定進行GPIO初始化
`GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(1));`將接口輸出高電位，將IO端口電平進行初始化
`GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(1));`將接口輸出高電位，將IO端口電平進行初始化
`TM1640_start();`開始寫入資料(底層函數)
`TM1640_write(TM1640MEDO_ADD);`寫入設定地址模式(底層函數)
`TM1640_stop();`停止寫入資料(底層函數)
`TM1640_start();`開始寫入資料(底層函數)
`TM1640_write(TM1640MEDO_DISPLAY);`寫入設定顯示亮度模式(底層函數)
`TM1640_stop();`停止寫入資料(底層函數)








