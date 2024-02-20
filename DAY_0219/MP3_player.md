# MP3撥放原理與驅動


<br>

## 效果呈現

* A、B觸摸按鍵切換音檔
* 編碼器旋轉控制音量大小
* 編碼器微動開關控制是否暫停

<br>

## 前置工作

* 連接編碼器的跳線(P18)

  ![編碼器跳線](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3_player_picture/%E7%B7%A8%E7%A2%BC%E5%99%A8%E8%B7%B3%E7%B7%9A.png)

* 斷開搖桿的跳線(P17)

  ![搖桿跳線](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3_player_picture/%E6%90%96%E6%A1%BF%E8%B7%B3%E7%B7%9A.png)

* 連接開發板右上角跳線，全部依照下面圖示

  ![開發板右上角跳線](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3_player_picture/%E9%96%8B%E7%99%BC%E6%9D%BF%E5%8F%B3%E4%B8%8A%E8%B7%B3%E7%B7%9A.png)

* 插入TF卡時，將TF卡格式化(FAT or FAT32)

  ![TF卡格式化_FAT](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3_player_picture/TF%E5%8D%A1%E6%A0%BC%E5%BC%8F%E5%8C%96_FAT.png)

* 音檔名稱開頭必須加入續號，後面可以加入其他名稱

  ![音檔名稱](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3_player_picture/%E9%9F%B3%E6%AA%94%E5%90%8D%E7%A8%B1.png)

<br>

## 電路原理

* 腳位與電路圖名稱定義
* PB10、PB11對應的腳位就是STM32上的USAER3，直接透過USART3向MP3撥放模組發送資料或接收資料

  ![電路圖1](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3_player_picture/%E9%9B%BB%E8%B7%AF%E5%9C%961.png)

* 讓MY1690讀取到SD卡的資料

  ![電路圖2](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3_player_picture/%E9%9B%BB%E8%B7%AF%E5%9C%962.jpg)

* 透過USB接口更換SD卡內的文件

  ![電路圖3](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3_player_picture/%E9%9B%BB%E8%B7%AF%E5%9C%963.jpg)

> 了解MY1690腳位定義參閱[MY1690語音晶片使用說明(P3)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3%E6%92%A5%E6%94%BE_%E7%9B%B8%E9%97%9C%E8%B3%87%E6%96%99/MY1690-16S%E8%AF%AD%E9%9F%B3%E6%99%B6%E7%89%87%E4%BD%BF%E7%94%A8%E8%AA%AA%E6%98%8E%E6%9B%B8.pdf)<br>
> MY1690串口通訊協議相關資料參閱[MY1690語音晶片使用說明(P4)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3%E6%92%A5%E6%94%BE_%E7%9B%B8%E9%97%9C%E8%B3%87%E6%96%99/MY1690-16S%E8%AF%AD%E9%9F%B3%E6%99%B6%E7%89%87%E4%BD%BF%E7%94%A8%E8%AA%AA%E6%98%8E%E6%9B%B8.pdf)<br>

<br>

## 主程序

```
int main (void)
{
  u8 b;
  u8 MP3=0;
  delay_ms(500);                                       //等待其他元件進入穩定狀態

  RCC_Configuration();                                 //系統時鐘初始化
  TOUCH_KEY_Init();                                    //觸摸開關初始化
  RELAY_Init();                                        //繼電器初始化
  ENCODER_Init();                                      //編碼器初始化
  MY1690_Init();                                       //MP3晶片初始化(MY1690)
  I2C_Configuration();                                 //I2C初始化
  OLED0561_Init();                                     //OLED初始化

  OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    ");      //顯示字串
  OLED_DISPLAY_8x16_BUFFER(3," MP3 PLAY TEST  ");      //顯示字串

  while(1)
  {
    //======觸摸按鍵======

    //判斷觸摸開關是否有任何一個被按下
    if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0 || GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 || GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 || GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0)
    {
      delay_ms(20);
      if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0)    // A被按下
      {	
        MY1690_PREV();                                           // 上一曲
        OLED_DISPLAY_8x16_BUFFER(6,"  -- PREV --    ");
        delay_ms(500);
        OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    ");
      }
      if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0)    // B被按下
      {
        MY1690_NEXT();                                           // 下一曲
        OLED_DISPLAY_8x16_BUFFER(6,"  -- NEXT --    ");
        delay_ms(500);
        OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    ");
      }
      if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0)    // C被按下
      {
        MY1690_CMD2(0x31,30);                                    // 將音量設置為30(最大)
        delay_ms(500);
      }
      if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0)    // D被按下
      {
        MY1690_CMD3(0x41,0x04);                                  // 直接播放第0004曲
        delay_ms(500);
      }

      //等待所有觸摸按鍵都被放開
      while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0 || GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 || GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 || GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0);
    }


    //======編碼器======

    b=ENCODER_READ();                                            //讀出編碼器動作狀態	
    if(b==1){MY1690_VUP();}                                      //順時針音量加
    if(b==2){MY1690_VDOWN();}                                    //逆時針音量減
    if(b==3)                                                     //按下播放或暂停
    {	
      if(MP3==0)                                                 //判斷現在是播放還是暂停
      {	
        MP3=1;                                                   //設定MP3狀態旗標(播放狀態)
	MY1690_PLAY();                                           //撥放音樂
        OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    ");
      }
      else if(MP3==1)
      {
        MP3=0;                                                   //設定MP3狀態旗標(暫停狀態)
	MY1690_PAUSE();                                          //暫停音樂
        OLED_DISPLAY_8x16_BUFFER(6,"  -- PAUSE --   ");
      }
      delay_ms(500);
    }


    //======MY1690 USART3串口接收處理======
    /*USART會透過中斷接收來自MY1690的資料*/

    if(USART3_RX_STA==1)                                         //如果旗標是1 表示收到STOP
    {
      MP3=0;                                                     //設定MP3狀態旗標(暫停狀態)
      OLED_DISPLAY_8x16_BUFFER(6,"  -- STOP --    ");
      USART3_RX_STA=0;                                           //將串口數據旗標清0
    }
    else if(USART3_RX_STA==2)                                    //如果旗標是2 表示收到OK
    {
      USART3_RX_STA=0;                                           //將串口數據旗標清0
    }
  }
}
```

## MY1690程序

* 控制命令參閱[MY1690語音晶片使用說明(P6)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0219/MP3%E6%92%A5%E6%94%BE_%E7%9B%B8%E9%97%9C%E8%B3%87%E6%96%99/MY1690-16S%E8%AF%AD%E9%9F%B3%E6%99%B6%E7%89%87%E4%BD%BF%E7%94%A8%E8%AA%AA%E6%98%8E%E6%9B%B8.pdf)

> `USART3_printf("\x7e\x03\x11\x12\xef");`中，`\x`表示傳送十六進制<br>
> 例如`USART3_printf("\x7e");`：發送`0x7e`

#### 【初始化】
```
void MY1690_Init(void)
{
  USART3_Init(9600);                     //初始化與MY1690溝通的USART串口
  MY1690_STOP();                         //發送停止撥放的指令(為了啟動MY1690)
}
```
#### 【播放音樂】
```
void MY1690_PLAY(void)
{
  USART3_printf("\x7e\x03\x11\x12\xef");
}
```
#### 【暫停音樂】
```
void MY1690_PAUSE(void)
{
  USART3_printf("\x7e\x03\x12\x11\xef");
}
```
#### 【上一首】
```
void MY1690_PREV(void)
{
  USART3_printf("\x7e\x03\x14\x17\xef");
}
```
#### 【下一首】
```
void MY1690_NEXT(void)
{
  USART3_printf("\x7e\x03\x13\x10\xef");
}
```
#### 【音量+1】
```
void MY1690_VUP(void)
{
  USART3_printf("\x7e\x03\x15\x16\xef");
}
```
#### 【音量-1】
```
void MY1690_VDOWN(void)
{
  USART3_printf("\x7e\x03\x16\x15\xef");
}
```
#### 【停止播放】
```
void MY1690_STOP(void)
{
  USART3_printf("\x7e\x03\x1E\x1D\xef");
}
```
#### 【發送無參數指令】
```
void MY1690_CMD1(u8 a)
{
  u8 i;
  i=3^a;                                                      //長度或上指令 -> 取得校驗碼

  USART_SendData(USART3 , 0x7e);                              //發送起始位
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , 0x03);                              //發送長度
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , a);                                 //發送指令
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , i);                                 //發送校驗碼
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , 0xef);                              //發送結束位
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標
}
```
#### 【發送有參數指令-8位】

* a -> 指令
* b -> 指令參數(8位)

```
void MY1690_CMD2(u8 a,u8 b)
{
  u8 i;
  i=4^a;                                                      //長度或上指令
  i=i^b;                                                      //上述結果再與指令的參數或上

  USART_SendData(USART3 , 0x7e);                              //發送起始位
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , 0x04);                              //發送長度
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , a);                                 //發送指令
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , b);                                 //發送參數
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , i);                                 //發送校驗碼
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , 0xef);                              //發送結束碼
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標
}
```
#### 【發送有參數指令-16位】

* a -> 指令
* b -> 指令參數(16位)

```
void MY1690_CMD3(u8 a,u16 b)
{
  u8 i,c,d;
  c=b/0x100;                                                  //將16位的指令參數取出高8位和低8位
  d=b%0x100;
  i=5^a; 
  i=i^c; 
  i=i^d;                                                      //長度、命令、參數高8位和參數低8位，將上述所有相互或上

  USART_SendData(USART3 , 0x7e);                              //發送起始位
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , 0x05);                              //發送長度
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , a);                                 //發送指令
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , c);                                 //發送參數高8位
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , d);                                 //發送參數低8位
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , i);                                 //發送校驗碼
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標

  USART_SendData(USART3 , 0xef);                              //發送結束碼
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);   //檢查發送中斷旗標
}
```







































