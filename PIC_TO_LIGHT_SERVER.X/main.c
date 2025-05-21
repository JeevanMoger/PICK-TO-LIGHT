#include "main.h"
#include "can.h"
#include "uart.h"

/* delay 1ms function */
void delay(unsigned short factor) {
    unsigned short i, j;

    for (i = 0; i < factor; i++) {
        for (j = 500; j--;);
    }
}

void init_config(void) {
    init_can();
    init_uart();
    GIE = 1;
    PEIE = 1;
    //puts("hello\n\r");
}

void receive_can(void) {
    if (can_receive()) {
        arr_st[0] = can_payload[D0 + 0];
        arr_st[1] = can_payload[D0 + 1];
        arr_st[2] = can_payload[D0 + 2];
        arr_st[3] = can_payload[D0 + 3];
        arr_st[4] = '\0';
        arr_id[0] = can_payload[D0 + 4];
        arr_id[1] = can_payload[D0 + 5];
        arr_id[2] = can_payload[D0 + 6];
        arr_id[3] = can_payload[D0 + 7];
        arr_id[4] = '\0';
        puts("\n\r\n\rRECIEVED DATA ->   \n\rPRODUCT ID: ");
        puts(arr_id);
        puts("\n\rUPDATED STOCK: ");
        puts(arr_st);
        puts("\n\r");
    }
}

void main(void) {
    init_config();

    while (1) 
    {
        receive_can();
        if(ust_pid_flag == 1)
        {
            puts("\n\rENTER THE UST: ");
            ust_pid_flag = 2;
        }else if(ust_pid_flag == 0){
            puts("ENTER THE PID: ");
            ust_pid_flag = 2;
        }
        if (ch != '\0') 
        {
            putch(ch);
            if(ch != '\r')
            {
                if(count == 0)
                {   
                    if(ch >= 48 && ch <= 57 && k < 4){
                        data[k] = ch;
                        k++;
                    }else
                        alph_flag = 1;
                }else  if(count == 1){
                    if(ch >= 48 && ch <= 57 && k >= 4 && k < 8){
                        data[k] = ch;
                        k++;
                    }else
                        alph_flag = 1;
                }
            }else{
                count++;
                puts("\n\r");
                if(count == 1)
                    ust_pid_flag = 0;
                else if(count == 2)
                    ust_pid_flag = 1;
                if(ust_pid_flag == 0 && k < 4) {
                    unsigned int ref = k;
                    for(int i = k; i < 4; i++)
                        data[k++];
                    for(int i = k - 1; i >= 0; i--)
                    {
                        if(ref-- > 0)
                            data[i] = data[ref];
                        else{
                            data[i] = '0';
                            ref = 0;
                        }
                    }
                    k = 4;
                }else if(ust_pid_flag && k < 8){
                    unsigned int ref1 = k;
                    for(int i = k; i < 8; i++)
                        data[k++];
                    puts("\n\r");
                    putch(k + 48);
                    puts("\n\r");
                    for(int i = k - 1; i >= 4; i--)
                    {
                        if(ref1-- > 4)
                            data[i] = data[ref1];
                        else{
                            data[i] = '0';
                            ref1 = 4;
                        }
                    }
                }
                if(alph_flag)
                {
                    //puts("WARNING: Please try to give numbers\n\r");
                    alph_flag = 0;
                }
            }
            if(count == 2){
                count = 0;
                data[8] = '\0';
//                for(int i = 0; i <= 8; i++)
//                {
//                    putch(data[i]);
//                    
//                }
                puts("\n\r");
                can_transmit();
                k = 0;
            }
            ch = '\0';
        } 
    }
}