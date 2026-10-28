/* 
 * File:   external_eeprom.h
 * Author: HP
 *
 * Created on 27 October, 2026, 8:00 AM
 */

#ifndef EXTERNAL_EEPROM_H
#define EXTERNAL_EEPROM_H

#define EEPROM_SLAVE_WRITE  0xA0
#define EEPROM_SLAVE_READ   0xA1

unsigned char read_external_eeprom(unsigned char address);
void write_external_eeprom(unsigned char address,
                           unsigned char data);

#endif
