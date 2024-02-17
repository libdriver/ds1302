[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1302

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1302/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der Erhaltungsladungs-Zeitnehmungschip DS1302 enthält eine Echtzeituhr/einen Echtzeitkalender und 31 Bytes statischen RAM. Die Kommunikation mit einem Mikroprozessor erfolgt über eine einfache serielle Schnittstelle. Die Echtzeituhr/der Echtzeitkalender bietet Informationen zu Sekunden, Minuten, Stunden, Tag, Datum, Monat und Jahr. Das Monatsendedatum wird für Monate mit weniger als 31 Tagen automatisch angepasst, einschließlich Korrekturen für Schaltjahre. Die Uhr arbeitet entweder im 24-Stunden- oder 12-Stunden-Format mit einer AM/PM-Anzeige. Die Verbindung des DS1302 mit einem Mikroprozessor wird durch die Verwendung synchroner serieller Kommunikation vereinfacht. Für die Kommunikation mit der Uhr/RAM sind nur drei Drähte erforderlich: CE, I/O (Datenleitung) und SCLK (serielle Uhr). Daten können jeweils 1 Byte oder in einem Burst von bis zu 31 Bytes zum und vom Takt/RAM übertragen werden. Der DS1302 ist für den Betrieb mit sehr geringem Stromverbrauch ausgelegt und speichert Daten und Taktinformationen mit weniger als 1 μW.

LibDriver DS1302 ist ein voll funktionsfähiger Treiber von ds1302, der von LibDriver gestartet wurde. Er bietet Zeitlesen, RAM-Lesen oder -Schreiben und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver DS1302-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver DS1302 GPIO.

/test enthält den Testcode des LibDriver DS1302-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver DS1302-Beispielcode.

/doc enthält das LibDriver DS1302-Offlinedokument.

/Datenblatt enthält DS1302-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige GPIO-Schnittstellenvorlage und stellen Sie Ihren Plattform-GPIO-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ds1302/index.html](https://www.libdriver.com/docs/ds1302/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.