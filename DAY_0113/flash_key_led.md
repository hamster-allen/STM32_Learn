# 如何使用Flash功能

## 寫入數據


* `RCC_HSICmd(ENABLE);` 開啟內部高速時鐘(HSI)，Flash操作必須在內部或外部高速時鐘，因為已開啟外部高速時鐘，所以這裡不需要開啟內部高速時鐘<br>
* `FLASH_Unlock();` 解鎖Flash擦除控制器(避免平時誤操作而設置)<br>
* `FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);` 清除Flash旗標<br>
* `FLASH_ErasePage(指定地址頁);` 每次寫入資料前都必須清除地址頁<br>
* `FLASH_ProgramHalfWord(指定地址, 欲寫入資料);` 從指定的地址開始寫入資料<br>
* `FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);` 清除Flash旗標<br>
* `FLASH_Lock();` 鎖定Flash擦除控制器<br>

> 在清除地址頁中，只需要給指定的地址就會自動判斷是哪一頁並清除

> 使用Flash存放資料時，盡可能將資料儲存在靠後的地址，避免與主程序相互衝突或破壞<br>
> 靠後的地址，例如：第8頁(0x08002000)~第11頁(0x08002FFF)

## 讀取數據

`u16 a`設定儲存返回資料的變數<br>
`a = *(u16*)(指定地址)`從指定的地址開始讀取資料返回<br>



