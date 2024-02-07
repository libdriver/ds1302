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
 * @file      driver_ds1302_advance.h
 * @brief     driver ds1302 advance header file
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
  
#ifndef DRIVER_DS1302_ADVANCE_H
#define DRIVER_DS1302_ADVANCE_H

#include "driver_ds1302_interface.h"
#include <time.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ds1302_example_driver
 * @{
 */

/**
 * @brief ds1302 burst type enumeration definition
 */
typedef enum
{
    DS1302_BURST_TYPE_CLOCK = 0x00,        /**< clock burst type */
    DS1302_BURST_TYPE_RAM   = 0x01,        /**< ram burst type */
} ds1302_burst_type_t;

/**
 * @brief  advance example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ds1302_advance_init(void);

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ds1302_advance_deinit(void);

/**
 * @brief     advance example set the time
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t ds1302_advance_set_time(ds1302_time_t *t);

/**
 * @brief     advance example set the time by a unix timestamp
 * @param[in] timestamp is a unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t ds1302_advance_set_timestamp(time_t timestamp);

/**
 * @brief     advance example set the local time zone
 * @param[in] zone is the local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t ds1302_advance_set_timestamp_time_zone(int8_t zone);

/**
 * @brief      advance example get the time
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t ds1302_advance_get_time(ds1302_time_t *t);

/**
 * @brief      advance example get the time in a unix timestamp
 * @param[out] *timestamp points to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t ds1302_advance_get_timestamp(time_t *timestamp);

/**
 * @brief      advance example get the local time zone
 * @param[out] *zone points to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t ds1302_advance_get_timestamp_time_zone(int8_t *zone);

/**
 * @brief      advance example get the ascii time
 * @param[out] *buf points to an ascii buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ds1302_advance_get_ascii_time(char *buf, uint8_t len);

/**
 * @brief      advance example read ram
 * @param[in]  addr is the ram address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read ram failed
 * @note       none
 */
uint8_t ds1302_advance_read_ram(uint8_t addr, uint8_t *buf, uint8_t len);

/**
 * @brief     advance example write ram
 * @param[in] addr is the ram address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write ram failed
 * @note      none
 */
uint8_t ds1302_advance_write_ram(uint8_t addr, uint8_t *buf, uint8_t len);

/**
 * @brief     advance example set charge
 * @param[in] charge is the set charge
 * @return    status code
 *            - 0 success
 *            - 1 set charge failed
 * @note      charge:
 *            DS1302_CHARGE_DISABLE
 *            DS1302_CHARGE_ENABLE | (DS1302_CHARGE_1_DIODE or DS1302_CHARGE_2_DIODE) | (DS1302_CHARGE_2K or DS1302_CHARGE_4K or DS1302_CHARGE_8K)
 */
uint8_t ds1302_advance_set_charge(uint8_t charge);

/**
 * @brief      advance example get charge
 * @param[out] *charge points to a charge buffer
 * @return     status code
 *             - 0 success
 *             - 1 get charge failed
 * @note       none
 */
uint8_t ds1302_advance_get_charge(uint8_t *charge);

/**
 * @brief     advance example burst write
 * @param[in] type is the burst type
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 burst write failed
 * @note      none
 */
uint8_t ds1302_advance_burst_write(ds1302_burst_type_t type, uint8_t *buf, uint8_t len);

/**
 * @brief      advance example burst read
 * @param[in]  type is the burst type
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 burst read failed
 * @note       none
 */
uint8_t ds1302_advance_burst_read(ds1302_burst_type_t type, uint8_t *buf, uint8_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
