#ifndef _XY_HAL_TIMER_H
#define _XY_HAL_TIMER_H

#define watchdog()    (WDTCON |= 0X10)

void hal_timer_Init(void);
void hal_timer_buzzer(buzzercontrol* p_bee);
void hal_timer_motor_blow(motorcontrol* p_motor);
void hal_timer_mode_deal(workmoduleflag* p_mode);

#endif
