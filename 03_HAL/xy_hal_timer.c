#include "xy_sys_headfile.h"

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_timer_Init(void)
{
	TMCON |= 0X05;
	/*	TIMER0	*/
	TMOD |= 0x01;	  
	TH0 = 0xF8; 	//F8
	TL0 = 0x70; 	//30
	TR0 = 0;
	ET0 = 1;
	TR0 = 1;

	/*	TIMER2	*/
	RCAP2H=0xfc;	 //溢出时间：时钟为Fsys，则12000*（1/Fsys）=0.5ms;
	RCAP2L=0xbe;	 // 7E	 7D - 81
	TR2 = 0;
	ET2 = 1;//定时器2允许
	TR2 = 1;
	IPT2 = 1;	
	/*	WDT */
	OPINX |= 0XC1;
	OPREG |= 0X82;
	WDTCON |= 0X10;
}
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_timer_buzzer(buzzercontrol* p_bee)
{	
	if(ON == p_bee->bee_state)
	{
		p_bee->bee_count++;
		if(p_bee->bee_count<=1)				
		{
			bee_on();
		}
		else
		{
			p_bee->bee_count=0;
			bee_off();
		}
	}
	else
	{
		p_bee->bee_count=0;
		bee_off();
	}

}
/***********************************************************************************************************************
* Function Name:
* Description  : 1ms
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_timer_motor_blow(motorcontrol* p_motor)
{
	p_motor->blow_motor_step_delay_count++;

	if(p_motor->blow_motor_step_delay_count>=4)
	{
		p_motor->blow_motor_step_delay_count=4;
		p_motor->blow_motor_step_delay_flag=1;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_timer_mode_deal(workmoduleflag* p_mode)
{
	if(SET == p_mode->flag_workdelay_10s)				
	{
		p_mode->workdelay_10s++;
		if(p_mode->workdelay_10s >= CLOCK_DELAYTIMER_10S)
		{
			p_mode->workdelay_10s = CLOCK_DELAYTIMER_10S;
		}
	}
	else
	{
		p_mode->workdelay_10s = CLOCK_DELAYTIMER_0S;
	}
	if(SET == p_mode->flag_workdelay_1s)
	{
		p_mode->workdelay_1s++;
		if(p_mode->workdelay_1s >= CLOCK_DELAYTIMER_1S)
		{
			p_mode->workdelay_1s = CLOCK_DELAYTIMER_1S;
		}
	}
	else
	{
		p_mode->workdelay_1s = CLOCK_DELAYTIMER_0S;
	}

	p_mode->workdelay_cyc++;
	if(p_mode->workdelay_cyc >= CLOCK_DELAYTIMER_CYC)
	{
		p_mode->workdelay_cyc = CLOCK_DELAYTIMER_0S;
	}
}
