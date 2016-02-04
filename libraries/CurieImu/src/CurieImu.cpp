/*
 * Copyright (c) 2015 Intel Corporation.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "CurieImu.h"
#include "internal/ss_spi.h"
#include "interrupt.h"

#define BMI160_GPIN_AON_PIN 4

/******************************************************************************/

/** Power on and prepare for general usage.
 * This will prepare the SPI communication interface for accessing the BMI160
 * on the Curie module, before calling BMI160::initialize() to activate the
 * BMI160 accelerometer and gyroscpoe with default settings.
 */
void CurieImuClass::initialize()
{
    /* Configure pin-mux settings on the Intel Curie module to 
     * enable SPI mode usage */
    SET_PIN_MODE(35, QRK_PMUX_SEL_MODEA); // SPI1_SS_MISO 
    SET_PIN_MODE(36, QRK_PMUX_SEL_MODEA); // SPI1_SS_MOSI
    SET_PIN_MODE(37, QRK_PMUX_SEL_MODEA); // SPI1_SS_SCK
    SET_PIN_MODE(38, QRK_PMUX_SEL_MODEA); // SPI1_SS_CS_B[0]
 
    ss_spi_init();

    /* Perform a dummy read from 0x7f to switch to spi interface */
    uint8_t dummy_reg = 0x7F;
    serial_buffer_transfer(&dummy_reg, 1, 1);

    /* The SPI interface is ready - now invoke the base class initialization */
    BMI160Class::initialize();
}

/** Provides a serial buffer transfer implementation for the BMI160 base class
 *  to use for accessing device registers.  This implementation uses the SPI
 *  bus on the Intel Curie module to communicate with the BMI160.
 */
int CurieImuClass::serial_buffer_transfer(uint8_t *buf, unsigned tx_cnt, unsigned rx_cnt)
{
    int flags, status;

    if (rx_cnt) /* For read transfers, assume 1st byte contains register address */
        buf[0] |= (1 << BMI160_SPI_READ_BIT);

    /* Lock interrupts here to
     * - avoid concurrent access to the SPI bus
     * - avoid delays in SPI transfer due to unrelated interrupts
     */
    flags = interrupt_lock();
    status = ss_spi_xfer(buf, tx_cnt, rx_cnt);
    interrupt_unlock(flags);

    return status;
}

/** Interrupt handler for interrupts from PIN1 on the BMI160
 *  Calls a user callback if available.  The user callback is
 *  responsible for checking the source of the interrupt using
 *  the relevant API functions from the BMI160Class base class.
 */
void bmi160_pin1_isr(void)
{
    soc_gpio_mask_interrupt(SOC_GPIO_AON, BMI160_GPIN_AON_PIN);
    //if (CurieImu._user_callback)
        //CurieImu._user_callback();
    soc_gpio_unmask_interrupt(SOC_GPIO_AON, BMI160_GPIN_AON_PIN);
}

/** Stores a user callback, and enables PIN1 interrupts from the
 *  BMI160 module.
 */
void CurieImuClass::attachInterrupt(void (*callback)(void))
{
    gpio_cfg_data_t cfg;

    _user_callback = callback;

    memset(&cfg, 0, sizeof(gpio_cfg_data_t));
    cfg.gpio_type = GPIO_INTERRUPT;
    cfg.int_type = EDGE;
    cfg.int_polarity = ACTIVE_LOW;
    cfg.int_debounce = DEBOUNCE_ON;
    cfg.gpio_cb = bmi160_pin1_isr;
    soc_gpio_set_config(SOC_GPIO_AON, BMI160_GPIN_AON_PIN, &cfg);

    setInterruptMode(1);  // Active-Low
    setInterruptDrive(0); // Push-Pull
    setInterruptLatch(BMI160_LATCH_MODE_10_MS); // 10ms pulse
    setIntEnabled(true);
}

/** Disables PIN1 interrupts from the BMI160 module.
 */
void CurieImuClass::detachInterrupt(void)
{
    setIntEnabled(false);

    soc_gpio_deconfig(SOC_GPIO_AON, BMI160_GPIN_AON_PIN);
}

/* Pre-instantiated Object for this class */
//CurieImuClass CurieImu;
