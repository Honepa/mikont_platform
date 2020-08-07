#include "DueTimer.h"
#include "typedef.h"

void motors(int on_off)
{
  motors_state = on_off;
  dw(10, motors_state);
}

void sensor(int on_off)
{
  sensor_state = on_off;
  dw( 9, sensor_state); 
}

void platformStop()
{
  motors(OFF);
  //sensor(OFF);
  dw(46, 0);
  dw(44, 0);
  dw(47, 0);
  dw(45, 0);
  aw(2, 0);
  aw(4, 0);
}

void platformForward(int lft, int rgt)
{
  motors(ON);
  dw(46, 0);
  dw(44, 0);
  dw(47, 1);
  dw(45, 1);
  aw(2, rgt);
  aw(4, lft);
}

ENLineSensorValue find_line()
{
  int l = S_lft <= S_lft_wht; dw(22, l);
  int c = S_cnt <= S_cnt_wht; dw(24, c);
  int r = S_rgt <= S_rgt_wht; dw(26, r);
  if ( l & !c & !r) return SV_LL;
  if ( l &  c & !r) return SV_L;
  if (!l &  c & !r) return SV_C;
  if (!l &  c &  r) return SV_R;
  if (!l & !c &  r) return SV_RR;

  if ( l &  c &  r) return SV_ALL;
}

void platformFollow(int lft, int rgt)
{
  static long t_0 = millis();
  static int i_calibrate = 0;

  ENLineSensorValue lsv = find_line();

  switch (follow_state)
  {
    case START:
      motors(OFF);
      sensor(ON);
      if (millis() - t_0 > 100) 
      {
        follow_state = CALIBRATE;
        t_0 = millis();
      }
      break;
    case CALIBRATE:
      if (millis() - t_0 > 1000) 
      {
        follow_state = CALIBRATED;
        t_0 = millis();
        S_cnt_wht /= i_calibrate;
        S_cnt_wht += 20;
        S_rgt_wht /= i_calibrate;
        S_rgt_wht += 20;
        S_lft_wht /= i_calibrate;
        S_lft_wht += 20;
      } else {
        i_calibrate++;
        S_cnt_wht += S_cnt;
        S_rgt_wht += S_rgt;
        S_lft_wht += S_lft;
      }
      break;
    case CALIBRATED:
      if (millis() - t_0 > 2000) 
      {
        follow_state = FOLLOW;
        t_0 = millis();
      } else {
        platformForward(100,100);
      }
      break;
    case FOLLOW:
      platformForward(lft,rgt);
      switch (lsv)
      {
        case SV_RR:
          platformForward(      0,     rgt);
          break;
        case SV_R:
          platformForward(lft / 2,     rgt);
          break;
        case SV_C:
          platformForward(    lft,     rgt);
          break;
        case SV_L:
          platformForward(    lft, rgt / 2);
          break;
        case SV_LL:
          platformForward(    lft,       0);
          break;
        case SV_ALL:
          //follow_state = FAIL;
          break;
      }
      break;
    case FAIL:
      platformStop();
      break;
  }
}

float get_voltage()
{
  return V_a / 70.9;
}

float get_current()
{
  return -I_a / 25.0 + 20.0;
}

void onCountTimer()
{
  lft_speed += lft_ticks - lft_avg;
  rgt_speed += rgt_ticks - rgt_avg;
  lft_avg = lft_speed / 5;
  rgt_avg = rgt_speed / 5;
  // lft_avg = lft_ticks;
  // rgt_avg = rgt_ticks;
  lft_ticks = 0;
  rgt_ticks = 0;

  // lft_pwm += round(0.3 * (lft_target - lft_avg));
  // rgt_pwm += round(0.3 * (rgt_target - rgt_avg));
  lft_pwm = lft_target;
  rgt_pwm = rgt_target;
  lft_pwm = min(255, max(0, lft_pwm));
  rgt_pwm = min(255, max(0, rgt_pwm));
}

void onPWMTimer()
{
  dw(13, ! dr(13));
  pwm_timer_cnt++;

  dw(4, pwm_timer_cnt < lft_pwm);
  dw(2, pwm_timer_cnt < rgt_pwm);
}

void onMeasureTimer()
{
  S_cnt_m = ar(A0);
  S_rgt_m = ar(A1);
  S_lft_m = ar(A2);
  S_cnt_c += S_cnt_m - S_cnt;
  S_rgt_c += S_rgt_m - S_rgt;
  S_lft_c += S_lft_m - S_lft;
  S_cnt = S_cnt_c / 10;
  S_rgt = S_rgt_c / 10;
  S_lft = S_lft_c / 10;

  I_m = ar(A4);
  V_m = ar(A3);
  I_c += I_m - I_a;
  V_c += V_m - V_a;
  I_a = I_c / 10;
  V_a = V_c / 10;
}

void onLogTimer()
{
  Serial.print(platform_state);
  Serial.print("\t");
  Serial.print(lft_avg);
  Serial.print("\t");
  Serial.print(rgt_avg);
  Serial.print("\t");
  Serial.print(lft_pwm);
  Serial.print("\t");
  Serial.print(rgt_pwm);
  Serial.print("\t");
  Serial.print(ar(A0));
  Serial.print("\t");
  Serial.print(ar(A1));
  Serial.print("\t");
  Serial.print(ar(A2));
  Serial.print("\n");
}

void onLFTEncoder()
{
  static int s, s0;
  s = dr(50);
  if (s != s0)
  {
    lft_ticks++;
    s0 = s;
  }
}

void onRGTEncoder()
{
  static int s, s0;
  s = dr(52);
  if (s != s0)
  {
    rgt_ticks++;
    s0 = s;
  }
}

void setupPins()
{
  pm( 2, 1);
  pm( 4, 1);
//  pm( 4, 1);
//  pm( 5, 1);
  pm( 8, 1);
  pm( 9, 1);
  pm(10, 1);
  pm(13, 1);

  pm(46, 1);
  pm(47, 1);
  pm(44, 1);
  pm(45, 1);

  pm(50, 0);
  pm(52, 0);

  pm(22, 1);
  pm(23, 1); dw(22, 1); dw(23, 0);
  pm(24, 1);
  pm(25, 1); dw(24, 1); dw(25, 0);
  pm(26, 1);
  pm(27, 1); dw(26, 1); dw(27, 0);

  dw( 8,1); dw( 9,1); dw(10,1);
}

void setupTimers()
{
  Timer4.attachInterrupt(onCountTimer);
  Timer4.start(1000000);
//  Timer2.attachInterrupt(onLogTimer);
//  Timer2.start(1000000);
  Timer2.attachInterrupt(onMeasureTimer);
  Timer2.start(100);
  // Timer3.attachInterrupt(onPWMTimer);
  // Timer3.start(50); 
}

void setupInterrupts()
{
  attachInterrupt(
    digitalPinToInterrupt(50), onLFTEncoder, CHANGE
  );
  attachInterrupt(
    digitalPinToInterrupt(52), onRGTEncoder, CHANGE
  );
}

void setupSerial()
{
  Serial.begin(9600);
  Serial.setTimeout(100);
}

void setup()
{
  setupPins();
  setupTimers();
  setupSerial();
  setupInterrupts();
}

void loop()
{
  if (Serial.available())
  {
    cmd = Serial.parseInt();
    if (cmd == CMD_TEST) 
    {
      Serial.parseInt();
      Serial.parseInt();
    }
    else if (cmd == CMD_FORWARD) 
    {
      lft_target = Serial.parseInt();
      rgt_target = Serial.parseInt();
      platform_state = FORWARD;
    }
    else if (cmd == CMD_BACK) 
    {
      lft_target = 0; Serial.parseInt();
      rgt_target = 0; Serial.parseInt();
      platform_state = BACK;
    }
    else if (cmd == CMD_LEFT) 
    {
      lft_target = Serial.parseInt();
      rgt_target = Serial.parseInt();
      platform_state = ROT_LFT;
    }
    else if (cmd == CMD_RIGHT) 
    {
      lft_target = Serial.parseInt();
      rgt_target = Serial.parseInt();
      platform_state = ROT_RGT;
    }
    else if (cmd == CMD_ON_LINE) 
    {
      lft_target = Serial.parseInt();
      rgt_target = Serial.parseInt();
      platform_state = FOLLOW_LINE;
    }
    else if (cmd == CMD_STOP) 
    {
      lft_target = 0; Serial.parseInt();
      rgt_target = 0; Serial.parseInt();
      platform_state = STOP;
    }
    Serial.println(cmd);
    Serial.println(platform_state);
    Serial.println(lft_target);
    Serial.println(rgt_target);
    Serial.println(lft_avg);
    Serial.println(rgt_avg);
    Serial.println(lft_pwm);
    Serial.println(rgt_pwm);
    Serial.println(S_cnt);
    Serial.println(S_rgt);
    Serial.println(S_lft);
    Serial.println(S_cnt_wht);
    Serial.println(S_rgt_wht);
    Serial.println(S_lft_wht);
    Serial.println(follow_state);
    Serial.println(get_voltage());  
    Serial.println(get_current());  
    Serial.println(motors_state);  
    Serial.println(sensor_state);  
  } else {
    switch (platform_state)
    {
      case INIT:
        platform_state = READY;
        motors(OFF);
        sensor(OFF);
        break;
      case READY:

        break;
      case FORWARD:
        platformForward(lft_target, rgt_target);
        break;
      case STOP:
        platformStop();
        platform_state = READY;
        break;
      case ROT_LFT:
        motors(ON);
        sensor(OFF);
        break;
      case ROT_RGT:
        motors(ON);
        sensor(OFF);
        break;
      case FOLLOW_LINE:
        platformFollow(lft_target, rgt_target);

        break;
      case BACK:
        motors(OFF);
        sensor(OFF);
        break;
    }
  }
}