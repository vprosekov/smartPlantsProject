
#include <Stepper.h>

#define dw digitalWrite

const int xCrds[20], yCrds[20], stepsPerRevolution = 200, magnetPin = 15,yEnc = 30, zEnc = 32, xSensor = 10, ySensor = 40, zSensor = 44, zSensor2 = 42;

int curX(0), curY(0), curZ(0);

Stepper stepperX = Stepper(stepsPerRevolution, 2, 3, 4, 5);
//изменить конструкцию к магниту прямую пластину 
void go(char num, int pos)
{
  switch(num)
  {
    case 'x':
      stepperX.step(pos - curX);
      curX = pos;
      break;

    case 'y':
      //stepperY.step(pos - curY);
      if(pos == 0)
      {
        dw(6,LOW);
        dw(7,HIGH);
        while(digitalRead(ySensor)!=0){Serial.println(digitalRead(ySensor));}
        dw(6,LOW);
        dw(7,LOW);
      }
      else if(pos<curY)
      {
        dw(6, LOW);
        dw(7, HIGH);
        int counter(0), last_step(digitalRead(yEnc)), now_step;
        while(counter <= (curY-pos))
        {
          now_step=digitalRead(yEnc);
          if(now_step != last_step)
          {
            last_step=now_step;
            counter++;
          }
        }
      }
      else if(pos>curY)
      {
        dw(6, HIGH);
        dw(7, LOW);
        int counter(0), last_step(digitalRead(yEnc)), now_step;
        while(counter <= (pos - curY))
        {
          now_step=digitalRead(yEnc);
          if(now_step != last_step)
          {
            last_step=now_step;
            counter++;
          }
        }
      }
      dw(6, LOW);
      dw(7, LOW);
      
           
      curY = pos;
      break;
      
    case 'z':

      if(pos == 0)
      {
        dw(9,LOW);
        dw(8,HIGH);
        while(digitalRead(zSensor)!=0){Serial.println(digitalRead(zSensor));}
        dw(9,LOW);
        dw(8,LOW);
      }
      else if(pos == 1)
      {
        dw(9,HIGH);
        dw(8,LOW);
        while(digitalRead(zSensor2)!=0){Serial.println(digitalRead(zSensor2));}
        dw(9,LOW);
        dw(8,LOW);
      }
      else if(pos<curZ)
      {
        dw(9, LOW);
        dw(8, HIGH);
        int counter(0), last_step(digitalRead(zEnc)), now_step;
        while(counter <= (curZ-pos))
        {
          now_step=digitalRead(zEnc);
          if(now_step != last_step)
          {
            last_step=now_step;
            counter++;
          }
        }
      }
      else if(pos>curZ)
      {
        dw(9, HIGH);
        dw(8, LOW);
        int counter(0), last_step(digitalRead(zEnc)), now_step;
        while(counter <= (pos - curZ))
        {
          now_step=digitalRead(zEnc);
          if(now_step != last_step)
          {
            last_step=now_step;
            counter++;
          }
        }
      }
      dw(9, LOW);
      dw(8, LOW);
      
           
      curZ = pos;
      Serial.println("ZALUPA: " + String(curZ));
      break;
  }
}

void goStartPoint()
{
  dw(8,HIGH);
  dw(9,LOW);
  while(digitalRead(zSensor)!=0){Serial.println(digitalRead(zSensor));}
  dw(8,LOW);
  dw(9,LOW);

  
  dw(6,LOW);
  dw(7,HIGH);
  while(digitalRead(ySensor)!=0){Serial.println(digitalRead(ySensor));}
  dw(6,LOW);
  dw(7,LOW);

  
  while(digitalRead(xSensor) != 1)
  {
    stepperX.step(-1);
  }
  Serial.println(digitalRead(xSensor));

  
  /*while(digitalRead(ySensor) != 1)
  {
    stepperY.step(-1);
  }
  while(digitalRead(ySensor) != 1)
  {
    //some code
  }*/
  curX = 0;
  curY = 0;
  curZ = 0;
}

void take()
{
    go('z', 1);
    delay(1000);
    dw(magnetPin, HIGH);
    delay(1000);
    go('z', 0);
}

void drop()
{
    go('x', xCrds[19]);
    go('y', yCrds[19]);
    go('z', 1);
    delay(1000);
    dw(magnetPin, LOW);
    delay(500);
    goStartPoint();
}

void setup() {
  stepperX.setSpeed(35);
//  stepperY.setSpeed(35);
  Serial.begin(9600);//salam arlan yoo
  //pizda
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  pinMode(magnetPin, OUTPUT);
  pinMode(xSensor, INPUT);
  pinMode(ySensor, INPUT);
  pinMode(zSensor, INPUT);
  
  pinMode(yEnc, INPUT);
  pinMode(zEnc, INPUT);

  goStartPoint();
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void loop() {
  delay(500);
  if (Serial.available())
  {
    String state = Serial.readString();
    String tmp_i_x = String(getValue(state, ' ', 0));
    String tmp_i_y = String(getValue(state, ' ', 1));
    String tmp_i_z = String(getValue(state, ' ', 2));
    int input_x = tmp_i_x.toInt(), input_y = tmp_i_y.toInt(), input_z = tmp_i_z.toInt();

    if(input_x==9999)goStartPoint();
    else
    {
      go('x', input_x);
      go('y', input_y);
      go('z', input_z);
    }
/*
    if(true)
  {
        dw(7, LOW);
        dw(6, HIGH);
        int counter(0), last_step(digitalRead(yEnc)), now_step;
        while(counter <= (input_y))
        {
          now_step=digitalRead(yEnc);
          if(now_step != last_step)
          {
            last_step=now_step;
            counter++;
          }
        }

        dw(6, LOW);
        
  }*/
    
    
    Serial.println(String(input_x));
    Serial.println(String(input_y));
    Serial.println(String(input_z));
  }
  //Serial.println(digitalRead(yEnc));
}


 
