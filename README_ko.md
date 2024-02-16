[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1302

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1302/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS1302 세류 충전 시간 유지 칩에는 실시간 클록/캘린더와 31바이트의 정적 RAM이 포함되어 있습니다. 간단한 직렬 인터페이스를 통해 마이크로프로세서와 통신합니다. 실시간 시계/달력은 초, 분, 시간, 일, 날짜, 월 및 연도 정보를 제공합니다. 윤년 수정을 포함하여 31일 미만의 달에 대해서는 월말 날짜가 자동으로 조정됩니다. 시계는 AM/PM 표시기를 사용하여 24시간 또는 12시간 형식으로 작동합니다. DS1302와 마이크로프로세서의 인터페이스는 동기식 직렬 통신을 사용하여 단순화됩니다. 클록/RAM과 통신하려면 CE, I/O(데이터 라인) 및 SCLK(직렬 클록)의 3개 와이어만 필요합니다. 데이터는 한 번에 1바이트씩 또는 최대 31바이트의 버스트로 시계/RAM과 주고받을 수 있습니다. DS1302는 매우 낮은 전력으로 작동하고 1μW 미만의 데이터 및 클록 정보를 유지하도록 설계되었습니다.

LibDriver DS1302는 LibDriver가 출시한 ds1302의 전체 기능 드라이버입니다. 시간 읽기, 램 읽기 또는 쓰기 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver DS1302의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver DS1302용 플랫폼 독립적인 GPIO버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver DS1302드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver DS1302프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver DS1302오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 DS1302데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 GPIO버스 템플릿을 참조하여 지정된 플랫폼에 대한 GPIO버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/ds1302/index.html](https://www.libdriver.com/docs/ds1302/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.