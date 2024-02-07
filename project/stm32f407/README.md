### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

GPIO Pin: CE/SCLK/IO PB1/PA0/PA8.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. DS1302

#### 3.1 Command Instruction

1. Show ds1302 chip and driver information.

   ```shell
   ds1302 (-i | --information)
   ```

2. Show ds1302 help.

   ```shell
   ds1302 (-h | --help)
   ```

3. Show ds1302 pin connections of the current board.

   ```shell
   ds1302 (-p | --port)
   ```

4. Run ds1302 register test.

   ```shell
   ds1302 (-t reg | --test=reg)
   ```

5. Run ds1302 ram test, num means test times.

   ```shell
   ds1302 (-t ram | --test=ram) [--times=<num>]
   ```

6. Run ds1302 basic set time function, time is the unix timestamp.

   ```shell
   ds1302 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
   ```

7. Run ds1302 basic get time function.

   ```shell
   ds1302 (-e basic-get-time | --example=basic-get-time)
   ```

8. Run ds1302 basic set ram function, address is the ram start address, hex is the set data and it is hexadecimal.

    ```shell
    ds1302 (-e basic-set-ram | --example=basic-set-ram) --addr=<address> --data=<hex>
    ```

9. Run ds1302 basic get ram function, address is the ram start address.

    ```shell
    ds1302 (-e basic-get-ram | --example=basic-get-ram) --addr=<address> 
    ```

10. Run ds1302 advance set time function, time is the unix timestamp.

      ```shell
      ds1302 (-e advance-set-time | --example=advance-set-time) --timestamp=<time>
      ```

11. Run ds1302 advance get time function.

       ```shell
       ds1302 (-e advance-get-time | --example=advance-get-time)
       ```

12. Run ds1302 advance set ram function, address is the ram start address, hex is the set data and it is hexadecimal.

     ```shell
     ds1302 (-e advance-set-ram | --example=advance-set-ram) --addr=<address> --data=<hex>
     ```

13. Run ds1302 advance get ram function, address is the ram start address.

     ```shell
     ds1302 (-e advance-get-ram | --example=advance-get-ram) --addr=<address> 
     ```

14. Run ds1302 advance charge function.

     ```shell
     ds1302 (-e advance-charge | --example=advance-charge) --charge=<ENABLE | DISABLE>
     ```

15. Run ds1302 advance write burst function, hex is the set buffer array, and it is hexadecimal.For clock burst type length is 8, ram burst type length is 31.

     ```shell
     ds1302 (-e advance-write-burst | --example=advance-write-burst) [--type=<CLOCK | RAM>] [--buffer=<hex>]
     ```

16. Run ds1302 advance read burst function.

     ```shell
     ds1302 (-e advance-read-burst | --example=advance-read-burst) [--type=<CLOCK | RAM>]
     ```

#### 3.2 Command Example

```shell
ds1302 -i

ds1302: chip is Maxim Integrated DS1302.
ds1302: manufacturer is Maxim Integrated.
ds1302: interface is GPIO.
ds1302: driver version is 1.0.
ds1302: min supply voltage is 2.0V.
ds1302: max supply voltage is 5.5V.
ds1302: max current is 1.28mA.
ds1302: max temperature is 85.0C.
ds1302: min temperature is -40.0C.
```

```shell
ds1302 -p

ds1302: CE connected to GPIOB PIN1.
ds1302: SCLK connected to GPIOA PIN0.
ds1302: IO connected to GPIOA PIN8.
```

```shell
ds1302 -t reg

ds1302: chip is Maxim Integrated DS1302.
ds1302: manufacturer is Maxim Integrated.
ds1302: interface is GPIO.
ds1302: driver version is 1.0.
ds1302: min supply voltage is 2.0V.
ds1302: max supply voltage is 5.5V.
ds1302: max current is 1.28mA.
ds1302: max temperature is 85.0C.
ds1302: min temperature is -40.0C.
ds1302: start register test.
ds1302: ds1302_set_time/ds1302_get_time test.
ds1302: set time 2068-04-10 PM 01:41:00 7.
ds1302: check time ok.
ds1302: set time 2067-04-05 19:54:33 4.
ds1302: check time ok.
ds1302: ds1302_set_charge/ds1302_get_charge test.
ds1302: set charge 0xAB.
ds1302: check time ok.
ds1302: set charge 0x00.
ds1302: check time ok.
ds1302: ds1302_clock_burst_write/ds1302_clock_burst_read test.
ds1302: set clock 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x00.
ds1302: check clock burst ok.
ds1302: ds1302_set_write_protect/ds1302_get_write_protect test.
ds1302: disable write protect.
ds1302: check write protect ok.
ds1302: enable write protect.
ds1302: check write protect ok.
ds1302: ds1302_set_oscillator/ds1302_get_oscillator test.
ds1302: enable oscillator.
ds1302: check oscillator ok.
ds1302: finish register test.
```

```shell
ds1302 -t ram --times=3

ds1302: chip is Maxim Integrated DS1302.
ds1302: manufacturer is Maxim Integrated.
ds1302: interface is GPIO.
ds1302: driver version is 1.0.
ds1302: min supply voltage is 2.0V.
ds1302: max supply voltage is 5.5V.
ds1302: max current is 1.28mA.
ds1302: max temperature is 85.0C.
ds1302: min temperature is -40.0C.
ds1302: start ram test.
ds1302: common ram test.
ds1302: 1/3 check passed.
ds1302: 2/3 check passed.
ds1302: 3/3 check passed.
ds1302: burst ram test.
ds1302: 1/3 check passed.
ds1302: 2/3 check passed.
ds1302: 3/3 check passed.
ds1302: finish ram test.
```

```shell
ds1302 -e basic-set-time --timestamp=1612686500

set timestamp 1612686500.
```

```shell
ds1302 -e basic-get-time

2021-02-07 16:28:30 7.
```

```shell
ds1302 -e basic-set-ram --addr=0 --data=0x08

write ram addr 0x00 data 0x08.
```

```shell
ds1302 -e basic-get-ram --addr=0

read ram addr 0x00 data 0x08.
```

```shell
ds1302 -e advance-set-time --timestamp=1612686500

set timestamp 1612686500.
```

```shell
ds1302 -e advance-get-time

2021-02-07 16:29:09 7.
```

```shell
ds1302 -e advance-set-ram --addr=0 --data=0x08

write ram addr 0x00 data 0x08.
```

```shell
ds1302 -e advance-get-ram --addr=0

read ram addr 0x00 data 0x08.
```

```shell
ds1302 -e advance-charge --charge=DISABLE

disable charge.
```

```shell
ds1302 -e advance-write-burst --type=RAM --buffer=01020304050607081112131415161718212223242526272831323334353637

ram burst write: 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x31 0x32 0x33 0x34 0x35 0x36 0x37 
```

```shell
ds1302 -e advance-read-burst --type=RAM

ram burst read: 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x31 0x32 0x33 0x34 0x35 0x36 0x37 
```

```shell
ds1302 -h

Usage:
  ds1302 (-i | --information)
  ds1302 (-h | --help)
  ds1302 (-p | --port)
  ds1302 (-t reg | --test=reg)
  ds1302 (-t ram | --test=ram) [--times=<num>]
  ds1302 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
  ds1302 (-e basic-get-time | --example=basic-get-time)
  ds1302 (-e basic-set-ram | --example=basic-set-ram) --addr=<address> --data=<hex>
  ds1302 (-e basic-get-ram | --example=basic-get-ram) --addr=<address>
  ds1302 (-e advance-set-time | --example=advance-set-time) --timestamp=<time>
  ds1302 (-e advance-get-time | --example=advance-get-time)
  ds1302 (-e advance-set-ram | --example=advance-set-ram) --addr=<address> --data=<hex>
  ds1302 (-e advance-get-ram | --example=advance-get-ram) --addr=<address>
  ds1302 (-e advance-charge | --example=advance-charge) --charge=<ENABLE | DISABLE>
  ds1302 (-e advance-write-burst | --example=advance-write-burst) [--type=<CLOCK | RAM>] [--buffer=<hex>]
  ds1302 (-e advance-read-burst | --example=advance-read-burst) [--type=<CLOCK | RAM>]

Options:
      --addr=<address>            Set ram address.([default: 0])
      --buffer=<hex>              Set burst buffer.([default: random])
      --charge=<ENABLE | DISABLE> Set battery charge.([default: DISABLE])
      --data=<hex>                Set ram data and it is hexadecimal.([default: 0x00])
  -e <basic-set-time | basic-get-time | basic-set-ram | basic-get-ram | advance-set-time | advance-get-time |
      advance-set-ram | advance-get-ram | advance-charge | advance-write-burst | advance-read-burst>
                                  Run the driver example.
  -h, --help                      Show the help.
  -i, --information               Show the chip information.
  -p, --port                      Display the pin connections of the current board.
  -t <reg | ram | output>, --test=<reg | ram | output>
                                  Run the driver test.
      --times=<num>               Set the running times.([default: 3])
      --timestamp=<time>          Set the the unix timestamp.
      --type=<CLOCK | RAM>        Set burst type.([default: RAM])
```
