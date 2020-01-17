#include <Stepper.h>

#define dw digitalWrite

const int xCrds[20], yCrds[20], stepsPerRevolution = 200, magnetPin = 12, zPin = 3, xSensor = 1, ySensor = 2, zSensor = 3;

int curX, curY, curZ;

Stepper stepperX = Stepper(stepsPerRevolution, 8, 9, 10, 11);
Stepper stepperY = Stepper(stepsPerRevolution, 8, 9, 10, 11);

void go(char num, int pos)
{
  switch(num)
  {
    case 'x':
      stepperX.step(pos - curX);
      curX = pos;
      break;

    case 'y':
      stepperY.step(pos - curY);
      curY = pos;
      break;
      
    case 'z':

      curZ = pos;
      break;
  }
}

void goStartPoint()
{
  while(digitalRead(xSensor) != 1)
  {
    stepperX.step(-1);
  }
  while(digitalRead(ySensor) != 1)
  {
    stepperY.step(-1);
  }
  while(digitalRead(ySensor) != 1)
  {
    //some code
  }
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
  stepperX.setSpeed(100);
  stepperY.setSpeed(100);
  Serial.begin(9600);

  pinMode(magnetPin, OUTPUT);
  pinMode(zPin, OUTPUT);
  pinMode(xSensor, INPUT);
  pinMode(ySensor, INPUT);
  pinMode(zSensor, INPUT);

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
  if (Serial.available())
  {
    String state = Serial.readString();
    String tmp_i_x = String(getValue(state, ' ', 0));
    String tmp_i_y = String(getValue(state, ' ', 1));
    String tmp_i_z = String(getValue(state, ' ', 2));
    int input_x = tmp_i_x.toInt(), input_y = tmp_i_y.toInt(), input_z = tmp_i_z.toInt();

    go('x', xCrds[input_x]);
    go('y', yCrds[input_y]);

    
    
    Serial.println(String(input_x));
    Serial.println(String(input_y));
    Serial.println(String(input_z));
  }
}
