# 透過觸摸按鍵控制繼電器

## 事前工作

* 短接繼電器的跳線


![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/%E7%B9%BC%E9%9B%BB%E5%99%A8%E8%B7%B3%E7%B7%9A.png)

* 短接觸摸按鍵的跳線
  
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/%E8%A7%B8%E6%91%B8%E6%8C%89%E9%8D%B5%E8%B7%B3%E7%B7%9A.png)

## 原理

### 【控制連接圖示】

* 透過STM32輸出控制繼電器的電磁鐵加以控制是否導通

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0206/relay_button_picture/%E6%8E%A7%E5%88%B6%E9%80%A3%E6%8E%A5%E5%9C%96%E7%A4%BA.png)

### 【JTAG電路圖與注意事項】

***在JTAG中也使用到控制繼電器的腳位(PA13、PA14)，並且在上電時為JTAG工作模式，如果要當作IO口使用必須透過軟體設置***

![image]()

### 【繼電器與步進馬達電路圖】

![image]()





























