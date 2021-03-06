#include "xy_sys_headfile.h"

void hal_port_init(void)
{
	P0CON = 0x1F;  	 // test P07
	P0PH  = 0x60;		
	P1CON = 0xE0;    // test P10
	P1PH  = 0x10;
	P2CON = 0xFC;  
	P2PH  = 0x03;
	/*	EX2	*/
	INT2R = 0x00;	
	INT2F = 0x02;
	IE1  |= 0X08;
	IP1  |= 0X08;
}