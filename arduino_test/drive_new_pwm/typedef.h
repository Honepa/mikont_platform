#ifndef typedef_h
#define typedef_h

#define pm pinMode
#define dr digitalRead
#define dw digitalWrite
#define ar analogRead
#define aw analogWrite

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

enum ENPlatformState
{
    INIT, READY, FORWARD, STOP, ROT_LFT, ROT_RGT, FOLLOW_LINE, BACK
};

enum ENFollowState
{
    CALIBRATE, CENTER, LEFT, RIGHT, FAIL
};

ENPlatformState platform_state = INIT;
ENFollowState follow_state = CALIBRATE;
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
volatile int S_cnt = 0;
volatile int S_rgt = 0;
volatile int S_lft = 0;
int lft_target = 0;
int rgt_target = 0;
int motors_state = OFF;
int sensor_state = OFF;

#endif
