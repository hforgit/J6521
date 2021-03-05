#ifndef _XY_SYS_VARIABLE_H
#define _XY_SYS_VARIABLE_H

#define XY_SYS_DEBUG_MODE							///< inside develop debug mode
#define XY_SYS_TEST_MODE							///< inside production test mode

typedef enum SwitchState
{
	RESET     = 0,
	SET,
	ON,
	OFF,
	OPEN,
	CLOSE,
	YES,
	NO,
	DRY,
	WET,
	STEP1,
	STEP2,
	STEP3,
	STEP4,
	POSITIVE,
	NEGATIVE,
	UNSUPPORT,
	REMOTE,
	TOUCH,
	WARM,
	COLD,
	OLD,
	NEW,
}switchstate;
	
typedef enum MotorStep
{
	ANGLE_0_STEP 		= 0,
	ANGLE_1_STEP 		= 12,
	ANGLE_3_STEP 		= 36,
	ANGLE_60_STEP 		= 683,
	ANGLE_90_STEP 		= 1024,
	ANGLE_120_STEP 		= 1365,
	ANGLE_133_STEP		= 1500,

	RESET_STEP_ZERO 	= ANGLE_0_STEP,

	TARGET_STEP_ABSORB 	= ANGLE_90_STEP,

	TARGET_STEP_BLOW	= ANGLE_90_STEP,
	
	TARGET_LOW_SWING 	= ANGLE_60_STEP,
	TARGET_HIGH_SWING	= ANGLE_120_STEP,
	COMPENSATE_STEP 	= ANGLE_3_STEP,
	MAX_RESET_STEP 		= ANGLE_133_STEP,
} motorstep;

typedef	enum TempAlarm
{
	ALARM_LOW_TEMP  = 0,
	ALARM_HIGH_TEMP = 80,
} tempalarm;

typedef enum TimerClock
{
	CLOCK_DELAYTIMER_0S = 0,
	CLOCK_DELAYTIMER_1S,
	CLOCK_DELAYTIMER_2S,
	CLOCK_DELAYTIMER_3S,
	CLOCK_DELAYTIMER_4S,
	CLOCK_DELAYTIMER_5S,
	CLOCK_DELAYTIMER_6S,
	CLOCK_DELAYTIMER_7S,
	CLOCK_DELAYTIMER_8S,
	CLOCK_DELAYTIMER_9S,
	CLOCK_DELAYTIMER_10S,
	CLOCK_DELAYTIMER_CYC = 120,

	#ifdef XY_SYS_DEBUG_MODE
		CLOCK_WORKTIME_120MIN 	  = 7200,	//72,
		CLOCK_WARMDRY_WARM_10MIN  = 600,	//30,
		CLOCK_COLDDRY_COLD_10MIN  = 600,	//30,
		CLOCK_WARMDRY_ABSORB_5MIN = 300,	//15,
	#else
		CLOCK_WORKTIME_120MIN 	  = 7200,		//7200,
		CLOCK_WARMDRY_WARM_10MIN  = 600,		//600,
		CLOCK_COLDDRY_COLD_10MIN  = 600,		//600,
		CLOCK_WARMDRY_ABSORB_5MIN = 300,		//300,	
	#endif	
		
	CLOCK_WORKDRY_ALL_60MIN = (CLOCK_WARMDRY_WARM_10MIN + CLOCK_WARMDRY_ABSORB_5MIN) << 2,
}timerclock;

typedef enum timerSecond_E
{
	MAX_RUNNING_SECOND = 7200,

} timerSecond_e;
typedef enum DisplaySymbol
{
	SYM_ALL_OFF = 0,
	SYM_ALL_ON,	
	SYM_BLOW_ON,
	SYM_BLOW_OFF,
	SYM_ABSORB_ON,
	SYM_ABSORB_OFF,
	SYM_WARM_ON,
	SYM_WARM_OFF,
	SYM_WARM_TWINKLE_DRY_WET,
	SYM_WARM_TWINKLE_STANDBY,
	SYM_WARM_TWINKLE_ABSORB,
} displaysymbol;

typedef enum WorkModule
{
	WORKMODULE_STANDBY = 0,
	WORKMODULE_BLOW,
	WORKMODULE_WARM,
	WORKMODULE_ABSORB,
	WORKMODULE_DEFAULT = WORKMODULE_STANDBY,
} workmodule;

typedef enum KeyState
{
	KEY_RELEASE = 0,
	KEY_LIGHT,
	KEY_BLOW,
	KEY_ABSORB,
	KEY_WARM,
	KEY_SWING,
	KEY_RAV,
	KEY_COLD_DRY,
	KEY_DRY_WET,
	KEY_STANDBY,
	KEY_WARM_DRY,
} keystate;
	
typedef enum DispCommand
{
	DISPLAY_CMD_REMOTE = 0x10,
	DISPLAY_CMD_TEMP   = 0x11,
	DISPLAY_CMD_DIGITAL= 0x20,
} dispcommand;
typedef union DataCommon
{
	unsigned int dat;
	struct{
		unsigned int b0  : 1;
		unsigned int b1  : 1;
		unsigned int b2  : 1;
		unsigned int b3  : 1;
		unsigned int b4  : 1;
		unsigned int b5  : 1;
		unsigned int b6  : 1;
		unsigned int b7  : 1;
		unsigned int b8  : 1;
		unsigned int b9  : 1;
		unsigned int b10 : 1;
		unsigned int b11 : 1;
		unsigned int b12 : 1;
		unsigned int b13 : 1;
		unsigned int b14 : 1;
		unsigned int b15 : 1;
	}bits;
}datacommon;

typedef struct timeBase	
{
    unsigned char    isTrigInterrupt;
	unsigned char    timebase_125us;
    unsigned char    timebase_1ms;
    unsigned char    timebase_10ms;
    unsigned char    timebase_50ms;
    unsigned char    timebase_100ms;
    unsigned char    timebase_250ms;
    unsigned char    timebase_500ms;
    unsigned char    timebase_1s;
} timebase;

typedef struct BuzzerControl
{
	switchstate		 bee_state;
	unsigned char	 bee_count;

	switchstate		 normal_bee_on;
	unsigned char 	 normal_bee_on_flag;
	unsigned int	 normal_bee_on_count;
 
	switchstate 	 long_bee_on;
	unsigned char 	 long_bee_on_flag;
	unsigned int 	 long_bee_on_count;

	switchstate		 burn_bee_on;
	unsigned char 	 burn_bee_count;

} buzzercontrol;

typedef struct MotorControl
{
	unsigned char 	 absorb_motor_step_delay_count;
	unsigned char 	 absorb_motor_step_delay_flag;
	unsigned int 	 absorb_motor_step;
	unsigned int 	 absorb_target_step;
	unsigned int 	 absorb_target_step_real;

	unsigned char 	 blow_motor_step_delay_flag;
	unsigned char 	 blow_motor_step_delay_count;
	unsigned int  	 blow_motor_step;
	unsigned int 	 blow_motor_step_pri;
	unsigned int  	 blow_target_step;
	unsigned int     blow_target_step_pri;
	unsigned int  	 blow_target_step_real;
} motorcontrol;

typedef struct DisplayContent
{
	unsigned char	 disp_icon_data;
	displaysymbol	 disp_icon_blow;
	displaysymbol	 disp_icon_warm;
	displaysymbol	 disp_icon_absorb;	
} displaycontent;

typedef struct UartData
{
	switchstate		 rec_protocol;
	unsigned char 	 rec_uart0_ok;
	unsigned char 	 rec_uart0_dat[5];
	unsigned char 	 rec_uart1_ok;	
	unsigned char 	 rec_uart1_dat[11];	
	unsigned char 	 send_uart1_ok;
} uartdata;

typedef struct workModuleFlag
{
	workmodule		 workmode_current;
	switchstate		 flag_work_colddry;
	switchstate		 flag_work_warmdry;
	switchstate		 flag_workdelay_1s;
	switchstate		 flag_workdelay_10s;
	unsigned char	 workdelay_1s;
	unsigned char	 workdelay_10s;
	unsigned char	 workdelay_cyc;
} workmoduleflag;

typedef struct keyInput
{
	keystate		 keysta;
	keystate		 keysta_pri;
	datacommon		 keyremote;	
} keyinput;

typedef struct normalCtrol
{
	switchstate		 keystate_light;
//	switchstate		 keystate_dry_wet;
//	switchstate		 keystate_dry_wet_pri;
	switchstate		 keystate_ptc;	
	switchstate		 keystate_ptc_pri;
	switchstate		 keystate_ptc_wait;
	switchstate		 keystate_ptc_delay;
	switchstate		 keystate_fan_blow;
	switchstate		 keystate_fan_absorb;

	switchstate		 keystate_swing;
	switchstate		 keystate_swing_pri;
	switchstate		 keystate_enter_swing;
	switchstate		 keystate_open_swing;
	switchstate		 keystate_move_target;	
} normalctrol;

typedef struct tempData
{
	unsigned char	 value;
} tempdata;

typedef struct touchData 
{
	datacommon		 current;
} touchdata;

typedef struct remoteData
{
	keyinput		 key;
	normalctrol		 keyctrl;
	workmoduleflag	 workmode;
}remotedata;

typedef struct displayControl
{
	unsigned char 	 length;
	unsigned char 	 version;
	unsigned char 	 sequence;
	dispcommand 	 command;
	unsigned char* 	 p_buffer;
	unsigned char 	 checkcode;
} displaycontrol;

typedef struct dataAll
{
	motorcontrol	 motor;
	displaycontent	 display;
	uartdata		 uart;
	buzzercontrol	 buzzer;
	tempdata		 temperature;

	switchstate  	 keytype;
	remotedata		 remote;
	touchdata		 touch;

	datacommon		 error;

	switchstate		 testmode;

}datall;

extern timebase g_timebase;
extern datall   g_datall;

#endif


