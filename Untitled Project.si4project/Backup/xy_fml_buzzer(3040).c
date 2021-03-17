#include "xy_sys_headfile.h"

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_buzzer_init_data(datall* p_data)
{
	p_data->buzzer.bee_state			= OFF;
	p_data->buzzer.normal_bee_on 		= OFF;
	p_data->buzzer.long_bee_on 			= OFF;
	p_data->buzzer.burn_bee_on 			= OFF;
	p_data->buzzer.bee_count			= 0;
	p_data->buzzer.normal_bee_on_count	= 0;
	p_data->buzzer.normal_bee_on_flag	= 0;
	p_data->buzzer.long_bee_on_count	= 0;
	p_data->buzzer.long_bee_on_flag		= 0;
	p_data->buzzer.burn_bee_count		= 0;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_buzzer_timer_ctrl(datall* p_data)
{
	hal_timer_buzzer(&p_data->buzzer);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_buzzer_timer_cnt(datall* p_data)
{	
	p_data->buzzer.normal_bee_on_count++;
	p_data->buzzer.long_bee_on_count++;
	if(p_data->buzzer.normal_bee_on_count>=150)			
	{
		p_data->buzzer.normal_bee_on_count=150;
		p_data->buzzer.normal_bee_on_flag=1;
	}	
	if(p_data->buzzer.long_bee_on_count>=800)				
	{
		p_data->buzzer.long_bee_on_count=800;
		p_data->buzzer.long_bee_on_flag=1;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void fml_buzzer_deal(datall* p_data)
{
	if(p_data->buzzer.normal_bee_on==ON)		
	{
		if(p_data->buzzer.bee_state==OFF)
		{		
			p_data->buzzer.bee_state = ON;
			p_data->buzzer.normal_bee_on_count=0;
			p_data->buzzer.normal_bee_on_flag=0;			
		}
		if(p_data->buzzer.normal_bee_on_flag)
		{
			p_data->buzzer.bee_state=OFF;
			p_data->buzzer.normal_bee_on=OFF;
		}
	}
	else if(p_data->buzzer.long_bee_on==ON)				
	{
		if(p_data->buzzer.bee_state==OFF)
		{		
			p_data->buzzer.bee_state = ON;
			p_data->buzzer.long_bee_on_count=0;
			p_data->buzzer.long_bee_on_flag=0; 		
		}
		if(p_data->buzzer.long_bee_on_flag)
		{
			p_data->buzzer.bee_state=OFF;
			p_data->buzzer.long_bee_on=OFF;
		}
	}	
	else if(p_data->buzzer.burn_bee_on==ON)					//bee rings twice 
	{
		if(p_data->buzzer.burn_bee_count==0)
		{
			if(p_data->buzzer.bee_state==OFF)
			{		
				p_data->buzzer.bee_state = ON;
				p_data->buzzer.normal_bee_on_count=0;
				p_data->buzzer.normal_bee_on_flag=0;			 
			}
			if(p_data->buzzer.normal_bee_on_flag)
			{
				p_data->buzzer.bee_state=OFF;
				p_data->buzzer.burn_bee_count=1;
				p_data->buzzer.normal_bee_on_count=0;
				p_data->buzzer.normal_bee_on_flag=0;
			}		
		}
		else if(p_data->buzzer.burn_bee_count==1)
		{
			if((p_data->buzzer.bee_state==OFF)&&(p_data->buzzer.normal_bee_on_flag))
			{
				p_data->buzzer.bee_state = ON;
				p_data->buzzer.normal_bee_on_count=0;
				p_data->buzzer.normal_bee_on_flag=0;						
			}
			if((p_data->buzzer.bee_state==ON)&&(p_data->buzzer.normal_bee_on_flag))
			{
				p_data->buzzer.burn_bee_count=0;
				p_data->buzzer.bee_state=OFF;
				p_data->buzzer.burn_bee_on=OFF;
			}					
		}
		else
		{
			p_data->buzzer.burn_bee_count=0;
			p_data->buzzer.bee_state=OFF;
			p_data->buzzer.burn_bee_on=OFF;
		}
	}

}