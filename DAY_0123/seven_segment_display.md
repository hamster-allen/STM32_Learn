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


### 腳位圖
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0123/TM1640%E6%8E%A5%E8%85%B3%E5%9C%96.png)

|腳位|定義|說明|
|:---:|:---:|:---:|
|DIN|數據輸入|輸入顯示資料|
|SCLK|時鐘輸入|用於時間同步|
|SEG1~SEG8|輸出(段)|控制七段顯示器上的七段管|
|GRID1-GRID11<br>GRID12-GRID16|輸出(位)|控制點亮哪一組七段顯示器|
|VDD|邏輯電源|接電源正|
|VSS|邏輯地|接系統地|















