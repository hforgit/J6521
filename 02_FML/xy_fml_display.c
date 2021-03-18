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
	static unsigned char s_table[10] = {0};
	
	#ifdef XY_SYS_DEBUG_MODE
		unsigned char temp;
		static unsigned char s_count = 0;
	#endif

	#ifdef XY_SYS_TEST_MODE
		if(YES == p_data->testmode)
			return;
	#endif

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
	
	p_data->uart.send_uart1_len = send_size;
	memcpy(p_data->uart.send_uart1_dat,s_table,send_size);
	p_data->uart.send_data = 1;
}