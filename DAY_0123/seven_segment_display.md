# 七段顯示器


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

***LED驅動專用IC，內部集成有MCU數字接口、數據鎖存器、LED驅動等電路***

* 顯示(8段*16位)
* 亮度調節(占空比8級)
* 串型接口(SCLK，DIN)
* 內置RC振盪
* 內置上電復位




















