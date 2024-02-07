/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-02-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/02/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ds1302_register_test.h"
#include "driver_ds1302_ram_test.h"
#include "driver_ds1302_basic.h"
#include "driver_ds1302_advance.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */

/**
 * @brief     ds1302 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ds1302(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"buffer", required_argument, NULL, 2},
        {"charge", required_argument, NULL, 3},
        {"data", required_argument, NULL, 4},
        {"times", required_argument, NULL, 5},
        {"timestamp", required_argument, NULL, 6},
        {"type", required_argument, NULL, 7},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    uint32_t t = 0;
    uint8_t time_flag = 0;
    uint8_t addr = 0x00;
    uint8_t addr_flag = 0;
    uint8_t data = 0x00;
    uint8_t data_flag = 0x00;
    uint8_t charge = DS1302_CHARGE_DISABLE;
    uint8_t charge_flag = 0x00;
    ds1302_burst_type_t burst_type = DS1302_BURST_TYPE_RAM;
    uint8_t burst_buffer[31];

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* address */
            case 1 :
            {
                addr = atol(optarg) & 0xFF;
                addr_flag = 1;

                break;
            }
            
            /* buffer */
            case 2 :
            {
                uint8_t size;
                uint8_t i;
                char *p;
                
                /* get the size */
                size = (uint8_t)strlen(optarg);
                
                /* check the size */
                if ((size == 16) || (size == 18) || (size == 62) || (size == 64))
                {
                    /* check ok */
                }
                else
                {
                    return 5;
                }

                /* check the size */
                if (strncmp(optarg, "0x", 2) == 0)
                {
                    p = optarg + 2;
                    size -= 2;
                }
                else if (strncmp(optarg, "0X", 2) == 0)
                {
                    p = optarg + 2;
                    size -= 2;
                }
                else
                {
                    p = optarg;
                }
                
                /* set the data */
                for (i = 0; i < size; i += 2)
                {
                    if (('A' <= p[i]) && (p[i] <= 'Z'))
                    {
                        burst_buffer[i / 2] = p[i] - 'A' + 10;
                        burst_buffer[i / 2] *= 16;
                    }
                    else if (('a' <= p[i]) && (p[i] <= 'z'))
                    {
                        burst_buffer[i / 2] = p[i] - 'a' + 10;
                        burst_buffer[i / 2] *= 16;
                    }
                    else if (('0' <= p[i]) && (p[i] <= '9'))
                    {
                        burst_buffer[i / 2] = p[i] - '0';
                        burst_buffer[i / 2] *= 16;
                    }
                    else
                    {
                        return 5;
                    }
                    if (('A' <= p[i + 1]) && (p[i + 1] <= 'Z'))
                    {
                        burst_buffer[i / 2 ] += p[i + 1] - 'A' + 10;
                    }
                    else if (('a' <= p[i + 1]) && (p[i + 1] <= 'z'))
                    {
                        burst_buffer[i / 2 ] += p[i + 1] - 'a' + 10;
                    }
                    else if (('0' <= p[i + 1]) && (p[i + 1] <= '9'))
                    {
                        burst_buffer[i / 2 ] += p[i + 1] - '0';
                    }
                    else
                    {
                        return 5;
                    }
                }
                
                break;
            }
            
            /* charge */
            case 3 :
            {
                if (strcmp(optarg, "ENABLE") == 0)
                {
                    charge = DS1302_CHARGE_ENABLE | DS1302_CHARGE_2_DIODE | DS1302_CHARGE_8K;
                    charge_flag = 1;
                }
                else if (strcmp(optarg, "DISABLE") == 0)
                {
                    charge = DS1302_CHARGE_DISABLE;
                    charge_flag = 1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* data */
            case 4 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = (uint16_t)strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the data */
                data = hex_data % 0xFF;
                data_flag = 1;

                break;
            }

            /* running times */
            case 5 :
            {
                /* set the times */
                times = atol(optarg);

                break;
            }

            /* timestamp */
            case 6 :
            {
                /* set the timestamp */
                t = atoi(optarg);
                time_flag = 1;

                break;
            }
            
            /* type */
            case 7 :
            {
                if (strcmp(optarg, "CLOCK") == 0)
                {
                    burst_type = DS1302_BURST_TYPE_CLOCK;
                }
                else if (strcmp(optarg, "RAM") == 0)
                {
                    burst_type = DS1302_BURST_TYPE_RAM;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (ds1302_register_test() != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_ram", type) == 0)
    {
        /* run ram test */
        if (ds1302_ram_test(times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_basic-set-time", type) == 0)
    {
        uint8_t res;
        
        /* check param */
        if (time_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no timestamp.\n");
            
            return 5;
        }
        
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
        
        /* set timestamp */
        res = ds1302_basic_set_timestamp(t);
        if (res != 0)
        {
            (void)ds1302_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ds1302_interface_debug_print("set timestamp %d.\n", t);
        
        (void)ds1302_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_basic-get-time", type) == 0)
    {
        uint8_t res;
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
        
        (void)ds1302_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_basic-set-ram", type) == 0)
    {
        uint8_t res;
        
        /* check param */
        if (addr_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no addr.\n");
            
            return 5;
        }
        
        /* check param */
        if (data_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no data.\n");
            
            return 5;
        }
        
        /* basic init */
        res = ds1302_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* write ram */
        res = ds1302_basic_write_ram(addr, &data, 1);
        if (res != 0)
        {
            (void)ds1302_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ds1302_interface_debug_print("write ram addr 0x%02X data 0x%02X.\n", addr, data);
        
        (void)ds1302_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_basic-get-ram", type) == 0)
    {
        uint8_t res;
        
        /* check param */
        if (addr_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no addr.\n");
            
            return 5;
        }
        
        /* basic init */
        res = ds1302_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* read ram */
        res = ds1302_basic_read_ram(addr, &data, 1);
        if (res != 0)
        {
            (void)ds1302_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ds1302_interface_debug_print("read ram addr 0x%02X data 0x%02X.\n", addr, data);
        
        (void)ds1302_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-set-time", type) == 0)
    {
        uint8_t res;
        
        /* check param */
        if (time_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no timestamp.\n");
            
            return 5;
        }
        
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
        
        (void)ds1302_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-get-time", type) == 0)
    {
        uint8_t res;
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
        memset(time_buffer, 0, sizeof(char) * 32);
        res = ds1302_advance_get_ascii_time(time_buffer, 32);
        if (res != 0)
        {
            (void)ds1302_advance_deinit();
            
            return 1;
        }
        
        /* output */
        ds1302_interface_debug_print(time_buffer);
        
        (void)ds1302_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-set-ram", type) == 0)
    {
        uint8_t res;
        
        /* check param */
        if (addr_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no addr.\n");
            
            return 5;
        }
        
        /* check param */
        if (data_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no data.\n");
            
            return 5;
        }
        
        /* advance init */
        res = ds1302_advance_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* write ram */
        res = ds1302_advance_write_ram(addr, &data, 1);
        if (res != 0)
        {
            (void)ds1302_advance_deinit();
            
            return 1;
        }
        
        /* output */
        ds1302_interface_debug_print("write ram addr 0x%02X data 0x%02X.\n", addr, data);
        
        (void)ds1302_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-get-ram", type) == 0)
    {
        uint8_t res;
        
        /* check param */
        if (addr_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no addr.\n");
            
            return 5;
        }
        
        /* advance init */
        res = ds1302_advance_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* read ram */
        res = ds1302_advance_read_ram(addr, &data, 1);
        if (res != 0)
        {
            (void)ds1302_advance_deinit();
            
            return 1;
        }
        
        /* output */
        ds1302_interface_debug_print("read ram addr 0x%02X data 0x%02X.\n", addr, data);
        
        (void)ds1302_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-charge", type) == 0)
    {
        uint8_t res;
        
        /* check charge */
        if (charge_flag == 0)
        {
            ds1302_interface_debug_print("ds1302: no charge.\n");
            
            return 5;
        }
        
        /* advance init */
        res = ds1302_advance_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set charge */
        res = ds1302_advance_set_charge(charge);
        if (res != 0)
        {
            (void)ds1302_advance_deinit();
            
            return 1;
        }
        
        if (charge != 0)
        {
            ds1302_interface_debug_print("enable charge.\n");
        }
        else
        {
            ds1302_interface_debug_print("disable charge.\n");
        }
        
        (void)ds1302_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-write-burst", type) == 0)
    {
        uint8_t i;
        uint8_t res;
        
        /* advance init */
        res = ds1302_advance_init();
        if (res != 0)
        {
            return 1;
        }
        
        if (burst_type == DS1302_BURST_TYPE_CLOCK)
        {
            res = ds1302_advance_burst_write(DS1302_BURST_TYPE_CLOCK, burst_buffer, 8);
            if (res != 0)
            {
                (void)ds1302_advance_deinit();
                
                return 1;
            }
            ds1302_interface_debug_print("clock burst write: ");
            for (i = 0; i < 8; i++)
            {
                ds1302_interface_debug_print("0x%02X ", burst_buffer[i]);
            }
        }
        else
        {
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
        }
        
        (void)ds1302_advance_deinit();
        
        return 0;
    }
    else if (strcmp("e_advance-read-burst", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        
        /* advance init */
        res = ds1302_advance_init();
        if (res != 0)
        {
            return 1;
        }
        
        if (burst_type == DS1302_BURST_TYPE_CLOCK)
        {
            res = ds1302_advance_burst_read(DS1302_BURST_TYPE_CLOCK, burst_buffer, 8);
            if (res != 0)
            {
                (void)ds1302_advance_deinit();
                
                return 1;
            }
            ds1302_interface_debug_print("clock burst read: ");
            for (i = 0; i < 8; i++)
            {
                ds1302_interface_debug_print("0x%02X ", burst_buffer[i]);
            }
        }
        else
        {
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
        }
        
        (void)ds1302_advance_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ds1302_interface_debug_print("Usage:\n");
        ds1302_interface_debug_print("  ds1302 (-i | --information)\n");
        ds1302_interface_debug_print("  ds1302 (-h | --help)\n");
        ds1302_interface_debug_print("  ds1302 (-p | --port)\n");
        ds1302_interface_debug_print("  ds1302 (-t reg | --test=reg)\n");
        ds1302_interface_debug_print("  ds1302 (-t ram | --test=ram) [--times=<num>]\n");
        ds1302_interface_debug_print("  ds1302 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>\n");
        ds1302_interface_debug_print("  ds1302 (-e basic-get-time | --example=basic-get-time)\n");
        ds1302_interface_debug_print("  ds1302 (-e basic-set-ram | --example=basic-set-ram) --addr=<address> --data=<hex>\n");
        ds1302_interface_debug_print("  ds1302 (-e basic-get-ram | --example=basic-get-ram) --addr=<address>\n");
        ds1302_interface_debug_print("  ds1302 (-e advance-set-time | --example=advance-set-time) --timestamp=<time>\n");
        ds1302_interface_debug_print("  ds1302 (-e advance-get-time | --example=advance-get-time)\n");
        ds1302_interface_debug_print("  ds1302 (-e advance-set-ram | --example=advance-set-ram) --addr=<address> --data=<hex>\n");
        ds1302_interface_debug_print("  ds1302 (-e advance-get-ram | --example=advance-get-ram) --addr=<address>\n");
        ds1302_interface_debug_print("  ds1302 (-e advance-charge | --example=advance-charge) --charge=<ENABLE | DISABLE>\n");
        ds1302_interface_debug_print("  ds1302 (-e advance-write-burst | --example=advance-write-burst) [--type=<CLOCK | RAM>] [--buffer=<hex>]\n");
        ds1302_interface_debug_print("  ds1302 (-e advance-read-burst | --example=advance-read-burst) [--type=<CLOCK | RAM>]\n");
        ds1302_interface_debug_print("\n");
        ds1302_interface_debug_print("Options:\n");
        ds1302_interface_debug_print("      --addr=<address>            Set ram address.([default: 0])\n");
        ds1302_interface_debug_print("      --buffer=<hex>              Set burst buffer.([default: random])\n");
        ds1302_interface_debug_print("      --charge=<ENABLE | DISABLE> Set battery charge.([default: DISABLE])\n");
        ds1302_interface_debug_print("      --data=<hex>                Set ram data and it is hexadecimal.([default: 0x00])\n");
        ds1302_interface_debug_print("  -e <basic-set-time | basic-get-time | basic-set-ram | basic-get-ram | advance-set-time | advance-get-time |\n");
        ds1302_interface_debug_print("      advance-set-ram | advance-get-ram | advance-charge | advance-write-burst | advance-read-burst>\n");
        ds1302_interface_debug_print("                                  Run the driver example.\n");
        ds1302_interface_debug_print("  -h, --help                      Show the help.\n");
        ds1302_interface_debug_print("  -i, --information               Show the chip information.\n");
        ds1302_interface_debug_print("  -p, --port                      Display the pin connections of the current board.\n");
        ds1302_interface_debug_print("  -t <reg | ram | output>, --test=<reg | ram | output>\n");
        ds1302_interface_debug_print("                                  Run the driver test.\n");
        ds1302_interface_debug_print("      --times=<num>               Set the running times.([default: 3])\n");
        ds1302_interface_debug_print("      --timestamp=<time>          Set the the unix timestamp.\n");
        ds1302_interface_debug_print("      --type=<CLOCK | RAM>        Set burst type.([default: RAM])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ds1302_info_t info;

        /* print ds1302 info */
        ds1302_info(&info);
        ds1302_interface_debug_print("ds1302: chip is %s.\n", info.chip_name);
        ds1302_interface_debug_print("ds1302: manufacturer is %s.\n", info.manufacturer_name);
        ds1302_interface_debug_print("ds1302: interface is %s.\n", info.interface);
        ds1302_interface_debug_print("ds1302: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ds1302_interface_debug_print("ds1302: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ds1302_interface_debug_print("ds1302: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ds1302_interface_debug_print("ds1302: max current is %0.2fmA.\n", info.max_current_ma);
        ds1302_interface_debug_print("ds1302: max temperature is %0.1fC.\n", info.temperature_max);
        ds1302_interface_debug_print("ds1302: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ds1302_interface_debug_print("ds1302: CE connected to GPIOB PIN1.\n");
        ds1302_interface_debug_print("ds1302: SCLK connected to GPIOA PIN0.\n");
        ds1302_interface_debug_print("ds1302: IO connected to GPIOA PIN8.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* shell init && register ds1302 function */
    shell_init();
    shell_register("ds1302", ds1302);
    uart_print("ds1302: welcome to libdriver ds1302.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("ds1302: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ds1302: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ds1302: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ds1302: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ds1302: param is invalid.\n");
            }
            else
            {
                uart_print("ds1302: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
