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




























