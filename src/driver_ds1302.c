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
 * @file      driver_ds1302.c
 * @brief     driver ds1302 source file
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

#include "driver_ds1302.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Maxim Integrated DS1302"        /**< chip name */
#define MANUFACTURER_NAME         "Maxim Integrated"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.0f                             /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                             /**< chip max supply voltage */
#define MAX_CURRENT               1.28f                            /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                           /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            1000                             /**< driver version */

/**
 * @brief chip register definition
 */
#define DS1302_REG_SECOND               (0 << 1)       /**< second register */
#define DS1302_REG_MINUTE               (1 << 1)       /**< minute register */
#define DS1302_REG_HOUR                 (2 << 1)       /**< hour register */
#define DS1302_REG_DATE                 (3 << 1)       /**< date register */
#define DS1302_REG_MONTH                (4 << 1)       /**< month register */
#define DS1302_REG_WEEK                 (5 << 1)       /**< week register */
#define DS1302_REG_YEAR                 (6 << 1)       /**< year register */
#define DS1302_REG_CONTROL              (7 << 1)       /**< control register */
#define DS1302_REG_CHARGE               (8 << 1)       /**< charge register */

/**
 * @brief chip command definition
 */
#define DS1302_COMMAND_RTC              (0 << 6)        /**< rtc command */
#define DS1302_COMMAND_RAM              (1 << 6)        /**< ram command */
#define DS1302_COMMAND_BURST            (0x1F << 1)     /**< burst command */

/**
 * @brief     write one byte
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] reg is the register address
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ds1302_write(ds1302_handle_t *handle, uint8_t reg, uint8_t data)
{
    uint8_t res;
    uint8_t i;
    uint8_t temp;
    uint8_t prev;
    
    prev = (1 << 7) | reg;                         /* set reg */
    res = handle->ce_gpio_write(1);                /* set ce high */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    temp = prev;                                   /* set reg */
    for (i = 0; i < 8; i++)                        /* loop */
    {
        if ((temp & 0x01) != 0)                    /* check lsb bit */
        {
            res = handle->io_gpio_write(1);        /* set io high */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        else
        {
            res = handle->io_gpio_write(0);        /* set io low */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        temp = temp >> 1;                          /* right shift 1 */
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(1);          /* set sclk high */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(0);          /* set sclk low */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
    }
    temp = data;                                   /* set data */
    for (i = 0; i < 8; i++)                        /* loop */
    {
        if ((temp & 0x01) != 0)                    /* check the lsb bit */
        {
            res = handle->io_gpio_write(1);        /* set io high */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        else
        {
            res = handle->io_gpio_write(0);        /* set io low */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        temp = temp >> 1;                          /* right shift 1 */
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(1);          /* set sclk high */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(0);          /* set sclk low */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
    }
    res = handle->io_gpio_write(0);                /* set io low */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    res = handle->ce_gpio_write(0);                /* set ce low */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    
    return 0;                                      /* success return 0 */
}

/**
 * @brief      read one byte
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[in]  reg is the register address
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ds1302_read(ds1302_handle_t *handle, uint8_t reg, uint8_t *data)
{
    uint8_t res;
    uint8_t i;
    uint8_t temp;
    uint8_t prev;
    
    prev = (1 << 7) | reg;                         /* set reg */
    res = handle->ce_gpio_write(1);                /* set ce high */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    temp = prev;                                   /* set reg */
    for (i = 0; i < 8; i++)                        /* loop */
    {
        if ((temp & 0x01) != 0)                    /* check the lsb */
        {
            res = handle->io_gpio_write(1);        /* set io high */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        else
        {
            res = handle->io_gpio_write(0);        /* set io low */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        temp = temp >> 1;                          /* right shift 1 */
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(1);          /* set sclk high */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(0);          /* set sclk low */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
    }
    temp = 0;                                      /* init temp 0 */
    for (i = 0; i < 8; i++)                        /* loop */
    {
        uint8_t level;
        
        temp = temp >> 1;                          /* right shift 1 */
        res = handle->io_gpio_read(&level);        /* read the level */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
        if (level != 0)                            /* check the level */
        {
            temp |= 1 << 7;                        /* set msb bit high */
        }
        else
        {
            temp &= ~(1 << 7);                     /* set msb bit low */
        }
        res = handle->sclk_gpio_write(1);          /* set sclk high */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(0);          /* set sclk low */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
    }
    *data = temp;                                  /* save data */
    res = handle->ce_gpio_write(0);                /* set ce low */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    
    return 0;                                      /* success return 0 */
}

/**
 * @brief     write multiple bytes
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ds1302_multiple_write(ds1302_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    
    for (i = 0; i < len; i++)                                                   /* loop */
    {
        res = a_ds1302_write(handle, (uint8_t)(reg + (i << 1)), buf[i]);        /* write one byte */
        if (res != 0)                                                           /* check the result */
        {
            return 1;                                                           /* return error */
        }
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief      read multiple bytes
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ds1302_multiple_read(ds1302_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    
    for (i = 0; i < len; i++)                                                          /* loop */
    {
        res = a_ds1302_read(handle, (uint8_t)(reg + (i << 1) | 0x01), &buf[i]);        /* read one byte */
        if (res != 0)                                                                  /* check the result */
        {
            return 1;                                                                  /* read error */
        }
    }
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     burst write bytes
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ds1302_burst_write(ds1302_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    uint8_t j;
    uint8_t temp;
    uint8_t prev;
    
    prev = (1 << 7) | reg;                         /* set reg */
    res = handle->ce_gpio_write(1);                /* set ce high */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    temp = prev;                                   /* set reg */
    for (i = 0; i < 8; i++)                        /* loop */
    {
        if ((temp & 0x01) != 0)                    /* check lsb bit */
        {
            res = handle->io_gpio_write(1);        /* set io high */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        else
        {
            res = handle->io_gpio_write(0);        /* set io low */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        temp = temp >> 1;                          /* right shift 1 */
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(1);          /* set sclk high */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(0);          /* set sclk low */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
    }
    for (j = 0; j < len; j++)                      /* write all */
    {
        temp = buf[j];                             /* set data */
        for (i = 0; i < 8; i++)                    /* loop */
        {
            if ((temp & 0x01) != 0)                /* check the lsb bit */
            {
                res = handle->io_gpio_write(1);    /* set io high */
                if (res != 0)                      /* check the result */
                {
                    return 1;                      /* return error */
                }
            }
            else
            {
                res = handle->io_gpio_write(0);    /* set io low */
                if (res != 0)                      /* check the result */
                {
                    return 1;                      /* return error */
                }
            }
            temp = temp >> 1;                      /* right shift 1 */
            handle->delay_us(1);                   /* delay 1us */
            res = handle->sclk_gpio_write(1);      /* set sclk high */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
            handle->delay_us(1);                   /* delay 1us */
            res = handle->sclk_gpio_write(0);      /* set sclk low */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
    }
    res = handle->io_gpio_write(0);                /* set io low */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    res = handle->ce_gpio_write(0);                /* set ce low */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    
    return 0;                                      /* success return 0 */
}

/**
 * @brief      burst read bytes
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ds1302_burst_read(ds1302_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint8_t i;
    uint8_t j;
    uint8_t temp;
    uint8_t prev;
    
    prev = (1 << 7) | reg | 0x01;                  /* set reg */
    res = handle->ce_gpio_write(1);                /* set ce high */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    temp = prev;                                   /* set reg */
    for (i = 0; i < 8; i++)                        /* loop */
    {
        if ((temp & 0x01) != 0)                    /* check the lsb */
        {
            res = handle->io_gpio_write(1);        /* set io high */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        else
        {
            res = handle->io_gpio_write(0);        /* set io low */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        temp = temp >> 1;                          /* right shift 1 */
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(1);          /* set sclk high */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
        handle->delay_us(1);                       /* delay 1us */
        res = handle->sclk_gpio_write(0);          /* set sclk low */
        if (res != 0)                              /* check the result */
        {
            return 1;                              /* return error */
        }
    }
    for (j = 0; j < len; j++)                      /* read all */
    {
        temp = 0;                                  /* init temp 0 */
        for (i = 0; i < 8; i++)                    /* loop */
        {
            uint8_t level;
            
            temp = temp >> 1;                      /* right shift 1 */
            res = handle->io_gpio_read(&level);    /* read the level */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
            if (level != 0)                        /* check the level */
            {
                temp |= 1 << 7;                    /* set msb bit high */
            }
            else
            {
                temp &= ~(1 << 7);                 /* set msb bit low */
            }
            res = handle->sclk_gpio_write(1);      /* set sclk high */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
            handle->delay_us(1);                   /* delay 1us */
            res = handle->sclk_gpio_write(0);      /* set sclk low */
            if (res != 0)                          /* check the result */
            {
                return 1;                          /* return error */
            }
        }
        buf[j] = temp;                             /* save to buffer */
    }
    res = handle->ce_gpio_write(0);                /* set ce low */
    if (res != 0)                                  /* check the result */
    {
        return 1;                                  /* return error */
    }
    
    return 0;                                      /* success return 0 */
}

/**
 * @brief     hex to bcd
 * @param[in] val is the hex data
 * @return    bcd data
 * @note      none
 */
static uint8_t a_ds1302_hex2bcd(uint8_t val)
{
    uint8_t i, j, k;
    
    i = val / 10;            /* get tens place */
    j = val % 10;            /* get ones place */
    k = j + (i << 4);        /* set bcd */
    
    return k;                /* return bcd */
}

/**
 * @brief     bcd to hex
 * @param[in] val is the bcd data
 * @return    hex data
 * @note      none
 */
static uint8_t a_ds1302_bcd2hex(uint8_t val)
{
    uint8_t temp;
    
    temp = val & 0x0F;              /* get ones place */
    val = (val >> 4) & 0x0F;        /* get tens place */
    val = val * 10;                 /* set tens place */
    temp = temp + val;              /* get hex */
    
    return temp;                    /* return hex */
}

/**
 * @brief     set the current time
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 *            - 4 time is invalid
 * @note      none
 */
uint8_t ds1302_set_time(ds1302_handle_t *handle, ds1302_time_t *t)
{
    uint8_t res;
    uint8_t reg;
    uint16_t year;
    
    if (handle == NULL)                                                                                      /* check handle */
    {
        return 2;                                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                                 /* check handle initialization */
    {
        return 3;                                                                                            /* return error */
    }
    if (t == NULL)                                                                                           /* check time */
    {
        handle->debug_print("ds1302: time is null.\n");                                                      /* time is null */
        
        return 2;                                                                                            /* return error */
    }
    if (t->format == DS1302_FORMAT_12H)                                                                      /* if 12H */
    {
        if ((t->year < 2000) || (t->year > 2100))                                                            /* check year */
        {
            handle->debug_print("ds1302: year can't be over 2100 or less than 2000.\n");                     /* year can't be over 2100 or less than 2000 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->month == 0) || (t->month > 12))                                                              /* check month */
        {
            handle->debug_print("ds1302: month can't be zero or over than 12.\n");                           /* month can't be zero or over than 12 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->week == 0) || (t->week > 7))                                                                 /* check week */
        {
            handle->debug_print("ds1302: week can't be zero or over than 7.\n");                             /* week can't be zero or over than 7 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->date == 0) || (t->date > 31))                                                                /* check data */
        {
            handle->debug_print("ds1302: date can't be zero or over than 31.\n");                            /* date can't be zero or over than 31 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->hour < 1) || (t->hour > 12))                                                                 /* check hour */
        {
            handle->debug_print("ds1302: hour can't be over than 12 or less 1.\n");                          /* hour can't be over than 12 or less 1 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->minute > 59)                                                                                  /* check minute */
        {
            handle->debug_print("ds1302: minute can't be over than 59.\n");                                  /* minute can't be over than 59 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->second > 59)                                                                                  /* check second */
        {
            handle->debug_print("ds1302: second can't be over than 59.\n");                                  /* second can't be over than 59 */
            
            return 4;                                                                                        /* return error */
        }
    }
    else if (t->format == DS1302_FORMAT_24H)                                                                 /* if 24H */
    {
        if ((t->year < 2000) || (t->year > 2100))                                                            /* check year */
        {
            handle->debug_print("ds1302: year can't be over 2100 or less than 2000.\n");                     /* year can't be over 2100 or less than 2000 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->month == 0) || (t->month > 12))                                                              /* check month */
        {
            handle->debug_print("ds1302: month can't be zero or over than 12.\n");                           /* month can't be zero or over than 12 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->week == 0) || (t->week > 7))                                                                 /* check week */
        {
            handle->debug_print("ds1302: week can't be zero or over than 7.\n");                             /* week can't be zero or over than 7 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->date == 0) || (t->date > 31))                                                                /* check data */
        {
            handle->debug_print("ds1302: date can't be zero or over than 31.\n");                            /* date can't be zero or over than 31 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->hour > 23)                                                                                    /* check hour */
        {
            handle->debug_print("ds1302: hour can't be over than 23.\n");                                    /* hour can't be over than 23 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->minute > 59)                                                                                  /* check minute */
        {
            handle->debug_print("ds1302: minute can't be over than 59.\n");                                  /* minute can't be over than 59 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->second > 59)                                                                                  /* check second */
        {
            handle->debug_print("ds1302: second can't be over than 59.\n");                                  /* second can't be over than 59 */
            
            return 4;                                                                                        /* return error */
        }
    }
    else
    {
        handle->debug_print("ds1302: format is invalid.\n");                                                 /* format is invalid */
        
        return 4;                                                                                            /* return error */
    }
    
    res = a_ds1302_multiple_read(handle, DS1302_COMMAND_RTC | DS1302_REG_SECOND, &reg, 1);                   /* read second */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds1302: read second failed.\n");                                                /* read second failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_SECOND, 
                         a_ds1302_hex2bcd(t->second) | reg & (1 << 7));                                      /* write second */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds1302: write second failed.\n");                                               /* write second failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_MINUTE, a_ds1302_hex2bcd(t->minute));       /* write minute */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds1302: write minute failed.\n");                                               /* write minute failed */
        
        return 1;                                                                                            /* return error */
    }
    if (t->format == DS1302_FORMAT_12H)                                                                      /* if 12H */
    {
        reg = (uint8_t)((1 << 7) | (t->am_pm << 5) | a_ds1302_hex2bcd(t->hour));                             /* set hour in 12H */
    }
    else                                                                                                     /* if 24H */
    {
        reg = (0 << 7) | a_ds1302_hex2bcd(t->hour);                                                          /* set hour in 24H */
    }
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_HOUR, reg);                                 /* write hour */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds1302: write hour failed.\n");                                                 /* write hour failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_WEEK, a_ds1302_hex2bcd(t->week));           /* write week */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds1302: write week failed.\n");                                                 /* write week failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_DATE, a_ds1302_hex2bcd(t->date));           /* write data */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds1302: write date failed.\n");                                                 /* write date failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_MONTH, a_ds1302_hex2bcd(t->month));         /* write month and century */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds1302: write century and month failed.\n");                                    /* write century and month failed */
        
        return 1;                                                                                            /* return error */
    }
    year = t->year - 2000;                                                                                   /* year - 2000 */
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_YEAR, a_ds1302_hex2bcd((uint8_t)year));     /* write year */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds1302: write year failed.\n");                                                 /* write year failed */
        
        return 1;                                                                                            /* return error */
    }
    
    return 0;                                                                                                /* success return 0 */
}

/**
 * @brief      get the current time
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1302_get_time(ds1302_handle_t *handle, ds1302_time_t *t)
{
    uint8_t res;
    uint8_t buf[7];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    if (t == NULL)                                                                        /* check time */
    {
        handle->debug_print("ds1302: time is null.\n");                                   /* time is null */
        
        return 2;                                                                         /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 7);                                                  /* clear the buffer */
    res = a_ds1302_multiple_read(handle, DS1302_COMMAND_RTC | DS1302_REG_SECOND, 
                                (uint8_t *)buf, 7);                                       /* multiple_read */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ds1302: multiple read failed.\n");                           /* multiple read failed */
        
        return 1;                                                                         /* return error */
    }
    t->year = a_ds1302_bcd2hex(buf[6]) + 2000;                                            /* get year */
    t->month = a_ds1302_bcd2hex(buf[4] & 0x1F);                                           /* get month */
    t->week = a_ds1302_bcd2hex(buf[5] & 0x7);                                             /* get week */
    t->date = a_ds1302_bcd2hex(buf[3] & 0x3F);                                            /* get date */
    t->am_pm = (ds1302_am_pm_t)((buf[2] >> 5) & 0x01);                                    /* get am pm */
    t->format = (ds1302_format_t)((buf[2] >> 7) & 0x01);                                  /* get format */
    if (t->format == DS1302_FORMAT_12H)                                                   /* if 12H */
    {
        t->hour = a_ds1302_bcd2hex(buf[2] & 0x1F);                                        /* get hour */
    }
    else
    {
        t->hour = a_ds1302_bcd2hex(buf[2] & 0x3F);                                        /* get hour */
    }
    t->minute = a_ds1302_bcd2hex(buf[1]);                                                 /* get minute */
    t->second = a_ds1302_bcd2hex(buf[0] & (~(1 << 7)));                                   /* get second */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     enable or disable the oscillator
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set oscillator failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1302_set_oscillator(ds1302_handle_t *handle, ds1302_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ds1302_multiple_read(handle, DS1302_COMMAND_RTC | DS1302_REG_SECOND, 
                                 &prev, 1);                                       /* read second */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ds1302: read second failed.\n");                     /* read second failed */
        
        return 1;                                                                 /* return error */
    }
    prev &= ~(1 << 7);                                                            /* clear config */
    prev |= (!enable) << 7;                                                       /* set enable */
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_SECOND, 
                         prev);                                                   /* write second */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ds1302: write second failed.\n");                    /* write second failed */
        
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

/**
 * @brief      get the chip oscillator status
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get oscillator failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1302_get_oscillator(ds1302_handle_t *handle, ds1302_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds1302_multiple_read(handle, DS1302_COMMAND_RTC | DS1302_REG_SECOND, 
                                (uint8_t *)&prev, 1);                                        /* multiple read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds1302: read second failed.\n");                                /* read second failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (ds1302_bool_t)(!((prev >> 7) & 0x01));                                        /* get enable */
    
    return 0;                                                                                /* success return 0 */
}

/**
 * @brief     enable or disable write protect
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set write protect failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1302_set_write_protect(ds1302_handle_t *handle, ds1302_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_ds1302_multiple_read(handle, DS1302_COMMAND_RTC | DS1302_REG_CONTROL, 
                                 &prev, 1);                                        /* read control */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("ds1302: read control failed.\n");                     /* read control failed */
        
        return 1;                                                                  /* return error */
    }
    prev &= ~(1 << 7);                                                             /* clear config */
    prev |= enable << 7;                                                           /* set bool */
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_CONTROL, prev);   /* write control */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("ds1302: write control failed.\n");                    /* write control failed */
        
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      get write protect status
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get write protect failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1302_get_write_protect(ds1302_handle_t *handle, ds1302_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_ds1302_multiple_read(handle, DS1302_COMMAND_RTC | DS1302_REG_CONTROL, 
                                 &prev, 1);                                        /* read control */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("ds1302: read control failed.\n");                     /* read control failed */
        
        return 1;                                                                  /* return error */
    }
    *enable = (ds1302_bool_t)((prev >> 7) & 0x01);                                 /* set bool */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     set charge
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] charge is the set charge
 * @return    status code
 *            - 0 success
 *            - 1 set charge failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1302_set_charge(ds1302_handle_t *handle, uint8_t charge)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_ds1302_write(handle, DS1302_COMMAND_RTC | DS1302_REG_CHARGE, charge);        /* write charge */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("ds1302: write charge failed.\n");                           /* write charge failed */
        
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get charge
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[out] *charge points to a charge buffer
 * @return     status code
 *             - 0 success
 *             - 1 get charge failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1302_get_charge(ds1302_handle_t *handle, uint8_t *charge)
{
    uint8_t res;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = a_ds1302_multiple_read(handle, DS1302_COMMAND_RTC | DS1302_REG_CHARGE, charge, 1);        /* read charge */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("ds1302: read charge failed.\n");                                       /* read charge failed */
        
        return 1;                                                                                   /* return error */
    }
    
    return 0;                                                                                       /* success return 0 */
}

/**
 * @brief      read ram
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[in]  addr is the ram address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read ram failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 addr > 30
 *             - 5 len is invalid
 * @note       none
 */
uint8_t ds1302_read_ram(ds1302_handle_t *handle, uint8_t addr, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (addr > 30)                                                                 /* check addr */
    {
        handle->debug_print("ds1302: addr > 30.\n");                               /* addr > 30 */
        
        return 4;                                                                  /* return error */
    }
    if (addr + len - 1 > 30)                                                       /* check len */
    {
        handle->debug_print("ds1302: len is invalid.\n");                          /* len is invalid */
        
        return 5;                                                                  /* return error */
    }
    
    res = a_ds1302_multiple_read(handle, (uint8_t)(DS1302_COMMAND_RAM + addr),
                                 buf, len);                                        /* read ram */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("ds1302: read ram failed.\n");                         /* read ram failed */
        
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     write ram
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] addr is the ram address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr > 30
 *            - 5 len is invalid
 * @note      none
 */
uint8_t ds1302_write_ram(ds1302_handle_t *handle, uint8_t addr, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (addr > 30)                                                                 /* check addr */
    {
        handle->debug_print("ds1302: addr > 30.\n");                               /* addr > 30 */
        
        return 4;                                                                  /* return error */
    }
    if (addr + len - 1 > 30)                                                       /* check len */
    {
        handle->debug_print("ds1302: len is invalid.\n");                          /* len is invalid */
        
        return 5;                                                                  /* return error */
    }
    
    res = a_ds1302_multiple_write(handle, (uint8_t)(DS1302_COMMAND_RAM + addr),
                                  buf, len);                                       /* write ram */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("ds1302: write ram failed.\n");                        /* write ram failed */
        
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a ds1302 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 gpio initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t ds1302_init(ds1302_handle_t *handle)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                 /* check debug_print */
    {
        return 3;                                                    /* return error */
    }
    if (handle->ce_gpio_init == NULL)                                /* check ce_gpio_init */
    {
        handle->debug_print("ds1302: ce_gpio_init is null.\n");      /* ce_gpio_init is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->ce_gpio_deinit == NULL)                              /* check ce_gpio_deinit */
    {
        handle->debug_print("ds1302: ce_gpio_deinit is null.\n");    /* ce_gpio_deinit is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->ce_gpio_write == NULL)                               /* check ce_gpio_write */
    {
        handle->debug_print("ds1302: ce_gpio_write is null.\n");     /* ce_gpio_write is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->sclk_gpio_init == NULL)                              /* check sclk_gpio_init */
    {
        handle->debug_print("ds1302: sclk_gpio_init is null.\n");    /* sclk_gpio_init is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->sclk_gpio_deinit == NULL)                            /* check sclk_gpio_deinit */
    {
        handle->debug_print("ds1302: sclk_gpio_deinit is null.\n");  /* sclk_gpio_deinit is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->sclk_gpio_write == NULL)                             /* check sclk_gpio_write */
    {
        handle->debug_print("ds1302: sclk_gpio_write is null.\n");   /* sclk_gpio_write is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->io_gpio_init == NULL)                                /* check io_gpio_init */
    {
        handle->debug_print("ds1302: io_gpio_init is null.\n");      /* io_gpio_init is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->io_gpio_deinit == NULL)                              /* check io_gpio_deinit */
    {
        handle->debug_print("ds1302: io_gpio_deinit is null.\n");    /* io_gpio_deinit is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->io_gpio_write == NULL)                               /* check io_gpio_write */
    {
        handle->debug_print("ds1302: io_gpio_write is null.\n");     /* io_gpio_write is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->io_gpio_read == NULL)                                /* check io_gpio_read */
    {
        handle->debug_print("ds1302: io_gpio_read is null.\n");      /* io_gpio_read is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                    /* check delay_ms */
    {
        handle->debug_print("ds1302: delay_ms is null.\n");          /* delay_ms is null */
       
        return 3;                                                    /* return error */
    }
    
    if (handle->ce_gpio_init() != 0)                                 /* ce gpio init */
    {
        handle->debug_print("ds1302: ce gpio init failed.\n");       /* ce gpio init failed */
       
        return 1;                                                    /* return error */
    }
    if (handle->sclk_gpio_init() != 0)                               /* sclk gpio init */
    {
        handle->debug_print("ds1302: sclk gpio init failed.\n");     /* sclk gpio init failed */
        (void)handle->ce_gpio_deinit();                              /* ce gpio deinit */
        
        return 1;                                                    /* return error */
    }
    if (handle->io_gpio_init() != 0)                                 /* io gpio init */
    {
        handle->debug_print("ds1302: io gpio init failed.\n");       /* io gpio init failed */
        (void)handle->ce_gpio_deinit();                              /* ce gpio deinit */
        (void)handle->sclk_gpio_deinit();                            /* sclk gpio deinit */
        
        return 1;                                                    /* return error */
    }
    handle->inited = 1;                                              /* flag finish initialization */
    
    return 0;                                                        /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to a ds1302 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 gpio deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1302_deinit(ds1302_handle_t *handle)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if (handle->ce_gpio_deinit() != 0)                              /* ce gpio deinit */
    {
        handle->debug_print("ds1302: ce gpio deinit failed.\n");    /* ce gpio deinit failed */
       
        return 1;                                                   /* return error */
    }
    if (handle->sclk_gpio_deinit() != 0)                            /* sclk gpio deinit */
    {
        handle->debug_print("ds1302: sclk gpio deinit failed.\n");  /* sclk gpio deinit failed */
       
        return 1;                                                   /* return error */
    }
    if (handle->io_gpio_deinit() != 0)                              /* io gpio deinit */
    {
        handle->debug_print("ds1302: io gpio deinit failed.\n");    /* io gpio deinit failed */
       
        return 1;                                                   /* return error */
    }
    handle->inited = 0;                                             /* flag close */
    
    return 0;                                                       /* success return 0 */
}

/**
 * @brief     clock burst write
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len > 8
 * @note      none
 */
uint8_t ds1302_clock_burst_write(ds1302_handle_t *handle, uint8_t *buf, uint8_t len)
{
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }
    if (len > 8)                                                                                       /* check len */
    {
        handle->debug_print("ds1302: len > 8.\n");                                                     /* len > 8 */
        
        return 4;                                                                                      /* return error */
    }
    
    if (a_ds1302_burst_write(handle, DS1302_COMMAND_RTC | DS1302_COMMAND_BURST, buf, len) != 0)        /* write data */
    {
        return 1;                                                                                      /* return error */
    }
    
    return 0;                                                                                          /* success return 0 */
}

/**
 * @brief      clock burst read
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 len > 8
 * @note       none
 */
uint8_t ds1302_clock_burst_read(ds1302_handle_t *handle, uint8_t *buf, uint8_t len)
{
    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }
    if (len > 8)                                                                                      /* check len */
    {
        handle->debug_print("ds1302: len > 8.\n");                                                    /* len > 8 */
        
        return 4;                                                                                     /* return error */
    }
    
    if (a_ds1302_burst_read(handle, DS1302_COMMAND_RTC | DS1302_COMMAND_BURST, buf, len) != 0)        /* read data */
    {
        return 1;                                                                                     /* return error */
    }
    
    return 0;                                                                                         /* success return 0 */
}

/**
 * @brief     ram burst write
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len > 31
 * @note      none
 */
uint8_t ds1302_ram_burst_write(ds1302_handle_t *handle, uint8_t *buf, uint8_t len)
{
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }
    if (len > 31)                                                                                      /* check len */
    {
        handle->debug_print("ds1302: len > 31.\n");                                                    /* len > 31 */
        
        return 4;                                                                                      /* return error */
    }
    
    if (a_ds1302_burst_write(handle, DS1302_COMMAND_RAM | DS1302_COMMAND_BURST, buf, len) != 0)        /* write data */
    {
        return 1;                                                                                      /* return error */
    }
    
    return 0;                                                                                          /* success return 0 */
}

/**
 * @brief      ram burst read
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 len > 31
 * @note       none
 */
uint8_t ds1302_ram_burst_read(ds1302_handle_t *handle, uint8_t *buf, uint8_t len)
{
    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }
    if (len > 31)                                                                                     /* check len */
    {
        handle->debug_print("ds1302: len > 31.\n");                                                   /* len > 31 */
        
        return 4;                                                                                     /* return error */
    }
    
    if (a_ds1302_burst_read(handle, DS1302_COMMAND_RAM | DS1302_COMMAND_BURST, buf, len) != 0)        /* read data */
    {
        return 1;                                                                                     /* return error */
    }
    
    return 0;                                                                                         /* success return 0 */
}

/**
 * @brief     set the chip register
 * @param[in] *handle points to a ds1302 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1302_set_reg(ds1302_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if (a_ds1302_multiple_write(handle, reg, buf, len) != 0)        /* write data */
    {
        return 1;                                                   /* return error */
    }
    
    return 0;                                                       /* success return 0 */
}

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a ds1302 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1302_get_reg(ds1302_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    if (a_ds1302_multiple_read(handle, reg, buf, len) != 0)        /* read data */
    {
        return 1;                                                  /* return error */
    }
    
    return 0;                                                      /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a ds1302 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ds1302_info(ds1302_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ds1302_info_t));                         /* initialize ds1302 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "GPIO", 8);                            /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
