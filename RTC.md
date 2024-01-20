# RTC原理與驅動程序

* 核心版背面32.768KHz的晶振為*外部震盪器*
* 內部有40KHz的*內部震盪器*
  * 會因為溫度變化飄移，導致計時精度不準確
* 如果將RTC用於系統的喚醒計時器則不需要外部震盪器，只需要使用內部的40KHz的內部震盪

- [x] STM32的RTC只使用一個32為的計數器來計時
- [x] 經過設定可以讓計數器1秒加1，從0~0xFFFFFFFF約可以計時136年
- [x] 時間起點一般為1970-01-01 00:00:00: