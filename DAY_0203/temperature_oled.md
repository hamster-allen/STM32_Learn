# OLED顯示溫度

<br>

## OLED特點

1. 型號: OLED0561
2. 分辨率: 128x64
3. 顏色: 白/藍
4. 通信: I2C
5. 尺寸: 1.3英吋
6. 工作電壓: 3.3V
7. 顯示: 單色，無灰度

* 字體像素
  * 中文
    * 最小 -> 16x16
  * 英文&數字
    * 一般 -> 8x16
    * 最小 -> 8x8

<br>

## 電路原理圖

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0203/temperature_oled_picture/OLED%E9%9B%BB%E8%B7%AF%E5%8E%9F%E7%90%86%E5%9C%96.png)

<br>

## 控制OLED點亮或熄滅原理

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0203/temperature_oled_picture/%E9%BB%9E%E4%BA%AE%E5%8E%9F%E7%90%86%E7%B0%A1%E5%9C%961.png)

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0203/temperature_oled_picture/%E9%BB%9E%E4%BA%AE%E5%8E%9F%E7%90%86%E7%B0%A1%E5%9C%962.png)

## 程序分析

* 【定義】
  * `#define OLED0561_ADD 0x78`OLED的I2C地址(禁止更改)
  * `#define COM 0x00`OLED指令(禁止更改)，在對從機寫入數據前發送這個Byte表示後面都以指令執行
  * `#define DAT 0x40`OLED數據(禁止更改)，在對從機寫入數據前發送這個Byte表示後面都是數據

* 【主迴圈流程圖】

 


* 【初始化函數`OLED0561_Init`】


































