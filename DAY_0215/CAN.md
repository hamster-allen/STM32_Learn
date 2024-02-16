# CAN

<br>

## 前置工作

* 連接CAN總線跳線(P24)

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/CAN%E8%B7%B3%E7%B7%9A.png)

* 斷開七段顯示器跳線(P9)

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E4%B8%83%E6%AE%B5%E9%A1%AF%E7%A4%BA%E5%99%A8%E8%B7%B3%E7%B7%9A.png)

* 連接方法
  * 將開發板上標示L接口連接到另一個開發板的L接口
  * 標示H接口同理
    
  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E9%80%A3%E6%8E%A5%E6%96%B9%E6%B3%95.png)

<br>

## 效果呈現

* 下方開發板發送`'D'`資料，上方開發板接收到`'D'`
* 上方開發板發送`'A'`資料，下方開發板接收到`'A'`

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%95%88%E6%9E%9C%E5%91%88%E7%8F%BE.png)

<br>

## 原理與說明

* STM32上連接CAN總線的接腳
* PA12 -> CAN_TX
* PA11 -> CAN_RX

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/STM32_CAN%E8%85%B3%E4%BD%8D%E5%AE%9A%E7%BE%A9.png)
  
  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/CAN%E6%8E%A5%E8%85%B3%E5%AE%9A%E7%BE%A9.png)

* CAN接腳透過跳線(P24)連接到CAN總線收發器(TJA1050)
* TJA1050工作在5V
* R33穩定電平

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

* 如果使用傳統8051單晶片，需要外接一個CAN總線控制器和轉發器
* 不過因為STM32裡已經有CAN總線控制器，因此只需要外接轉發器

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/TJA1050%E9%9B%BB%E8%B7%AF%E5%9C%96.png)

> 詳情參閱[TJA1050數據手冊(P9)](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN%E7%9B%B8%E9%97%9C%E8%B3%87%E6%96%99/TJA1050%E6%95%B0%E6%8D%AE%E6%89%8B%E5%86%8C%EF%BC%88%E8%8B%B1%E6%96%87%EF%BC%89%20.pdf)

<br>

## CAN特點與相關概念

* 兩個設備: 當成USART使用
* 多個設備: 使用CAN協議(使用郵箱、識別符、過濾器功能)

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E5%85%A9%E5%80%8B_%E5%A4%9A%E5%80%8B%E8%A8%AD%E5%82%99.png)

* 以`ISO 11898(High speed)`為例，電位差為0時，表示數據為1
* 電位差為2.0V時，表示數據為0

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/CAN%E7%89%A9%E7%90%86%E5%B1%A4.png)

> 顯性電平(0V)具有優先權，只要有設備輸出顯性電平，總線上即為顯性電平<br>
> 只有所有的設備都輸出隱性電平，總線才為隱性電平<br>

<br>

## CAN發送
  
  |||
  |:---:|:---:|
  |報文|一次發送的完整數據訊息|
  |郵箱|發送報文的發送調度器|
  |幀種類|報文種類:<br>數據幀、遙控幀、錯誤幀、過載幀、幀間隔|
  |幀格式|一個報文理包含的內容|
  |標釋符|總線上的設備透過此數據是不是給自己的|

#### 【CAN總線發送郵箱】

* 將數據放到優先級最高的空郵箱，再發送到其他CAN設備
* 如果最高的郵箱已經被放了數據，將會往下放到第二個郵箱
* 郵箱就像緩衝區，只要將數據放到郵箱中就會依序傳送出去，不需要再管理

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E7%B8%BD%E7%B7%9A%E7%99%BC%E9%80%81%E9%83%B5%E7%AE%B1_%E5%85%A8%E7%A9%BA.png)
  
  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E7%B8%BD%E7%B7%9A%E7%99%BC%E9%80%81%E9%83%B5%E7%AE%B1_%E9%83%B5%E7%AE%B11%E6%BB%BF.png)

#### 【多個設備連接傳輸原理】

* 所有設備都會接收報文(數據)，但標釋符不符的會被過濾器刪除
* 標釋符屬於報文中的其中一個數據

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E5%A4%9A%E5%80%8B%E8%A8%AD%E5%82%99%E9%80%A3%E6%8E%A5%E5%82%B3%E8%BC%B8%E5%8E%9F%E7%90%86.png)

#### 【報文種類】

* 最常用到為**數據幀**、**遙控幀**

|幀|幀用途|
|:---:|:---:|
|數據幀|發送向接收傳送數據的幀|
|遙控幀(遠程幀)|接收向發送請求數據的幀|
|錯誤幀|檢測出錯誤時，向其他單元通知錯誤的幀|
|過載幀|接收單元通知其未做好接收準備的幀|
|幀間隔|將數據幀及遙控幀與前面的幀分開的幀|

> 用戶只能控制數據幀與遙控幀

#### 【數據幀與遙控幀的報文格式】

* 都具有標準模式和擴展格式兩種格式
* 兩種格式就像I2C協議裡的7位和10位地址

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%95%B8%E6%93%9A%E5%B9%80%E8%88%87%E9%81%99%E6%8E%A7%E5%B9%80%E7%9A%84%E5%A0%B1%E6%96%87%E6%A0%BC%E5%BC%8F.png)

#### 【標準格式】

* 每個長條方塊表示一個bit
* Data字節數不固定，最多64bits
* 仲裁段也就是標釋符

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%A8%99%E6%BA%96%E6%A0%BC%E5%BC%8F.png)

#### 【標準格式與擴展格式的比較】

* 識別符
  * 標準格式只有`ID28`到`ID18`
  * 擴展格式只多了`Identifier(extension)`，也就是`ID17`到`ID0`

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%A8%99%E6%BA%96%E6%A0%BC%E5%BC%8F%E8%88%87%E6%93%B4%E5%B1%95%E6%A0%BC%E5%BC%8F.png)

* 完整的比較圖

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%A8%99%E6%BA%96%E6%A0%BC%E5%BC%8F%E8%88%87%E6%93%B4%E5%B1%95%E6%A0%BC%E5%BC%8F%E6%AF%94%E8%BC%83.png)

<br>

## CAN接收

#### 【接收郵箱】

* FIFO：`先進先出`，有層級深度的接收郵箱
* STM32F103系列有2個FIFO郵箱，每個FIFO有3層深度
* 成功通過過濾器的報文會被放入FIFO郵箱

<br>

* 發送郵箱
  * 每個郵箱都是平行的，且一個郵箱只能儲存一個報文
* 接收郵箱
  * 具有兩個郵箱(FIFO0、FIFO1)
  * 每個郵箱有三層，先接收到的會在郵箱的最上面，第二到的會在郵箱的中間，第三到的會在郵箱的最後
  * 每次讀取郵箱會從最先到的開始讀取，一次讀取讀出一個報文

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%8E%A5%E6%94%B6_%E7%99%BC%E9%80%81_FIFO%E9%83%B5%E7%AE%B1.png)

#### 【過濾器】

* 過濾器：透過硬體判斷報文中的標釋符，過濾掉標釋符不同的報文
* STM32F103系列中的CAN總線控制器有14個過濾器組
* 過濾器是由硬體實現的，只有成功通過過濾器報文才會被放到FIFO郵箱裡

#### 【過濾器配置】

`FSCx=0`時，表示設置寬度為`16位`<br>
`FSCx=1`時，表示設置寬度為`32位`<br>

`FBMx=0`時，表示`標釋符屏蔽`<br>
`FBMx=1`時，表示`標釋符列表`<br>

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E9%81%8E%E6%BF%BE%E5%99%A8%E9%85%8D%E7%BD%AE.png)

#### 【過濾器-標釋符列表模式】

* 收到的報文會與過濾器中的ID進行對比
* 過濾器中的ID值透過初始化時設置
* **收到報文的標釋符要與過濾器設定的ID完全相同，才會被放到郵箱中**

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%A8%99%E9%87%8B%E7%AC%A6%E5%88%97%E8%A1%A8%E6%A8%A1%E5%BC%8F.png)

#### 【過濾器-標釋符屏蔽模式】

* 收到的報文會與過濾器中的ID進行對比
* 過濾器中的ID值與屏蔽值透過初始化時設置
* 屏蔽值中的`1`表示必須與設定的ID值相同，`0`則表示允許不相同

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E6%A8%99%E9%87%8B%E7%AC%A6%E5%B1%8F%E8%94%BD%E6%A8%A1%E5%BC%8F.png)

#### 【不同設置下的過濾器編號】

透過*過濾器配置*的圖示可以知道一個過濾器有64bits可進行設置<br>
因此不同設置下所佔用的bits會影響可儲存的過濾數<br>

* 過濾器的編號會按照過濾器組的編號優先順序進行編號<br>
下列為在全部過濾器組設定不同的設置之範例圖<br>

  ![image](https://github.com/hamster-allen/STM32_Learn/blob/master/DAY_0215/CAN_picture/%E4%B8%8D%E5%90%8C%E8%A8%AD%E7%BD%AE%E4%B8%8B%E7%9A%84%E9%81%8E%E6%BF%BE%E5%99%A8%E7%B7%A8%E8%99%9F.png)

也因此可以繪製成表格<br>

|模式|在單個過濾器可存的過濾值|
|:---:|:---:|
|列表模式(32位)|2個|
|屏蔽模式(32位)|1個|
|列表模式(16位)|4個|
|屏蔽模式(16位)|2個|

#### 【過濾器的優先級】

1. 32位的過濾器優先於16位的過濾器
2. 位相同時，標釋符列表模式優先於屏蔽模式
3. 位和模式都相同時，優先級由過濾器的編號決定，過濾器編號越小優先級越高

#### 【CAN總線的其他功能】

* 工作模式：正常、睡眠、測試
* 測試模式：靜默、回環、回環靜默
* 時間觸發通信模式(通常用於多個CAN總線連接時，防止通訊衝突)
* 暫存器訪問保護
* 中斷
* 紀錄接收SOF時刻的時間戳




<br>

## 程序

* 定義
  * `#define CAN_INT_ENABLE	0`1為啟用CAN中斷，0為關閉CAN中斷
  * `#define tsjw CAN_SJW_1tq`設置選項(1-4)
  * `#define tbs1 CAN_BS1_8tq`設置選項(1-16)
  * `#define tbs2 CAN_BS2_7tq`設置選項(1-8)
  * `#define brp 9`設置選項
  > 波特率=(pclk1/((1+8+7)*9))=36MHz/16/9=250Kbits<br>
  > pclk1為STM32時鐘設置部分<br>
  
#### 【初始化】

##### CAN設置
|名稱|解釋|
|:---:|:---:|
|TTCM(時間觸發通訊模式)|訊息分配時間窗口，以滿足一些即時性要求|
|ABOM(自動匯流排關閉管理)|如果傳輸錯誤計數器(TEC)大於255，則CAN硬體會切換到匯流排關閉狀態|
|AWUM(自動喚醒模式)|該位元為 0，則必須手動退出睡眠模式|
|NART(無自動重傳)|先前的嘗試因仲裁遺失等原因失敗，CAN硬體會重試傳輸訊息|
|RFLM(接收 FIFO 鎖定模式)|當所有接收郵箱都已經滿了會發生溢出情況，再接收到資料後要覆蓋或丟棄<br>1為丟棄最新的資料，0為覆蓋最後一筆資料|
|TXFP(傳輸 FIFO 優先權)|1為先到先傳送，0為ID較低的優先|

***返回0表示成功，返回其他值表示失敗***
```
u8 CAN1_Configuration(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;                        //聲明GPIO結構體
  CAN_InitTypeDef         CAN_InitStructure;                         //聲明CAN結構體
  CAN_FilterInitTypeDef   CAN_FilterInitStructure;                   //聲明CAN過濾器結構體

#if CAN_INT_ENABLE
    NVIC_InitTypeDef      NVIC_InitStructure;                        //如果啟用，聲明CAN的中斷接收
#endif

  //GPIO初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);              //啟用PORTA時鐘
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);               //啟用CAN1時鐘  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                         //設定GPIO(CAN_TX)
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  //設定GPIO速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                    //設定GPIO模式為推挽輸出
  GPIO_Init(GPIOA, &GPIO_InitStructure);                             //初始化IO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                         //設定GPIO(CAN_RX)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                      //上拉輸入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                             //初始化IO

  //CAN設置
  CAN_InitStructure.CAN_TTCM=DISABLE;                                //不啟用時間觸發通信模式
  CAN_InitStructure.CAN_ABOM=DISABLE;                                //不啟用自動離線管理
  CAN_InitStructure.CAN_AWUM=DISABLE;                                //不啟用自動睡眠模式，睡眠模式透過軟體喚醒(清除CAN->MCR的SLEEP位)
  CAN_InitStructure.CAN_NART=ENABLE;                                 //禁止自動重新發送報文
  CAN_InitStructure.CAN_RFLM=DISABLE;                                //報文不鎖定,新的覆蓋舊的  
  CAN_InitStructure.CAN_TXFP=DISABLE;                                //發送優先級由報文標釋符決定
  CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;                       //模式設置：CAN_Mode_Normal 普通模式;

  //設置波特率
  CAN_InitStructure.CAN_SJW=tsjw;                                    //重新同步跳跃宽度(Tsjw)为tsjw+1個時間單位  CAN_SJW_1tq    CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  CAN_InitStructure.CAN_BS1=tbs1;                                    //Tbs1=tbs1+1個時間單位CAN_BS1_1tq ~ CAN_BS1_16tq
  CAN_InitStructure.CAN_BS2=tbs2;                                    //Tbs2=tbs2+1個時間單位CAN_BS2_1tq ~ CAN_BS2_8tq
  CAN_InitStructure.CAN_Prescaler=brp;                               //分頻係數(Fdiv)為brp+1  
  CAN_Init(CAN1, &CAN_InitStructure);                                //初始化CAN1

  //設置過濾器
  CAN_FilterInitStructure.CAN_FilterNumber=0;                        //過濾器0(0~13)
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;      //屏蔽位模式
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;     //32位寬度
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;                   //32位ID值，高16位
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;                    //32位ID值，低16位
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;               //32位的屏蔽值(MASK)，高16位
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;                //32位的屏蔽值(MASK)，低16位           
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0; //將此過濾器指定到FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;               //啟用過濾器
  CAN_FilterInit(&CAN_FilterInitStructure);                          //過濾器初始化

#if CAN_INT_ENABLE                                                   //以下用於如果啟用CAN中斷接收之設置
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                           //允許FIFO0產生中斷           
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;        //主優先級為1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;               //次優先級為0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //啟用中斷
    NVIC_Init(&NVIC_InitStructure);
#endif
  return 0;		
}
```





#### 【發送數據】
**函數返回是否傳送成功(0為成功，其他為失敗)**<br>

* 輸入參數
  * msg -> 傳送的數據
  * len -> 傳送數據的長度

* IDE：表示為標準格式or擴展格式
  * 標準格式`CAN_Id_Standard`
  * 擴展格式`CAN_Id_Extended`

* RTR：表示為數據幀or遙控幀
  * 數據幀`CAN_RTR_Data`
  * 遙控幀`CAN_RTR_Remote`

```
u8 CAN_Send_Msg(u8* msg,u8 len)
{
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;                                                          // 聲明結構體
  TxMessage.StdId = 0x12;                                                      // 設置標準標釋符 
  TxMessage.ExtId = 0x00;                                                      // 設置擴展標釋符
  TxMessage.IDE = CAN_Id_Standard;                                             // 設置為標準幀
  TxMessage.RTR = CAN_RTR_Data;                                                // 設置為數據幀
  TxMessage.DLC = len;                                                         // 發送數據的長度
  for(i=0;i<len;i++)TxMessage.Data[i]=msg[i];                                  // 將數據寫入CAN總線控制器
  mbox = CAN_Transmit(CAN1,&TxMessage);                                        // 將TxMessage中的數據寫入CAN1，返回值表示寫入哪一個郵箱
  i=0;                                                                         // 重置i變量
  while((CAN_TransmitStatus(CAN1,mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;   //等待發送結束，每次迴圈+1，超過0xfff跳出
  if(i>=0XFFF)return 1;                                                        // 超過0xfff表示超時
  return 0;
}
```

#### 【接收數據】
```
u8 CAN_Receive_Msg(u8 *buf)
{
  u32 i;
  CanRxMsg RxMessage;                                  //定義接收數據結構體
  if(CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;   //檢查FIFO0是否有數據，沒有數據直接退出
  CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);              //讀取數據
  for(i=0;i<8;i++)buf[i]=RxMessage.Data[i];            //把8個數據放入定義的數組裡
  return RxMessage.DLC;                                //返回接收到的數據數量
}
```

#### 【中斷接收數據】

```
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  vu8 CAN_ReceiveBuff[8];                         //CAN總線中斷接收數據暫存器
  vu8 i = 0;
  vu8 u8_RxLen = 0;
  CAN_ReceiveBuff[0] = 0;                         //清空CAN中斷接收數據的暫存器
  RxMessage.StdId = 0x00;                         //--------------------------------------|
  RxMessage.ExtId = 0x00;                         //   |                                  |
  RxMessage.IDE = 0;                              //   |                                  |
  RxMessage.RTR = 0;                              //   |                                  |
  RxMessage.DLC = 0;                              //   |清空CAN總線接收數據結構體的所有數據  |
  RxMessage.FMI = 0;                              //   |                                  |
  for(i=0;i<8;i++)                                //   |                                  |
  {                                               //   |                                  |
    RxMessage.Data[i]=0x00;                       //   |                                  |
  }                                               //--------------------------------------|

  CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);        //讀出FIFO0中的數據(因為只有收到數據時才會產生中斷，因此可以直接讀取)
  u8_RxLen = RxMessage.DLC;                      //讀出數據的數量，並存在自訂的變數中
  if(RxMessage.StdId==0x12)                      //讀出收到報文中的標釋符是否相同
  {
    CAN_ReceiveBuff[0] = RxMessage.DLC;          //將數據的長度儲存到自訂數組的第0位
    for( i=0;i<u8_RxLen; i++)                    //再將數據資料儲存在自訂數組的其他位
    {
      CAN_ReceiveBuff[i] = RxMessage.Data[i];    //將8位數據儲存到自訂數組裡
    }
  }
}
```





















