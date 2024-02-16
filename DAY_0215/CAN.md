# CAN

<br>

## 前置工作

* 連接CAN總線跳線(P24)

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/CAN%E8%B7%B3%E7%B7%9A.png)

* 斷開七段顯示器跳線(P9)

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E4%B8%83%E6%AE%B5%E9%A1%AF%E7%A4%BA%E5%99%A8%E8%B7%B3%E7%B7%9A.png)

* 連接方法
  * 將開發板上標示L接口連接到另一個開發板的L接口
  * 標示H接口同理
    
  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E9%80%A3%E6%8E%A5%E6%96%B9%E6%B3%95.png)

<br>

## 效果呈現

* 下方開發板發送`'D'`資料，上方開發板接收到`'D'`
* 上方開發板發送`'A'`資料，下方開發板接收到`'A'`

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%95%88%E6%9E%9C%E5%91%88%E7%8F%BE.png)

<br>

## 原理與說明

* STM32上連接CAN總線的接腳
* PA12 -> CAN_TX
* PA11 -> CAN_RX

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/STM32_CAN%E8%85%B3%E4%BD%8D%E5%AE%9A%E7%BE%A9.png)
  
  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/CAN%E6%8E%A5%E8%85%B3%E5%AE%9A%E7%BE%A9.png)

* CAN接腳透過跳線(P24)連接到CAN總線收發器(TJA1050)
* TJA1050工作在5V
* R33穩定電平

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

* 如果使用傳統8051單晶片，需要外接一個CAN總線控制器和轉發器
* 不過因為STM32裡已經有CAN總線控制器，因此只需要外接轉發器

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/TJA1050%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

> 詳情參閱[TJA1050數據手冊(P9)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN%E7%9B%B8%E9%97%9C%E8%B3%87%E6%96%99/TJA1050%E6%95%B0%E6%8D%AE%E6%89%8B%E5%86%8C%EF%BC%88%E8%8B%B1%E6%96%87%EF%BC%89%20.pdf)

<br>

## CAN特點與相關概念

* 兩個設備: 當成USART使用
* 多個設備: 使用CAN協議(使用郵箱、識別符、過濾器功能)

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E5%85%A9%E5%80%8B_%E5%A4%9A%E5%80%8B%E8%A8%AD%E5%82%99.png)

* 以`ISO 11898(High speed)`為例，電位差為0時，表示數據為1
* 電位差為2.0V時，表示數據為0

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/CAN%E7%89%A9%E7%90%86%E5%B1%A4.png)

> 顯性電平(0V)具有優先權，只要有設備輸出顯性電平，總線上即為顯性電平<br>
> 只有所有的設備都輸出隱性電平，總線才為隱性電平<br>

<br>

## CAN發送
  
  |||
  |:---:|:---:|
  |報文|一次發送的完整數據訊息|
  |郵箱|發送報文的發送調度器|
  |幀種類|報文種類:<br>數據幀、遙控幀、錯誤幀、過載幀、幀間隔|
  |幀格式|一個報文理包含的內容|
  |標示符|總線上的設備透過此數據是不是給自己的|

#### 【CAN總線發送郵箱】

* 將數據放到優先級最高的空郵箱，再發送到其他CAN設備
* 如果最高的郵箱已經被放了數據，將會往下放到第二個郵箱
* 郵箱就像緩衝區，只要將數據放到郵箱中就會依序傳送出去，不需要再管理

  ![image]()
  
  ![image]()

#### 【多個設備連接傳輸原理】

* 所有設備都會接收報文(數據)，但標示符不符的會被過濾器刪除
* 標示符屬於報文中的其中一個數據

  ![image]()

#### 【報文種類】

* 最常用到為**數據幀**、**遙控幀**

|幀|幀用途|
|:---:|:---:|
|數據幀|發送向接收傳送數據的幀|
|遙控幀(遠程幀)|接收向發送請求數據的幀|
|錯誤幀|檢測出錯誤時，向其他單元通知錯誤的幀|
|過載幀|接收單元通知其未做好接收準備的幀|
|幀間隔|將數據幀及遙控幀與前面的幀分開的幀|

#### 【數據幀與遙控幀的報文格式】

* 都具有標準模式和擴展格式兩種格式
* 兩種格式就像I2C協議裡的7位和10位地址

  ![image]()

#### 【標準格式】

* 每個長條方塊表示一個bit
* Data字節數不固定，最多64bits
* 仲裁段也就是標示符

  ![image]()

#### 【標準格式與擴展格式的比較】

* 識別符
  * 標準格式只有`ID28`到`ID18`
  * 擴展格式只多了`Identifier(extension)`，也就是`ID17`到`ID0`

  ![image]()

* 完整的比較圖

  ![image]()




















<br>

## 程序

* 定義
  * `#define CAN_INT_ENABLE	0`1為啟用CAN中斷，0為關閉CAN中斷
  * `#define tsjw CAN_SJW_1tq`設置選項(1-4)
  * `#define tbs1 CAN_BS1_8tq`設置選項(1-16)
  * `#define tbs2 CAN_BS2_7tq`設置選項(1-8)
  * `#define brp 9`設置選項
  > 波特率=(pclk1/((1+8+7)*9))=36MHz/16/9=250Kbits<br>
  > pclk1為STM32時鐘設置部分<br>
  
#### 【初始化】


#### 【發送數據】


#### 【接收數據】




























