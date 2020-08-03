#define line1 A0
#define line2 A1
#define line3 A2
void setup() 
{
  pinMode(line1, INPUT);
  pinMode(line2, INPUT);
  pinMode(line3, INPUT);
  
  Serial.begin(9600);
  
  Serial.print("Line 1");
  Serial.print("\t");
  Serial.print("Line 2");
  Serial.print("\t");
  Serial.print("Line 3");
  Serial.print("\t");
  Serial.print("\n");
}

void loop() 
{
  int s_line1, s_line2, s_line3, a_line1, a_line2, a_line3 = 0;
  int n = 1;
  s_line1 = s_line1 + analogRead(line1) - a_line1;
  a_line1 = s_line1 / n;

  s_line2 = s_line2 + analogRead(line2) - a_line2;
  a_line2 = s_line2 / n;

  s_line3 = s_line3 + analogRead(line3) - a_line3;
  a_line3 = s_line3 / n;
  
  Serial.print(a_line1);
  Serial.print("\t");
  Serial.print(a_line2);
  Serial.print("\t");
  Serial.print(a_line3);
  Serial.print("\n");
  
}
