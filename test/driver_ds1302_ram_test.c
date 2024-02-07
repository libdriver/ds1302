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
 * @file      driver_ds1302_ram_test.c
 * @brief     driver ds1302 ram test source file
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

#include "driver_ds1302_ram_test.h"
#include <stdlib.h>

static ds1302_handle_t gs_handle;        /**< ds1302 handle */
static uint8_t gs_buf[31];               /**< buffer */
static uint8_t gs_check_buf[31];         /**< check buffer */

/**
 * @brief     ram test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ds1302_ram_test(uint32_t times)
{
    uint8_t res;
    uint32_t i;
    uint8_t j;
    ds1302_info_t info;
    
    /* link functions */
    DRIVER_DS1302_LINK_INIT(&gs_handle, ds1302_handle_t);
    DRIVER_DS1302_LINK_CE_GPIO_INIT(&gs_handle, ds1302_interface_ce_gpio_init);
    DRIVER_DS1302_LINK_CE_GPIO_DEINIT(&gs_handle, ds1302_interface_ce_gpio_deinit);
    DRIVER_DS1302_LINK_CE_GPIO_WRITE(&gs_handle, ds1302_interface_ce_gpio_write);
    DRIVER_DS1302_LINK_SCLK_GPIO_INIT(&gs_handle, ds1302_interface_sclk_gpio_init);
    DRIVER_DS1302_LINK_SCLK_GPIO_DEINIT(&gs_handle, ds1302_interface_sclk_gpio_deinit);
    DRIVER_DS1302_LINK_SCLK_GPIO_WRITE(&gs_handle, ds1302_interface_sclk_gpio_write);
    DRIVER_DS1302_LINK_IO_GPIO_INIT(&gs_handle, ds1302_interface_io_gpio_init);
    DRIVER_DS1302_LINK_IO_GPIO_DEINIT(&gs_handle, ds1302_interface_io_gpio_deinit);
    DRIVER_DS1302_LINK_CS_GPIO_WRITE(&gs_handle, ds1302_interface_io_gpio_write);
    DRIVER_DS1302_LINK_CS_GPIO_READ(&gs_handle, ds1302_interface_io_gpio_read);
    DRIVER_DS1302_LINK_DELAY_US(&gs_handle, ds1302_interface_delay_us);
    DRIVER_DS1302_LINK_DELAY_MS(&gs_handle, ds1302_interface_delay_ms);
    DRIVER_DS1302_LINK_DEBUG_PRINT(&gs_handle, ds1302_interface_debug_print);
    
    /* get ds1302 info */
    res = ds1302_info(&info);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print ds1302 info */
        ds1302_interface_debug_print("ds1302: chip is %s.\n", info.chip_name);
        ds1302_interface_debug_print("ds1302: manufacturer is %s.\n", info.manufacturer_name);
        ds1302_interface_debug_print("ds1302: interface is %s.\n", info.interface);
        ds1302_interface_debug_print("ds1302: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ds1302_interface_debug_print("ds1302: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ds1302_interface_debug_print("ds1302: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ds1302_interface_debug_print("ds1302: max current is %0.2fmA.\n", info.max_current_ma);
        ds1302_interface_debug_print("ds1302: max temperature is %0.1fC.\n", info.temperature_max);
        ds1302_interface_debug_print("ds1302: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start ram test */
    ds1302_interface_debug_print("ds1302: start ram test.\n");
    
    /* init ds1302 */
    res = ds1302_init(&gs_handle);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: init failed.\n");
        
        return 1;
    }
    
    /* set oscillator */
    res = ds1302_set_oscillator(&gs_handle, DS1302_BOOL_TRUE);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set oscillator failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable write protect */
    res = ds1302_set_write_protect(&gs_handle, DS1302_BOOL_FALSE);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set write protect failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    ds1302_interface_debug_print("ds1302: common ram test.\n");
    
    for (i = 0; i < times; i++)
    {
        /* rand the data */
        for (j = 0; j < 31; j++)
        {
            gs_buf[j] = rand() & 0xFF;
        }
        
        /* write ram */
        res = ds1302_write_ram(&gs_handle, 0x00, gs_buf, 31);
        if (res != 0)
        {
            ds1302_interface_debug_print("ds1302: write ram failed.\n");
            (void)ds1302_deinit(&gs_handle);
            
            return 1;
        }
        
        /* read ram */
        res = ds1302_read_ram(&gs_handle, 0x00, gs_check_buf, 31);
        if (res != 0)
        {
            ds1302_interface_debug_print("ds1302: read ram failed.\n");
            (void)ds1302_deinit(&gs_handle);
            
            return 1;
        }
        
        /* check data */
        for (j = 0; j < 31; j++)
        {
            if (gs_buf[j] != gs_check_buf[j])
            {
                ds1302_interface_debug_print("ds1302: check buffer failed.\n");
                (void)ds1302_deinit(&gs_handle);
                
                return 1;
            }
        }
        
        /* output */
        ds1302_interface_debug_print("ds1302: %d/%d check passed.\n", i + 1, times);
    }
    
    /* output */
    ds1302_interface_debug_print("ds1302: burst ram test.\n");
    
    for (i = 0; i < times; i++)
    {
        /* rand the data */
        for (j = 0; j < 31; j++)
        {
            gs_buf[j] = rand() & 0xFF;
        }
        
        /* write ram */
        res = ds1302_ram_burst_write(&gs_handle, gs_buf, 31);
        if (res != 0)
        {
            ds1302_interface_debug_print("ds1302: ram burst write failed.\n");
            (void)ds1302_deinit(&gs_handle);
            
            return 1;
        }
        
        /* read ram */
        res = ds1302_ram_burst_read(&gs_handle, gs_check_buf, 31);
        if (res != 0)
        {
            ds1302_interface_debug_print("ds1302: ram burst read failed.\n");
            (void)ds1302_deinit(&gs_handle);
            
            return 1;
        }
        
        /* check data */
        for (j = 0; j < 31; j++)
        {
            if (gs_buf[j] != gs_check_buf[j])
            {
                ds1302_interface_debug_print("ds1302: check buffer failed.\n");
                (void)ds1302_deinit(&gs_handle);
                
                return 1;
            }
        }
        
        /* output */
        ds1302_interface_debug_print("ds1302: %d/%d check passed.\n", i + 1, times);
    }
    
    /* finish ram test */
    ds1302_interface_debug_print("ds1302: finish ram test.\n");
    (void)ds1302_deinit(&gs_handle);
    
    return 0;
}
