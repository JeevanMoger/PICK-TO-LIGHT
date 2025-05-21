/* 
 * File:   main.h
 * Author: VINAY
 *
 * Created on 4 December, 2024, 1:07 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>

#define U_ALPH 0XE5
#define UNDER_SCORE 0X40
#define S_ALPH 0X6E
#define t_ALPH 0XCC
#define P_ALPH 0X8F
#define i_ALPH 0X84
#define d_ALPH 0XE9

#define DP 0X10

#define TRUE 1
#define FALSE 0

unsigned int interrupt_key = 0,field_flag = 0;
unsigned char key;

unsigned int in_field_ust = 1,in_field_pid = 1;
unsigned int increment_ust_flag = 0,increment_pid_flag = 0;

unsigned int ssd3 = 0,ssd2 = 0,ssd1 = 0,ssd0 = 0;

unsigned int rec_cond_flag = 1,update_data_flag = 1;
unsigned char can_id[5];

unsigned int num = 0;

void init_external_interrupt(void);
void two_fields();
void update_stock();
void read_ust_data_from_eeprom();
void normal_stock();
void compare_and_check();
void can_stock();
void receive_can_data();

void product_id();
void read_pid_data_from_eeprom();


unsigned int disp_flag = 1;

void third_ssd_ust();
void second_ssd_ust();
void first_ssd_ust();
void zeroth_ssd_ust();

void third_ssd_pid();
void second_ssd_pid();
void first_ssd_pid();
void zeroth_ssd_pid();

#endif	/* MAIN_H */

