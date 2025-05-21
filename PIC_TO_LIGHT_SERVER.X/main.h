#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>

#define _XTAL_FREQ 20000000

/* Defines the data */
#define TRUE			1
#define FALSE			0

unsigned int k = 0,count = 0;

unsigned char arr_id[5],arr_st[5],ch;

unsigned char key,data[9];
unsigned int ust_pid_flag = 1,alph_flag = 0;

unsigned char shift_id[4],shift_st[4];

void store_in_arr();

#endif	/* MAIN_H */

