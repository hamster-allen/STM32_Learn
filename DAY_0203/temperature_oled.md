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

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0203/temperature_oled_picture/%E4%B8%BB%E8%BF%B4%E5%9C%88%E6%B5%81%E7%A8%8B%E5%9C%96.png)


* 【初始化函數`OLED0561_Init`】

1. 關閉OLED顯示函數，避免出現亂碼

```
u8 buf[3]={
    0xae,           //0xae:關顯示，0xaf:開顯示
    0x8d,0x10,      //VCC電源
};
I2C_SAND_BUFFER(OLED0561_ADD,COM,buf,3);
```

2. 清除螢幕內容

```
u8 j,t;
  for(t=0xB0;t<0xB8;t++){	                 //设置起始页地址为0xB0
  I2C_SAND_BYTE(OLED0561_ADD,COM,t); 	     //页地址（从0xB0到0xB7）
  I2C_SAND_BYTE(OLED0561_ADD,COM,0x10);    //起始列地址的高4位
  I2C_SAND_BYTE(OLED0561_ADD,COM,0x00);   	//起始列地址的低4位
  for(j=0;j<132;j++){	                     //整页内容填充
    I2C_SAND_BYTE(OLED0561_ADD,DAT,0x00);
  }
}
```

3. 初始化OLED並開啟顯示

```
u8 buf[28]={
    0xae,           //0xae:關顯示，0xaf:開顯示
    0x00,0x10,      //開始地址(雙字節) 
    0xd5,0x80,      //設定顯示時鐘分頻比/振盪器頻率
    0xa8,0x3f,      //設定複用比
    0xd3,0x00,      //設定偏移
    0XB0,           //寫入頁地址(0xB0~7)
    0x40,           //設定起始行
    0x8d,0x14,      //VCC電源
    0xa1,           //設置段重映射
    0xc8,           //設定列輸出掃描方向
    0xda,0x12,      //設定列引腳硬件配置
    0x81,0xff,      //對比度，指令:0x81，数据:0~255（255最高）
    0xd9,0xf1,      //設定預充電週期
    0xdb,0x30,      //調整VCOMH輸出
    0x20,0x00,      //水平尋址設置
    0xa4,           //0xa4:正常顯示，0xa5:整體點亮
    0xa6,           //0xa6:正常顯示，0xa7:反色顯示
    0xaf            //0xae:關顯示，0xaf:開顯示
};
I2C_SAND_BUFFER(OLED0561_ADD,COM,buf,28);  //將上述的設定寫入暫存器
```






























