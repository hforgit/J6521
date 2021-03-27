#include "xy_sys_headfile.h"

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_serial_Init(void)
{	
	OTCON |= 0xC0;    //串行接口SSI选择Uart1通信
	SSCON0 = 0x50;   //设置通信方式为模式一，允许接收
	SSCON1 = 0X82;   //波特率低位控制
	SSCON2 = 0X06;   //波特率高位控制
	IE1 |= 0x01;      //开启SSI中断	
	EA = 1;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_serial_uart_rx_touch(unsigned char dat)
{
	uartdata* s_p_uart = &g_datall.uart;							//?
	static switchstate   s_state = STEP1;
	static unsigned char s_uart_numb = 0;
	static unsigned char s_uart_buffer[sizeof(s_p_uart->rec_uart0_dat)] = {0};	
	
	s_uart_buffer[s_uart_numb] = dat;
	switch(s_state)
	{
		case STEP1:
			if(s_uart_buffer[s_uart_numb] == 0x68)
			{
				s_uart_buffer[0] = 0x68;
				s_state = STEP2;				
			}
			s_uart_numb = 0;
			break;
		case STEP2:
			if(s_uart_numb >= (sizeof(s_p_uart->rec_uart0_dat)-1))
			{
				s_state = STEP1;
				s_uart_numb = 0;
				s_p_uart->rec_uart0_ok = 1;
				memcpy(s_p_uart->rec_uart0_dat, s_uart_buffer, sizeof(s_p_uart->rec_uart0_dat));
				memset(s_uart_buffer, 0, sizeof(s_uart_buffer));
			}
			break;
		default:
			break;
	}
	s_uart_numb++;
	if(s_uart_numb >= sizeof(s_p_uart->rec_uart0_dat))
	{
		s_uart_numb = 0;
	}
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hal_serial_uart_rx_display(unsigned char dat)
{
	static unsigned char data_length;
	uartdata* s_p_uart = &g_datall.uart;
	static switchstate 	 s_state = STEP1;
	static unsigned char s_uart_numb = 0;
	static unsigned char s_uart_buffer[sizeof(s_p_uart->rec_uart1_dat)] = {0};

	s_uart_buffer[s_uart_numb] = dat;	
	s_uart_numb++;
	switch(s_state)
	{
		case STEP1:
			/* old version protocol about display */
			if(s_uart_buffer[0] == (DISPLAY_DATA_HEAD_PRI >> 8 & 0xFF))
			{
				if(s_uart_buffer[1] == (DISPLAY_DATA_HEAD_PRI & 0xFF))
				{
					/* finish match about head data */
					s_p_uart->rec_protocol = OLD;
					s_uart_numb = 2;
					s_state = STEP2;
				}
				else if(s_uart_buffer[1] == (DISPLAY_DATA_HEAD_PRI >> 8 & 0xFF))
				{
					/* wait for next legal command */
					s_uart_numb = 1;
				}
				else if(s_uart_numb == 2)
				{
					s_uart_numb = 0;
				}
//				else if(s_uart_buffer[1] == (DISPLAY_DATA_HEAD_XY >> 8 & 0xFF))
//				{
//					s_uart_buffer[0] = (DISPLAY_DATA_HEAD_XY >> 8 & 0xFF)
//					s_uart_numb = 1;
//				}
			}
			/* new version protocol about display */
			else if(s_uart_buffer[0] == (DISPLAY_DATA_HEAD_XY >> 8 & 0xFF))
			{
				if(s_uart_buffer[1] == (DISPLAY_DATA_HEAD_XY & 0xFF))
				{
					/* finish match about head data */
					s_p_uart->rec_protocol = NEW;
					s_uart_numb = 2;
					s_state = STEP2;
				}
				else if(s_uart_buffer[1] == (DISPLAY_DATA_HEAD_XY >> 8 & 0xFF))
				{
					/* wait for next legal command */
					s_uart_numb = 1;
				}
				else
				{
					s_uart_numb = 0;
				}
			}
			else
			{
				s_uart_numb = 0;
			}
			break;
		case STEP2:
			if(OLD == s_p_uart->rec_protocol)
			{
				data_length = 7;
				s_state = STEP3;
			}
			else if(NEW  == s_p_uart->rec_protocol)
			{
				data_length = s_uart_buffer[2] + 3;		///< data + head(2) + len(1)
				s_state = STEP3;
			}
			else
			{
				s_state = STEP1;
				s_uart_numb = 0;
			}
			break;
		case STEP3:
			if(s_uart_numb >= data_length)
			{
				s_p_uart->rec_uart1_ok = 1;
				memcpy(s_p_uart->rec_uart1_dat, s_uart_buffer, data_length);
				memset(s_uart_buffer, 0, data_length);
				s_state = STEP1;
				s_uart_numb = 0;
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
void hal_serial_uart_tx_display(unsigned char* buffer, unsigned int len)
{
	hal_irq_uart2_send(buffer, len);
}
