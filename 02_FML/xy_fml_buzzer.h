#ifndef _XY_FML_BUZZER_H
#define _XY_FML_BUZZER_H

#define  bee_on()						(P15 = 0)
#define  bee_off()						(P15 = 0)

void fml_buzzer_init_data(datall* p_data);
void fml_buzzer_timer_ctrl(datall* p_data);
void fml_buzzer_timer_cnt(datall* p_data);
void fml_buzzer_deal(datall* p_data);

#endif