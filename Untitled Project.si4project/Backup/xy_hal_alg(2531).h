#ifndef _XY_HAL_ALG_H
#define _XY_HAL_ALG_H

#define ALG_CHK_BACK_DIFF(_V1_, _V2_, _D_)		(_V1_ < _V2_ ? (_V2_ - _V1_ > _D_ ? YES : NO) :\
															   	(_V1_ - _V2_ > _D_ ? YES : NO))

#define ALG_CHK_ROUNDING(_V_)					(_V_ % 10 > 4 ? YES : NO)

unsigned char hal_alg_chk_crc8(unsigned char* p_buffer, unsigned char buf_size);

#endif