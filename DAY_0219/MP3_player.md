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

> 了解MY1690腳位定義參閱[MY1690語音晶片使用說明(P3)]()<br>
> MY1690串口通訊協議相關資料參閱[MY1690語音晶片使用說明(P4)]()<br>




















































