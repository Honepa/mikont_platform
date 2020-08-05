#include "DueTimer.h"
#include "typedef.h"

void motors(int on_off)
{
  motors_state = on_off;
  dw( 9, motors_state);
}

void sensor(int on_off)
{
  sensor_state = on_off;
  dw(10, sensor_state); 
}

float get_voltage()
{
  return ar(A3) / 70.9;
}

float get_current()
{
  //return ar(A4);
  return ((float) ar(A4) - 503.0) / -25.0;
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
}

void onPWMTimer()
{
  dw(13, ! dr(13));
  pwm_timer_cnt++;

  dw(4, pwm_timer_cnt < lft_pwm);
  dw(2, pwm_timer_cnt < rgt_pwm);
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
  pm( 3, 1);
  pm( 4, 1);
  pm( 5, 1);
  pm( 8, 1);
  pm( 9, 1);
  pm(10, 1);
  pm(13, 1);
  pm(50, 0);
  pm(52, 0);
  dw( 8,1); dw( 9,1); dw(10,1);
}

void setupTimers()
{
  Timer4.attachInterrupt(onCountTimer);
  Timer4.start(1000000);
//  Timer2.attachInterrupt(onLogTimer);
//  Timer2.start(1000000);
  Timer3.attachInterrupt(onPWMTimer);
  Timer3.start(50); 
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
      lft_pwm = Serial.parseInt();
      rgt_pwm = Serial.parseInt();
      motors(ON);
    }
    else if (cmd == CMD_BACK) 
    {
      lft_pwm = 0; Serial.parseInt();
      rgt_pwm = 0; Serial.parseInt();
    }
    else if (cmd == CMD_LEFT) 
    {
      lft_pwm = Serial.parseInt();
      rgt_pwm = Serial.parseInt();
    }
    else if (cmd == CMD_RIGHT) 
    {
      lft_pwm = Serial.parseInt();
      rgt_pwm = Serial.parseInt();
    }
    else if (cmd == CMD_ON_LINE) 
    {
      lft_pwm = Serial.parseInt();
      rgt_pwm = Serial.parseInt();
      sensor(ON);
    }
    else if (cmd == CMD_STOP) 
    {
      lft_pwm = 0; Serial.parseInt();
      rgt_pwm = 0; Serial.parseInt();
      motors(OFF);
      sensor(OFF);
    }
    Serial.println(cmd);
    Serial.println(lft_pwm);
    Serial.println(rgt_pwm);
    Serial.println(lft_avg);
    Serial.println(rgt_avg);
    Serial.println(ar(A0));
    Serial.println(ar(A1));
    Serial.println(ar(A2));  
    Serial.println(get_voltage());  
    Serial.println(get_current());  
    Serial.println(motors_state);  
    Serial.println(sensor_state);  
  } else {
  }
}
