# 觸摸按鍵

## 初始化
* 初始化設定與微動開關的初始化設定一樣<br>

## 讀取處理程序
* 判斷觸摸按鍵的方式與微動開關的方式相同<br>
* 透過`GPIO_ReadInputDataBit(TOUCH_KEYPORT, TOUCH_KEY_A);`判斷按鍵是否被觸摸，低電平觸發<br>
































