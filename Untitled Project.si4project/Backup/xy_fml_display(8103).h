#ifndef _XY_FML_DISPLAY_H
#define _XY_FML_DISPLAY_H

#define DISPLAY_DATA_HEAD_PRI		(0x6801)

#define DISPLAY_DATA_HEAD_XY		(0x5859)
#define DISPLAY_DATA_VERSION		(10)
#define DISPLAY_DATA_COMMAND		(0x20)			///< display cmd

/* 0x10 - remote key command */
#define DISPLAY_REMOTE_KEY_HEAD		(0x5AA5)

#define DISPLAY_REMOTE_KEY_STANDBY	(0x01)
#define DISPLAY_REMOTE_KEY_RAV		(0x02)
#define DISPLAY_REMOTE_KEY_LIGHT	(0x03)
#define DISPLAY_REMOTE_KEY_WARM		(0x04)
#define DISPLAY_REMOTE_KEY_SWING	(0x05)
#define DISPLAY_REMOTE_KEY_ABSORB	(0x06)
#define DISPLAY_REMOTE_KEY_BLOW		(0x07)
#define DISPLAY_REMOTE_KEY_WARMDRY	(0x08)
#define DISPLAY_REMOTE_KEY_COLDDRY	(0x09)

/* 0x20 - display command */
#define DISPLAY_DATA_LENGTH			(7)

#define DISPLAY_GET_KEY_VALUE(_V_)	(_V_ == DISPLAY_REMOTE_KEY_STANDBY 	? KEY_STANDBY :\
									 _V_ == DISPLAY_REMOTE_KEY_RAV 		? KEY_RAV :\
									 _V_ == DISPLAY_REMOTE_KEY_LIGHT 	? KEY_LIGHT :\
									 _V_ == DISPLAY_REMOTE_KEY_WARM 	? KEY_WARM :\
									 _V_ == DISPLAY_REMOTE_KEY_SWING 	? KEY_SWING :\
									 _V_ == DISPLAY_REMOTE_KEY_ABSORB 	? KEY_ABSORB :\
									 _V_ == DISPLAY_REMOTE_KEY_BLOW 	? KEY_BLOW :\
									 _V_ == DISPLAY_REMOTE_KEY_WARMDRY 	? KEY_WARM_DRY :\
									 _V_ == DISPLAY_REMOTE_KEY_COLDDRY 	? KEY_COLD_DRY :\
									 									  KEY_RELEASE)


#define DISPLAY_DATA_CHK_LEN(_L_)	(_L_ < 4 ? NO : (_L_ > 120 ? NO : YES))
#define DISPLAY_DATA_CHK_VER(_V_)	(_V_ < 10 ? NO : (_V_ > 99 ? NO : YES))

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
void fml_display_ack_remote(displaycontrol* p_data);
keystate fml_display_deal_remote(unsigned char* p_buffer);

#endif
