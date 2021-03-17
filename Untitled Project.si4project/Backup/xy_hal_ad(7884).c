#include "xy_sys_headfile.h"

const unsigned int AdcTable[100] = 
{ 
	1022,	1056,	1092,	1128,	1164,	1200,	1237,	1275,	1312,	1350,		// 0 	 9
	1389,	1428,	1467,	1506,	1546,	1585,	1625,	1665,	1705,	1745,		// 10 	 19
	1785,	1825,	1865,	1905,	1945,	1984,	2024,	2063,	2102,	2140,		// 20 	 29
	2179,	2217,	2255,	2292,	2329,	2366,	2402,	2438,	2473,	2508,		// 30 	 39
	2543,	2577,	2610,	2643,	2675,	2707,	2739,	2770,	2800,	2830,		// 40 	 49
	2859,	2888,	2916,	2944,	2971,	2997,	3023,	3049,	3074,	3098,		// 50 	 59
	3122,	3145,	3169,	3191,	3213,	3234,	3255,	3275,	3295,	3314,		// 60 	 69
	3334,	3352,	3370,	3388,	3405,	3422,	3438,	3454,	3470,	3485,		// 70 	 79
	3500,	3515,	3529,	3542,	3556,	3569,	3582,	3594,	3606,	3618,		// 80 	 89
	3630,	3641,	3652,	3662,	3673,	3683,	3693,	3702,	3712,	3721,		// 90 	 99 
};

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char hal_ad_cal_temperature(unsigned char* pbuffer)
{
	unsigned char i = 0;
	unsigned int temp;
	static unsigned int s_pri_tmp;

	temp = pbuffer[0];
	temp <<= 8;
	temp += pbuffer[1];

	if(temp < AdcTable[i])
	{
		return (0);
	}
	else
	{
		while(++i)
		{
			if(temp < AdcTable[i])
				break;
		}
		
		if(i < (sizeof(AdcTable) / sizeof(unsigned int)))
		{
			temp -= AdcTable[i-1];
			temp *= 10;
			temp /= AdcTable[i] - AdcTable[i-1];
			temp += (i-1)*10;
			
			if(YES == ALG_CHK_BACK_DIFF(s_pri_tmp, temp, 5))		///< 0.5
			{
				s_pri_tmp = temp;
			}
			
			temp = s_pri_tmp;
					
			if(YES == ALG_CHK_ROUNDING(temp))
			{
				temp += 10;
			}
					
			if(temp > 999)
				temp = 999;

			return (temp/10);
		}
		else
		{
			return (99);
		}
	}
}
