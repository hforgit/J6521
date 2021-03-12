#include "xy_sys_headfile.h"

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_display_conf_blow(datall* p_data)
{
	if(SYM_BLOW_ON == p_data->display.disp_icon_blow)
	{
		DISPLAY_SYMBOL_BLOW_ON;
	}
	else
	{
		DISPLAY_SYMBOL_BLOW_OFF;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_display_conf_warm(datall* p_data)
{
	static unsigned char s_count_2s = 0;
	
	if(SYM_WARM_ON == p_data->display.disp_icon_warm)
	{
		s_count_2s = 0;
		DISPLAY_SYMBOL_WARM_ON;
	}
	else if((SYM_WARM_TWINKLE_STANDBY == p_data->display.disp_icon_warm)
		|| (SYM_WARM_TWINKLE_ABSORB == p_data->display.disp_icon_warm))
	{
		s_count_2s++;
		if(s_count_2s >= 4)
			s_count_2s = 0;
		
		if(s_count_2s < 2)
		{
			DISPLAY_SYMBOL_WARM_ON;
		}
		else
		{
			DISPLAY_SYMBOL_WARM_OFF;
		}
	}
	else
	{
		s_count_2s = 0;
		DISPLAY_SYMBOL_WARM_OFF;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_display_conf_absorb(datall* p_data)
{
	if(SYM_ABSORB_ON == p_data->display.disp_icon_absorb)
	{
		DISPLAY_SYMBOL_ABSORB_ON;
	}
	else
	{
		DISPLAY_SYMBOL_ABSORB_OFF;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_display_conf_symbol(datall* p_data)
{
	#ifdef XY_SYS_TEST_MODE
		if(YES == p_data->testmode)
			return;
	#endif
	
	fml_display_conf_warm(p_data);
	fml_display_conf_blow(p_data);
	fml_display_conf_absorb(p_data);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_display_tx_data(datall* p_data)
{
	unsigned char send_size = 0;
	unsigned char temp_disp = 0;
	static unsigned char s_send_mode = 0;
	static unsigned char s_table[10] = {0};
	static unsigned char s_sequence  = 0;
	
	#ifdef XY_SYS_DEBUG_MODE
		unsigned char temp;
		static unsigned char s_count = 0;
	#endif

	#ifdef XY_SYS_TEST_MODE
		if(YES == p_data->testmode)
			return;
	#endif

	switch(p_data->uart.rec_protocol)
	{
		case RESET:
			s_send_mode ^= 0;				//s_send_mode ^= 1;
			temp_disp = 100;
			break;
		case OLD:
			s_send_mode = 0;
			break;
		case NEW:
			s_send_mode = 0;				//s_send_mode = 1;
			temp_disp = p_data->temperature.value;
			break;
		default:
			break;
	}
	if(0 == s_send_mode)
	{
		s_table[0]=((DISPLAY_DATA_HEAD_PRI>>8) & 0xFF);
	
		#ifdef XY_SYS_DEBUG_MODE
			if(p_data->error.dat)
			{
				s_table[1]=0x02;
				s_table[2]=p_data->display.disp_icon_data;
				if(s_count < 2)
				{
					temp = DISPLAY_GET_ERR_NUMB(p_data->error.dat);
					s_table[3]=DISPLAY_TABLE_SEG(temp);
				}
				else
				{
					s_table[3]=0;
				}
				s_count++;
				if(s_count >= 4)
					s_count = 0;
			}
			else
		#endif
			{
				s_table[1]=(DISPLAY_DATA_HEAD_PRI & 0xFF);
				s_table[2]=p_data->display.disp_icon_data;
				s_table[3]=0x00;
			}

		s_table[4]=s_table[0]+s_table[1]+s_table[2]+s_table[3];
		s_table[5]=0x16;

		send_size = 6;
	}
	else
	{
		s_table[0] = DISPLAY_DATA_HEAD_XY >> 8 & 0xFF;
		s_table[1] = DISPLAY_DATA_HEAD_XY & 0xFF;
		s_table[2] = DISPLAY_DATA_LENGTH;
		s_table[3] = DISPLAY_DATA_VERSION;
		s_table[4] = s_sequence;
		s_table[5] = DISPLAY_DATA_COMMAND;
		if(temp_disp > 99)
		{
			s_table[6] = 0x40;
			s_table[7] = 0x40;
			s_table[8] = 0x00;
		}
		else
		{
			s_table[6] = DISPLAY_TABLE_SEG(temp_disp/10) | (DISPLAY_GET_SYMBOL_BLOW   ? 0x80 : 0);
			s_table[7] = DISPLAY_TABLE_SEG(temp_disp%10) | (DISPLAY_GET_SYMBOL_ABSORB ? 0x80 : 0);
			s_table[8] = (DISPLAY_GET_SYMBOL_WARM ? 0x80 : 0);
		}

		s_table[9] = hal_alg_chk_crc8(s_table, 9);

		s_sequence++;
		send_size = 10;
	}
	p_data->uart.send_uart1_len = send_size;
	memcpy(p_data->uart.send_uart1_dat,s_table,send_size);
	p_data->uart.send_data = SET;
//	hal_serial_uart_tx_display(s_table, send_size);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_display_ack_remote(displaycontrol* p_disp)
{
	static unsigned char s_table[7] = {0};
//	static uartdata* p_data = &g_datall.uart;

	s_table[0] = DISPLAY_DATA_HEAD_XY >> 8 & 0xFF;
	s_table[1] = DISPLAY_DATA_HEAD_XY & 0xFF;
	s_table[2] = 4;
	s_table[3] = DISPLAY_DATA_VERSION;
	s_table[4] = p_disp->sequence;
	s_table[5] = p_disp->command;
	s_table[6] = hal_alg_chk_crc8(s_table, 6);
	
//	p_data->uart.send_uart1_len = 7;
//	memcpy(p_data->uart.send_uart1_dat,s_table,7);
//	p_data->uart.send_data = 1;
//	hal_serial_uart_tx_display(s_table, 7);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
keystate fml_display_deal_remote(unsigned char* p_buffer)
{
	unsigned char i;
	unsigned int  temp;
	unsigned char chk_sum = 0;
	keystate	  sta = KEY_RELEASE;

	for(i=0; i<3; i++)
	{
		chk_sum += p_buffer[i];
	}

	if(chk_sum == p_buffer[i])		///< check sum
	{
		temp = p_buffer[0];
		temp <<= 8;
		temp += p_buffer[1];
		if(temp == DISPLAY_REMOTE_KEY_HEAD)		///< check head
		{
			sta = DISPLAY_GET_KEY_VALUE(p_buffer[2]);
		}
	}

	return sta;
}

