/*******************************************************************************
 *
 * Synopsys DesignWare Sensor and Control IP Subsystem IO Software Driver and
 * documentation (hereinafter, "Software") is an Unsupported proprietary work
 * of Synopsys, Inc. unless otherwise expressly agreed to in writing between
 * Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto. You are permitted to use and
 * redistribute this Software in source and binary forms, with or without
 * modification, provided that redistributions of source code must retain this
 * notice. You may not view, use, disclose, copy or distribute this file or
 * any information contained herein except pursuant to this license grant from
 * Synopsys. If you do not agree with this notice, including the disclaimer
 * below, then you are not authorized to use the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * Modifications Copyright (c) 2015, Intel Corporation. All rights reserved.
 *
 ******************************************************************************/

#include <stdint.h>

#include "eiaextensions.h"
#include "spi_priv.h"
#include "common_spi.h"
#include "soc_gpio.h"
#include "portable.h"

#include "ss_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_FIFO_DEPTH (8UL)

#define SS_SPI_REG_WRITE(reg, x) \
    WRITE_ARC_REG((x), AR_IO_SPI_MST1_CTRL + (reg))
#define SS_SPI_REG_READ(reg) \
    READ_ARC_REG(AR_IO_SPI_MST1_CTRL + (reg))

void ss_spi_init()
{
    SET_ARC_BIT(AR_IO_CREG_MST0_CTRL, CREG_CLK_CTRL_SPI1);

    /* disable SPI controller */
    SS_SPI_REG_WRITE(SPIEN, SPI_DISABLE);
    /* enable controller clock to allow register writes */
    SS_SPI_REG_WRITE(CTRL, SPI_CLK_ENABLED);

    /* SPI Mode-0, 8-bit frame size */
    SS_SPI_REG_WRITE(CTRL, SPI_CLK_ENABLED | SPI_8_BIT | (SPI_EPROM_RD << 8));
    SS_SPI_REG_WRITE(TIMING, 16); /* 2MHz (32MHz/16) */

    /* Disable interrupts */
    SS_SPI_REG_WRITE(INTR_MASK, SPI_DISABLE_INT);
}

static inline
void spi_transmit(uint8_t *buf, size_t count, boolean_t waitCompletion) {
    while (count--)
        SS_SPI_REG_WRITE(DR, *buf++ | SPI_PUSH_DATA);
    /* Wait for transfer to complete */
    if (waitCompletion)
        while ((SS_SPI_REG_READ(TXFLR) > 0) || (SS_SPI_REG_READ(SR) & SPI_STATUS_BUSY));
}

static inline
void spi_receive(uint8_t *buf, size_t count) {
    while (count) {
        if (SS_SPI_REG_READ(RXFLR) > 0) {
            SS_SPI_REG_WRITE(DR, SPI_POP_DATA);
            *buf++ = SS_SPI_REG_READ(DR);
            count--;
        }
        SS_SPI_REG_READ(RXFLR); /* Extra read of RXFLR, apparently necessary */
    }
}

/* Polling-based SPI transfer to allow use within an ISR */
int ss_spi_xfer(uint8_t *buf, unsigned tx_cnt, unsigned rx_cnt)
{
    uint32_t ctrl = SS_SPI_REG_READ(CTRL) & SPI_NDF_SET_MASK & SPI_TMOD_SET_MASK;

    if (rx_cnt == 0)
        ctrl |= (SPI_TX_ONLY << 8);
    else if (tx_cnt == 0)
        ctrl |= (SPI_RX_ONLY << 8) | ((rx_cnt - 1) << 16);
    else
        ctrl |= (SPI_EPROM_RD << 8) | ((rx_cnt - 1) << 16);
    SS_SPI_REG_WRITE(CTRL, ctrl);

    // Assert the slave-select and start the SPI transfer
    SS_SPI_REG_WRITE(SPIEN, (SPI_SE_1 << 4) | SPI_ENABLE);

    if (tx_cnt)
        spi_transmit(buf, tx_cnt, !(rx_cnt)); // No wait for TX-RX transfers
    else
        SS_SPI_REG_WRITE(DR, SPI_PUSH_DATA | 0x56); // start rx-only transfer

    if (rx_cnt)
        spi_receive(buf, rx_cnt);

    // De-assert the slave-select and end the SPI transfer
    SS_SPI_REG_WRITE(SPIEN, 0);

    return 0;
}

#ifdef __cplusplus
}
#endif
