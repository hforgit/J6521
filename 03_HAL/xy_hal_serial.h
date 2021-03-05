#ifndef _XY_HAL_SERIAL_H
#define _XY_HAL_SERIAL_H

void hal_serial_Init(void);
void hal_serial_uart_rx_touch(unsigned char dat);
void hal_serial_uart_rx_display(unsigned char dat);
void hal_serial_uart_tx_display(unsigned char* buffer, unsigned int len);

#endif
