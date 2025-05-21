
#include "digital_keypad.h"
#include "main.h"
#include "ssd_display.h"
#include "eeprom.h"
#include "can.h"

 
void update_stock() {
    if(update_data_flag)
        normal_stock();
    else
        can_stock();
}

void read_ust_data_from_eeprom() {

    ssd0 = read_internal_eeprom(0X00) - 48;
    ssd1 = read_internal_eeprom(0X01) - 48;
    ssd2 = read_internal_eeprom(0X02) - 48;
    ssd3 = read_internal_eeprom(0X03) - 48;
}


void normal_stock()
{
    unsigned static int once_ust = 1;
    if (once_ust) {
        once_ust = 0;
        read_ust_data_from_eeprom();
    }
    if (increment_ust_flag == 0) {
        third_ssd_ust();
    } else if (increment_ust_flag == 1) {
        second_ssd_ust();
    } else if (increment_ust_flag == 2) {
        first_ssd_ust();
    } else if (increment_ust_flag == 3) {
        zeroth_ssd_ust();
    }
    if (key == SW2) {
        if (increment_ust_flag < 3)
            increment_ust_flag++;
        else
            increment_ust_flag = 0;
    } else if (key == SW3) {
        field_flag = 0;
        in_field_ust = 1;
        interrupt_key = 0;
        increment_ust_flag = 0;
        disp_flag = 1;
        once_ust = 1;
        rec_cond_flag = 1;
        update_data_flag = 1;
        write_internal_eeprom(0X00, ssd0 + 48);
        write_internal_eeprom(0X01, ssd1 + 48);
        write_internal_eeprom(0X02, ssd2 + 48);
        write_internal_eeprom(0X03, ssd3 + 48);
        ssd0 = 0, ssd1 = 0, ssd2 = 0, ssd3 = 0;
        num = 0;
        can_transmit();
    }
    display(ssd);
}


void third_ssd_ust() {
    ssd[0] = digit[ssd0];
    ssd[1] = digit[ssd1];
    ssd[2] = digit[ssd2];
    ssd[3] = digit[ssd3] | DP;
    if (key == SW1) {
        if (ssd3 < 9)
            ssd3++;
        else
            ssd3 = 0;
    }
}

void second_ssd_ust() {
    ssd[0] = digit[ssd0];
    ssd[1] = digit[ssd1];
    ssd[2] = digit[ssd2] | DP;
    ssd[3] = digit[ssd3];
    if (key == SW1) {
        if (ssd2 < 9)
            ssd2++;
        else
            ssd2 = 0;
    }
}

void first_ssd_ust() {
    ssd[0] = digit[ssd0];
    ssd[1] = digit[ssd1] | DP;
    ssd[2] = digit[ssd2];
    ssd[3] = digit[ssd3];
    if (key == SW1) {
        if (ssd1 < 9)
            ssd1++;
        else
            ssd1 = 0;
    }
}

void zeroth_ssd_ust() {
    ssd[0] = digit[ssd0] | DP;
    ssd[1] = digit[ssd1];
    ssd[2] = digit[ssd2];
    ssd[3] = digit[ssd3];
    if (key == SW1) {
        if (ssd0 < 9)
            ssd0++;
        else
            ssd0 = 0;
    }
}

void can_stock()
{
    unsigned static int delay = 0;
    if(key == SW1)
    {
        if(num < 9999 && delay++ == 10){
            num++;
            delay = 0;
        }
    }else if(key == SW2){
        if(num > 0 && delay++ == 10){
            num--;
            delay = 0;
        }
    }else if (key == SW3) {
        field_flag = 0;
        in_field_ust = 1;
        interrupt_key = 0;
        increment_ust_flag = 0;
        disp_flag = 1;
        rec_cond_flag = 1;
        update_data_flag = 1;
        write_internal_eeprom(0X00, (num / 1000) + 48);
        write_internal_eeprom(0X01, ((num / 100) % 10) + 48);
        write_internal_eeprom(0X02, ((num / 10) % 10) + 48);
        write_internal_eeprom(0X03, (num % 10) + 48);
        for(unsigned long wait = 5000; wait--;);
        num = 0;
        can_transmit();
    }
    ssd[0] = digit[num / 1000];
    ssd[1] = digit[(num / 100) % 10];
    ssd[2] = digit[(num / 10) % 10];
    ssd[3] = digit[num % 10];
    display(ssd);
}


void product_id() {
    unsigned static int once_pid = 1;
    if (once_pid) {
        once_pid = 0;
        read_pid_data_from_eeprom();
    }
    if (increment_pid_flag == 0) {
        third_ssd_pid();
    } else if (increment_pid_flag == 1) {
        second_ssd_pid();
    } else if (increment_pid_flag == 2) {
        first_ssd_pid();
    } else if (increment_pid_flag == 3) {
        zeroth_ssd_pid();
    }
    if (key == SW2) {
        if (increment_pid_flag < 3)
            increment_pid_flag++;
        else
            increment_pid_flag = 0;
    } else if (key == SW3) {
        field_flag = 0;
        in_field_pid = 1;
        interrupt_key = 0;
        increment_pid_flag = 0;
        disp_flag = 1;
        once_pid = 1;
        rec_cond_flag = 1;
        update_data_flag = 1;
        write_internal_eeprom(0X04, ssd0 + 48);
        write_internal_eeprom(0X05, ssd1 + 48);
        write_internal_eeprom(0X06, ssd2 + 48);
        write_internal_eeprom(0X07, ssd3 + 48);
        ssd0 = 0, ssd1 = 0, ssd2 = 0, ssd3 = 0;
        //can_transmit();
    }
    display(ssd);
}

void read_pid_data_from_eeprom() {
    ssd0 = read_internal_eeprom(0X04) - 48;
    ssd1 = read_internal_eeprom(0X05)- 48;
    ssd2 = read_internal_eeprom(0X06) - 48;
    ssd3 = read_internal_eeprom(0X07) - 48;
}

void third_ssd_pid() {
    ssd[0] = digit[ssd0];
    ssd[1] = digit[ssd1];
    ssd[2] = digit[ssd2];
    ssd[3] = digit[ssd3] | DP;
    if (key == SW1) {
        if (ssd3 < 9)
            ssd3++;
        else
            ssd3 = 0;
    }
}

void second_ssd_pid() {
    ssd[0] = digit[ssd0];
    ssd[1] = digit[ssd1];
    ssd[2] = digit[ssd2] | DP;
    ssd[3] = digit[ssd3];
    if (key == SW1) {
        if (ssd2 < 9)
            ssd2++;
        else
            ssd2 = 0;
    }
}

void first_ssd_pid() {
    ssd[0] = digit[ssd0];
    ssd[1] = digit[ssd1] | DP;
    ssd[2] = digit[ssd2];
    ssd[3] = digit[ssd3];
    if (key == SW1) {
        if (ssd1 < 9)
            ssd1++;
        else
            ssd1 = 0;
    }
}

void zeroth_ssd_pid() {
    ssd[0] = digit[ssd0] | DP;
    ssd[1] = digit[ssd1];
    ssd[2] = digit[ssd2];
    ssd[3] = digit[ssd3];
    if (key == SW1) {
        if (ssd0 < 9)
            ssd0++;
        else
            ssd0 = 0;
    }
}