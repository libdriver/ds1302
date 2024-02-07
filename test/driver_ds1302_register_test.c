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
 * @file      driver_ds1302_register_test.c
 * @brief     driver ds1302 register test source file
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
#include <stdlib.h>

static ds1302_handle_t gs_handle;        /**< ds1302 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ds1302_register_test(void)
{
    uint8_t res;
    uint8_t reg;
    uint8_t reg_check;
    uint8_t buf[8];
    uint8_t buf_check[8];
    ds1302_info_t info;
    ds1302_time_t time_in;
    ds1302_time_t time_out;
    ds1302_bool_t enable;
    
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
    
    /* start register test */
    ds1302_interface_debug_print("ds1302: start register test.\n");
    
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
    
    /* ds1302_set_time/ds1302_get_time test */
    ds1302_interface_debug_print("ds1302: ds1302_set_time/ds1302_get_time test.\n");
    
    /* 12H format */
    time_in.format = DS1302_FORMAT_12H;
    time_in.am_pm = DS1302_PM;
    time_in.year = rand() % 100 + 2000;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7 + 1;
    time_in.hour = rand() % 11 + 1;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds1302_interface_debug_print("ds1302: set time %04d-%02d-%02d PM %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res  = ds1302_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set time failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    res = ds1302_get_time(&gs_handle, &time_out);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: get time failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: check time %s.\n", (memcmp((uint8_t *)&time_in, (uint8_t *)&time_out, sizeof(ds1302_time_t)) == 0) ? "ok" : "error");
    
    /* 24H format */
    time_in.format = DS1302_FORMAT_24H;
    time_in.am_pm = DS1302_AM;
    time_in.year = rand() % 100 + 2000;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds1302_interface_debug_print("ds1302: set time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res  = ds1302_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set time failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    res = ds1302_get_time(&gs_handle, &time_out);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: get time failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: check time %s.\n", (memcmp((uint8_t *)&time_in, (uint8_t *)&time_out, sizeof(ds1302_time_t)) == 0) ? "ok" : "error");
    
    /* ds1302_set_charge/ds1302_get_charge test */
    ds1302_interface_debug_print("ds1302: ds1302_set_charge/ds1302_get_charge test.\n");
    
    /* set charge */
    reg = DS1302_CHARGE_ENABLE | DS1302_CHARGE_2_DIODE | DS1302_CHARGE_8K;
    res = ds1302_set_charge(&gs_handle, reg);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set charge failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: set charge 0x%02X.\n", reg);
    res = ds1302_get_charge(&gs_handle, &reg_check);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: get charge failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: check time %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* disable charge */
    reg = DS1302_CHARGE_DISABLE;
    res = ds1302_set_charge(&gs_handle, reg);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set charge failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: set charge 0x%02X.\n", reg);
    res = ds1302_get_charge(&gs_handle, &reg_check);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: get charge failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: check time %s.\n", (reg == reg_check) ? "ok" : "error");
    
    /* ds1302_clock_burst_write/ds1302_clock_burst_read test */
    ds1302_interface_debug_print("ds1302: ds1302_clock_burst_write/ds1302_clock_burst_read test.\n");
    
    buf[0] = 0x01;
    buf[1] = 0x02;
    buf[2] = 0x03;
    buf[3] = 0x04;
    buf[4] = 0x05;
    buf[5] = 0x06;
    buf[6] = 0x07;
    buf[7] = 0x00;
    res = ds1302_clock_burst_write(&gs_handle, buf, 8);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: clock burst write failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: set clock 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                 buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
    res = ds1302_clock_burst_read(&gs_handle, buf_check, 8);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: clock burst read failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: check clock burst %s.\n", 
                                (memcmp((uint8_t *)buf, (uint8_t *)buf_check, sizeof(uint8_t) * 8) == 0) ? "ok" : "error");
    
    /* ds1302_set_write_protect/ds1302_get_write_protect test */
    ds1302_interface_debug_print("ds1302: ds1302_set_write_protect/ds1302_get_write_protect test.\n");
    
    /* disable write protect */
    res = ds1302_set_write_protect(&gs_handle, DS1302_BOOL_FALSE);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set write protect failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: disable write protect.\n");
    res = ds1302_get_write_protect(&gs_handle, &enable);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: get write protect failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: check write protect %s.\n", (enable == DS1302_BOOL_FALSE) ? "ok" : "error");
    
    /* enable write protect */
    res = ds1302_set_write_protect(&gs_handle, DS1302_BOOL_TRUE);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set write protect failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: enable write protect.\n");
    res = ds1302_get_write_protect(&gs_handle, &enable);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: get write protect failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: check write protect %s.\n", (enable == DS1302_BOOL_TRUE) ? "ok" : "error");
    
    /* ds1302_set_oscillator/ds1302_get_oscillator test */
    ds1302_interface_debug_print("ds1302: ds1302_set_oscillator/ds1302_get_oscillator test.\n");
    
    /* enable */
    res = ds1302_set_oscillator(&gs_handle, DS1302_BOOL_TRUE);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: set oscillator failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: enable oscillator.\n");
    res = ds1302_get_oscillator(&gs_handle, &enable);
    if (res != 0)
    {
        ds1302_interface_debug_print("ds1302: get oscillator failed.\n");
        (void)ds1302_deinit(&gs_handle);
        
        return 1;
    }
    ds1302_interface_debug_print("ds1302: check oscillator %s.\n", (enable == DS1302_BOOL_TRUE) ? "ok" : "error");
    
    /* finish register test */
    ds1302_interface_debug_print("ds1302: finish register test.\n");
    (void)ds1302_deinit(&gs_handle);
    
    return 0;
}
