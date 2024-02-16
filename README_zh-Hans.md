[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1302

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1302/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS1302涓流充电计时芯片包含实时时钟/日历和31字节静态RAM。它通过简单的串行接口与微处理器通信。实时时钟/日历提供秒、分、时、星期、日期、月和年信息。对于少于31天的月份，月底的日期会自动调整，包括针对闰年的更正。时钟以24小时制或带AM/PM指示器的12小时制运行。通过使用同步串行通信，简化了DS1302与微处理器的接口。只需三根线即可与时钟/RAM通信：CE、I/O（数据线）和SCLK（串行时钟）。在与时钟/RAM之间进行数据传输时，可以一次1个字节，也可以在一次突发中包含多达31个字节。DS1302工作时的功耗非常低，并以低于1µW的功耗保留数据和时钟信息。

LibDriver DS1302是LibDriver推出的DS1302全功能驱动，该驱动提供时间读取和RAM读取或写入等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver DS1302的源文件。

/interface目录包含了LibDriver DS1302与平台无关的GPIO总线模板。

/test目录包含了LibDriver DS1302驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver DS1302编程范例。

/doc目录包含了LibDriver DS1302离线文档。

/datasheet目录包含了DS1302数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的GPIO总线模板，完成指定平台的GPIO总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/ds1302/index.html](https://www.libdriver.com/docs/ds1302/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。