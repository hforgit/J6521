#include "xy_sys_headfile.h"


unsigned char hal_alg_chk_crc8(unsigned char* p_buffer, unsigned char buf_size)
{
	unsigned char i;
    unsigned char crc = 0;
	
    while(buf_size--)
    {
        for(i=0x80; i>0; i>>=1)
        {
            if((crc & 0x80) != 0)
            {
                crc <<= 1;
                crc ^= 0x07; // X8 + X2 + X + 1
            }
            else
            {
                crc <<= 1;
            }

            if((*p_buffer & i) != 0)
            {
                crc ^= 0x07;
            }
        }
        p_buffer++;
    }

    return crc;
}