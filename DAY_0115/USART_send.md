# 如何設定初始化USART

***USART有三組，以下以第一組為例***

### 枚舉與時鐘
`GPIO_InitTypeDef GPIO_InitStructure;`設定 **GPIO** 初始化枚舉<br>
`USART_InitTypeDef USART_InitStructure;`設定 **USART** 初始化枚舉<br>
`NVIC_InitTypeDef NVIC_InitStructure;`設定 **NVIC(中斷控制器)** 初始化枚舉<br>
`RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);`啟動 **USART1&GPIOA** 時鐘<br>
### USART1_TX  PA9<br>
`GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;`設定GPIO時鐘速度<br>
`GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;`將PA9設定到結構體的腳位變數裡<br>
`GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;`將腳位輸出模式設定為 **推挽輸出**<br>
`GPIO_Init(GPIOA, &GPIO_InitStructure);`初始化PA9<br>
### USART1_RX  PA10<br>
`GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;`將PA10設定到結構體的腳位變數裡<br>
`GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;`將腳位輸出模式設定為 **懸空輸入**<br>
`GPIO_Init(GPIOA, &GPIO_InitStructure);`初始化PA10<br>
### USART1的NVIC(中斷控制器)設置
`NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;`設定USART1為全局中斷，***STM3.0中USART1_IRQChannel已改成USART1_IRQn***<br>
`NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;`設定搶占優先級為3<br>
`NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;`設定子優先級為3<br>
`NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;`啟動IRQ通道<br>
`NVIC_Init(&NVIC_InitStructure);`根據指定參數初始化VIC佔存器<br>
### USART初始化
`USART_InitStructure.USART_BaudRate = bound;`設定波特率(bound)<br>
`USART_InitStructure.USART_WordLength = USART_WordLength_8b;`設定字長數據格式為8位<br>
`USART_InitStructure.USART_StopBits = USART_StopBits_1;`設定一個停止位<br>
`USART_InitStructure.USART_Parity = USART_Parity_No;`設定無奇偶校驗位<br>
`USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;`設定無硬件數據流控制<br>
`USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;`設定模式為收發模式<br>
`USART_Init(USART1, &USART_InitStructure);`初始化串口<br>
`USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);`開啟串口中斷<br>
`USART_Cmd(USART1, ENABLE);`啟動串口<br>























