#include "main.h"

void __interrupt() isr(void)
{
	if (INT0F == 1)
	{
        if(rec_cond_flag)
            interrupt_key = !interrupt_key;
        else
            update_data_flag = !update_data_flag;
		INT0F = 0;
	}
}
