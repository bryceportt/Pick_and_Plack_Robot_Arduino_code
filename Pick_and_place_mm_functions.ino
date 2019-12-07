#include <Servo.h>
#include <Stepper.h>

#define stepPin 5
#define dirPin 4
#define endstop 9
#define magnet 3
#define magnetled 6

Servo base;
Servo endefector;

double Distance = 0;
int Degree = 0;
int Shape = 0;
int Down = 0;
int Up = 0;
int val[2];

void setup()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(magnet, OUTPUT);
  pinMode(magnetled, OUTPUT);
  pinMode(endstop, INPUT_PULLUP);

  Serial.begin(9600);

  base.attach(10);
  endefector.attach(11);

}
void loop()
{
  // call function home
  Home();
  Test();
}
void Home()
{
  HomeServos();

  if (!digitalRead(endstop))
  {
    return 0;
  }
  else
  {
    do {
      digitalWrite(dirPin, LOW);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    } while (digitalRead(endstop));
  }
}
void Test()
{
  if (Serial.available() > 0)
  {
    // Read Serial
    Degree = Serial.read();
    Distance = Serial.read();
    Shape = Serial.read();

    //edge of paper starts exactly 1400 steps away
    Distance = (Distance * 2);
    Distance = (Distance / 0.1016);
    Distance = Distance + 200;

    // write base position
    base.write(Degree);
    delay(1500);

    // call functions
    SendDistance(Distance);
    SortShape(Shape);
    DropPart();
    HomeServos();
  }
}
void HomeServos()
{
  //home end
  endefector.write(0);
  delay(500);

  //home base
  base.write(0);
  delay(500);
}
void SendDistance(double Distance)
{
  //forward
  digitalWrite(dirPin, HIGH);
  for (double x = 0; x < Distance; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  delay(500);

  PickUpPart();

  //backward
  digitalWrite(dirPin, LOW);
  for (double x = 0; x < Distance; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  delay(500);
}
void SortShape(int Shape)
{
  if (Shape == 0) // square
  {
    base.write(0);
    delay(500);
  }
  else if (Shape == 1) // Triangle
  {
    base.write(180);
    delay(500);
  }
}
void PickUpPart()
{
  // pick up
  endefector.write(160);

  // magnet on
  digitalWrite(magnet, HIGH);
  digitalWrite(magnetled, HIGH);
  delay(500);

  //up
  endefector.write(0);
  delay(500);
}
void DropPart()
{
  // magnet off
  digitalWrite(magnet, LOW);
  digitalWrite(magnetled, LOW);
  delay(500);
  Serial.write(7);
}
