#include "xy_sys_headfile.h"

static datall* s_p_data = &g_datall;

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void apl_init_data(void)
{
	fml_buzzer_init_data(s_p_data);
	fml_ctrl_init_data(s_p_data);
	fml_touch_init_data(s_p_data);
	fml_motor_init_data(s_p_data);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void apl_config_data(void)
{
	s_p_data->buzzer.normal_bee_on	= ON; 
	s_p_data->keytype 			= REMOTE;
	memset(&g_timebase.isTrigInterrupt, 0, sizeof(g_timebase));
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void apl_config_register(void)
{
	fml_ctrl_init_reg();
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void apl_initialize(void)
{
	apl_init_data();
	apl_config_data();
	apl_config_register();
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void apl_logic_ctrl(void)
{
	/* period: 125us */
	if (g_timebase.isTrigInterrupt)
    {
		g_timebase.isTrigInterrupt = 0;
		g_timebase.timebase_125us++;
    }

	/* period: 1ms */
    if (g_timebase.timebase_125us >= 8)
    {
        g_timebase.timebase_125us = 0;
		g_timebase.timebase_1ms++;

		fml_buzzer_timer_cnt(s_p_data);
		fml_motor_timer_ctrl(s_p_data);
		fml_motor_ctrl_move(s_p_data);
		
		if(s_p_data->uart.send_data == 1)
		{
			hal_irq_uart1_send(s_p_data->uart.send_uart1_dat,s_p_data->uart.send_uart1_len);		
		}
	}

	/* period: 10ms */
    if (g_timebase.timebase_1ms >= 10)
    {
        g_timebase.timebase_1ms = 0;
        g_timebase.timebase_10ms++;
		g_timebase.timebase_100ms++;
        g_timebase.timebase_250ms++;

		fml_ctrl_key_logic(s_p_data);
		fml_touch_ctrl_logic(s_p_data);
    }

	/* period: 50ms */
    if (g_timebase.timebase_10ms >= 5)
    {
		g_timebase.timebase_10ms = 0;

		fml_buzzer_deal(s_p_data);
    }

	/* period: 100ms */
    if (g_timebase.timebase_100ms >= 9)
    {
        g_timebase.timebase_100ms = 0;

		fml_ctrl_icon_display(s_p_data);
    }

	/* period: 250ms */
    if (g_timebase.timebase_250ms >= 24)
    {
        g_timebase.timebase_250ms = 0;
		g_timebase.timebase_500ms++;

		fml_relay_ctrl(s_p_data);
		fml_display_conf_symbol(s_p_data);
		fml_display_tx_data(s_p_data);
		watchdog();	
    }

	/* period: 500ms */
    if (g_timebase.timebase_500ms >= 2)
    {
        g_timebase.timebase_500ms = 0;
        g_timebase.timebase_1s++;

		#ifdef XY_SYS_TEST_MODE
			s_p_data->testmode = fml_test_check();
			fml_test_logic(s_p_data);
		#endif
    }

	/* period: 1s */
    if (g_timebase.timebase_1s >= 2)
    {
        g_timebase.timebase_1s = 0;

		fml_ctrl_deal_timer(s_p_data);
		fml_ctrl_work_time(s_p_data);
		fml_ctrl_deal_alarm(s_p_data);
    }
}
