# 如何在USART接收時取得{}中間的資料


中斷函數
```
void USART1_IRQHandler(void)
{
  u8 Res; //設定接收變數
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    Res = USART_ReceiveData(USART1); //讀取USART的資料
    if(Res == '{')
    {
      USART1_STATUS = 0x80; //當接收到大括號的開頭時，將自訂旗標狀態設為1000 0000
    }
    if(USART1_STATUS == 0x80) //表示收到大括號開頭，開始讀取內部資料
    {
      USART1_RX_BUF[USART1_i++] = Res; //將資料存到list裡面
      if(Res == '}') //收到大括號尾部表示資料結束
      {
        printf("%s", USART1_RX_BUF); //將資料印出來
        USART1_i = 0; //將全區變數的i設為原來的0
        USART1_STATUS = 0x00; //將狀態旗標設為原來的0x00
        memset(&USART1_RX_BUF,0,200); //將list清空
      }
    }
  }
}
```
