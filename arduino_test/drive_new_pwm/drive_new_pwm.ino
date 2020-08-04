#include "DueTimer.h"
#include "typedef.h"

void platformStop()
{
  lft_target = 0;
  rgt_target = 0;
}

void platformForward()
{
  lft_target = 100;
  rgt_target = 100;
}

void onCountTimer()
{
  // lft_speed += lft_ticks - lft_avg;
  // rgt_speed += rgt_ticks - rgt_avg;
  // lft_avg = lft_speed / 5;
  // rgt_avg = rgt_speed / 5;
  lft_avg = lft_ticks;
  rgt_avg = rgt_ticks;
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
  lft_ticks++;
}

void onRGTEncoder()
{
  rgt_ticks++;
}

void setupPins()
{
  pm( 2, 1);
  pm( 3, 1);
  pm( 4, 1);
  pm( 5, 1);
  pm(13, 1);
  pm(50, 0);
  pm(52, 0);
}

void setupTimers()
{
  Timer4.attachInterrupt(onCountTimer);
  Timer4.start(1000000);
  Timer2.attachInterrupt(onLogTimer);
  Timer2.start(1000000);
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
  char c = ' ';
  if (Serial.available())
  {
    c = Serial.read();
    lft_pwm = Serial.read();
    rgt_pwm = Serial.read();
    switch (c)
    {
      case 'f':
        platform_state = FORWARD;
        Serial.println("FORWARD");
        break;
      case 's':
        platform_state = STOP;
        Serial.println("STOP");
        break;
      case 'c':
        Serial.println(1);
        break;
      case 't':
        platform_state = FOLLOW_LINE;
        Serial.println("FOLLOW_LINE");
        break;
    }
  } else {
    switch (platform_state)
        break;
 }


}
