/*
 * File:   external_eeprom.c
 * Author: HP
 *
 * Created on 27 October, 2026, 7:58 AM
 */


#include "main.h"
#include "i2c.h"
#include "external_eeprom.h"

void write_external_eeprom(unsigned char address,
                           unsigned char data)
{
    i2c_start();

    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(address);
    i2c_write(data);

    i2c_stop();

    /* Allow the EEPROM write cycle to complete. */
    __delay_ms(5);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(address);

    i2c_rep_start();
    i2c_write(EEPROM_SLAVE_READ);

    data = i2c_read();

    i2c_stop();

    return data;
}