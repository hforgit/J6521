#ifndef _XY_FML_TOUCH_H
#define _XY_FML_TOUCH_H

/* DATA0 */
#define TOUCH_SYMBOL_BLOW				(g_datall.touch.current.bits.b0)
#define TOUCH_SYMBOL_ABSORB				(g_datall.touch.current.bits.b1)
#define TOUCH_SYMBOL_WARM				(g_datall.touch.current.bits.b2)

/* DATA1 */
#define TOUCH_RELAY_MOTOR_BLOW			(g_datall.touch.current.bits.b8)
#define TOUCH_RELAY_LIGHT				(g_datall.touch.current.bits.b9)
#define TOUCH_RELAY_PTC					(g_datall.touch.current.bits.b10)
#define TOUCH_RELAY_FAN_BLOW			(g_datall.touch.current.bits.b11)
#define TOUCH_RELAY_FAN_ABSORB			(g_datall.touch.current.bits.b12)
#define TOUCH_RELAY_MOTOR_ABSORB		(g_datall.touch.current.bits.b13)
#define TOUCH_RELAY_SWING_BLOW			(g_datall.touch.current.bits.b14)
#define TOUCH_RELAY_SWING_ABSORB		(g_datall.touch.current.bits.b15)


void fml_touch_init_data(datall* p_data);
void fml_touch_ctrl_logic(datall* p_data);


#endif
