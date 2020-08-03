#define GET_STATUS 48
#define GO_FORW    49
#define GO_BACK    50
#define GO_RIGHT   51
#define GO_LEFT    52
#define GO_ON_LINE 53
#define STOP       54

#define SYNC0 0
#define SYNC1 1
#define CMD   2
#define SPD   3
#define END   4
#define ERR   5

void setup() 
{
  Serial.begin(9600);

}

int RS, cmd, spd = 0;
char b  = ' ';

void loop() 
{
  if(Serial.available())
  {
    b = Serial.read();
    switch(RS)
    {
      case SYNC0:
        b == '0' ? RS = SYNC1 : RS = ERR;
        break;
      case SYNC1:
        b == '0' ? RS = CMD : RS = ERR;
        break;
      case CMD:
        cmd = b;
        RS = SPD;
        break;
      case SPD:
        spd = atoi(b);
        RS = END;
      case END:
        b == 0 ? RS = SYNC0 : RS = ERR;
        break;
      case ERR:
        break;
    }
  }
  switch(cmd)
  {
    case GET_STATUS:
      Serial.println('1');
      break;
    case GO_FORW:
      break;
    case GO_BACK:
      break;
    case GO_RIGHT:
      break;
    case GO_LEFT:   
      break;
    case GO_ON_LINE:
      break;
    case STOP:
      break;
  }
}
