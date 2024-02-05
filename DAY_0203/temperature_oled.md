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

<br>

## 程序分析

* 【定義】
  * `#define OLED0561_ADD 0x78`OLED的I2C地址(禁止更改)
  * `#define COM 0x00`OLED指令(禁止更改)，在對從機寫入數據前發送這個Byte表示後面都以指令執行
  * `#define DAT 0x40`OLED數據(禁止更改)，在對從機寫入數據前發送這個Byte表示後面都是數據

<br>

* 【主迴圈流程圖】

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0203/temperature_oled_picture/%E4%B8%BB%E8%BF%B4%E5%9C%88%E6%B5%81%E7%A8%8B%E5%9C%96.png)

<br>

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
for(t=0xB0;t<0xB8;t++){	                   //設置起始Page為0xB0
  I2C_SAND_BYTE(OLED0561_ADD,COM,t); 	     //設定頁地址(從0xB0到0xB7)
  I2C_SAND_BYTE(OLED0561_ADD,COM,0x10);    //起始列地址的高4位
  I2C_SAND_BYTE(OLED0561_ADD,COM,0x00);   	//起始列地址的低4位
  for(j=0;j<132;j++){	                     //列開始填充內容
  I2C_SAND_BYTE(OLED0561_ADD,DAT,0x00);
}
```
> 起始列地址高位低位查看下方說明<br>

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

> 初始化設置可以參閱[OLED屏SPEC數據手冊(P.21、22)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0203/OLED%E7%9B%B8%E9%97%9C%E6%AA%94%E6%A1%88/OLED%E5%B1%8FSPEC%20QG-2864KSWLG01%20VER%20A%E6%95%B0%E6%8D%AE%E6%89%8B%E5%86%8C%EF%BC%88%E8%8B%B1%E6%96%87%EF%BC%89.pdf)

<br>

* 【在指定X、Y座標顯示文字`OLED_DISPLAY_8x16(u8 x, u8 y, u8 w)`】

```
u8 j,t,c=0;
y=y+2;                                                          //因OLED的內置驅動晶片是從0x02列為最左一列，所以要加上偏移量
for(t=0;t<2;t++){
  I2C_SAND_BYTE(OLED0561_ADD,COM,0xb0+x);                       //頁地址(0xB0到0xB7)
  I2C_SAND_BYTE(OLED0561_ADD,COM,y/16+0x10);                    //起始列地址的高4位
  I2C_SAND_BYTE(OLED0561_ADD,COM,y%16);	                        //起始列地址的低4位
  for(j=0;j<8;j++){                                             //整個螢幕内容填充迴圈
    I2C_SAND_BYTE(OLED0561_ADD,DAT,ASCII_8x16[(w*16)+c-512]);   //為了和ASII表對應要减512
    c++;
  }
  x++; //頁地址加1
}
```
> ASCII的算法查看下方說明<br>
> 起始列地址高位低位查看下方說明<br>
> 【OLED取模方式】與【寫入方式】查看下方說明<br>

* 【在指定X軸顯示整行文字`OLED_DISPLAY_8x16_BUFFER(u8 row, u8 *str)`】

```
u8 r=0;                               //將起始列座標設為0
while(*str != '\0'){                  //直到字串結束符前都持續執行迴圈
  OLED_DISPLAY_8x16(row,r*8,*str++);  //繪出文字
  r++;                                //每繪出一個文字就換下一列
}	
```

<br>

## ASCII字模數據表顯示換算
```
//程序裡的換算
ASCII_8x16[(w*16)+c-512]
```
假設輸入為`'C'`，在ASCII中十進制為`67`<br>
在字模中`'C'`的起始資料在第35個，並且每個字符的顯示資料都為16bits，所以也就是`35*16=560`<br>
兩者分別為`67*16=1072`和`35*16=560`，所以要將`1072-512=560`<br>

<br>

## 起始列地址高位低位

`D4`表示高位或低位

* 假設要在`120列`開始顯示，會先`+2`(因起始列為2，加入偏移量)<br>
1. 取出商和餘數`122/16=7...10`，`商=7` `餘數=10`<br>
> 由此可知`122`的十六進制為`0x7A`<br>
2. 將`0x07+0x10`和`0x0A`寫入高位`D0-D4`<br>
> 因為`0x07`為高位，所以要加上`0x10`<br>

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0203/temperature_oled_picture/OLED%E8%B5%B7%E5%A7%8B%E5%88%97%E5%9C%B0%E5%9D%80%E6%9A%AB%E5%AD%98%E5%99%A8.png)

<br>

## OLED取模方式 與 寫入方式

* 取模寫入(方式)順序

  ***從左到右，從上到下***

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0203/temperature_oled_picture/OLED%E5%AD%97%E6%A8%A1%E5%AF%AB%E5%85%A5%E6%96%B9%E5%BC%8F.png)






