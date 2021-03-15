#include "xy_sys_headfile.h"

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_init_reg(void)
{
	hal_port_init();
	hal_timer_Init();
	hal_serial_Init();
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_init_data(datall* p_data)
{
	p_data->remote.workmode.workmode_current 	= WORKMODULE_DEFAULT;
	p_data->remote.keyctrl.keystate_light	 	= OFF;
	p_data->remote.keyctrl.keystate_swing 		= OFF;
	p_data->remote.keyctrl.keystate_ptc 		= OFF;
	p_data->remote.keyctrl.keystate_fan_blow 	= OFF;
	p_data->remote.keyctrl.keystate_fan_absorb 	= OFF;
	p_data->remote.keyctrl.keystate_ptc_wait 	= OFF;

	p_data->remote.keyctrl.keystate_swing_pri 	= OFF;
	p_data->remote.keyctrl.keystate_enter_swing = RESET;

	p_data->remote.workmode.flag_workdelay_1s	= SET;
	p_data->remote.workmode.flag_workdelay_10s	= SET;
	p_data->remote.workmode.workdelay_1s		= CLOCK_DELAYTIMER_0S;
	p_data->remote.workmode.workdelay_10s		= CLOCK_DELAYTIMER_10S;

	p_data->remote.workmode.flag_work_colddry	= NO;
	p_data->remote.workmode.flag_work_warmdry	= NO;

	p_data->uart.rec_protocol 					= RESET;

	p_data->uart.send_data						= 0;
	p_data->display.disp_icon_data 				= 0;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_chk_error(datall* p_data)
{
	datacommon 	 err;
	unsigned int motor[4] = {0};
	switchstate  state[3] = {0};

	motor[0] = p_data->motor.blow_target_step;
	motor[1] = p_data->motor.absorb_target_step;
	motor[2] = p_data->motor.blow_motor_step;
	motor[3] = p_data->motor.absorb_motor_step;
	state[0] = p_data->remote.keyctrl.keystate_fan_blow;
	state[1] = p_data->remote.keyctrl.keystate_fan_absorb;
	state[2] = p_data->remote.keyctrl.keystate_ptc;
	err.dat  = 0;

//	if(p_data->remote.workmode.workmode_current != WORKMODULE_STANDBY)
//	{
		if(YES == CTRL_CHECK_ERROR_NUMB_1(motor[0], motor[1], state[0], state[1], state[2]))
		{
			p_data->motor.blow_target_step = RESET_STEP_ZERO;
			err.bits.b0 = 1;
		}
//	}

	if(YES == CTRL_CHECK_ERROR_NUMB_2(motor[0], motor[1], state[0], state[1], state[2]))
	{
		p_data->motor.absorb_target_step = RESET_STEP_ZERO;
		err.bits.b1 = 1;
	}

	if(YES == CTRL_CHECK_ERROR_NUMB_3(motor[0], motor[1], state[0], state[1], state[2]))
	{
		p_data->motor.absorb_target_step = RESET_STEP_ZERO;
		err.bits.b2 = 1;
	}

	if(YES == CTRL_CHECK_ERROR_NUMB_4(motor[0], motor[1], state[0], state[1], state[2])) 
	{
		p_data->remote.keyctrl.keystate_fan_blow = OFF;
		err.bits.b3 = 1;
	}

	if(YES == CTRL_CHECK_ERROR_NUMB_5(motor[0], motor[2], state[0], state[1], state[2]))
	{
		if(p_data->motor.blow_motor_step && (NO == MOTOR_CHK_STEP_IS_LEGAL(p_data->motor.blow_motor_step)))		//?
		{
			p_data->remote.keyctrl.keystate_open_swing = OFF;
		}

		if(OFF == p_data->remote.keyctrl.keystate_open_swing)
		{
			p_data->remote.keyctrl.keystate_fan_blow = OFF;
			err.bits.b4 = 1;
			//?
		}
	}

	if(YES == CTRL_CHECK_ERROR_NUMB_6(motor[1], motor[3], state[0], state[1], state[2]))
	{
		p_data->remote.keyctrl.keystate_fan_blow = OFF;
		err.bits.b5 = 1;
	}

	if(YES == CTRL_CHECK_ERROR_NUMB_7(motor[0], motor[1], state[0], state[1], state[2]))
	{
		p_data->remote.keyctrl.keystate_ptc = OFF;
		err.bits.b6 = 1;
	}
	p_data->error.dat = err.dat;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_icon_display(datall* p_data)
{	
	#ifdef XY_SYS_TEST_MODE
		if(YES == p_data->testmode)
			return;
	#endif
	
	if(REMOTE != p_data->keytype)
		return;
	
	switch(p_data->remote.workmode.workmode_current)
	{
		case WORKMODULE_STANDBY:
			if(p_data->remote.workmode.workdelay_10s < CLOCK_DELAYTIMER_10S)		//workdelay!
			{
				p_data->display.disp_icon_warm = SYM_WARM_TWINKLE_STANDBY;
			}
			else
			{
				p_data->display.disp_icon_warm = SYM_WARM_OFF;
			}
			p_data->display.disp_icon_absorb = SYM_ABSORB_OFF;
			p_data->display.disp_icon_blow	   = SYM_BLOW_OFF;
			break;
		case WORKMODULE_BLOW:
			p_data->display.disp_icon_warm     = SYM_WARM_OFF;
			p_data->display.disp_icon_absorb = SYM_ABSORB_OFF;
			p_data->display.disp_icon_blow 	    = SYM_BLOW_ON;
			break;
		case WORKMODULE_WARM:
			p_data->display.disp_icon_warm 	    = SYM_WARM_ON;
			p_data->display.disp_icon_absorb = SYM_ABSORB_OFF;
			p_data->display.disp_icon_blow 	   = SYM_BLOW_OFF;
			break;
		case WORKMODULE_ABSORB:
			if(p_data->remote.workmode.workdelay_10s < CLOCK_DELAYTIMER_10S)
			{
				p_data->display.disp_icon_warm = SYM_WARM_TWINKLE_ABSORB;
			}
			else
			{
				p_data->display.disp_icon_warm = SYM_WARM_OFF;
			}	
			p_data->display.disp_icon_absorb  = SYM_ABSORB_ON;
			p_data->display.disp_icon_blow 	   = SYM_BLOW_OFF;
			break;
		default:
			p_data->display.disp_icon_warm     = SYM_WARM_OFF;				
			p_data->display.disp_icon_absorb = SYM_ABSORB_OFF;
			p_data->display.disp_icon_blow 	   = SYM_BLOW_OFF;
			break;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_recv_old_version(datall* p_data)
{
	unsigned char i;
	unsigned char temp_sum = 0;
	unsigned char buffer[sizeof(p_data->uart.rec_uart1_dat)] = {0};

	memcpy(buffer, p_data->uart.rec_uart1_dat, sizeof(buffer));
	memset(p_data->uart.rec_uart1_dat, 0, sizeof(buffer));

	for(i=0; i<5; i++)
	{
		temp_sum += buffer[i];
	}

	if(temp_sum == buffer[i])			///< check data 
	{
		p_data->remote.key.keyremote.dat = buffer[2] + (buffer[3]<<8);

		if(p_data->remote.key.keyremote.bits.b0)
		{
			p_data->remote.key.keysta = KEY_LIGHT;
		}
		else if(p_data->remote.key.keyremote.bits.b1)
		{
			p_data->remote.key.keysta = KEY_BLOW;
		}
		else if(p_data->remote.key.keyremote.bits.b2)
		{
			p_data->remote.key.keysta = KEY_ABSORB;
		}
		else if(p_data->remote.key.keyremote.bits.b3)
		{
			p_data->remote.key.keysta = KEY_WARM;
		}
		else if(p_data->remote.key.keyremote.bits.b4)
		{
			p_data->remote.key.keysta = KEY_SWING;
		}
		else if(p_data->remote.key.keyremote.bits.b5)
		{
			p_data->remote.key.keysta = KEY_RAV;
		}
		else if(p_data->remote.key.keyremote.bits.b6)
		{
				p_data->remote.key.keysta = KEY_COLD_DRY;
		}
		else if(p_data->remote.key.keyremote.bits.b7)
		{
			p_data->remote.key.keysta = KEY_STANDBY;
		}
		else if(p_data->remote.key.keyremote.bits.b8)
		{
			p_data->remote.key.keysta = KEY_WARM_DRY;
		}

		if(KEY_RELEASE != p_data->remote.key.keysta)
		{
			if(TOUCH == p_data->keytype)
			{
				memset(&p_data->touch, 0, sizeof(p_data->touch));
				fml_ctrl_init_data(p_data);
				p_data->keytype = REMOTE;
			}
		}

		p_data->temperature.value = buffer[4];
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_recv_new_version(datall* p_data)
{
	static displaycontrol	s_dispbuffer;
	
	s_dispbuffer.length    = p_data->uart.rec_uart1_dat[2];
	s_dispbuffer.version   = p_data->uart.rec_uart1_dat[3];
	s_dispbuffer.sequence  = p_data->uart.rec_uart1_dat[4];
	s_dispbuffer.command   = (dispcommand)p_data->uart.rec_uart1_dat[5];
	s_dispbuffer.p_buffer  = &p_data->uart.rec_uart1_dat[6];
	s_dispbuffer.checkcode = p_data->uart.rec_uart1_dat[s_dispbuffer.length + 2];

	/* match check code about CRC-8 */
	if(hal_alg_chk_crc8(p_data->uart.rec_uart1_dat,s_dispbuffer.length + 2) == s_dispbuffer.checkcode)			///< check data
	{
		switch(s_dispbuffer.command)
		{
			case DISPLAY_CMD_REMOTE:
				fml_display_ack_remote(&s_dispbuffer);
				p_data->remote.key.keysta = fml_display_deal_remote(s_dispbuffer.p_buffer);
				if(KEY_RELEASE != p_data->remote.key.keysta)
				{
					if(TOUCH == p_data->keytype)
					{
						memset(&p_data->touch, 0, sizeof(p_data->touch));
						fml_ctrl_init_data(p_data);
						p_data->keytype = REMOTE;
					}
				}
				break;
			case DISPLAY_CMD_TEMP:
				p_data->temperature.value = hal_ad_cal_temperature(s_dispbuffer.p_buffer);
				break;
			case DISPLAY_CMD_DIGITAL:
				break;
			default:
				break;
		}
	}	
	memset(p_data->uart.rec_uart1_dat, 0, s_dispbuffer.length + 3);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_recv_display(datall* p_data)
{
	if(p_data->uart.rec_uart1_ok)
	{
		p_data->uart.rec_uart1_ok = 0;
		switch(p_data->uart.rec_protocol)
		{
			case OLD:
				fml_ctrl_recv_old_version(p_data);			///< old version(only use remote key)
				break;
			case NEW:
				fml_ctrl_recv_new_version(p_data);			///< new vesion protocol
				break;
			default:
				break;
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_deal_key(datall* p_data)
{	
	switch(p_data->remote.key.keysta)
	{
		case KEY_STANDBY:
			if(KEY_STANDBY != p_data->remote.key.keysta_pri)
			{
				p_data->remote.key.keysta_pri = KEY_STANDBY;

				p_data->buzzer.normal_bee_on = ON;
				p_data->remote.workmode.workmode_current = WORKMODULE_STANDBY;

				p_data->remote.workmode.flag_work_warmdry = NO;
				p_data->remote.workmode.flag_work_colddry = NO;
			}
			else
			{
				p_data->buzzer.burn_bee_on = ON;
			}

			break;
		case KEY_BLOW:
			if(KEY_BLOW != p_data->remote.key.keysta_pri)
			{
				p_data->remote.key.keysta_pri = KEY_BLOW;

				p_data->buzzer.normal_bee_on = ON;
				p_data->remote.workmode.workmode_current = WORKMODULE_BLOW;

				p_data->remote.workmode.flag_work_warmdry = NO;
				p_data->remote.workmode.flag_work_colddry = NO;
			}
			else
			{
				p_data->buzzer.burn_bee_on = ON;
			}
			break;
		case KEY_ABSORB:
			if(KEY_ABSORB != p_data->remote.key.keysta_pri)
			{
				p_data->remote.key.keysta_pri = KEY_ABSORB;
				
				p_data->buzzer.normal_bee_on = ON;
				p_data->remote.workmode.workmode_current = WORKMODULE_ABSORB;

				p_data->remote.workmode.flag_work_warmdry = NO;
				p_data->remote.workmode.flag_work_colddry = NO;
			}
			else
			{
				p_data->buzzer.burn_bee_on = ON;
			}
			break;
		case KEY_WARM:
			if(KEY_WARM != p_data->remote.key.keysta_pri)					
			{
				p_data->remote.key.keysta_pri = KEY_WARM;

				p_data->buzzer.normal_bee_on = ON;
				p_data->remote.workmode.workmode_current = WORKMODULE_WARM;

				p_data->remote.workmode.flag_work_warmdry = NO;
				p_data->remote.workmode.flag_work_colddry = NO;
			}
			else
			{
				p_data->buzzer.burn_bee_on = ON;
			}
			break;

		case KEY_COLD_DRY:
			if(KEY_COLD_DRY != p_data->remote.key.keysta_pri)
			{
				p_data->remote.key.keysta_pri = KEY_COLD_DRY;
				
				p_data->buzzer.normal_bee_on = ON;
				//p_data->remote.workmode.workmode_cur = WORKMODULE_S1_BLOW;

				p_data->remote.workmode.flag_work_colddry = YES;
				p_data->remote.workmode.flag_work_warmdry = NO;
			}
			else
			{
				p_data->buzzer.burn_bee_on = ON;
			}
			break;

		case KEY_WARM_DRY:
			if(KEY_WARM_DRY != p_data->remote.key.keysta_pri)
			{
				p_data->remote.key.keysta_pri = KEY_WARM_DRY;
				
				p_data->buzzer.normal_bee_on = ON;
				//p_data->remote.workmode.workmode_cur = WORKMODULE_S2_WARM;
				
				p_data->remote.workmode.flag_work_warmdry = YES;
				p_data->remote.workmode.flag_work_colddry = NO;
			}
			else
			{
				p_data->buzzer.burn_bee_on = ON;
			}
			break;
		case KEY_SWING:
			/* only blow mode or warm mode and dry can enter this condition */
			if(((WORKMODULE_BLOW == p_data->remote.workmode.workmode_current)
				|| (WORKMODULE_WARM == p_data->remote.workmode.workmode_current)))
			{
				p_data->buzzer.normal_bee_on = ON;
				if(OFF == p_data->remote.keyctrl.keystate_swing)
				{
					/* open swing page */
					p_data->remote.keyctrl.keystate_swing = ON;
				}
				else
				{
					/* close swing page */
					p_data->remote.keyctrl.keystate_swing = OFF;
				}
			}
			else
			{
				p_data->buzzer.burn_bee_on = ON;
			}
			break;
		case KEY_LIGHT:
			p_data->buzzer.normal_bee_on = ON;
			if(OFF == p_data->remote.keyctrl.keystate_light)
			{
				p_data->remote.keyctrl.keystate_light = ON;
			}
			else
			{
				p_data->remote.keyctrl.keystate_light = OFF;
			}
			break;
		default:
			break;
	}

	p_data->remote.key.keysta = KEY_RELEASE;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_deal_automode(workmoduleflag* p_mode)							//check
{
	static switchstate	 s_onetime  = RESET;
	static unsigned char s_time_dry_tmp = 0;
	static unsigned int  s_time_dry_min = 0;
	static switchstate 	 s_state    = RESET;

	if((YES == p_mode->flag_work_warmdry) || (YES == p_mode->flag_work_colddry))
	{
		if(YES == p_mode->flag_work_warmdry)
		{
			if(WARM != s_state)
			{
				s_onetime = RESET;
				s_time_dry_min = CLOCK_DELAYTIMER_0S;
			}
			s_state = WARM;
		}
		else
		{
			if(COLD != s_state)
			{
				s_onetime = RESET;
				s_time_dry_min = CLOCK_DELAYTIMER_0S;
			}
			s_state = COLD;
		}
		if(YES == p_mode->flag_work_warmdry)
		{
			if((s_time_dry_min % (CLOCK_WARMDRY_WARM_10MIN + CLOCK_WARMDRY_ABSORB_5MIN)) 
				< (CLOCK_WARMDRY_WARM_10MIN - CLOCK_DELAYTIMER_10S))
			{
				p_mode->workmode_current = WORKMODULE_WARM;
			}
			else
			{
				p_mode->workmode_current = WORKMODULE_ABSORB;
			}
		}
		else
		{
			if((s_time_dry_min % (CLOCK_COLDDRY_COLD_10MIN + CLOCK_WARMDRY_ABSORB_5MIN)) 
				< CLOCK_COLDDRY_COLD_10MIN)
			{
				p_mode->workmode_current = WORKMODULE_BLOW;
			}
			else
			{
				p_mode->workmode_current = WORKMODULE_ABSORB;
			}
		}
		
		if(RESET == s_onetime)
		{
			s_onetime = SET;
			s_time_dry_tmp = p_mode->workdelay_cyc;
		}
		if(YES == CTRL_EXCEED_DELAY_TIMER(1, p_mode->workdelay_cyc, s_time_dry_tmp))
		{
			s_time_dry_tmp = p_mode->workdelay_cyc;
			s_time_dry_min++;
			if(s_time_dry_min >= CLOCK_WORKDRY_ALL_60MIN)
			{
				p_mode->workmode_current = WORKMODULE_STANDBY;
				if(YES == p_mode->flag_work_warmdry)
				{	
					p_mode->flag_work_warmdry = NO;
				}
				else
				{
					p_mode->flag_work_colddry = NO;
				}
			}
		}
	}
	else
	{
		s_state = RESET;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_deal_mode(datall* p_data)
{	
	static switchstate   s_onetime[2] = {STEP1, RESET};
	static unsigned char s_timecount_cur = 0, s_timecount_pri = 0;
	static unsigned char s_time_ptc_worktime = 0;

	fml_ctrl_deal_automode(&p_data->remote.workmode);
	switch(p_data->remote.workmode.workmode_current)
	{
		case WORKMODULE_STANDBY:
			if(YES == p_data->remote.keyctrl.keystate_ptc_delay)
			{
				p_data->remote.workmode.workdelay_10s = CLOCK_DELAYTIMER_0S;
			}
			p_data->remote.keyctrl.keystate_ptc = OFF;
			p_data->remote.workmode.flag_workdelay_1s = RESET;
			p_data->remote.workmode.workdelay_1s = CLOCK_DELAYTIMER_0S;
			p_data->remote.key.keysta_pri = KEY_STANDBY;
			break;
		case WORKMODULE_BLOW:
			if(YES == p_data->remote.keyctrl.keystate_ptc_delay)
			{
				p_data->remote.workmode.workdelay_10s = CLOCK_DELAYTIMER_0S;
			}
			p_data->remote.keyctrl.keystate_ptc = OFF;
			p_data->remote.workmode.flag_workdelay_1s = RESET;
			p_data->remote.workmode.workdelay_1s = CLOCK_DELAYTIMER_1S;
			break;
		case WORKMODULE_WARM:

				if(p_data->remote.workmode.workdelay_10s >= CLOCK_DELAYTIMER_10S) 	///< delay 10s
				{
					if(ON == p_data->remote.keyctrl.keystate_move_target)			///< move target step
					{
						if(ON == p_data->remote.keyctrl.keystate_ptc_wait)
						{
							p_data->remote.workmode.flag_workdelay_1s = RESET;
							p_data->remote.workmode.workdelay_1s = CLOCK_DELAYTIMER_0S;
						}
						
						p_data->remote.keyctrl.keystate_ptc_wait = OFF;

					}
					else
					{
						p_data->motor.blow_target_step = p_data->motor.blow_motor_step_pri;		///< start from last step
					}
				}
				else
				{
					if(STEP3 == s_onetime[0])
					{
						if(ON == p_data->remote.keyctrl.keystate_ptc_wait)
						{
							if(NO == CTRL_EXCEED_DELAY_TIMER(10, s_timecount_cur, s_timecount_pri))
							{
								p_data->remote.workmode.flag_workdelay_1s = SET;
								p_data->remote.keyctrl.keystate_ptc_wait = OFF;
								p_data->remote.workmode.workdelay_10s = CLOCK_DELAYTIMER_10S;
								p_data->remote.workmode.workdelay_1s = CLOCK_DELAYTIMER_1S;
								p_data->remote.keyctrl.keystate_move_target = ON;
							}
						}
					}
				}
			
			if(p_data->remote.workmode.workdelay_1s >= CLOCK_DELAYTIMER_1S)
			{
				if((OFF == p_data->remote.keyctrl.keystate_ptc_wait) 
			//		&& (YES == MOTOR_CHK_MOVE_TO_TARGET))
					&& (ON == p_data->remote.keyctrl.keystate_move_target))
				{
					p_data->remote.keyctrl.keystate_ptc   = ON;			///< restart ptc
					p_data->remote.workmode.workdelay_10s = CLOCK_DELAYTIMER_10S;
				}
			}
			break;
		case WORKMODULE_ABSORB:
			if(YES == p_data->remote.keyctrl.keystate_ptc_delay)
			{
				p_data->remote.workmode.workdelay_10s = CLOCK_DELAYTIMER_0S;
			}
			p_data->remote.keyctrl.keystate_ptc = OFF;
			p_data->remote.workmode.flag_workdelay_1s = RESET;
			p_data->remote.workmode.workdelay_1s = CLOCK_DELAYTIMER_0S;
			break;
		default:
			break;
	}
	if(p_data->remote.keyctrl.keystate_ptc_pri != p_data->remote.keyctrl.keystate_ptc)
	{
		if(ON == p_data->remote.keyctrl.keystate_ptc)
		{
			if(RESET == s_onetime[1])
			{
				s_time_ptc_worktime = p_data->remote.workmode.workdelay_cyc;
			}
			s_onetime[1] = SET;
		}
		else
		{
			s_onetime[1] = RESET;
		}
		p_data->remote.keyctrl.keystate_ptc_pri = p_data->remote.keyctrl.keystate_ptc;	
	}
	else
	{
		if(ON == p_data->remote.keyctrl.keystate_ptc_pri)
		{
			if(YES == CTRL_EXCEED_DELAY_TIMER(0, p_data->remote.workmode.workdelay_cyc, s_time_ptc_worktime))
			{
				p_data->remote.keyctrl.keystate_ptc_delay = YES;
			}
		}
		else
		{
			p_data->remote.keyctrl.keystate_ptc_delay = NO;
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_deal_motor(datall* p_data)
{
	static switchstate s_step = STEP1;

	switch(p_data->remote.workmode.workmode_current)
	{
		case WORKMODULE_STANDBY:
			if(p_data->remote.workmode.workdelay_10s >= CLOCK_DELAYTIMER_10S)
			{
				p_data->motor.blow_target_step 	 = RESET_STEP_ZERO;
				p_data->motor.absorb_target_step = RESET_STEP_ZERO;

				p_data->remote.keyctrl.keystate_enter_swing = RESET;
			}
			else
			{
				if(ON == p_data->remote.keyctrl.keystate_swing)
				{
					p_data->motor.blow_target_step 	= p_data->motor.blow_motor_step_pri;
				}
			}
			break;	
		case WORKMODULE_BLOW:
		case WORKMODULE_WARM:
			if((p_data->remote.workmode.workdelay_10s = CLOCK_DELAYTIMER_10S)
				|| (ON == p_data->remote.keyctrl.keystate_open_swing))
			{
				if(RESET == p_data->remote.keyctrl.keystate_enter_swing)
				{
					s_step = STEP1;
					p_data->remote.keyctrl.keystate_enter_swing = SET;
					p_data->motor.blow_target_step	   = TARGET_STEP_BLOW;
					p_data->motor.blow_motor_step_pri  = TARGET_STEP_BLOW;
					p_data->motor.blow_target_step_pri = TARGET_LOW_SWING;
				}
				else
				{
					if(ON == p_data->remote.keyctrl.keystate_swing)
					{
						switch(s_step)
						{
							case STEP1:
								p_data->motor.blow_target_step = p_data->motor.blow_motor_step_pri;
								if(p_data->motor.blow_target_step == p_data->motor.blow_motor_step)
								{
									p_data->remote.keyctrl.keystate_move_target = ON;
									s_step = STEP2;
								}
								break;
							case STEP2:
								p_data->remote.keyctrl.keystate_move_target = ON;
								p_data->motor.blow_target_step = p_data->motor.blow_target_step_pri;
								if(p_data->motor.blow_target_step == p_data->motor.blow_motor_step)
								{
									s_step = STEP3;
								}
								break;
							case STEP3:
								p_data->remote.keyctrl.keystate_move_target = ON;
								if(p_data->motor.blow_motor_step == TARGET_HIGH_SWING)
								{
									p_data->motor.blow_target_step 		= TARGET_LOW_SWING;
									p_data->motor.blow_target_step_pri	= TARGET_LOW_SWING;		///< save target step
								}
								else if(p_data->motor.blow_motor_step == TARGET_LOW_SWING)
								{
									p_data->motor.blow_target_step 		= TARGET_HIGH_SWING;
									p_data->motor.blow_target_step_pri	= TARGET_HIGH_SWING;	///< save target step
								}
								else
								{
									if(p_data->motor.blow_target_step == p_data->motor.blow_motor_step)
									{
										p_data->motor.blow_target_step = p_data->motor.blow_target_step_pri;
									}
								}
								break;
							default:
								break;
						}
					}
					else
					{
						p_data->motor.blow_target_step = p_data->motor.blow_motor_step_pri;
					}
					p_data->motor.absorb_target_step = RESET_STEP_ZERO;

					if(YES == MOTOR_CHK_MOVE_TO_TARGET)
					{
						p_data->remote.keyctrl.keystate_move_target = ON;
					}
				}
			}
			break;
		case WORKMODULE_ABSORB:
			if(p_data->remote.workmode.workdelay_10s >= CLOCK_DELAYTIMER_10S)
			{
				p_data->motor.blow_target_step 	 = RESET_STEP_ZERO;
				p_data->motor.absorb_target_step = TARGET_STEP_ABSORB;

				p_data->remote.keyctrl.keystate_enter_swing = RESET;
			}
			else
			{
				if(ON == p_data->remote.keyctrl.keystate_swing)
				{
					p_data->motor.blow_target_step 	= p_data->motor.blow_motor_step_pri;	
				}
			}
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
void fml_ctrl_deal_fan(datall* p_data)
{
	switch(p_data->remote.workmode.workmode_current)
	{
		case WORKMODULE_STANDBY:
			if(p_data->remote.workmode.workdelay_10s >= CLOCK_DELAYTIMER_10S)
			{
				p_data->remote.keyctrl.keystate_fan_blow 	= OFF;
				p_data->remote.keyctrl.keystate_move_target = OFF;
			}
			p_data->remote.keyctrl.keystate_fan_absorb 		= OFF;
			break;
		case WORKMODULE_BLOW:
		case WORKMODULE_WARM:
			if((ON == p_data->remote.keyctrl.keystate_move_target) ///< move to target step.
				|| (ON == p_data->remote.keyctrl.keystate_open_swing))	///< need open blow fan when work swing func.
			{
				p_data->remote.workmode.flag_workdelay_1s = SET;
				p_data->remote.keyctrl.keystate_fan_blow  = ON;
			}
			else
			{
				if(p_data->remote.workmode.workdelay_10s >= CLOCK_DELAYTIMER_10S)
				{
					p_data->remote.workmode.flag_workdelay_1s	= RESET;
					p_data->remote.keyctrl.keystate_move_target = OFF;
					p_data->remote.keyctrl.keystate_fan_blow 	= OFF;
				}								
			}
			p_data->remote.keyctrl.keystate_fan_absorb 		= OFF;
			break;
		case WORKMODULE_ABSORB:
			if(p_data->remote.workmode.workdelay_10s >= CLOCK_DELAYTIMER_10S)
			{
				p_data->remote.keyctrl.keystate_fan_blow 	= OFF;
				p_data->remote.keyctrl.keystate_move_target = OFF;
					if(YES == MOTOR_CHK_MOVE_TO_TARGET)
					{
						p_data->remote.keyctrl.keystate_fan_absorb 	= ON;
					}
			}
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
void fml_ctrl_deal_swing(datall* p_data)
{
	switch(p_data->remote.workmode.workmode_current)
	{
		case WORKMODULE_STANDBY:
			p_data->remote.keyctrl.keystate_open_swing = OFF;
			p_data->remote.keyctrl.keystate_swing 	   = OFF;
			break;
		case WORKMODULE_BLOW:
		case WORKMODULE_WARM:

				if(p_data->remote.keyctrl.keystate_swing_pri != p_data->remote.keyctrl.keystate_swing)
				{
					p_data->remote.keyctrl.keystate_swing_pri = p_data->remote.keyctrl.keystate_swing;
					if(ON == p_data->remote.keyctrl.keystate_swing)						//?
					{
						p_data->remote.workmode.workdelay_10s = CLOCK_DELAYTIMER_10S;
					}
					p_data->remote.keyctrl.keystate_enter_swing = SET;
					p_data->motor.blow_target_step = p_data->motor.blow_motor_step_pri;	///< move to last step
				}
				else
				{
					if((ON == p_data->remote.keyctrl.keystate_swing)				///< open swing func
						&& (ON == p_data->remote.keyctrl.keystate_move_target))		///< move to target step
					{
						p_data->remote.keyctrl.keystate_open_swing = ON;
					}
					else
					{
						p_data->remote.keyctrl.keystate_open_swing = OFF;
					}
				}

			break;
		case WORKMODULE_ABSORB:
			p_data->remote.keyctrl.keystate_open_swing = OFF;
			if((NO == p_data->remote.workmode.flag_work_warmdry)
				&& (NO == p_data->remote.workmode.flag_work_colddry))
			{
				p_data->remote.keyctrl.keystate_swing 	   = OFF;
			}
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
void fml_ctrl_deal_timer(datall* p_data)
{
	if(REMOTE != p_data->keytype)
		return;
	
	hal_timer_mode_deal(&p_data->remote.workmode);
}
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_key_logic(datall* p_data)
{
	#ifdef XY_SYS_TEST_MODE
		if(YES == p_data->testmode)
			return;
	#endif
	fml_ctrl_recv_display(p_data);							///< recive remote key value

	if(REMOTE != p_data->keytype)
		return;

	fml_ctrl_deal_key(p_data);								///< analysis key value 
	fml_ctrl_deal_mode(p_data);								///< control relay by diff mode
	fml_ctrl_deal_motor(p_data);								///< update motor step
	fml_ctrl_deal_swing(p_data);								///< control swing func
	fml_ctrl_deal_fan(p_data);								///< control fan machine
	fml_ctrl_chk_error(p_data);								///< check data result legal
}


/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_work_time(datall* p_data)
{
	static workmodule  s_workmode = WORKMODULE_STANDBY;
	static unsigned int  s_work_min = 0;

	if(REMOTE != p_data->keytype)
		return;
	
	if(s_workmode != p_data->remote.workmode.workmode_current)
	{
		s_workmode = p_data->remote.workmode.workmode_current;
		s_work_min = 0;
	}
	
	switch(p_data->remote.workmode.workmode_current)
	{
		case WORKMODULE_STANDBY:
			s_work_min = 0;
			break;
		case WORKMODULE_BLOW:
		case WORKMODULE_WARM:
		case WORKMODULE_ABSORB:
			if((NO == p_data->remote.workmode.flag_work_warmdry)
				&& (NO == p_data->remote.workmode.flag_work_colddry))
			{
				s_work_min++;
			}
			break;
		default:
			break;
	}
	
	if(s_work_min >= CLOCK_WORKTIME_120MIN)
	{
		p_data->remote.workmode.workmode_current = WORKMODULE_STANDBY;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_ctrl_deal_alarm(datall* p_data)
{
	if((p_data->temperature.value >= ALARM_HIGH_TEMP))
	{
		p_data->remote.workmode.workmode_current = WORKMODULE_STANDBY;
	}
}

