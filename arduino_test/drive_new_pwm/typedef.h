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

enum ENPlatformState
{
    INIT, READY, FORWARD, STOP, ROT_LFT, ROT_RGT, FOLLOW_LINE, BACK
};

ENPlatformState platform_state = INIT;
int cmd = CMD_NONE;

volatile uint8_t pwm_timer_cnt = 0;

volatile uint8_t lft_pwm = 0;
volatile uint8_t rgt_pwm = 0;
volatile int lft_ticks = 0;
volatile int rgt_ticks = 0;
volatile int lft_speed = 0;
volatile int rgt_speed = 0;
volatile int lft_avg = 0;
volatile int rgt_avg = 0;
int lft_target = 0;
int rgt_target = 0;

#endif
