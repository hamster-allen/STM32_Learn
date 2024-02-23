# USB插拔讀取-SPI

<br>

## 效果呈現

* `CH376 OK!`表示STM32已經成功與CH376進行通訊
* `U DISK Ready!`表示已讀取到有USB插入

  ![效果呈現1](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E6%95%88%E6%9E%9C%E5%91%88%E7%8F%BE1.png)

  ![效果呈現2](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E6%95%88%E6%9E%9C%E5%91%88%E7%8F%BE2.png)

<br>

## 前置作業

* 開發板右上角跳線連接

  ![開發板右上角跳線連接](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E9%96%8B%E7%99%BC%E6%9D%BF%E5%8F%B3%E4%B8%8A%E8%A7%92%E8%B7%B3%E7%B7%9A%E9%80%A3%E6%8E%A5.png)


* 將USB格式化(FAT or FAT32)

  ![USB格式化](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/USB%E6%A0%BC%E5%BC%8F%E5%8C%96.png)

<br>

## 通訊原理圖

* STM32與CH376可以透過UART或SPI通訊
* CH376可以連接USB或SD卡

  ![通訊原理圖](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E9%80%9A%E8%A8%8A%E5%8E%9F%E7%90%86%E5%9C%96.png)

<br>

## STM32的SPI

#### 【特性】

* 2個SPI
* 可設為主從兩種模式
* 全雙工或半雙工
* 最大速度18M/s
* 3位的預分頻器可產生8種主模式頻率
* 支持SD卡讀寫模式
* 支援DMA

#### 【SPI腳位定義圖】

* SPI1

  ![SPI1](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/SPI1.png)

* SPI2

  ![SPI2](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/SPI2.png)

<br>

## 電路圖

![整體電路圖](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E9%9B%BB%E8%B7%AF%E5%9C%96_%E6%95%B4%E9%AB%94%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

* SPI腳位定義

  ![電路圖_腳位定義](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E9%9B%BB%E8%B7%AF%E5%9C%96_%E8%85%B3%E4%BD%8D%E5%AE%9A%E7%BE%A9.png)

* 當連接P16的9~16的跳線時，SPI可以直接連接到TF卡槽，但在這裡不連接

  ![電路圖_P16跳線](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E9%9B%BB%E8%B7%AF%E5%9C%96_P16%E8%B7%B3%E7%B7%9A.png)

  ![實際圖_P16跳線](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E5%AF%A6%E9%9A%9B%E5%9C%96_P16%E8%B7%B3%E7%B7%9A.png)

* 當連接P15的9~16的跳線時，也就表示連接到CH376晶片的SPI

  ![電路圖_P15跳線](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E9%9B%BB%E8%B7%AF%E5%9C%96_P15%E8%B7%B3%E7%B7%9A.png)

  ![實際圖_P15跳線](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E5%AF%A6%E9%9A%9B%E5%9C%96_P15%E8%B7%B3%E7%B7%9A.png)

#### 【CH376整體電路與接腳說明】

![電路圖_CH376整體電路](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_picture/%E9%9B%BB%E8%B7%AF%E5%9C%96_CH376%E6%95%B4%E9%AB%94%E9%9B%BB%E8%B7%AF.png)

|腳號|腳位名稱|功能|
|:---:|:---:|:---:|
|1|INT#|READY與TF卡槽的插卡狀態(第9腳)連接<br>當TF有SD卡插入時，為低電平，會傳送給CH376中斷信號|
|2|RSTI|復位腳，此處懸空使用內部復位電路|
|3|SPI#|選擇溝通模式<br>高電平->UART，低電平->SPI|
|4|TXD|串口溝通腳位|
|5|RXD|串口溝通腳位|
|6|SD_DI|連接TF卡SPI連線的接腳|
|7|V3|3.3V電壓源|
|8|UD+|USB數據線|
|9|UD-|USB數據線|
|10|GND|接地|
|11|XI|連接外部12兆(M)的震盪器|
|12|XO|連接外部12兆(M)的震盪器|
|13|SCS|與STM32的SPI連線的接腳|
|14|SCK|與STM32的SPI連線的接腳|
|15|SDI|與STM32的SPI連線的接腳|
|16|SDO|與STM32的SPI連線的接腳|
|17|SD_CS|連接TF卡SPI連線的接腳|
|18|SD_DO|連接TF卡SPI連線的接腳|
|19|SD_CK|連接TF卡SPI連線的接腳|
|20|VCC|電源正極|


## 程序分析

#### 【主程序】

```
int main (void)
{
  u8 s;
  delay_ms(500);                                       //等待所有元件都進入穩定狀態
  RCC_Configuration();                                 //系統時鐘初始化 
  OLED0561_Init();                                     //OLED初始化
  I2C_Configuration();                                 //I2C初始化

  OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    ");
  OLED_DISPLAY_8x16_BUFFER(2,"  U DISK TEST   ");

  //======CH376初始化======//
  SPI2_Init();                                         //SPI2初始化
  if(mInitCH376Host() == USB_INT_SUCCESS)              //CH376初始化並判斷是否成功
  {
    OLED_DISPLAY_8x16_BUFFER(4,"   CH376 OK!    ");
  }
  while(1)
  {
    s = CH376DiskConnect();                            //讀出USB接口狀態
    if(s == USB_INT_SUCCESS)                           //檢查讀出的狀態是否為USB成功連接
    { 
      OLED_DISPLAY_8x16_BUFFER(6," U DISK Ready!  ");
    }
    else
    {
      OLED_DISPLAY_8x16_BUFFER(6,"                ");
    } 
    delay_ms(500);                                     //每次讀取並更新USB接口狀態的間格
  }
}
```

<br>

#### 【SPI2函數定義】

`#define SPI2PORT  GPIOB`SPI腳位組定義<br>
`#define SPI2_MOSI  GPIO_Pin_15`SPI腳位定義<br>
`#define SPI2_MISO  GPIO_Pin_14`SPI腳位定義<br>
`#define SPI2_SCK  GPIO_Pin_13`SPI腳位定義<br>
`#define SPI2_NSS  GPIO_Pin_12`SPI腳位定義

#### 【SPI2初始化函數】
```
void SPI2_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);                      //使能SPI_2时钟

  //======設定MISO(PB14)為浮接輸入======//
  GPIO_InitStructure.GPIO_Pin = SPI2_MISO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI2PORT,&GPIO_InitStructure);

  //======設定MOSI(PB15)、SCK(PB13)為復用推挽輸出======//
  GPIO_InitStructure.GPIO_Pin = SPI2_MOSI | SPI2_SCK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI2PORT,&GPIO_InitStructure);

  //======設定NSS(PB12)為推挽輸出======//
  GPIO_InitStructure.GPIO_Pin = SPI2_NSS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI2PORT,&GPIO_InitStructure);

  //======SPI功能設置======
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      //雙線雙向全雙工模式
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           //設置此裝置為主SPI(SCK主動產生時鐘)
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       //SPI數據大小為8bits
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                             //空閒狀態時SCK的狀態為高電平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                            //設置是在時鐘上升沿接收傳送資料
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               //NSS片選
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;    //時鐘預分頻器為256(0~256)
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      //MSB高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;                                //CRC較驗和的多項式，7表示不使用
  SPI_Init(SPI2,&SPI_InitStructure);                                      //初始化SPI2的配置項
  SPI_Cmd(SPI2,ENABLE);                                                   //啟動SPI2  
}
```
> SPI功能設置詳情參閱[STM32F103固件庫手冊(P223)](https://github.com/hamster-allen/STM32_Learn/blob/master/Development_Information/STM32F103%E5%9B%BA%E4%BB%B6%E5%87%BD%E6%95%B8%E5%BA%AB%E7%94%A8%E6%88%B7%E6%89%8B%E5%86%8A.pdf)



#### 【SPI2發送接收資料函數】
**主要用在發送**<br>
**將需要發送的數據放到輸入參數中**<br>
**將接收到的數據存到返回值返回**
```
u8 SPI2_SendByte(u8 Byte)
{
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);      //當發送暫存器數據没有發送完，循環等待
  SPI_I2S_SendData(SPI2,Byte);                                       //向發送暫存器寫入想要發送的數據
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);     //當接收暫存器没有收到數據，循環等待
  return SPI_I2S_ReceiveData(SPI2);
}
```

<br>

#### 【CH376定義】

`#define  CH376_INTPORT  GPIOA`定義腳位組<br>
`#define  CH376_INT  GPIO_Pin_8`定義腳位(此處為預留，程序中未使用)

> CH376的指令集表另外分裝在`ch276inc.h`<br>
> 全部指令與用途詳細資料參閱[CH376中文數據手冊(P5)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0221/USB_SPI_%E7%9B%B8%E9%97%9C%E8%B3%87%E6%96%99/CH376%E4%B8%AD%E6%96%87%E6%95%B0%E6%8D%AE%E6%89%8B%E5%86%8C.pdf)

#### 【CH376通訊腳位初始化函數】
```
void CH376_PORT_INIT(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = CH376_INT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //設定腳位為上拉電阻輸入
  GPIO_Init(CH376_INTPORT,&GPIO_InitStructure);	
  
  GPIO_SetBits(CH376_INTPORT,CH376_INT);           //設置中斷輸入腳的初始狀態為高電平
  GPIO_SetBits(SPI2PORT,SPI2_NSS);                 //設置NSS也為高電平
}
```
#### 【CH376結束SPI命令函數】
```
void xEndCH376Cmd(void)
{
  GPIO_SetBits(SPI2PORT,SPI2_NSS);              //直接將NSS設為高電平
}
```
#### 【CH376發送一個字節函數】
```
void Spi376OutByte(u8 d)
{
  SPI2_SendByte(d);                             //調用SPI2的發送函數
}
```
#### 【CH376接收一個字節函數】
```
u8 Spi376InByte(void)
{
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);    //等待接收暫存器有數據
  return SPI_I2S_ReceiveData(SPI2);                                 //只要一有數據將會回傳
}
```

#### 【CH376寫命令函數】
```
void xWriteCH376Cmd(u8 mCmd)
{
  GPIO_SetBits(SPI2PORT,SPI2_NSS);              //先將NSS復位(高電平)
  delay_us(20);                                 //延遲
  GPIO_ResetBits(SPI2PORT,SPI2_NSS);            //將NSS選中(低電平)
  Spi376OutByte(mCmd);                          //調用【CH376發送一個字節函數】來發送命令數據 
  delay_us(1700);                               //確保讀寫週期大於1.5us
}
```
#### 【CH376寫數據函數】
```
void xWriteCH376Data(u8 mData)
{
  Spi376OutByte(mData);                         //調用SPI2的發送函數
  delay_us(800);                                //為了確保讀寫週期大於0.6ms
}
```
#### 【CH376讀出數據函數】
```
u8 xReadCH376Data(void)
{
  u8 i;
  delay_us(10);
  i = SPI2_SendByte(0xFF);                      //調用SPI2.h的數據發送函數，因為此函數每次發送後會接收
  return(i);
}
```
#### 【CH376查詢中斷(INT#)】
**當腳位INT#為低電平時，表示有中斷信號**<br>
**回傳：0->無中斷、1->有中斷**
```
u8 Query376Interrupt(void)
{
	u8 i;
 	i = GPIO_ReadInputDataBit(CH376_INTPORT,CH376_INT); 	    //透過IO口讀出中斷狀態
	return(i);                                                //回傳
}			
```
#### 【CH376初始化函數】
```
u8 mInitCH376Host(void)
{
  u8	res;	
  delay_ms(600);                                  //等待周邊元件上電後，進入正常工作狀態
  CH376_PORT_INIT();                              //中斷接腳初始化

  xWriteCH376Cmd(CMD11_CHECK_EXIST);              //發送命令，測試通訊接口和工作狀態
  xWriteCH376Data(0x55);                          //隨意發送數據用於測試通訊
  res = xReadCH376Data();                         //成功取得通訊 -> 回傳數據為傳送的取反
  //printf("res =%02x \n",(unsigned short)res);
  xEndCH376Cmd( );                                //結束通訊

  if (res != 0xAA) return(ERR_USB_UNKNOWN);       //如果接收到的數據不等於發送數據的按位取反，函數回傳通訊接口不正常

  xWriteCH376Cmd(CMD11_SET_USB_MODE);             //發送命令，設置USB的工作模式
  xWriteCH376Data(0x06);                          //發送參數，設置為切換到已啟用的USB主機模式，自動產生SOF包
  delay_us(20);                                   //設置USB工作模式在10us內完成，因此保守延遲20us
  res = xReadCH376Data();                         //讀出回傳的最終操作模式
  //printf("res =%02x \n",(unsigned short)res);
  xEndCH376Cmd();                                 //結束通訊
  
  if(res == CMD_RET_SUCCESS)                      //RES=51  命令操作成功
  {  
    return(USB_INT_SUCCESS);                      //函數回傳USB設置成功
  }
  else
  {
    return(ERR_USB_UNKNOWN);                      //函數回傳USB設置失敗
  }
}
```


> 通訊IO不正常的可能原因有<br>
> IO口連接異常、其他設備影響(NSS不唯一)、串口波特率、一直在復位、晶振不工作












