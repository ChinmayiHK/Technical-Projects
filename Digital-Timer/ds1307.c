/*
 * File:   ds1307.c
 * Author: HP
 *
 * Created on 26 October, 2026, 11:29 PM
 */

#include "main.h"
#include "ds1307.h"
#include "i2c.h"

void init_ds1307(void)
{
    unsigned char data;

    /* Start the RTC oscillator by clearing the CH bit. */
    data = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, data & 0x7F);

    /* Select 24-hour mode. */
    data = read_ds1307(HOUR_ADDR);
    write_ds1307(HOUR_ADDR, data & 0x3F);

    /* Disable square-wave output; no alarm output is needed here. */
    write_ds1307(CNTL_ADDR, 0x00);
}

void write_ds1307(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_write(RTC_SLAVE_WRITE);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
}

unsigned char read_ds1307(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_write(RTC_SLAVE_WRITE);
    i2c_write(address);

    i2c_rep_start();
    i2c_write(RTC_SLAVE_READ);
    data = i2c_read();
    i2c_stop();

    return data;
}