#include "xy_sys_headfile.h"

static datall* s_p_data = &g_datall;

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_irq_tim0_isr(void) interrupt 1
{
	TH0 = 0xF8;
	TL0 = 0x90;
	
	g_timebase.isTrigInterrupt = 1;
	fml_buzzer_timer_ctrl(s_p_data);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_irq_tim2_isr() interrupt 5
{
	static unsigned char s_buff = 0;
	static unsigned char s_count = 0;
	
	TF2 = 0;   //溢出清零
	s_count++;
	if((s_count > 1) && ((s_count % 2) == 1))
	{	
		s_buff >>= 1;
		if(P05 == 1)
		{
			s_buff |= 0x80;
		}
	}
	if(s_count >= 17)
	{	
		s_count = 0;		
		
		INT2F |= 0x02; 
		IE1 |= 0X08; 	  
		TR2 = 0;	  
		RCAP2H=0xfc;     //溢出时间：时钟为Fsys，则12000*（1/Fsys）=0.5ms;
		RCAP2L=0xbe;
		
		hal_serial_uart_rx_touch(s_buff);
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_irq_uart1_isr(void) interrupt 7
{
	if(SSCON0&0x02)    //发送标志位判断
	{
		SSCON0 &= 0xFD;
		s_p_data->uart.send_byte_ok = SET;
//		s_p_data->uart.send_uart1_ok = 1;
		
    }
	if((SSCON0&0x01))  //接收标志位判断
	{
		SSCON0 &= 0xFE;

		hal_serial_uart_rx_display(SSDAT);
    }
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_irq_ex2_isr(void) interrupt 10
{	
	IE1 &= 0XF7;
	INT2F = 0x00;
	TR2 = 1;
	RCAP2H=0xfc;   		 
	RCAP2L=0xbe;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_irq_uart1_send(unsigned char* p_buffer, unsigned char length)
{
	static unsigned char i = 0;
	static unsigned char s_count = 0;
	
	if(s_p_data->uart.send_data)
	{	
		if(s_p_data->uart.send_byte_ok)
		{
			s_p_data->uart.send_byte_ok = RESET;
			SSDAT = p_buffer[i];
			s_count = 0;
			i++;
			if(i >= length)
			{
				i = 0;
				s_p_data->uart.send_data = RESET;
			}
		}
		else
		{
			s_count++;
			if(s_count >= 200)
			{
				s_p_data->uart.send_data = RESET;
			}
		}
	}
	else
	{
		i = 0;
		s_count = 0;
		s_p_data->uart.send_byte_ok = SET;
	}
}

//void hal_irq_uart1_send(unsigned char* p_buffer, unsigned char length)
//{
//	unsigned char i = 0;

//	for(i=0; i<length; i++)
//	{
//		SSDAT = p_buffer[i];
//		while(!s_p_data->uart.send_uart1_ok);
//		s_p_data->uart.send_uart1_ok = 0;
//	}
//}
