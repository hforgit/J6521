#ifndef _XY_FML_DISPLAY_H
#define _XY_FML_DISPLAY_H

#define DISPLAY_DATA_HEAD_PRI		(0x6801)


#define	DISPLAY_GET_ERR_NUMB(_V_)	 (_V_ & ((unsigned int)0x01<<0x0) ? 0x0:\
									  _V_ & ((unsigned int)0x01<<0x1) ? 0x1:\
									  _V_ & ((unsigned int)0x01<<0x2) ? 0x2:\
									  _V_ & ((unsigned int)0x01<<0x3) ? 0x3:\
									  _V_ & ((unsigned int)0x01<<0x4) ? 0x4:\
									  _V_ & ((unsigned int)0x01<<0x5) ? 0x5:\
									  _V_ & ((unsigned int)0x01<<0x6) ? 0x6:\
									  _V_ & ((unsigned int)0x01<<0x7) ? 0x7:\
									  _V_ & ((unsigned int)0x01<<0x8) ? 0x8:\
									  _V_ & ((unsigned int)0x01<<0x9) ? 0x9:\
									  _V_ & ((unsigned int)0x01<<0xA) ? 0xA:\
									  _V_ & ((unsigned int)0x01<<0xB) ? 0xB:\
									  _V_ & ((unsigned int)0x01<<0xC) ? 0xC:\
									  _V_ & ((unsigned int)0x01<<0xD) ? 0xD:\
									  _V_ & ((unsigned int)0x01<<0xE) ? 0xE:\
									 				   				    0xF)
																		
#define DISPLAY_SYMBOL_BLOW_ON		(g_datall.display.disp_icon_data |= 0x01)
#define DISPLAY_SYMBOL_BLOW_OFF		(g_datall.display.disp_icon_data &= ~0x01)
#define DISPLAY_GET_SYMBOL_BLOW		(g_datall.display.disp_icon_data & 0x01)

#define DISPLAY_SYMBOL_ABSORB_ON	(g_datall.display.disp_icon_data |= 0x02)
#define DISPLAY_SYMBOL_ABSORB_OFF	(g_datall.display.disp_icon_data &= ~0x02)
#define DISPLAY_GET_SYMBOL_ABSORB	(g_datall.display.disp_icon_data & 0x02)

#define DISPLAY_SYMBOL_WARM_ON		(g_datall.display.disp_icon_data |= 0x04)
#define DISPLAY_SYMBOL_WARM_OFF		(g_datall.display.disp_icon_data &= ~0x04)
#define DISPLAY_GET_SYMBOL_WARM		(g_datall.display.disp_icon_data & 0x04)

#define DISPLAY_TABLE_SEG(_N_)		(_N_ == 0x00 ? 0x3F :\
									 _N_ == 0x01 ? 0x06 :\
									 _N_ == 0x02 ? 0x5B :\
									 _N_ == 0x03 ? 0x4F :\
									 _N_ == 0x04 ? 0x66 :\
									 _N_ == 0x05 ? 0x6D :\
									 _N_ == 0x06 ? 0x7D :\
									 _N_ == 0x07 ? 0x07 :\
									 _N_ == 0x08 ? 0x7F :\
									 _N_ == 0x09 ? 0x6F :\
									 _N_ == 0x0A ? 0x77 :\
									 _N_ == 0x0B ? 0x7C :\
									 _N_ == 0x0C ? 0x39 :\
									 _N_ == 0x0D ? 0x5E :\
									 _N_ == 0x0E ? 0x79 :\
									 _N_ == 0x0F ? 0x71 :\
									 			   0x00)
									 			   
void fml_display_conf_symbol(datall* p_data);
void fml_display_tx_data(datall* p_data);

#endif
