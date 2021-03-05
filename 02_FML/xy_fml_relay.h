#ifndef _XY_FML_RELAY_H
#define _XY_FML_RELAY_H

#define	 light_on()						(P00 = 1)
#define	 light_off()					(P00 = 0)
#define	 blow_fan_on()					(P26 = 1)
#define	 blow_fan_off()					(P26 = 0)
#define	 rav_on()						(P17 = 1)
#define	 rav_off()						(P17 = 0)
#define	 absorb_fan_on()				(P27 = 1)
#define	 absorb_fan_off()				(P27 = 0)
#define	 PTC_on()						(P16 = 1)
#define	 PTC_off()						(P16 = 0)
#define	 self_test()					(P14)

void fml_relay_ctrl(datall* p_data);

#endif
