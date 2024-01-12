# 如何控制GPIO

## GPIO_WriteBit(GPIO組, 腳位編號, 狀態)

***設定單一腳位為高電平或低電平***

* GPIO組
  
      GPIOx(x為A、B、C、D或E)
  
* 腳位編號

      GPIO_Pin_x(x為0~15)

* 狀態

      BitVal: 使用BitAction枚舉(例如:(BitAction)(1))
      Bit_RESET: 將輸出設為低電平
      Bit_SET: 將輸出設為高電平

> 例如:<br>
> 設置GPIOA的第15腳為高電平<br>
> `GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET)`<br>
> `GPIO_WriteBit(GPIOA, GPIO_Pin_15, (BitAction)(1))`

-------------

## GPIO_SetBits(GPIO組, 腳位編號)

***設定指定腳位為高電平***

* GPIO組
  
      GPIOx(x為A、B、C、D或E)
  
* 腳位編號

      GPIO_Pin_x(x為0~15)

> 例如:<br>
> 設置GPIOA的第15腳為高電平<br>
> `GPIO_SetBits(GPIOA, GPIO_Pin_15)`

------------

## GPIO_ResetBits(GPIO組, 腳位編號)

***設定指定腳位為低電平***

* GPIO組
  
      GPIOx(x為A、B、C、D或E)
  
* 腳位編號

      GPIO_Pin_x(x為0~15)

> 例如:<br>
> 設置GPIOA的第15腳為低電平<br>
> `GPIO_ResetBits(GPIOA, GPIO_Pin_15)`

-----------

## GPIO_Write(GPIO組, GPIO整組腳位變量)

***一次設定整組哪些腳位為高電平或低電平***

* GPIO組
  
      GPIOx(x為A、B、C、D或E)
  
* GPIO整組腳位變量

      0x0000~0xFFFF
      0xFFFF 為二進制 1111 1111 1111 1111，也就是每個腳位的輸出或輸入


> 例如:<br>
> 設置GPIOA的第15腳為高電平，而其他都設為低電平<br>
> 二進制為 1000 0000 0000 0000，轉換成十六進制為0x8000
> `GPIO_Write(GPIOA, 0x8000)`

------------------

# 如何編寫delay函數(us微秒)

`SysTick -> LOAD = 72*要延遲的時間(us微秒);`將計數值裝入暫存器(因主頻為72MHz，72次為一微秒)<br>
`SysTick -> VAL = 0x00;`清空定時器的計數器，避免連續調用時造成時間不準確<br>
`SysTick -> CTRL = 0x00000005;`時鐘源HCLK，打開定時器<br>
`while(!(SysTick -> CTRL&0x00010000));`等待計數到0時退出迴圈<br>
`SysTick -> CTRL = 0x00000004;`關閉定時器<br>






  
