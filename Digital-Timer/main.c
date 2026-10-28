/*
 * File:   main.c
 * Author: HP
 *
 * Created on 26 October, 2026, 11:25 PM
 */


#include "main.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"

#define UP_KEY          1
#define DOWN_KEY        2
#define LEFT_KEY        3
#define RIGHT_KEY       4

#define DEFAULT_SCREEN  0
#define MAIN_MENU       1
#define SET_TIME_SCREEN 2

#define LONG_PRESS_COUNT 40

#define ALARM_LIST_SCREEN  3
#define ALARM_EDIT_SCREEN  4

#define MAX_ALARMS         8

#define ALARM_EEPROM_MAGIC_ADDR  0x00
#define ALARM_EEPROM_BASE        0x10
#define ALARM_RECORD_SIZE        7
#define ALARM_EEPROM_MAGIC       0xA5

#define ALARM_ONCE    0
#define ALARM_DAILY   1
#define ALARM_WEEKLY  2

typedef struct
{
    unsigned char hour;
    unsigned char minute;
    unsigned char type;
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char enabled;
} Alarm;

static unsigned char screen = DEFAULT_SCREEN;
static unsigned char menu_index = 0;
static unsigned char previous_key = 0xFF;
static unsigned char pressed_key = 0xFF;
static unsigned char long_press_handled = 0;
static unsigned int hold_count = 0;

/* Time and date being edited */
static unsigned char edit_hour;
static unsigned char edit_min;
static unsigned char edit_sec;
static unsigned char edit_date;
static unsigned char edit_month;
static unsigned char edit_year;
static unsigned char selected_field = 0;

static unsigned int blink_count = 0;
static unsigned char blink_on = 1;

/* Alarm Variables */
static Alarm alarms[MAX_ALARMS];

static unsigned char selected_alarm = 0;
static unsigned char alarm_field = 0;
static Alarm edit_alarm;

static unsigned char get_days_in_month(unsigned char month,
                                       unsigned char year);

static unsigned char bcd_to_decimal(unsigned char value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}

static unsigned char decimal_to_bcd(unsigned char value)
{
    return ((value / 10) << 4) | (value % 10);
}

static unsigned char alarm_eeprom_address(
    unsigned char index,
    unsigned char offset)
{
    return ALARM_EEPROM_BASE +
           (index * ALARM_RECORD_SIZE) + offset;
}

static void save_alarm_to_eeprom(unsigned char index)
{
    unsigned char address;

    address = alarm_eeprom_address(index, 0);
    write_external_eeprom(address, alarms[index].hour);

    address = alarm_eeprom_address(index, 1);
    write_external_eeprom(address, alarms[index].minute);

    address = alarm_eeprom_address(index, 2);
    write_external_eeprom(address, alarms[index].type);

    address = alarm_eeprom_address(index, 3);
    write_external_eeprom(address, alarms[index].day);

    address = alarm_eeprom_address(index, 4);
    write_external_eeprom(address, alarms[index].month);

    address = alarm_eeprom_address(index, 5);
    write_external_eeprom(address, alarms[index].year);

    address = alarm_eeprom_address(index, 6);
    write_external_eeprom(address, alarms[index].enabled);
}

static void load_alarms_from_eeprom(void)
{
    unsigned char i;
    unsigned char address;

    if (read_external_eeprom(ALARM_EEPROM_MAGIC_ADDR)
        != ALARM_EEPROM_MAGIC)
    {
        /* First initialization: all alarm slots disabled. */
        for (i = 0; i < MAX_ALARMS; i++)
        {
            alarms[i].hour = 0;
            alarms[i].minute = 0;
            alarms[i].type = ALARM_DAILY;
            alarms[i].day = 1;
            alarms[i].month = 1;
            alarms[i].year = 0;
            alarms[i].enabled = 0;

            save_alarm_to_eeprom(i);
        }

        write_external_eeprom(
            ALARM_EEPROM_MAGIC_ADDR,
            ALARM_EEPROM_MAGIC);
    }
    else
    {
        for (i = 0; i < MAX_ALARMS; i++)
        {
            address = alarm_eeprom_address(i, 0);
            alarms[i].hour =
                read_external_eeprom(address);

            address = alarm_eeprom_address(i, 1);
            alarms[i].minute =
                read_external_eeprom(address);

            address = alarm_eeprom_address(i, 2);
            alarms[i].type =
                read_external_eeprom(address);

            address = alarm_eeprom_address(i, 3);
            alarms[i].day =
                read_external_eeprom(address);

            address = alarm_eeprom_address(i, 4);
            alarms[i].month =
                read_external_eeprom(address);

            address = alarm_eeprom_address(i, 5);
            alarms[i].year =
                read_external_eeprom(address);

            address = alarm_eeprom_address(i, 6);
            alarms[i].enabled =
                read_external_eeprom(address);

            /* Reject corrupted or uninitialized records. */
            if (alarms[i].hour > 23 ||
                alarms[i].minute > 59 ||
                alarms[i].type > ALARM_WEEKLY ||
                alarms[i].enabled > 1 ||
                (alarms[i].type == ALARM_WEEKLY &&
                 (alarms[i].day < 1 || alarms[i].day > 7)) ||
                (alarms[i].type == ALARM_ONCE &&
                 (alarms[i].month < 1 ||
                  alarms[i].month > 12 ||
                  alarms[i].day < 1 ||
                  alarms[i].day >
                  get_days_in_month(alarms[i].month, alarms[i].year))))
            {
                alarms[i].enabled = 0;
            }
        }
    }
}

static unsigned char get_days_in_month(unsigned char month,
                                       unsigned char year)
{
    switch (month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;

        case 4:
        case 6:
        case 9:
        case 11:
            return 30;

        case 2:
            /* Leap year: year is stored as 00?99 */
            if ((year % 4) == 0)
                return 29;
            else
                return 28;

        default:
            return 31;
    }
}

static void init_config(void)
{
    init_i2c();
    init_clcd();
    init_matrix_keypad();
    init_ds1307();
    load_alarms_from_eeprom();
}

static void display_default_screen(void)
{
    unsigned char sec, min, hour;
    unsigned char date, month, year;

    sec = bcd_to_decimal(read_ds1307(SEC_ADDR) & 0x7F);
    min = bcd_to_decimal(read_ds1307(MIN_ADDR) & 0x7F);
    hour = bcd_to_decimal(read_ds1307(HOUR_ADDR) & 0x3F);

    date = bcd_to_decimal(read_ds1307(DATE_ADDR) & 0x3F);
    month = bcd_to_decimal(read_ds1307(MONTH_ADDR) & 0x1F);
    year = bcd_to_decimal(read_ds1307(YEAR_ADDR));

    clcd_print("TIME:  ", LINE1(0));

    clcd_putch((hour / 10) + '0', LINE1(6));
    clcd_putch((hour % 10) + '0', LINE1(7));
    clcd_putch(':', LINE1(8));
    clcd_putch((min / 10) + '0', LINE1(9));
    clcd_putch((min % 10) + '0', LINE1(10));
    clcd_putch(':', LINE1(11));
    clcd_putch((sec / 10) + '0', LINE1(12));
    clcd_putch((sec % 10) + '0', LINE1(13));

    clcd_print("DATE:", LINE2(0));

    clcd_putch((date / 10) + '0', LINE2(5));
    clcd_putch((date % 10) + '0', LINE2(6));
    clcd_putch('/', LINE2(7));
    clcd_putch((month / 10) + '0', LINE2(8));
    clcd_putch((month % 10) + '0', LINE2(9));
    clcd_putch('/', LINE2(10));
    clcd_putch((year / 10) + '0', LINE2(11));
    clcd_putch((year % 10) + '0', LINE2(12));
}

static void display_main_menu(void)
{
    clcd_print("MAIN MENU       ", LINE1(0));

    if (menu_index == 0)
    {
        clcd_print(">Set Time & Date", LINE2(0));
    }
    else
    {
        clcd_print(">Set/View Alarms", LINE2(0));
    }
}

static void display_alarm_list(void)
{
    char type;

    clcd_print("SELECT ALARM", LINE1(0));
    clcd_print("                ", LINE2(0));

    clcd_putch('0' + selected_alarm + 1, LINE2(0));
    clcd_print(": ", LINE2(1));

    if (alarms[selected_alarm].enabled)
    {
        clcd_putch((alarms[selected_alarm].hour / 10) + '0', LINE2(3));
        clcd_putch((alarms[selected_alarm].hour % 10) + '0', LINE2(4));
        clcd_putch(':', LINE2(5));
        clcd_putch((alarms[selected_alarm].minute / 10) + '0', LINE2(6));
        clcd_putch((alarms[selected_alarm].minute % 10) + '0', LINE2(7));

        if (alarms[selected_alarm].type == ALARM_ONCE)
            type = 'O';
        else if (alarms[selected_alarm].type == ALARM_DAILY)
            type = 'D';
        else
            type = 'W';

        clcd_putch(type, LINE2(9));
        clcd_print(" ON", LINE2(10));
    }
    else
    {
        clcd_print("EMPTY", LINE2(3));
    }
}


static void display_alarm_edit(void)
{
    clcd_print("EDIT ALARM", LINE1(0));
    clcd_print("                ", LINE2(0));

    switch (alarm_field)
    {
        case 0:
            clcd_print("Hour", LINE1(0));
            clcd_putch((edit_alarm.hour / 10) + '0', LINE2(0));
            clcd_putch((edit_alarm.hour % 10) + '0', LINE2(1));
            break;

        case 1:
            clcd_print("Minute", LINE1(0));
            clcd_putch((edit_alarm.minute / 10) + '0', LINE2(0));
            clcd_putch((edit_alarm.minute % 10) + '0', LINE2(1));
            break;

        case 2:
            clcd_print("Alarm Type", LINE1(0));

            if (edit_alarm.type == ALARM_ONCE)
                clcd_print("ONCE", LINE2(0));
            else if (edit_alarm.type == ALARM_DAILY)
                clcd_print("DAILY", LINE2(0));
            else
                clcd_print("WEEKLY", LINE2(0));
            break;

        case 3:
            clcd_print("Day", LINE1(0));
            clcd_putch((edit_alarm.day / 10) + '0', LINE2(0));
            clcd_putch((edit_alarm.day % 10) + '0', LINE2(1));
            break;

        case 4:
            clcd_print("Month", LINE1(0));
            clcd_putch((edit_alarm.month / 10) + '0', LINE2(0));
            clcd_putch((edit_alarm.month % 10) + '0', LINE2(1));
            break;

        case 5:
            clcd_print("Year (00-99)", LINE1(0));
            clcd_putch((edit_alarm.year / 10) + '0', LINE2(0));
            clcd_putch((edit_alarm.year % 10) + '0', LINE2(1));
            break;

        case 6:
            clcd_print("Alarm Status", LINE1(0));

            if (edit_alarm.enabled)
                clcd_print("ENABLED", LINE2(0));
            else
                clcd_print("DISABLED", LINE2(0));
            break;
    }
}

static void increment_alarm_field(void)
{
    unsigned char max_day;

    switch (alarm_field)
    {
        case 0:  /* Hour */
            edit_alarm.hour = (edit_alarm.hour + 1) % 24;
            break;

        case 1:  /* Minute */
            edit_alarm.minute = (edit_alarm.minute + 1) % 60;
            break;

        case 2:  /* Alarm type */
            edit_alarm.type = (edit_alarm.type + 1) % 3;

            if (edit_alarm.type == ALARM_WEEKLY &&
                edit_alarm.day > 7)
            {
                edit_alarm.day = 1;
            }

            break;

        case 3:  /* Day */
            if (edit_alarm.type == ALARM_WEEKLY)
            {
                if (edit_alarm.day >= 7)
                    edit_alarm.day = 1;
                else
                    edit_alarm.day++;
            }
            else
            {
                max_day = get_days_in_month(edit_alarm.month,
                                            edit_alarm.year);

                if (edit_alarm.day >= max_day)
                    edit_alarm.day = 1;
                else
                    edit_alarm.day++;
            }

            break;

        case 4:  /* Month */
            if (edit_alarm.month >= 12)
                edit_alarm.month = 1;
            else
                edit_alarm.month++;

            max_day = get_days_in_month(edit_alarm.month,
                                        edit_alarm.year);

            if (edit_alarm.day > max_day)
                edit_alarm.day = max_day;

            break;

        case 5:  /* Year */
            edit_alarm.year = (edit_alarm.year + 1) % 100;

            max_day = get_days_in_month(edit_alarm.month,
                                        edit_alarm.year);

            if (edit_alarm.day > max_day)
                edit_alarm.day = max_day;

            break;

        case 6:  /* Enable/Disable */
            edit_alarm.enabled = !edit_alarm.enabled;
            break;
    }
}

static void load_time_date(void)
{
    edit_sec = bcd_to_decimal(read_ds1307(SEC_ADDR) & 0x7F);
    edit_min = bcd_to_decimal(read_ds1307(MIN_ADDR) & 0x7F);
    edit_hour = bcd_to_decimal(read_ds1307(HOUR_ADDR) & 0x3F);

    edit_date = bcd_to_decimal(read_ds1307(DATE_ADDR) & 0x3F);
    edit_month = bcd_to_decimal(read_ds1307(MONTH_ADDR) & 0x1F);
    edit_year = bcd_to_decimal(read_ds1307(YEAR_ADDR));

    selected_field = 0;
}

static void display_time_date_edit(void)
{
    clcd_print("TIME: 00:00:00  ", LINE1(0));
    clcd_print("DATE: 00/00/00  ", LINE2(0));

    /* Display hours */
    clcd_putch((edit_hour / 10) + '0', LINE1(6));
    clcd_putch((edit_hour % 10) + '0', LINE1(7));

    /* Display minutes */
    clcd_putch((edit_min / 10) + '0', LINE1(9));
    clcd_putch((edit_min % 10) + '0', LINE1(10));

    /* Display seconds */
    clcd_putch((edit_sec / 10) + '0', LINE1(12));
    clcd_putch((edit_sec % 10) + '0', LINE1(13));

    /* Display date */
    clcd_putch((edit_date / 10) + '0', LINE2(6));
    clcd_putch((edit_date % 10) + '0', LINE2(7));

    /* Display month */
    clcd_putch((edit_month / 10) + '0', LINE2(9));
    clcd_putch((edit_month % 10) + '0', LINE2(10));

    /* Display year */
    clcd_putch((edit_year / 10) + '0', LINE2(12));
    clcd_putch((edit_year % 10) + '0', LINE2(13));

    /* Toggle blink state approximately every 250 ms */
    blink_count++;

    if (blink_count >= 12)
    {
        blink_count = 0;
        blink_on = !blink_on;
    }

    if (!blink_on)
    {
        switch (selected_field)
        {
            case 0:
                clcd_print("  ", LINE1(6));
                break;

            case 1:
                clcd_print("  ", LINE1(9));
                break;

            case 2:
                clcd_print("  ", LINE1(12));
                break;

            case 3:
                clcd_print("  ", LINE2(6));
                break;

            case 4:
                clcd_print("  ", LINE2(9));
                break;

            case 5:
                clcd_print("  ", LINE2(12));
                break;
        }
    }
}

static void increment_selected_field(void)
{
    switch (selected_field)
    {
        case 0:
            edit_hour = (edit_hour + 1) % 24;
            break;

        case 1:
            edit_min = (edit_min + 1) % 60;
            break;

        case 2:
            edit_sec = (edit_sec + 1) % 60;
            break;

        case 3:
            edit_date++;

            if (edit_date > get_days_in_month(edit_month, edit_year))
                edit_date = 1;

            break;

        case 4:
            edit_month++;

            if (edit_month > 12)
                edit_month = 1;

            if (edit_date > get_days_in_month(edit_month, edit_year))
                edit_date = get_days_in_month(edit_month, edit_year);

            break;

        case 5:
            edit_year = (edit_year + 1) % 100;

            if (edit_date > get_days_in_month(edit_month, edit_year))
                edit_date = get_days_in_month(edit_month, edit_year);

            break;
    }
}

static void save_time_date(void)
{
    write_ds1307(HOUR_ADDR, decimal_to_bcd(edit_hour));
    write_ds1307(MIN_ADDR, decimal_to_bcd(edit_min));
    write_ds1307(SEC_ADDR, decimal_to_bcd(edit_sec));

    write_ds1307(DATE_ADDR, decimal_to_bcd(edit_date));
    write_ds1307(MONTH_ADDR, decimal_to_bcd(edit_month));
    write_ds1307(YEAR_ADDR, decimal_to_bcd(edit_year));
}

void main(void)
{
    unsigned char key;

    init_config();

    while (1)
    {
        
        key = read_switches(LEVEL_CHANGE);

        if (key != 0xFF)
        {
            if (key != pressed_key)
            {
                pressed_key = key;
                hold_count = 0;
                long_press_handled = 0;
            }
            else if (hold_count < LONG_PRESS_COUNT)
            {
                hold_count++;
            }
        }
        else
        {
            if (pressed_key != 0xFF && !long_press_handled)
            {
                /*
                 * Process a short press only after release.
                 * Use pressed_key because key is now 0xFF.
                 */
                if (screen == MAIN_MENU)
                {
                    if (pressed_key == UP_KEY ||
                        pressed_key == DOWN_KEY)
                    {
                        menu_index = (menu_index + 1) % 2;
                    }
                }
                else if (screen == SET_TIME_SCREEN)
                {
                    if (pressed_key == UP_KEY)
                    {
                        increment_selected_field();
                    }
                    else if (pressed_key == DOWN_KEY)
                    {
                        selected_field = (selected_field + 1) % 6;
                    }
                    else if (pressed_key == LEFT_KEY)
                    {
                        selected_field =
                            (selected_field == 0) ? 5 : selected_field - 1;
                    }
                    else if (pressed_key == RIGHT_KEY)
                    {
                        save_time_date();
                        screen = MAIN_MENU;
                        CLEAR_DISP_SCREEN;
                    }
                }
                else if (screen == ALARM_LIST_SCREEN)
                {
                    if (pressed_key == UP_KEY)
                    {
                        selected_alarm = (selected_alarm == 0)
                                         ? MAX_ALARMS - 1
                                         : selected_alarm - 1;

                        CLEAR_DISP_SCREEN;
                    }
                    else if (pressed_key == DOWN_KEY)
                    {
                        selected_alarm = (selected_alarm + 1) % MAX_ALARMS;

                        CLEAR_DISP_SCREEN;
                    }
                    else if (pressed_key == LEFT_KEY)
                    {
                        screen = MAIN_MENU;
                        CLEAR_DISP_SCREEN;
                    }
                }
                else if (screen == ALARM_EDIT_SCREEN)
                {
                    if (pressed_key == UP_KEY)
                    {
                        increment_alarm_field();
                    }
                    else if (pressed_key == DOWN_KEY)
                    {
                        alarm_field = (alarm_field + 1) % 7;
                    }
                    else if (pressed_key == LEFT_KEY)
                    {
                        alarm_field = (alarm_field == 0) ? 6 : alarm_field - 1;
                    }
                    else if (pressed_key == RIGHT_KEY)
                    {
                        alarms[selected_alarm] = edit_alarm;
                        save_alarm_to_eeprom(selected_alarm);

                        screen = ALARM_LIST_SCREEN;
                        CLEAR_DISP_SCREEN;
                    }
                }
                else if (screen == DEFAULT_SCREEN)
                {
                    if (pressed_key == UP_KEY ||
                        pressed_key == DOWN_KEY)
                    {
                        screen = MAIN_MENU;
                        menu_index = 0;
                        CLEAR_DISP_SCREEN;
                    }
                }
            }

            pressed_key = 0xFF;
            hold_count = 0;
            long_press_handled = 0;
        }
        
        if (pressed_key != 0xFF && hold_count >= LONG_PRESS_COUNT && !long_press_handled)
        {
            long_press_handled = 1;

            if (screen == MAIN_MENU)
            {
                if (pressed_key == UP_KEY)
                {
                    if (menu_index == 0)
                    {
                        load_time_date();
                        screen = SET_TIME_SCREEN;
                        CLEAR_DISP_SCREEN;
                    }
                    else
                    {
                        selected_alarm = 0;
                        screen = ALARM_LIST_SCREEN;
                        CLEAR_DISP_SCREEN;
                    }
                }
                else if (pressed_key == DOWN_KEY)
                {
                    screen = DEFAULT_SCREEN;
                    CLEAR_DISP_SCREEN;
                }
            }
            else if (screen == ALARM_LIST_SCREEN &&
                     pressed_key == RIGHT_KEY)
            {
                edit_alarm = alarms[selected_alarm];
                alarm_field = 0;

                screen = ALARM_EDIT_SCREEN;
                CLEAR_DISP_SCREEN;
            }
            else if (screen == SET_TIME_SCREEN &&
                     pressed_key == DOWN_KEY)
            {
                /* Cancel editing without saving */
                screen = MAIN_MENU;
                CLEAR_DISP_SCREEN;
            }
            else if (screen == ALARM_EDIT_SCREEN &&
                     pressed_key == DOWN_KEY)
            {
                /* Cancel alarm editing without saving */
                screen = ALARM_LIST_SCREEN;
                CLEAR_DISP_SCREEN;
            }
        }
        
        if (screen == DEFAULT_SCREEN)
        {
            display_default_screen();
        }
        else if (screen == MAIN_MENU)
        {
            display_main_menu();
        }
        else if (screen == SET_TIME_SCREEN)
        {
            display_time_date_edit();
        }
        else if (screen == ALARM_LIST_SCREEN)
        {
            display_alarm_list();
        }
        else if (screen == ALARM_EDIT_SCREEN)
        {
            display_alarm_edit();
        }

        __delay_ms(20);
    }
}