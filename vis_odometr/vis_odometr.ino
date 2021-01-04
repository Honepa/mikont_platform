#define INIT 0
#define WORK 1
#define STOP 2

#define WAIT     0
#define GET_COOR 1
#define GO       2
#define GO_INIT  3
#define STOP     4

#define X_STEPS 4.375
#define Y_STEPS 160

#define X_ENDSTOP 14
#define Y_ENDSTOP 3

#define CAM_PWR 10
#define FAN_PWR  9

#define xEN 38
#define xDR A1
#define xST A0

#define yEN A2
#define yDR A7
#define yST A6

#define zEN A8
#define zDR 48
#define zST 46

#define FRW 1
#define BCK -1

#define pm pinMode
#define dr digitalRead
#define dw digitalWrite
#define ar analogRead
#define aw analogWrite

int state, state_work, id_state = 0;
float x, y, target_x, target_y;
float k, b;

void setup_endstop()
{
  pm(X_ENDSTOP,  INPUT_PULLUP);
  pm(Y_ENDSTOP,  INPUT_PULLUP);
}

void setup_camera()
{
  pm(CAM_PWR, 1); dw(CAM_PWR, 1);
  pm(13, 1);      dw(13, 1);
}

void setup_fans()
{
  pm(FAN_PWR, 1); dw(FAN_PWR, 1);
}

void setup_x()
{
  pm(xEN, 1); dw(xEN, 1);
  pm(xDR, 1); dw(xDR, 0);
  pm(xST, 1); dw(xST, 0);
}

void setup_y()
{
  pm(yEN, 1); dw(xEN, 1);
  pm(yDR, 1); dw(xDR, 0);
  pm(yST, 1); dw(xST, 0);
}

void setup_z()
{

}

void setup_steppers()
{
  setup_x();
  setup_y();
  setup_z();
}

void setup()
{
  setup_endstop();
  setup_camera();
  setup_fans();
  setup_steppers();

  Serial.begin(9600);
}

void x_home()
{
  if (digitalRead(X_ENDSTOP))
  {
    while (digitalRead(X_ENDSTOP))
    {
      x_go(1);
    }
    Serial.println("X in Home!");
    x = 0;
  }
  else
  {
    x_go(-1);
    x_home();
  }
}

void y_home()
{
  if (digitalRead(Y_ENDSTOP))
  {
    while (digitalRead(Y_ENDSTOP))
    {
      y_go(1);
    }
    Serial.println("Y in Home!");
    y = 0;
  }
  else
  {
    y_go(-1);
    y_home();
  }
}

void x_enable(int a)
{
  dw(xEN, !a);
}

void y_enable(int a)
{
  dw(yEN, !a);
}

void z_enable(int a)
{
  dw(zEN, !a);
}

void x_step(int dir)
{
  bool d = dir == FRW ? 1 : 0;
  dw(xST, 0); dw(xDR, d);
  delayMicroseconds(8000);
  dw(xST, 1); dw(xDR, !d);
  delayMicroseconds(8000);
  x += dir * (1.0 / X_STEPS);
}

void y_step(int dir)
{
  bool d = dir == FRW ? 1 : 0;
  dw(yST, 0); dw(yDR, d);
  delayMicroseconds(700);
  dw(yST, 1); dw(yDR, !d);
  delayMicroseconds(700);
  y += dir * (1.0 / Y_STEPS);
}

void x_go(float mm)
{
  float steps = mm * X_STEPS;
  x_enable(1);
  int d = steps > 0 ? FRW : BCK;
  for (long i = 0; i < abs(steps); i++)
  {
    x_step(d);
  }
  x_enable(0);
}

void y_go(float mm)
{
  float steps = mm * Y_STEPS;
  y_enable(1);
  int d = steps > 0 ? FRW : BCK;
  for (long i = 0; i < abs(steps); i++)
  {
    y_step(d);
  }
  y_enable(0);
}

void line(float x1, float y1, float x2, float y2)
{
  k = (y2 - y1) / (x2 - x1);
  b = (-(x1 * y2 - x2 * y1)) / (x2 - x1);
}

float line_x(float y_now, float target_x, float target_y)
{
  float x_end = 0;

  x_end = (-(x * target_y - target_x * y) - (target_x - x) * y_now) / (y - target_y);

  return x_end;
}

float line_y(float x_now, float target_x, float target_y)
{
  float y_end = 0;

  y_end = (-(x * target_y - target_x * y) - (y - target_y) * x_now) / (target_x - x);

  return y_end;
}

void x_y_go(float target_x, float target_y)
{
  x_enable(1);
  y_enable(1);
  
  float steps_x = (target_x - x) * X_STEPS;
  float steps_y = (target_y - y) * Y_STEPS;
  
  int d_x = steps_x > 0 ? FRW : BCK;
  int d_y = steps_y > 0 ? FRW : BCK;

  

  if(abs(steps_x) < 1)
  {
    y_go(target_y - y);
  }
  else if(abs(steps_y) < 1)
  {
    x_go(target_x - x);
  }
  else 
  {
    line(x * X_STEPS, y * Y_STEPS, target_x * X_STEPS, target_y * Y_STEPS);
    //honepa
    if((k <= 1) and (k > 0))
    {
      int dy = steps_y > 0 ? FRW : BCK;
      int dx = steps_x > 0 ? FRW : BCK;
      for(int i = 0; i < steps_y; i++)
      {        
        y_step(dy);
        while(y > k * x + b)
        {
          x_step(dx);  
          Serial.println(x);
          Serial.println(y);
          Serial.println(k * x + b);      
        }
      }
    }
  }
  y_enable(0);
  x_enable(0);
}

void loop()
{
  switch (state)
  {
    case INIT:
      Serial.println("INIT GO");
      x_home();
      y_home();
      Serial.println("INIT END");
      state = WORK;
      break;
    case WORK:
      switch (state_work)
      {
        case WAIT:
          if (Serial.available())
          {
            id_state = Serial.parseInt();
          }
          state_work = id_state;
          id_state = 0;
          break;
        case GET_COOR:
          Serial.println("GET COOR");
          if (Serial.available())
          {
            target_x = Serial.parseInt();
            target_y = Serial.parseInt();
          }
          Serial.println(target_x);
          Serial.println(target_y);     
          state_work = GO;
          break;
        case GO:
          Serial.println("GO");
          x_y_go(target_x, target_y);
          state_work = WAIT;
          break;
        case GO_INIT:
          state = INIT;
          state_work = WAIT;
          break;
        case STOP:
          state = STOP;
          break;
      }
      break;
    case STOP:
      Serial.println("STOP");
      break;
  }
}
