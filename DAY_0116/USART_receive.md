# 如何接收USART

接收方式分為*中斷*與*查詢*方式

## 查詢法

* `USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);` 在設定方面要把USART中斷改為Disable 
<br>

* `USART_GetFlagStatus(串口號, 串口的旗標)`查詢指定串口號的旗標狀態

|USART_FLAG|描述|
|---|---|
|USART_FLAG_CTS|CTS旗標|
|USART_FLAG_LBD|LIN中斷檢測旗標|
|USART_FLAG_TXE|發送數據暫存器空旗標|
|USART_FLAG_TC|發送完成旗標|
|USART_FLAG_RXNE|接收數據暫存器非空旗標|
|USART_FLAG_IDLE|空閒總線旗標|
|USART_FLAG_ORE|溢出錯誤旗標|
|USART_FLAG_NE|噪聲錯誤旗標|
|USART_FLAG_FE|幀錯誤旗標|
|USART_FLAG_PE|奇偶錯誤旗標|

* `USART_GetFlagStatus(USART1, USART_FLAG_RXNE)`取得USART1的*接收數據暫存器非空旗標*使否為1，為1表示有數據
* `u8 a = USART_ReceiveData(USART1);`接收數據，因為回傳的資料為8位元，所以用8位元的變數接收即可

> 由於接收方式需要一直循環判斷並接收，因此效率較不理想

<br>
<br>

## 中斷法

* `USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);` 在設定方面要把USART中斷改為Enable

|USART_IT|描述|
|---|---|
|USART_IT_CTS|CTS中斷旗標
|USART_IT_LBD|LIN中斷檢測中斷旗標
|USART_IT_TXE|發送中斷旗標
|USART_IT_TC|發送完成旗標
|USART_IT_RXNE|接收中斷旗標
|USART_IT_IDLE|空閒總線中斷旗標
|USART_IT_ORE|溢出錯誤中斷旗標
|USART_IT_NE|噪音錯誤中斷旗標
|USART_IT_FE|幀錯誤中斷旗標
|USART_IT_PE|奇偶錯誤中斷

* ***USART1中斷函數名稱只能為`USART1_IRQHandler`，不能更改***
<br>

* `if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)`在中斷裡依舊要判斷是否因為接收到數據而觸發中斷
* `u8 a = USART_ReceiveData(USART1);`接收數據，因為回傳的資料為8位元，所以用8位元的變數接收即可






















