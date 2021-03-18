#include "xy_sys_headfile.h"

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_relay_light(datall* p_data)
{
	if(ON == p_data->remote.keyctrl.keystate_light)
	{
		light_on();
	}
	else
	{
		light_off();
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_relay_ptc(datall* p_data)
{
	if(ON == p_data->remote.keyctrl.keystate_ptc)
	{
		PTC_on();
	}
	else
	{
		PTC_off();
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_relay_fan_blow(datall* p_data)
{
	if(ON == p_data->remote.keyctrl.keystate_fan_blow)
	{
		blow_fan_on();
	}
	else
	{
		blow_fan_off();
	}
}
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_relay_fan_absorb(datall* p_data)
{
	if(ON == p_data->remote.keyctrl.keystate_fan_absorb)
	{
		rav_on();
	}
	else
	{
		rav_off();
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_relay_ctrl(datall* p_data)
{
	#ifdef XY_SYS_TEST_MODE
		if(YES == p_data->testmode)
			return;
	#endif
	
	fml_relay_light(p_data);
	fml_relay_ptc(p_data);
	fml_relay_fan_blow(p_data);
	fml_relay_fan_absorb(p_data);
}
