[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1302

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1302/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS1302涓流充電計時晶片包含即時時鐘/行事曆和31位元組靜態RAM。 它通過簡單的序列介面與微處理器通信。 即時時鐘/行事曆提供秒、分、時、星期、日期、月和年資訊。 對於少於31天的月份，月底的日期會自動調整，包括針對閏年的更正。 時鐘以24小時制或帶AM/PM指示器的12小時制運行。 通過使用同步串列通信，簡化了DS1302與微處理器的介面。 只需三根線即可與時鐘/RAM通信：CE、I/O（數據線）和SCLK（串列時鐘）。 在與時鐘/RAM之間進行資料傳輸時，可以一次1個位元組，也可以在一次突發中包含多達31個位元組。 DS1302工作時的功耗非常低，並以低於1µW的功耗保留數據和時鐘資訊。

LibDriver DS1302是LibDriver推出的DS1302全功能驅動，該驅動提供時間讀取和RAM讀取或寫入等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver DS1302的源文件。

/interface目錄包含了LibDriver DS1302與平台無關的GPIO總線模板。

/test目錄包含了LibDriver DS1302驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver DS1302編程範例。

/doc目錄包含了LibDriver DS1302離線文檔。

/datasheet目錄包含了DS1302數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的GPIO總線模板，完成指定平台的GPIO總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

#### example basic

```C
#include "driver_ds1302_basic.h"

uint8_t res;
uint8_t addr = 0x00;
uint8_t data = 0x00;
uint32_t t = 0;
char time_buffer[32];

/* basic init */
res = ds1302_basic_init();
if (res != 0)
{
    return 1;
}

/* set time zone */
res = ds1302_basic_set_timestamp_time_zone(8);
if (res != 0)
{
    (void)ds1302_basic_deinit();

    return 1;
}

...
    
/* set timestamp */
res = ds1302_basic_set_timestamp(t);
if (res != 0)
{
    (void)ds1302_basic_deinit();

    return 1;
}

/* output */
ds1302_interface_debug_print("set timestamp %d.\n", t);

...

/* set timestamp */
memset(time_buffer, 0, sizeof(char) * 32);
res = ds1302_basic_get_ascii_time(time_buffer, 32);
if (res != 0)
{
    (void)ds1302_basic_deinit();

    return 1;
}

/* output */
ds1302_interface_debug_print(time_buffer);

...

/* write ram */
res = ds1302_basic_write_ram(addr, &data, 1);
if (res != 0)
{
    (void)ds1302_basic_deinit();

    return 1;
}

/* output */
ds1302_interface_debug_print("write ram addr 0x%02X data 0x%02X.\n", addr, data);    

...

/* read ram */
res = ds1302_basic_read_ram(addr, &data, 1);
if (res != 0)
{
    (void)ds1302_basic_deinit();

    return 1;
}

/* output */
ds1302_interface_debug_print("read ram addr 0x%02X data 0x%02X.\n", addr, data);

...
    
(void)ds1302_basic_deinit();

return 0;
```

#### example advance

```C
#include "driver_ds1302_advance.h"

uint8_t res;
uint8_t i;
uint8_t addr = 0x00;
uint8_t data = 0x00;
uint32_t t = 0;
uint8_t burst_buffer[31];
char time_buffer[32];

/* advance init */
res = ds1302_advance_init();
if (res != 0)
{
    return 1;
}

/* set time zone */
res = ds1302_advance_set_timestamp_time_zone(8);
if (res != 0)
{
    (void)ds1302_advance_deinit();

    return 1;
}

/* set timestamp */
res = ds1302_advance_set_timestamp(t);
if (res != 0)
{
    (void)ds1302_advance_deinit();

    return 1;
}

/* output */
ds1302_interface_debug_print("set timestamp %d.\n", t);

...
    
/* set timestamp */
memset(time_buffer, 0, sizeof(char) * 32);
res = ds1302_advance_get_ascii_time(time_buffer, 32);
if (res != 0)
{
    (void)ds1302_advance_deinit();

    return 1;
}

/* output */
ds1302_interface_debug_print(time_buffer);

...
    
/* write ram */
res = ds1302_advance_write_ram(addr, &data, 1);
if (res != 0)
{
    (void)ds1302_advance_deinit();

    return 1;
}

/* output */
ds1302_interface_debug_print("write ram addr 0x%02X data 0x%02X.\n", addr, data);

...
    
/* read ram */
res = ds1302_advance_read_ram(addr, &data, 1);
if (res != 0)
{
    (void)ds1302_advance_deinit();

    return 1;
}

/* output */
ds1302_interface_debug_print("read ram addr 0x%02X data 0x%02X.\n", addr, data);

...
    
/* set charge */
res = ds1302_advance_set_charge(DS1302_CHARGE_DISABLE);
if (res != 0)
{
    (void)ds1302_advance_deinit();

    return 1;
}

...
    
res = ds1302_advance_burst_write(DS1302_BURST_TYPE_RAM, burst_buffer, 31);
if (res != 0)
{
    (void)ds1302_advance_deinit();

    return 1;
}
ds1302_interface_debug_print("ram burst write: ");
for (i = 0; i < 31; i++)
{
    ds1302_interface_debug_print("0x%02X ", burst_buffer[i]);
}

...
    
res = ds1302_advance_burst_read(DS1302_BURST_TYPE_RAM, burst_buffer, 31);
if (res != 0)
{
    (void)ds1302_advance_deinit();

    return 1;
}
ds1302_interface_debug_print("ram burst read: ");
for (i = 0; i < 31; i++)
{
    ds1302_interface_debug_print("0x%02X ", burst_buffer[i]);
}

...
    
(void)ds1302_advance_deinit();

return 0;
```

### 文檔

在線文檔: [https://www.libdriver.com/docs/ds1302/index.html](https://www.libdriver.com/docs/ds1302/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
