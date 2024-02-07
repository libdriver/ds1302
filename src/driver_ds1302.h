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
 * @file      driver_ds1302.h
 * @brief     driver ds1302 header file
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

#ifndef DRIVER_DS1302_H
#define DRIVER_DS1302_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ds1302_driver ds1302 driver function
 * @brief    ds1302 driver modules
 * @{
 */

/**
 * @addtogroup ds1302_basic_driver
 * @{
 */

/**
 * @brief ds1302 bool enumeration definition
 */
typedef enum
{
    DS1302_BOOL_FALSE = 0x00,        /**< disable function */
    DS1302_BOOL_TRUE  = 0x01,        /**< enable function */
} ds1302_bool_t;

/**
 * @brief ds1302 am pm enumeration definition
 */
typedef enum
{
    DS1302_AM = 0x00,        /**< am */
    DS1302_PM = 0x01,        /**< pm */
} ds1302_am_pm_t;

/**
 * @brief ds1302 format enumeration definition
 */
typedef enum
{
    DS1302_FORMAT_12H = 0x01,        /**< 12h format */
    DS1302_FORMAT_24H = 0x00,        /**< 24h format */
} ds1302_format_t;

/**
 * @brief ds1302 charge enumeration definition
 */
typedef enum
{
    DS1302_CHARGE_ENABLE  = (0xA << 4),      /**< enable charge */
    DS1302_CHARGE_DISABLE = (0x0 << 0),      /**< disable charge */
    DS1302_CHARGE_1_DIODE = (1 << 2),        /**< 1 diode */
    DS1302_CHARGE_2_DIODE = (2 << 2),        /**< 2 diode */
    DS1302_CHARGE_2K      = (1 << 0),        /**< 2k */
    DS1302_CHARGE_4K      = (2 << 0),        /**< 4k */
    DS1302_CHARGE_8K      = (3 << 0),        /**< 8k */
} ds1302_charge_t;

/**
 * @brief ds1302 time structure definition
 */
typedef struct ds1302_time_s
{
    uint16_t year;                 /**< year */
    uint8_t month;                 /**< month */
    uint8_t week;                  /**< week */
    uint8_t date;                  /**< date */
    uint8_t hour;                  /**< hour */
    uint8_t minute;                /**< minute */
    uint8_t second;                /**< second */
    ds1302_format_t format;        /**< data format */
    ds1302_am_pm_t am_pm;          /**< am pm */
} ds1302_time_t;

/**
 * @brief ds1302 handle structure definition
 */
typedef struct ds1302_handle_s
{
    uint8_t (*ce_gpio_init)(void);                          /**< point to a ce_gpio_init function address */
    uint8_t (*ce_gpio_deinit)(void);                        /**< point to a ce_gpio_deinit function address */
    uint8_t (*ce_gpio_write)(uint8_t value);                /**< point to a ce_gpio_write function address */
    uint8_t (*sclk_gpio_init)(void);                        /**< point to an sclk_gpio_init function address */
    uint8_t (*sclk_gpio_deinit)(void);                      /**< point to an sclk_gpio_deinit function address */
    uint8_t (*sclk_gpio_write)(uint8_t value);              /**< point to an sclk_gpio_write function address */
    uint8_t (*io_gpio_init)(void);                          /**< point to an io_gpio_init function address */
    uint8_t (*io_gpio_deinit)(void);                        /**< point to an io_gpio_deinit function address */
    uint8_t (*io_gpio_write)(uint8_t value);                /**< point to an io_gpio_write function address */
    uint8_t (*io_gpio_read)(uint8_t *value);                /**< point to an io_gpio_read function address */
    void (*debug_print)(const char *const fmt, ...);        /**< point to a debug_print function address */
    void (*delay_ms)(uint32_t ms);                          /**< point to a delay_ms function address */
    void (*delay_us)(uint32_t us);                          /**< point to a delay_us function address */
    uint8_t inited;                                         /**< inited flag */
} ds1302_handle_t;

/**
 * @brief ds1302 information structure definition
 */
typedef struct ds1302_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ds1302_info_t;

/**
 * @}
 */

/**
 * @defgroup ds1302_link_driver ds1302 link driver function
 * @brief    ds1302 link driver modules
 * @ingroup  ds1302_driver
 * @{
 */

/**
 * @brief     initialize ds1302_handle_t structure
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] STRUCTURE is ds1302_handle_t
 * @note      none
 */
#define DRIVER_DS1302_LINK_INIT(HANDLE, STRUCTURE)          memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link ce_gpio_init function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a ce_gpio_init function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_CE_GPIO_INIT(HANDLE, FUC)        (HANDLE)->ce_gpio_init = FUC

/**
 * @brief     link ce_gpio_deinit function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a ce_gpio_deinit function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_CE_GPIO_DEINIT(HANDLE, FUC)      (HANDLE)->ce_gpio_deinit = FUC

/**
 * @brief     link ce_gpio_write function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a ce_gpio_write function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_CE_GPIO_WRITE(HANDLE, FUC)       (HANDLE)->ce_gpio_write = FUC

/**
 * @brief     link sclk_gpio_init function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a sclk_gpio_init function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_SCLK_GPIO_INIT(HANDLE, FUC)      (HANDLE)->sclk_gpio_init = FUC

/**
 * @brief     link sclk_gpio_deinit function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a sclk_gpio_deinit function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_SCLK_GPIO_DEINIT(HANDLE, FUC)    (HANDLE)->sclk_gpio_deinit = FUC

/**
 * @brief     link sclk_gpio_write function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a sclk_gpio_write function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_SCLK_GPIO_WRITE(HANDLE, FUC)     (HANDLE)->sclk_gpio_write = FUC

/**
 * @brief     link io_gpio_init function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a io_gpio_init function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_IO_GPIO_INIT(HANDLE, FUC)        (HANDLE)->io_gpio_init = FUC

/**
 * @brief     link io_gpio_deinit function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a io_gpio_deinit function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_IO_GPIO_DEINIT(HANDLE, FUC)      (HANDLE)->io_gpio_deinit = FUC

/**
 * @brief     link ce_gpio_write function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a ce_gpio_write function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_CS_GPIO_WRITE(HANDLE, FUC)       (HANDLE)->io_gpio_write = FUC

/**
 * @brief     link ce_gpio_read function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a ce_gpio_read function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_CS_GPIO_READ(HANDLE, FUC)        (HANDLE)->io_gpio_read = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_DEBUG_PRINT(HANDLE, FUC)         (HANDLE)->debug_print = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_DELAY_MS(HANDLE, FUC)            (HANDLE)->delay_ms = FUC

/**
 * @brief     link delay_us function
 * @param[in] HANDLE points to a ds1302 handle structure
 * @param[in] FUC points to a delay_us function address
 * @note      none
 */
#define DRIVER_DS1302_LINK_DELAY_US(HANDLE, FUC)            (HANDLE)->delay_us = FUC

/**
 * @}
 */

/**
 * @defgroup ds1302_basic_driver ds1302 basic driver function
 * @brief    ds1302 basic driver modules
 * @ingroup  ds1302_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a ds1302 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ds1302_info(ds1302_info_t *info);

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
uint8_t ds1302_init(ds1302_handle_t *handle);

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
uint8_t ds1302_deinit(ds1302_handle_t *handle);

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
uint8_t ds1302_set_time(ds1302_handle_t *handle, ds1302_time_t *t);

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
uint8_t ds1302_get_time(ds1302_handle_t *handle, ds1302_time_t *t);

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
uint8_t ds1302_set_oscillator(ds1302_handle_t *handle, ds1302_bool_t enable);

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
uint8_t ds1302_get_oscillator(ds1302_handle_t *handle, ds1302_bool_t *enable);

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
uint8_t ds1302_set_write_protect(ds1302_handle_t *handle, ds1302_bool_t enable);

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
uint8_t ds1302_get_write_protect(ds1302_handle_t *handle, ds1302_bool_t *enable);

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
uint8_t ds1302_set_charge(ds1302_handle_t *handle, uint8_t charge);

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
uint8_t ds1302_get_charge(ds1302_handle_t *handle, uint8_t *charge);

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
uint8_t ds1302_read_ram(ds1302_handle_t *handle, uint8_t addr, uint8_t *buf, uint8_t len);

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
uint8_t ds1302_write_ram(ds1302_handle_t *handle, uint8_t addr, uint8_t *buf, uint8_t len);

/**
 * @}
 */

/**
 * @defgroup ds1302_advance_driver ds1302 advance driver function
 * @brief    ds1302 advance driver modules
 * @ingroup  ds1302_driver
 * @{
 */

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
uint8_t ds1302_clock_burst_write(ds1302_handle_t *handle, uint8_t *buf, uint8_t len);

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
uint8_t ds1302_clock_burst_read(ds1302_handle_t *handle, uint8_t *buf, uint8_t len);

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
uint8_t ds1302_ram_burst_write(ds1302_handle_t *handle, uint8_t *buf, uint8_t len);

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
uint8_t ds1302_ram_burst_read(ds1302_handle_t *handle, uint8_t *buf, uint8_t len);

/**
 * @}
 */

/**
 * @defgroup ds1302_extern_driver ds1302 extern driver function
 * @brief    ds1302 extern driver modules
 * @ingroup  ds1302_driver
 * @{
 */

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
uint8_t ds1302_set_reg(ds1302_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len);

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
uint8_t ds1302_get_reg(ds1302_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
