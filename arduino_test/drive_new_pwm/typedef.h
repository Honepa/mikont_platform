#ifndef typedef_h
#define typedef_h

#define pm pinMode
#define dr digitalRead
#define dw digitalWrite
#define ar analogRead
#define aw analogWrite

enum ENPlatformState
{
    INIT, READY, FORWARD, STOP, ROT_LFT, ROT_RGT, FOLLOW_LINE
};

enum ENCommand
{
    CMD_NONE, CMD_FORWARD, CMD_STOP, CMD_ROT_LFT, CMD_ROT_RGT, CMD_FOLLOW_LINE
};


ENPlatformState platform_state = INIT;
ENCommand cmd = CMD_NONE;

volatile uint8_t pwm_timer_cnt = 0;

volatile int lft_pwm = 0;
volatile int rgt_pwm = 0;
volatile int lft_ticks = 0;
volatile int rgt_ticks = 0;
volatile int lft_speed = 0;
volatile int rgt_speed = 0;
volatile int lft_avg = 0;
volatile int rgt_avg = 0;
int lft_target = 0;
int rgt_target = 0;

#endif
