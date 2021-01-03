#define INIT 0
#define WORK 1
#define STOP 2

#define WAIT     0
#define GET_COOR 1
#define GO       2
#define STOP     3

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
#define BCK 2

#define pm pinMode
#define dr digitalRead
#define dw digitalWrite
#define ar analogRead
#define aw analogWrite

int state, state_work, id_state = 0;
float x_coor, y_coor, x0, y0 =  0;

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
  if(digitalRead(X_ENDSTOP))
  {
    while(digitalRead(X_ENDSTOP))
    {
      x_go(1);
    }
    Serial.println("X in Home!");
  }
  else
  {
    x_go(-1);
    x_home();
  }
}

void y_home()
{
  if(digitalRead(Y_ENDSTOP))
  {
    while(digitalRead(Y_ENDSTOP))
    {
      y_go(1);
    }
    Serial.println("Y in Home!");
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
  dw(xST, 0);dw(xDR, d);
  delayMicroseconds(8000);
  dw(xST, 1);dw(xDR,!d);
  delayMicroseconds(8000);
}

void y_step(int dir)
{
  bool d = dir == FRW ? 1 : 0;
  dw(yST, 0);dw(yDR, d);
  delayMicroseconds(700);
  dw(yST, 1);dw(yDR,!d);
  delayMicroseconds(700);
}

void x_go(float mm)
{
  float steps = mm * 4.375;
  x_enable(1);
  int d = steps > 0? FRW : BCK;
  for(long i = 0; i < abs(steps); i++)
  {
    x_step(d);
  }
  x_enable(0);
}

void y_go(float mm)
{
  float steps = mm * 160;
  y_enable(1);
  int d = steps > 0? FRW : BCK;
  for(long i = 0; i < abs(steps); i++)
  {
    y_step(d);
  }
  y_enable(0);
}

void x_y_go(float x, float y)
{
  float steps_x  = x  * 4.375;
  float steps_y  = y  * 160;
  float steps_x0 = x0 * 4.375;
  float steps_y0 = y0 * 160;
  
  x_enable(1);
  y_enable(1);
  
  int d_x = steps_x > 0? FRW : BCK;
  int d_y = steps_y > 0? FRW : BCK;
  
  for(long i = 0; i < abs(steps_y); i++)
  {
    y_step(d_y);
    for(long j = 0; j < ((steps_y * i) / steps_x);  j++)
    {
      x_step(d_x);
    }
  }
  
  y_enable(0);
  x_enable(0);
}

void loop() 
{
  switch(state)
  {
    case INIT:
      Serial.println("INIT GO");
      x_home();
      y_home();
      Serial.println("INIT END");
      state = WORK;
    break;
    case WORK:
      switch(state_work)
      {
        case WAIT:
          Serial.println("WAIT");
          if(Serial.available())
          {
            id_state = Serial.parseInt();
          }      
          state_work = id_state;
          id_state = 0;
        break;
        case GET_COOR:
          Serial.println("GET COOR");
          if(Serial.available())
          {
            x_coor = Serial.parseInt();
            y_coor = Serial.parseInt();
          }
          state_work = GO;
        break;
        case GO:
          Serial.println("GO");
          x_y_go(x_coor, y_coor);
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
