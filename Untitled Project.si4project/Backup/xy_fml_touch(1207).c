#include "xy_sys_headfile.h"

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_touch_init_data(datall* p_data)
{
	p_data->touch.current.dat = 0;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_touch_recv_data(datall* p_data)
{
	unsigned char		i;
	unsigned char		temp_sum = 0;
	unsigned char		buffer[sizeof(p_data->uart.rec_uart0_dat)] = {0};
	static datacommon 	s_data;
	
	if(p_data->uart.rec_uart0_ok)
	{
		p_data->uart.rec_uart0_ok = 0;
		memcpy(buffer, p_data->uart.rec_uart0_dat, sizeof(buffer));
		memset(p_data->uart.rec_uart0_dat, 0, sizeof(buffer));
		for(i=0; i<4; i++)
		{
			temp_sum += buffer[i];
		}		
		if(temp_sum == buffer[i])			///< check data 
		{
			s_data.dat = buffer[1] + (buffer[2] << 8);
			if(s_data.dat != p_data->touch.current.dat)
			{
				if(REMOTE == p_data->keytype)
				{
					memset(&p_data->remote, 0, sizeof(p_data->remote));
					p_data->keytype = TOUCH;
				}			
				p_data->touch.current.dat = s_data.dat;
			}
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_touch_deal_disp(displaycontent* p_disp)
{
	if(TOUCH_SYMBOL_BLOW)
	{
		p_disp->disp_icon_blow = SYM_BLOW_ON;
	}
	else
	{
		p_disp->disp_icon_blow = SYM_BLOW_OFF;
	}
	if(TOUCH_SYMBOL_ABSORB)
	{
		p_disp->disp_icon_absorb = SYM_ABSORB_ON;
	}
	else
	{
		p_disp->disp_icon_absorb = SYM_ABSORB_OFF;
	}
	if(TOUCH_SYMBOL_WARM)
	{
		p_disp->disp_icon_warm = SYM_WARM_ON;
	}
	else
	{
		p_disp->disp_icon_warm = SYM_WARM_OFF;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_touch_deal_relay(normalctrol* p_ctrl)
{
	if(TOUCH_RELAY_LIGHT)
	{
		p_ctrl->keystate_light = ON;
	}
	else
	{
		p_ctrl->keystate_light = OFF;
	}

	if(TOUCH_RELAY_PTC)
	{
		p_ctrl->keystate_ptc = ON;
	}
	else
	{
		p_ctrl->keystate_ptc = OFF;
	}
	if(TOUCH_RELAY_FAN_BLOW)
	{
		p_ctrl->keystate_fan_blow = ON;
	}
	else
	{
		p_ctrl->keystate_fan_blow = OFF;
	}
	if(TOUCH_RELAY_FAN_ABSORB)
	{
		p_ctrl->keystate_fan_absorb = ON;
	}
	else
	{
		p_ctrl->keystate_fan_absorb = OFF;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_touch_deal_motor(motorcontrol* p_motor)
{
	if(TOUCH_RELAY_MOTOR_BLOW)
	{
		if(RESET_STEP_ZERO  == p_motor->blow_target_step)
		{
			p_motor->blow_target_step = TARGET_STEP_BLOW;
			//p_motor->blow_target_step_pri = TARGET_STEP_BLOW;				//?
		}
	}
	else
	{
		p_motor->blow_target_step = RESET_STEP_ZERO;
		//p_motor->blow_target_step_pri = RESET_STEP_ZERO;
	}
		if(TOUCH_RELAY_MOTOR_ABSORB)
		{
			if(RESET_STEP_ZERO == p_motor->absorb_target_step)
			{
				p_motor->absorb_target_step = TARGET_STEP_ABSORB;
			}
		}
		else
		{
			p_motor->absorb_target_step = RESET_STEP_ZERO;
		}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_touch_deal_swing(datall* p_data)
{
	if(TOUCH_RELAY_SWING_BLOW)
	{
		p_data->remote.keyctrl.keystate_open_swing = ON;	
		if(TARGET_LOW_SWING == p_data->motor.blow_motor_step)
		{
			p_data->motor.blow_target_step = TARGET_HIGH_SWING;
			p_data->motor.blow_target_step_pri = p_data->motor.blow_target_step;
		}
		else if(TARGET_HIGH_SWING == p_data->motor.blow_motor_step)
		{
			p_data->motor.blow_target_step = TARGET_LOW_SWING;
			p_data->motor.blow_target_step_pri = p_data->motor.blow_target_step;
		}
		else
		{
			if(p_data->motor.blow_target_step == p_data->motor.blow_motor_step)
			{
				p_data->motor.blow_target_step = p_data->motor.blow_target_step_pri;
			}
		}
	}
	else
	{
		if(ON == p_data->remote.keyctrl.keystate_open_swing)
		{
			p_data->motor.blow_target_step = p_data->motor.blow_motor_step;
		}
		p_data->remote.keyctrl.keystate_open_swing = OFF;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_touch_ctrl_logic(datall* p_data)
{
	#ifdef XY_SYS_TEST_MODE
		if(YES == p_data->testmode)
			return;
	#endif

	fml_touch_recv_data(p_data);	
	if(TOUCH != p_data->keytype)
		return;
	
	fml_touch_deal_disp(&p_data->display);
	fml_touch_deal_relay(&p_data->remote.keyctrl);
	fml_touch_deal_motor(&p_data->motor);
	fml_touch_deal_swing(p_data);
}

