//pins motor
#define motor1     5
#define motor1_pwm 4

#define motor2     3
#define motor2_pwm 2

void setup()
{
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);

}

int Forward_left(int in_pwm)
{
  digitalWrite(motor2, 1);
  analogWrite(motor2_pwm, in_pwm);
}

int Forward_right(int in_pwm)
{
  digitalWrite(motor1, 1);
  analogWrite(motor1_pwm, in_pwm);
}

int Back_left(int in_pwm)
{
  digitalWrite(motor2, 0);
  analogWrite(motor2_pwm, in_pwm);
}

int Back_right(int in_pwm)
{
  digitalWrite(motor1, 0);
  analogWrite(motor1_pwm, in_pwm);
}

void Left()
{
  for(int i = 0; i < 255; i++)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
  }
  digitalWrite(motor1, 1);
  digitalWrite(motor2, 0);
  delay(100);
  for(int i = 255; i > 0; i--)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
  }
}

void Right()
{
  for(int i = 0; i < 255; i++)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
  }
  digitalWrite(motor1, 0);
  digitalWrite(motor2, 1);
  delay(100);
  for(int i = 255; i > 0; i--)
  {
    analogWrite(motor1_pwm, i);
    analogWrite(motor2_pwm, i);
  }
}

void Stop_motor()
{
  digitalWrite(motor1, 0);
  digitalWrite(motor2, 0);
  while(1);  
}

void loop()
{
  for(int i = 0; i < 255; i++)
  {
    Forward_left(i);
    delay(25);
  }
  
  delay(5000);
  
  for(int i = 255; i > 0; i--)
  {
    Forward_left(i);
    delay(25);
  }
  
  for(int i = 0; i < 255; i++)
  {
    Forward_right(i);
    delay(25);
  }
  
  delay(5000);
  
  for(int i = 255; i > 0; i--)
  {
    Forward_right(i);
    delay(25);
  }
  
  for(int i = 0; i < 255; i++)
  {
    Back_left(i);
    delay(25);
  }
  
  delay(5000);
  
  for(int i = 255; i > 0; i--)
  {
    Back_left(i);
    delay(25);
  }
  
  for(int i = 0; i < 255; i++)
  {
    Back_right(i);
    delay(25);
  }
  
  delay(5000);
  
  for(int i = 255; i > 0; i--)
  {
    Back_right(i);
    delay(25);
  }
  while(1);
}
