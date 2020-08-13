#ifndef typedef_h
#define typedef_h

#define pm pinMode
#define dr digitalRead
#define dw digitalWrite
#define ar analogRead
#define aw analogWrite
#define dl_us delayMicroseconds
#define dl_ms delay

#define CMD_TEST 1001
#define CMD_FORWARD 1
#define CMD_BACK 2
#define CMD_LEFT 3
#define CMD_RIGHT 4
#define CMD_ON_LINE 5
#define CMD_STOP 0
#define CMD_NONE -1

#define ON  0
#define OFF 1

#define FRW_TRG 38
#define FRW_ECH 40

#define BCK_TRG 42
#define BCK_ECH 36

enum ENPlatformState
{
    INIT, READY, FORWARD, STOP, ROT_LFT, ROT_RGT, FOLLOW_LINE, BACK
};

enum ENFollowState
{
    START, CALIBRATE, CALIBRATED, FOLLOW, FAIL
};

enum ENLineSensorValue
{
  SV_LL, SV_L, SV_C, SV_R, SV_RR, SV_ALL
};

enum ENDistanceSensorState
{
  ST_FRW_TRG, ST_FRW_ECH_FF, ST_FRW_ECH_BF, ST_BCK_TRG, ST_BCK_ECH_FF, ST_BCK_ECH_BF
};

ENPlatformState platform_state = INIT;
ENFollowState follow_state = START;
ENDistanceSensorState distance_sensor_state = ST_FRW_TRG;
int cmd = CMD_NONE;

volatile uint8_t pwm_timer_cnt = 0;

volatile int lft_pwm = 0;
volatile int rgt_pwm = 0;
volatile int lft_ticks = 0;
volatile int rgt_ticks = 0;
volatile int lft_speed = 0;
volatile int rgt_speed = 0;
volatile int lft_avg = 0;
volatile int rgt_avg = 0;
volatile int I_m = 0;
volatile int V_m = 0;
volatile int I_a = 0;
volatile int V_a = 0;
volatile int I_c = 0;
volatile int V_c = 0;
volatile int S_cnt_m = 0;
volatile int S_rgt_m = 0;
volatile int S_lft_m = 0;

volatile int S_cnt_c = 0;
volatile int S_rgt_c = 0;
volatile int S_lft_c = 0;

volatile int S_cnt = 0;
volatile int S_rgt = 0;
volatile int S_lft = 0;

volatile ENLineSensorValue lsv = SV_ALL;

long S_cnt_wht = 0;
long S_rgt_wht = 0;
long S_lft_wht = 0;

int lft_target = 0;
int rgt_target = 0;
int motors_state = OFF;
int sensor_state = OFF;

double frw_dst_m = 0;
double bck_dst_m = 0;

double frw_dst_c = 0;
double bck_dst_c = 0;

double frw_dst_a = 0;
double bck_dst_a = 0;

#endif
