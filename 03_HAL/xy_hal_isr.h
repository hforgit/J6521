#ifndef _XY_HAL_ISR_H
#define _XY_HAL_ISR_H

void hal_irq_uart1_send(unsigned char* p_buffer, unsigned char length);
void hal_irq_uart2_send(unsigned char* p_buffer, unsigned char length);

#endif
