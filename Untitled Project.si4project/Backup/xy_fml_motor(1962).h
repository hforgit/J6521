#ifndef _XY_FML_MOTOR_H
#define _XY_FML_MOTOR_H

#define  blow_motor_step1_on()			(P25 = 1)		//28
#define  blow_motor_step1_off()			(P25 = 0)
#define  blow_motor_step2_on()			(P24 = 1)
#define  blow_motor_step2_off()			(P24 = 0)
#define  blow_motor_step3_on()			(P23 = 1)
#define  blow_motor_step3_off()			(P23 = 0)
#define  blow_motor_step4_on()			(P22 = 1)
#define  blow_motor_step4_off()			(P22 = 0)

#define  MOTOR_CHK_BLOW_STEP(_X_)		( g_datall.motor.blow_target_step > g_datall.motor.blow_motor_step\
										? (g_datall.motor.blow_target_step - g_datall.motor.blow_motor_step > _X_ ? YES : NO)\
										: (g_datall.motor.blow_motor_step - g_datall.motor.blow_target_step > _X_ ? YES : NO))

#define  MOTOR_CHK_MOVE_TO_TARGET		( g_datall.motor.blow_target_step != g_datall.motor.blow_motor_step 	  ? NO\
										: g_datall.motor.absorb_target_step != g_datall.motor.absorb_motor_step ? NO\
										: YES)

#define  MOTOR_CHK_ABSORB_STEP(_X_)		( g_datall.motor.absorb_target_step > g_datall.motor.absorb_motor_step\
										? (g_datall.motor.absorb_target_step - g_datall.motor.absorb_motor_step > _X_ ? YES : NO)\
										: (g_datall.motor.absorb_motor_step - g_datall.motor.absorb_target_step > _X_ ? YES : NO))

#define  MOTOR_CHK_STEP_IS_LEGAL(_X_)	( _X_ < TARGET_LOW_SWING  ? NO\
										: _X_ > TARGET_HIGH_SWING ? NO\
										: YES)

void fml_motor_init_data(datall* p_data);
void fml_motor_timer_ctrl(datall* p_data);
void fml_motor_ctrl_move(datall* p_data);

#endif
