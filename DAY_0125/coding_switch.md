# 編碼器

<br>

## 事前工作

* 將印有旋轉編碼器的跳線接上
  
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0125/coding_switch_picture/%E6%97%8B%E8%BD%89%E7%B7%A8%E7%A2%BC%E5%99%A8%E7%9F%AD%E6%8E%A5%E5%9C%96%E7%A4%BA.png)
  
* 將模擬搖桿的跳線斷開，因為與編碼器複用
  
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0125/coding_switch_picture/%E6%A8%A1%E6%93%AC%E6%90%96%E6%A1%BF%E6%96%B7%E9%96%8B%E5%9C%96%E7%A4%BA.png)

<br>

## 模擬開關&編碼器之電路圖

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0125/coding_switch_picture/%E6%A8%A1%E6%93%AC%E6%90%96%E6%A1%BF%E8%88%87%E7%B7%A8%E7%A2%BC%E5%99%A8%E4%B9%8B%E6%8E%A5%E7%B7%9A%E5%9C%96.png)

<br>

## 編碼開關之原理
* 按下按鍵可以導通K1按鍵<br>
* 左右旋轉K2和K3導通的順序會不同<br>

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0125/coding_switch_picture/%E7%B7%A8%E7%A2%BC%E9%96%8B%E9%97%9C%E8%AA%AA%E6%98%8E%E5%9C%96.png)


* 向右旋轉：K3先導通再導通K2<br>
* 向右旋轉：K2先導通再導通K3<br>
> 根據不同廠家，旋轉導通的順序將不同
  
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0125/coding_switch_picture/%E7%B7%A8%E7%A2%BC%E9%96%8B%E9%97%9C%E6%97%8B%E8%BD%89%E6%B3%A2%E5%9E%8B.png)

<br>

## 程序實現

### 【初始化函數】

`GPIO_InitTypeDef  GPIO_InitStructure;`定義枚舉
`RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);`打開三組GPIO的時鐘源
`GPIO_InitStructure.GPIO_Pin = ENCODER_L | ENCODER_D;`定義要初始化的腳位(PA組的兩個腳位)
`GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;`選擇腳位為上拉電阻模式     
`GPIO_Init(ENCODER_PORT_A,&GPIO_InitStructure);`初始化PA組腳位

`GPIO_InitStructure.GPIO_Pin = ENCODER_R;`定義要初始化的腳位(PB組的腳位)                
`GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;`選擇腳位為上拉電阻模式      
`GPIO_Init(ENCODER_PORT_B,&GPIO_InitStructure);`初始化PB組腳位

### 【狀態讀取函數】

* 全區變數
  * `u8 KUP`卡死標誌位
  * `u16 cou`卡死計數器
```
u8 a;                                                                    //存放返回按鍵狀態的值
u8 kt;                                                                   //暫存現在按鍵狀態
a=0;
if(GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L))KUP=0;                //判斷是否在高電平(沒有被旋轉或卡死)
if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L)&&KUP==0){            //判斷是否被旋轉(低電平)，同時判斷是否有被卡死
  delay_us(100);                                                         //等待電平狀態穩定
  kt=GPIO_ReadInputDataBit(ENCODER_PORT_B,ENCODER_R);                    //紀錄另一端的電平狀態
  delay_ms(3);                                                           //延遲(去除機械彈跳)
  if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L)){                  //再次讀取是否被旋轉(低電平)
    if(kt==0){                                                           //根據另一端的紀錄狀態判斷旋轉方向
      a=1;                                                               //設定返回值為右轉(1)
    }
    else{
      a=2;                                                               //設定返回值為左轉(2)
    }
    cou=0;                                                               //歸零卡死計數值
    while(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L)&&cou<60000){  //等待放開旋鈕(電平回高電平)
      cou++;                                                             //卡死計數值每次+1，只要超過60000(約1.2秒)就跳出不繼續等待
      KUP=1;                                                             //卡死狀態旗標設為1
      delay_us(20);
    }
  }
}
if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D)&&KUP==0){            //判斷旋鈕的按鍵是否被按下
  delay_ms(20);                                                          //延遲(去除機械彈跳)
  if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D)){                  //再次讀取電平是否真的被按下
    a=3;                                                                 //設置返回值為被按下按鍵(3)
    //while(ENCODER_D==0);                                               //等待按鈕被放開
  }
}
return a;
```









