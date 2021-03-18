#include "xy_sys_headfile.h"

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_init_data(datall* p_data)
{
	memset((char *)&p_data->motor, 0, sizeof(p_data->motor));				//reset motor step

	p_data->motor.blow_target_step_pri	= TARGET_LOW_SWING;
	p_data->motor.blow_motor_step_pri   = TARGET_HIGH_SWING;
	
	p_data->motor.blow_motor_step		= MAX_RESET_STEP;			///< checkself max step
	p_data->motor.absorb_motor_step = MAX_RESET_STEP;			///< checkself max step
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_reset_step(motorcontrol* p_motor)
{
	static switchstate s_onetime[2] = {RESET, RESET};
	static switchstate s_resetstep = STEP1;
	
	if(STEP2 == s_resetstep)
	{
		if(RESET_STEP_ZERO == p_motor->blow_target_step)
		{
			if(RESET_STEP_ZERO == p_motor->blow_motor_step)
			{
				if(RESET == s_onetime[0])
				{
				 	s_onetime[0] = SET;
				 	p_motor->blow_motor_step = COMPENSATE_STEP;
				}
			}
		}
		else
		{
			s_onetime[0] = RESET;
		}
		p_motor->blow_target_step_real = p_motor->blow_target_step;
			if(RESET_STEP_ZERO == p_motor->absorb_target_step)
			{
				if(RESET_STEP_ZERO == p_motor->absorb_motor_step)
				{
					if(RESET == s_onetime[1])
					{
						s_onetime[1] = SET;
						p_motor->absorb_motor_step = COMPENSATE_STEP;
					}
				}
			}
			else
			{
				s_onetime[1] = RESET;
			}
			p_motor->absorb_target_step_real = p_motor->absorb_target_step;
	}
	else
	{
		/* only enter work one time about checkself. */
		p_motor->blow_target_step_real = RESET_STEP_ZERO;
			p_motor->absorb_target_step_real = RESET_STEP_ZERO;
			if((RESET_STEP_ZERO == p_motor->blow_motor_step)
				&& (RESET_STEP_ZERO == p_motor->absorb_motor_step))
			{
				s_resetstep = STEP2;
			}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_timer_ctrl(datall* p_data)
{
	static unsigned char s_count = 0;

	if(ON == p_data->remote.keyctrl.keystate_open_swing)  
	{
		s_count++;
		if(s_count > 2)
		{
			s_count = 0;
			hal_timer_motor_blow(&p_data->motor);
		}
	}
	else
	{
		hal_timer_motor_blow(&p_data->motor);
	}
	hal_timer_motor_absorb(&p_data->motor);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_blow_step(unsigned char n)
{
	switch(n)
	{
		case 0:
			blow_motor_step1_on();blow_motor_step2_off();blow_motor_step3_off();blow_motor_step4_off();
			break;
		case 1:
			blow_motor_step1_on();blow_motor_step2_on();blow_motor_step3_off();blow_motor_step4_off();
			break;
		case 2:
			blow_motor_step1_off();blow_motor_step2_on();blow_motor_step3_off();blow_motor_step4_off();
			break;
		case 3:
			blow_motor_step1_off();blow_motor_step2_on();blow_motor_step3_on();blow_motor_step4_off();
			break;	
		case 4:
			blow_motor_step1_off();blow_motor_step2_off();blow_motor_step3_on();blow_motor_step4_off();
			break;
		case 5:
			blow_motor_step1_off();blow_motor_step2_off();blow_motor_step3_on();blow_motor_step4_on();
			break;
		case 6:
			blow_motor_step1_off();blow_motor_step2_off();blow_motor_step3_off();blow_motor_step4_on();
			break;
		case 7:
			blow_motor_step1_on();blow_motor_step2_off();blow_motor_step3_off();blow_motor_step4_on();
			break;		
		default:
			break;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_close_blow(void)
{
	blow_motor_step1_off();
	blow_motor_step2_off();
	blow_motor_step3_off();
	blow_motor_step4_off();
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_close_absorb(void)
{
	absorb_motor_step1_off();
	absorb_motor_step2_off();
	absorb_motor_step3_off();
	absorb_motor_step4_off();
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_blow_move(motorcontrol* p_motor)
{
	if(p_motor->blow_motor_step_delay_flag)
	{
		p_motor->blow_motor_step_delay_flag=0;
		p_motor->blow_motor_step_delay_count=0;
		if(p_motor->blow_motor_step < p_motor->blow_target_step_real)
		{
			fml_motor_blow_step(p_motor->blow_motor_step%8);
			p_motor->blow_motor_step++;
		}
		else if(p_motor->blow_motor_step > p_motor->blow_target_step_real)
		{
			fml_motor_blow_step(p_motor->blow_motor_step%8);
			p_motor->blow_motor_step--;
		}
		else
		{
			fml_motor_close_blow();
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_absorb_move(motorcontrol* p_motor)
{
	if(p_motor->absorb_motor_step_delay_flag)
	{
		p_motor->absorb_motor_step_delay_flag=0;
		p_motor->absorb_motor_step_delay_count=0;
		if(p_motor->absorb_motor_step < p_motor->absorb_target_step_real)
		{
			fml_motor_absorb_step(p_motor->absorb_motor_step%8);
			p_motor->absorb_motor_step++;
		}
		else if(p_motor->absorb_motor_step > p_motor->absorb_target_step_real)
		{
			fml_motor_absorb_step(p_motor->absorb_motor_step%8);
			p_motor->absorb_motor_step--;		
		}
		else
		{
			fml_motor_close_absorb();
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_save_step(datall* p_data)
{
	if(((WORKMODULE_BLOW == p_data->remote.workmode.workmode_current)
		|| (WORKMODULE_WARM == p_data->remote.workmode.workmode_current)))
	{
		if((ON == p_data->remote.keyctrl.keystate_fan_blow) && (p_data->motor.blow_motor_step > TARGET_LOW_SWING))
		{
			p_data->motor.blow_motor_step_pri = p_data->motor.blow_motor_step;
		}
	}
}
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_motor_ctrl_move(datall* p_data)
{
	#ifdef XY_SYS_TEST_MODE
		if(YES == p_data->testmode)
			return;
	#endif

	fml_motor_reset_step(&p_data->motor);
	fml_motor_blow_move(&p_data->motor);
	fml_motor_absorb_move(&p_data->motor);
	fml_motor_save_step(p_data);
}
