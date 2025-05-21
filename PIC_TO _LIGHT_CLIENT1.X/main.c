#include "main.h"
#include "ssd_display.h"
#include "digital_keypad.h"
#include "eeprom.h"
#include "can.h"

void delay(unsigned short factor) {
    unsigned short i, j;

    for (i = 0; i < factor; i++) {
        for (j = 500; j--;);
    }
}

void init_external_interrupt(void) { //selecting the edge
    INTEDG0 = 1;
    //clear interrut flag
    INT0F = 0;
    //enable external interrut
    INT0E = 1;
}

void init_config() {
    GIE = 1;
    PEIE = 1;
    init_external_interrupt();
    init_digital_keypad();
    init_ssd_control();
    init_can();
}

void main(void) {
    init_config();
    while (1) {
        receive_can_data();
        if (interrupt_key) {
            if(update_data_flag)
                key = read_digital_keypad(STATE_CHANGE);
            else
                key = read_digital_keypad(LEVEL);
            two_fields();

        } else {
            PORTD = BLANK;
        }

    }
    return;
}

void two_fields() {
    if (field_flag == 0) {

        if (in_field_ust) {
            ssd[0] = U_ALPH;
            ssd[1] = UNDER_SCORE;
            ssd[2] = S_ALPH;
            ssd[3] = t_ALPH;
            if (key == SW3) {
                field_flag = !field_flag;
            } else if (key == SW2) {
                in_field_ust = 0;
                disp_flag = 0;
            }
            if (disp_flag)
                display(ssd);
        } else {
            update_stock();
        }

    } else {
        
        if (in_field_pid) {
            ssd[0] = P_ALPH;
            ssd[1] = UNDER_SCORE;
            ssd[2] = i_ALPH;
            ssd[3] = d_ALPH;
            if (key == SW3) {
                field_flag = !field_flag;
            } else if (key == SW2) {
                in_field_pid = 0;
                disp_flag = 0;
            }
            if (disp_flag)
                display(ssd);
        } else {
            product_id();
        }

    }
}

void receive_can_data() {
    if (can_receive()) {
        num = 0;
        num = can_payload[D0 + 0] - 48;
        num = (num * 10) + (can_payload[D0 + 1] - 48);
        num = (num * 10) + (can_payload[D0 + 2] - 48);
        num = (num * 10) + (can_payload[D0 + 3] - 48);
        
        can_id[0] = can_payload[D0 + 4];
        can_id[1] = can_payload[D0 + 5];
        can_id[2] = can_payload[D0 + 6];
        can_id[3] = can_payload[D0 + 7];
        can_id[4] = '\0';
        
        compare_and_check();
    }
}


void compare_and_check()
{
    int confirm = 0;
    for(int i = 4; i < 8; i++)
    {
        char ch = read_internal_eeprom(i);
        if(can_id[confirm] == ch)
            confirm++;
    }
    if(confirm == 4){
        rec_cond_flag = 0;
        update_data_flag = 0;
        interrupt_key = 1;
        field_flag = 0;
        in_field_ust = 0;
    }else{
        rec_cond_flag = 1;
        interrupt_key = 0;
        field_flag = 0;
        in_field_ust = 1;
        update_data_flag = 1;
    }
}