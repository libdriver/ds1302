[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1302

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1302/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The DS1302 trickle-charge timekeeping chip contains a real-time clock/calendar and 31 bytes of static RAM. It communicates with a microprocessor via a simple serial interface. The real-time clock/calendar provides seconds, minutes, hours, day, date, month, and year information. The end of the month date is automatically adjusted for months with fewer than 31 days, including corrections for leap year. The clock operates in either the 24-hour or 12-hour format with an AM/PM indicator.Interfacing the DS1302 with a microprocessor is simplified by using synchronous serial communication. Only three wires are required to communicate with the clock/RAM: CE, I/O (data line), and SCLK (serial clock). Data can be transferred to and from the clock/RAM 1 byte at a time or in a burst of up to 31 bytes. The DS1302 is designed to operate on very low power and retain data and clock information on less than 1μW.

LibDriver DS1302 is a full function driver of ds1302 launched by LibDriver.It provides time reading, ram reading or writing and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver DS1302 source files.

/interface includes LibDriver DS1302 GPIO platform independent template.

/test includes LibDriver DS1302 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver DS1302 sample code.

/doc includes LibDriver DS1302 offline document.

/datasheet includes DS1302 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface GPIO platform independent template and finish your platform GPIO driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

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

### Document

Online documents: [https://www.libdriver.com/docs/ds1302/index.html](https://www.libdriver.com/docs/ds1302/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.