#ifndef _XY_FML_CTRL_H
#define _XY_FML_CTRL_H

#define CTRL_EXCEED_DELAY_TIMER(_TIM_, _NOW_, _PRI_)	(_NOW_ >= _PRI_ ? ((_NOW_ - _PRI_) < _TIM_ ? NO : YES) :\
													((_NOW_ + CLOCK_DELAYTIMER_CYC - _PRI_) < _TIM_ ? NO : YES))

#define CTRL_CHECK_ERROR_NUMB_1(_A_, _B_, _C_, _D_, _E_)		(\
	_A_ ? (_A_ < TARGET_LOW_SWING ? YES : (_A_ > TARGET_HIGH_SWING ? YES : NO)) : NO)

#define CTRL_CHECK_ERROR_NUMB_2(_A_, _B_, _C_, _D_, _E_)		(\
	_A_ ? NO : (_C_ == ON ? YES : NO))

#define CTRL_CHECK_ERROR_NUMB_3(_A_, _B_, _C_, _D_, _E_)		(\
	_C_ == ON ? (YES == MOTOR_CHK_BLOW_STEP(10) ? YES : NO) : NO)

#define CTRL_CHECK_ERROR_NUMB_4(_A_, _B_, _C_, _D_, _E_)		(\
	_D_ == ON ? (_E_ == ON ? YES : NO) : NO)

#define CTRL_CHECK_ERROR_NUMB_5(_A_, _B_, _C_, _D_, _E_)		(\
	_C_ == ON ? (_D_ == ON ? YES : NO) : NO)

#define CTRL_CHECK_ERROR_NUMB_6(_A_, _B_, _C_, _D_, _E_)		(\
	_C_ == OFF ? (_E_ == ON ? YES : NO) : NO)

void fml_ctrl_init_reg(void);
void fml_ctrl_init_data(datall* p_data);
void fml_ctrl_icon_display(datall* p_data);
void fml_ctrl_key_logic(datall* p_data);
void fml_ctrl_deal_timer(datall* p_data);
void fml_ctrl_work_time(datall* p_data);
void fml_ctrl_deal_alarm(datall* p_data);

#endif
