# 如何實現在任何一點X上顯示想要的文字(8x16)


## 效果呈現之圖示

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0205/OLED_Function_picture/%E9%A1%AF%E7%A4%BA%E7%AF%84%E4%BE%8B%E5%9C%96.jpg)

## 顯示函數
```
void OLED_Write_8x16(u8 x, u8 y, u16 data)
{
  u8 j;                               //for迴圈使用的變數
  u8 c=0;                             //讀取ASCII數據的偏移量
  u8 display_data;                    //經過處理的最終顯示資料
  u8 page_num1[8];                    //儲存顯示文字上8 Bytes的資料
  u8 page_num2[8];                    //儲存顯示文字下8 Bytes的資料
  u16 CombineByte;                    //16bits，儲存上8 Bytes和下8 Bytes的結合資料
  u8 AndByte;                         //AND時使用的資料，取出1Byte想要的bits
  u16 OrByte1_u16;                    //16bits，取出整合後已處理過的最上方bits
  u16 OrByte2_u16;                    //16bits，取出整合後已處理過的最下方bits
  u16 OrByte;                         //上面兩個合起來後中間未處理的資料
 	
  u8 x_intger = x / 8;                //最上方在第幾頁
  u8 x_remainder = x % 8;             //最上方在那一頁有幾個bits未使用
  u8 x_intger_end = (x+16)/8;         //最下方在第幾頁
  u8 x_remainder_end =  (x+16)%8;     //最下方在那一頁有幾個bits已使用
	
  y = y+2;                            //列的偏移量，因為列的最左邊起始為2
	
  //=======================最上方=======================
  OLED_Read_Display(x_intger, y, page_num1);                   //讀取最上方的顯示8bits資料，並儲存在page_num1裡
  AndByte = Get_AND_Byte(1, (8-x_remainder)-1);                //利用算法取得欲顯示文字上面的分割資料，AND變數
  I2C_SAND_BYTE(OLED0561_ADD,COM,0xb0+x_intger);               //設定頁地址
  I2C_SAND_BYTE(OLED0561_ADD,COM,y/16+0x10);                   //設定列地址的高位
  I2C_SAND_BYTE(OLED0561_ADD,COM,y%16);                        //設定列地址的低位
  for(j=0;j<8;j++)                                             //因為一次寫入為一列，所以要循環8次
  {
    display_data = ASCII_8x16[(data*16)+c-512] & AndByte;      //利用先前設定好的AND變數取出欲顯示文字的分割資料
    display_data = display_data << x_remainder;                //向左移動未使用的數(可透過下方圖片說明了解)
    if(j == 7)                                                 
    {                                                          //因為從OLED讀出的資料第0個為最後一個
      display_data = display_data | page_num1[0];              //所以要判斷如果迴圈到最後一個(也就是7)時，要將資料指向第0個
    }                                                          //又因為0為最後一個資料，所以迴圈到最後一個前的資料指向都要+1
    else
    {
      display_data = display_data | page_num1[j+1];
    }
    I2C_SAND_BYTE(OLED0561_ADD,DAT,display_data);              //寫入資料到顯示暫存器
    c++;                                                       //ASCII碼指向下一列資料
  }

  //=======================最下方=======================
  OLED_Read_Display(x_intger_end, y, page_num2);               //讀取最上方的顯示8bits資料，並儲存在page_num2裡
  AndByte = Get_AND_Byte(1, x_remainder_end-1);                //利用算法取得欲顯示文字上面的分割資料，AND變數
  AndByte = AndByte << 8-x_remainder_end;                      //因為要取出後面的bits，所以要向左移
  I2C_SAND_BYTE(OLED0561_ADD,COM,0xb0+x_intger_end);           //設定頁地址
  I2C_SAND_BYTE(OLED0561_ADD,COM,y/16+0x10);                   //設定列地址的高位
  I2C_SAND_BYTE(OLED0561_ADD,COM,y%16);                        //設定列地址的低位
  c = 8;                                                       //重新指定讀取ASCII碼數據的偏移量(文字下面的8 Bytes)
  for(j=0;j<8;j++)                                             //因為一次寫入為一列，所以要循環8次
  {
    display_data = ASCII_8x16[(data*16)+c-512] & AndByte;      //利用先前設定好的AND變數取出欲顯示文字的分割資料
    display_data = display_data >> 8-x_remainder_end;          //向右移動未使用的數(可透過下方圖片說明了解)
    if(j == 7)
    {                                                          //因為從OLED讀出的資料第0個為最後一個
      display_data = display_data | page_num2[0];              //所以要判斷如果迴圈到最後一個(也就是7)時，要將資料指向第0個
    }                                                          //又因為0為最後一個資料，所以迴圈到最後一個前的資料指向都要+1
    else
    {
      display_data = display_data | page_num2[j+1];
    }
    I2C_SAND_BYTE(OLED0561_ADD,DAT,display_data);              //寫入資料到顯示暫存器
    c++;                                                       //ASCII碼指向下一列資料
  }
	
  //=======================中間=======================
  if(x_intger_end - x_intger > 1)                                                        //判斷中間是否有一頁是完全被使用到的
  {
    I2C_SAND_BYTE(OLED0561_ADD,COM,0xb0+x_intger+((x_intger_end-x_intger)-1));           //設定頁地址
    I2C_SAND_BYTE(OLED0561_ADD,COM,y/16+0x10);                                           //設定列地址的高位
    I2C_SAND_BYTE(OLED0561_ADD,COM,y%16);                                                //設定列地址的低位
    c = 0;                                                                               //重新指定讀取ASCII碼數據的偏移量
    for(j=0;j<8;j++)                                                                     //因為一次寫入為一列，所以要循環8次
    {
      CombineByte = (ASCII_8x16[(data*16)+c+8-512]<<8) | ASCII_8x16[(data*16)+c-512];    //將上下兩組Bytes整合成一個16 bites的資料
      OrByte1_u16 = Get_AND_Byte(1, (8-x_remainder)-1);                                  //取得上面已處理的資料
      OrByte2_u16 = (Get_AND_Byte(1, x_remainder_end-1)) << (16-x_remainder_end);        //取得下面已處理的資料，並向左位到最後的bit
      OrByte = OrByte1_u16 | OrByte2_u16;                                                //兩個相加獲得一筆16 bits資料(已處理的bit為1，未處理的為0)
      OrByte = ~OrByte;                                                                  //取反(已處理的bit為0，未處理的為1)
                                                                                         //用於AND上整合後的16 bits資料，可得欲寫入的資料

      display_data = (CombineByte & OrByte) >> (8-x_remainder);                          //經過OrByte AND上並向右移的資料獲得欲寫入的資料(中間整頁)，
      I2C_SAND_BYTE(OLED0561_ADD,DAT,display_data);                                      //寫入數據
      c++;                                                                               //ASCII碼指向下一列資料
    }    
  }
}
```


## 取得AndByte函數

```
u8 Get_AND_Byte(u8 data, u8 times)   //起始值、要左移並+1的次數
{
  u8 i;
  for(i=times; i>0; i--)
  {
    data = (data<<1)+1;
  }
  return data;
}
```

## 讀取指定的顯示資料

```
void OLED_Read_Display(u8 Page, u8 Line,u8 *ReturnBuffer)
{
  I2C_SAND_BYTE(OLED0561_ADD,COM,0xb0+Page);              //頁地址(0xB0到0xB7)  
  I2C_SAND_BYTE(OLED0561_ADD,COM,Line/16+0x10);           //起始列地址的高4位
  I2C_SAND_BYTE(OLED0561_ADD,COM,Line%16);	              //起始列地址的低4位
  I2C_READ_BUFFER(OLED0561_ADD, 0x40, ReturnBuffer, 8);   //讀取數據並儲存到指針(此函數可以從I2C的筆記查到)
}
```

## 手寫筆記

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0205/OLED_Function_picture/%E6%89%8B%E5%AF%AB%E7%AD%86%E8%A8%98.jpg)




















