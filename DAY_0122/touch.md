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
* 
























