# 溫度傳感器七段顯示器顯示

<br>

## 前置作業

* 將I2C的跳線連接(PB6、PB7)
* 將七段顯示器的跳線連接(PA11、PA12)
![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0131/temperature_seven_display_picture/I2C%E8%B7%B3%E7%B7%9A%E9%80%A3%E6%8E%A5.png)

<br>

## STM32F103上的I2C接口

* 接口數量
  * 第一組：PB6、PB7
  * 第二組：PB10、PB11
* 重點
  * 連接SDA和SCL，需要1-10K上拉電阻(實際依照電路更改)
  * 讀取I2C時，與I2C復用的IO端口需要設為開漏模式
  * 每個器件都有唯一的地址，最多可有1023個
 
## I2C函數分析
* 【定義】
  * `#define I2CPORT GPIOB`定義IO組別
  * `#define I2C_SCL GPIO_Pin_6`定義SCL接口
  * `#define I2C_SDA GPIO_Pin_7`定義SDA接口
  * `#define HostAddress 0xc0`STM32的地址(主機)
  * `#define BusSpeed 200000`總線速度(不能高於400000)

<br>

* 【GPIO初始化】`I2C_GPIO_Init`<br>
`GPIO_InitTypeDef  GPIO_InitStructure;`定義GPIO設置結構體<br>
`RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);`設置GPIO端口相關的時鐘<br>
`RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);`開啟I2C對應的時鐘，否則I2C無法正常工作<br>
`GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;`選擇設置腳位<br>
`GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;`將GPIO設為復用開漏輸出模式(浮接)，因為上拉電阻為外部，所以不需要STM32內的上拉電阻<br>
`GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;`設置腳位的速度<br>
`GPIO_Init(I2CPORT, &GPIO_InitStructure);`將以上設置的內容初始化<br>

<br>

* 【I2C初始化】`I2C_Configuration`<br>
`I2C_InitTypeDef  I2C_InitStructure;`定義I2C設置結構體<br>
`I2C_GPIO_Init();`將GPIO初始化<br>
`I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;`設置為I2C模式<br>
`I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;`設置時鐘相位為2，1為上升沿，2為下降沿<br>
`I2C_InitStructure.I2C_OwnAddress1 = HostAddress;`設置自身地址<br>
`I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;`允許應答<br>
`I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;`設置為7位地址模式<br>
`I2C_InitStructure.I2C_ClockSpeed = BusSpeed;`設置總線速度<br>
`I2C_Init(I2C1,&I2C_InitStructure);`將以上設置內容初始化<br>
`I2C_Cmd(I2C1,ENABLE);`開啟I2C<br>

> 接收資料的設備接收到後，會回傳設定的低電平脈衝，表示收到資料

<br>

* 【I2C發送一個字節】`I2C_SAND_BYTE(u8 SlaveAddr, u8 writeAddr, u8 pBuffer)`設備地址、設備內部暫存器地址、數據<br>
`I2C_GenerateSTART(I2C1,ENABLE);`發送開始信號<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));`等待完成(EV5)<br>
`I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter);`發送設備地址<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));`等待完成(EV6)<br>
`I2C_SendData(I2C1,writeAddr);`發送設備內暫存器地址<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));`等待完成(EV8)<br>
`I2C_SendData(I2C1,pBuffer);`發送數據<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));`等待完成發送數據(EV8)<br>
`I2C_GenerateSTOP(I2C1,ENABLE);`發送停止位<br>

<br>

* 【I2C發送多個數據】`I2C_SAND_BUFFER(u8 SlaveAddr, u8 WriteAddr, u8* pBuffer, u16 NumByteToWrite)`設備地址、設備內暫存器地址、指向數據指針、指針長度(數據長度)<br>
`I2C_GenerateSTART(I2C1,ENABLE);`發送開始信號<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));`等待完成(EV5)<br>
`I2C_Send7bitAddress(I2C1,SlaveAddr,I2C_Direction_Transmitter);`發送設備地址<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));`等待完成(EV6)<br>
`I2C_SendData(I2C1,WriteAddr);`發送設備內暫存器地址<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));`等待完成(EV8)<br>
```
連續發送數據，並在每次發送後等待發送完畢
while(NumByteToWrite--){                                            //持續發送數據直到NumByteToWrite為0
   I2C_SendData(I2C1,*pBuffer);                                     //發送指針指向的數據
   pBuffer++;                                                       //每次指針+1
   while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待發送數據完畢
 }
```
`I2C_GenerateSTOP(I2C1,ENABLE);`發送停止位<br>

<br>

* 【I2C讀取一個字節】`I2C_READ_BYTE(u8 SlaveAddr, u8 readAddr)`讀取的設備地址、讀取設備的暫存器地址<br>
`u8 a;`接收數據的8位元變數<br>
`while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));`等待總線不為Busy狀態<br>
```
//根據【LM75A數據手冊】加入這段在基本的I2C讀取程序上依舊可以讀取數據
I2C_GenerateSTART(I2C1,ENABLE);                                           //發送開始信號
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));                //等待完成發送開始信號
I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter);           //發送設備地址
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  //等待完成發送設備地址
I2C_Cmd(I2C1,ENABLE);                                                     //啟動I2C外設
I2C_SendData(I2C1,readAddr);                                              //發送設備暫存器地址
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));           //等待完成發送設備暫存器地址
```
`I2C_GenerateSTART(I2C1,ENABLE);`發出開始信號<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));`等待完成(EV5)<br>
`I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Receiver);`發送設備地址，最後一位改為讀取狀態(I2C_Direction_Receiver)<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));`等待完成，等待接收數據(EV6)<br>
`I2C_AcknowledgeConfig(I2C1,DISABLE);`在接收最後一個數據時，關閉應答位<br>
`I2C_GenerateSTOP(I2C1,ENABLE);`發送停止位，表示讀取通信結束<br>
`a = I2C_ReceiveData(I2C1);`將I2C暫存器接收到的數據移動到自訂的8位元變數<br>
`return a;`從函數回傳數據<br>

<br>

* 【I2C讀取數多個據串】`I2C_READ_BUFFER(u8 SlaveAddr,u8 readAddr,u8* pBuffer,u16 NumByteToRead)`讀取的設備地址、讀取設備的暫存器地址、讀取的資料(指針)、指針長度<br>
`while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));`等待總線不為Busy狀態<br>
```
//經過測試就算沒有這段也可以正常讀取，不過網路上範例都有這一段
I2C_GenerateSTART(I2C1,ENABLE);                                           //發送開始信號
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));                //等待完成發送開始信號
I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Transmitter);           //發送設備地址
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  //等待完成發送設備地址
I2C_Cmd(I2C1,ENABLE);                                                     //啟動I2C外設
I2C_SendData(I2C1,readAddr);                                              //發送設備暫存器地址
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));           //等待完成發送設備暫存器地址
```
`I2C_GenerateSTART(I2C1,ENABLE);`發出開始信號<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));`等待完成(EV5)<br>
`I2C_Send7bitAddress(I2C1,SlaveAddr, I2C_Direction_Receiver);`發送設備地址，最後一位改為讀取狀態(I2C_Direction_Receiver)<br>
`while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));`等待完成，等待接收數據(EV6)<br>
```
連續讀取數據
while(NumByteToRead){
 if(NumByteToRead == 1){                                    //當數據剩下最後一個進入判斷
  I2C_AcknowledgeConfig(I2C1,DISABLE);                      //關閉應答位
  I2C_GenerateSTOP(I2C1,ENABLE);                            //發送停止位，表示通信結束
 }
 if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){   //等待接收數據
  *pBuffer = I2C_ReceiveData(I2C1);                         //將I2C暫存器的資料轉移到 pBuffer
  pBuffer++;                                                //指針移位+1
  NumByteToRead--;                                          //讀取數據數量-1 
 }
}
```
`I2C_AcknowledgeConfig(I2C1,ENABLE);`啟動應答位<br>

<br>

> 上述所有EVx等旗標詳情查看下方【發送和接收之旗標】

<br>

## LM75A讀操作時序圖

* 注意
  * 每個從裝置的讀操作不盡相同，需要閱讀相關手冊
  * 使用基本的I2C時序一定可以讀取

a. 基本I2C時序讀取<br>
b. 在基本I2C時序前先發送一次裝置地址與暫存器地址<br>
c. 僅讀取1Byte<br>

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0131/temperature_seven_display_picture/LM75A%E8%AE%80%E6%93%8D%E4%BD%9C.png)

## 發送和接收之旗標

* 【主發送序列圖】

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0131/temperature_seven_display_picture/I2C%E4%B8%BB%E7%99%BC%E9%80%81%E5%82%B3%E9%80%81%E5%BA%8F%E5%88%97%E5%9C%96.png)

* 【主接收序列圖】

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0131/temperature_seven_display_picture/I2C%E4%B8%BB%E6%8E%A5%E6%94%B6%E5%82%B3%E9%80%81%E5%BA%8F%E5%88%97%E5%9C%96.png)

* 【I2C_Event值列表】

|IC2_Event|值|
|:---:|:---:|
|I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED|EV1|
|I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED|EV1|
|I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED|EV1|
|I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED|EV1|
|I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED|EV1|
|I2C_EVENT_SLAVE_BYTE_RECEIVED|EV2|
|I2C_EVENT_SLAVE_BYTE_TRANSMITTED|EV3|
|I2C_EVENT_SLAVE_ACK_FAILURE|EV3-1|
|I2C_EVENT_SLAVE_STOP_DETECTED|EV4|
|I2C_EVENT_MASTER_MODE_SELECT|EV5|
|I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED|EV6|
|I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED|EV6|
|I2C_EVENT_MASTER_BYTE_RECEIVED|EV7|
|I2C_EVENT_MASTER_BYTE_TRANSMITTED|EV8|
|I2C_EVENT_MASTER_MODE_ADDRESS10|EV9|

> 詳情閱讀下列文件<br>
> 序列圖 -> [溫度傳感器相關數據資料/STM32F10XXX參考手冊(P.497)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0131/%E6%BA%AB%E5%BA%A6%E5%82%B3%E6%84%9F%E5%99%A8%E7%9B%B8%E9%97%9C%E6%95%B8%E6%93%9A%E8%B3%87%E6%96%99/STM32F10XXX%E5%8F%82%E8%80%83%E6%89%8B%E5%86%8C%EF%BC%88%E4%B8%AD%E6%96%87%EF%BC%89-20150727-CD00171190_ZHV10.pdf)<br>
> I2C_Event值列表 -> [溫度傳感器相關數據資料/I2C總線規範(P.151)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0131/%E6%BA%AB%E5%BA%A6%E5%82%B3%E6%84%9F%E5%99%A8%E7%9B%B8%E9%97%9C%E6%95%B8%E6%93%9A%E8%B3%87%E6%96%99/STM32F10XXX%E5%8F%82%E8%80%83%E6%89%8B%E5%86%8C%EF%BC%88%E4%B8%AD%E6%96%87%EF%BC%89-20150727-CD00171190_ZHV10.pdf)<br>


<br>

## 溫度傳感器(LM75A)程序分析

* 【定義】
  * `#define LM75A_ADD 0x9E`溫度傳感器地址

<br>

* 【讀取溫度函數】`LM75A_GetTemp(u8 *Tempbuffer)`透過指針變量將數據儲存到指定的位置<br>
`u8 buf[2];`定義兩個字節的8位元變數<br>
`u8 t=0,a=0;`<br>
`I2C_READ_BUFFER(LM75A_ADD,0x00,buf,2);`讀出溫度數據<br>
`t = buf[0];`取出溫度數據中的整數位<br>
`*Tempbuffer = 0;`將溫度正負值設為默認(正)<br>
```
if(t & 0x80){       //判斷溫度的正負位是否為負
  *Tempbuffer = 1;  //設置溫度正負值為負
  t = ~t;           //因為是負，所以要取補碼，這裡先反轉
  t++;              //補碼取反後要+1
}
```
`if(t & 0x01){ a=a+1; }`如果第一位為1就累加對應數值，其他以此類推<br>
`if(t & 0x02){ a=a+2; }`<br>
`if(t & 0x04){ a=a+4; }`<br>
`if(t & 0x08){ a=a+8; }`<br>
`if(t & 0x10){ a=a+16; }`<br>
`if(t & 0x20){ a=a+32; }`<br>
`if(t & 0x40){ a=a+64; }`<br>
`Tempbuffer++;`將儲存數據的變數改為下一項<br>
`*Tempbuffer = a;`輸入資料<br>
`a = 0;`<br>
`t = buf[1];`取出小數部分，取0.125精度的前2位（12、25、37、50、62、75、87）<br>
`if(t & 0x20){ a=a+12; }`如果第六位為1就累加對應數值，其他以此類推<br>
`if(t & 0x40){ a=a+25; }`<br>
`if(t & 0x80){ a=a+50; }`<br>
`Tempbuffer++;`將儲存數據的變數改為下一項<br>
`*Tempbuffer = a;`輸入資料<br>

<br>

* 【溫度傳感器進入掉電模式函數】`LM75A_POWERDOWN(void)`<br>
`I2C_SAND_BYTE(LM75A_ADD,0x01,1);`在設備的設備配置暫存器設置中(0x01)寫入關閉溫度傳感器(1)<br>

> 掉電模式指在讀取溫度後關閉傳感器，可以達到省電的效果，一般不常使用

<br>

## 溫度暫存器表

* 上方Temp MS和Temp LS表示兩個字節一般正常狀態
* 下方D10為存放正負值
* 下方D9至D3為存放溫度的整數值
* 下方D2為存放小數值，高精度的溫度傳感器小數點會由D2至D0

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0131/temperature_seven_display_picture/LM75A%E6%BA%AB%E5%BA%A6%E6%9A%AB%E5%AD%98%E5%99%A8%E8%A1%A8.png)

## 溫度暫存器的溫度換算

* 當溫度傳感器的精度較高時，小數的D6和D5分別為0.25和0.12<br>
* 換算為累加式<br>
> 例如：D15-D7的暫存器資料為 000100101<br>
> 因為D15為0 -> 溫度為正<br>
> 因為D14-D8只有D12和D9為1 -> 溫度整數為18<br>
> 因為D7為1 -> 溫度整數18還要加上0.5，所以整體測到的溫度是+18.5度<br>

![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0131/temperature_seven_display_picture/%E6%BA%AB%E5%BA%A6%E8%88%87%E6%9A%AB%E5%AD%98%E5%99%A8%E6%8F%9B%E7%AE%97%E9%97%9C%E4%BF%82.png)














