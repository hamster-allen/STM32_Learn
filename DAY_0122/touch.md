# 觸摸按鍵

## 初始化
* 初始化設定與微動開關的初始化設定一樣<br>

## 讀取處理程序
* 判斷觸摸按鍵的方式與微動開關的方式相同<br>
* 透過`GPIO_ReadInputDataBit(TOUCH_KEYPORT, TOUCH_KEY_A);`判斷按鍵是否被觸摸，低電平觸發<br>

<br>

## 觸摸按鍵電路圖
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0122/%E8%A7%B8%E6%91%B8%E6%8C%89%E9%8D%B5%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

## TTP223-BA6

* 為觸摸按鍵檢測IC，低功號、寬工作電壓<br>
* 工作電壓 2V~5.5V<br>
* 工作電流 VDD=3V
  * 低功耗模式下 經典值1.5uA，最大值3.0uA
  * 快速模式下 典型值3.5uA，最大值7.0uA


![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0122/%E8%A7%B8%E6%91%B8%E6%8C%89%E9%8D%B5%E6%AA%A2%E6%B8%ACIC.png)

|腳位號|腳位名|腳位定義|
|:---:|:---:|:---:|
|1|Q|CMOS輸出腳位|
|2|VSS|接地腳(負電源電壓)|
|3|I|傳感輸入(觸摸腳位)|
|4|AHLB|按鍵觸發時輸出選擇<br>1(默認)=>低電平有效；0=>高電平有效|
|5|VDD|正電源電壓|
|6|TOG|輸出類型選擇腳位<br>1(默認)=>觸發模式；0=>直接模式|

> AHLB<br>
> 接在高電平，低電平表示觸發<br>
> 接在低電平，高電平表示觸發<br>

> TOG<br>
> 觸發模式(有鎖存)：被按下放開後保持狀態<br>
> 直接模式(無鎖存)：被按下放開後不保持狀態<br>

















