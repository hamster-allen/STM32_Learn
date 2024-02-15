# CAN


## 前置工作

* 連接CAN總線跳線(P24)

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/CAN%E8%B7%B3%E7%B7%9A.png)

* 斷開七段顯示器跳線(P9)

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E4%B8%83%E6%AE%B5%E9%A1%AF%E7%A4%BA%E5%99%A8%E8%B7%B3%E7%B7%9A.png)

* 連接方法
  * 將開發板上標示L接口連接到另一個開發板的L接口
  * 標示H接口同理
    
  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E9%80%A3%E6%8E%A5%E6%96%B9%E6%B3%95.png)

## 效果呈現

* 下方開發板發送`'D'`資料，上方開發板接收到`'D'`
* 上方開發板發送`'A'`資料，下方開發板接收到`'A'`

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%95%88%E6%9E%9C%E5%91%88%E7%8F%BE.png)

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

* 如果使用傳統8051單晶片，需要外接一個CAN總線控制器和收發器
* 不過因為STM32裡已經有CAN總線控制器，因此只需要外接收發器

  ![image]()





































