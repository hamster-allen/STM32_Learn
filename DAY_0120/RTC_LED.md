# RTC原理與驅動程序

* 核心版背面32.768KHz的晶振為*外部震盪器*
* 內部有40KHz的*內部震盪器*
  * 會因為溫度變化飄移，導致計時精度不準確
* 如果將RTC用於系統的喚醒計時器則不需要外部震盪器，只需要使用內部的40KHz的內部震盪

- [x] STM32的RTC只使用一個32為的計數器來計時
- [x] 經過設定可以讓計數器1秒加1，從0~0xFFFFFFFF約可以計時136年
- [x] 時間起點一般為1970-01-01 00:00:00:

# 初始化設置

`void RTC_First_Config(void);`首次啟用RTC設置或RTC備用電池斷開過導致內部數據全部丟失時<br>
`void RTC_Config(void);`如果備用電池沒有斷開只需要調用此函數<br>
`u8 Is_Leap_Year(u16 year);`判斷是否為閏年<br>    
`u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);`寫入現在時間<br>
`u8 RTC_Get_Week(u16 year,u8 month,u8 day);`計算當前的星期值














