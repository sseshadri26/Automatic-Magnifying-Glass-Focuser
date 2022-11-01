//cosntants for the pins where sensors are plugged into.
const int sensorPin1 = 14;
const int sensorPin2 = 12;

const int sensorPin3 = 26;
const int sensorPin4 = 27;

bool horizontalRead = false;
#include <TinyStepper_28BYJ_48.h>


int lightVal1;   // light reading
int lightVal2;   // light reading
int lightVal3;   // light reading
int lightVal4;   // light reading

int horDif;
int vertDif;


const int MOTOR_IN1_PIN = 15;
const int MOTOR_IN2_PIN = 2;
const int MOTOR_IN3_PIN = 0;
const int MOTOR_IN4_PIN = 4;
const int STEPS_PER_REVOLUTION = 2048;
//
// create the stepper motor object
//
TinyStepper_28BYJ_48 stepper;

#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 75;    // variable to store the servo position
int servoPin = 22;

Servo mycontservo;  // create servo object to control a servo
int servocontPin = 21;


void setup()
{

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2700); // attaches the servo on pin 18 to the servo object

  mycontservo.setPeriodHertz(50);    // standard 50 hz servo
  mycontservo.attach(servocontPin, 500, 2700); // attaches the servo on pin 18 to the servo object


  // We'll set up the LED pin to be an output.
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);

  stepper.connectToPins(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_IN3_PIN, MOTOR_IN4_PIN);
  stepper.setSpeedInStepsPerSecond(256);
  stepper.setAccelerationInStepsPerSecondPerSecond(512);
  Serial.begin(115200);

  myservo.write(80);
  //we will take a single reading from the light sensor and store it in the lightCal        //variable. This will give us a prelinary value to compare against in the loop
}

char incomingByte; // for incoming serial data

void loop()
{

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if(incomingByte=='u')
    {
      mycontservo.write(96);
      delay(100);
      mycontservo.write(87);
    }
    if(incomingByte=='d')
    {
      mycontservo.write(75);
      delay(100);
      mycontservo.write(88);
    }
  }

  //  mycontservo.write(105);
  lightVal1 = analogRead(sensorPin1); // read the current light levels
  lightVal2 = analogRead(sensorPin2); // read the current light levels

  lightVal3 = analogRead(sensorPin3); // read the current light levels
  lightVal4 = analogRead(sensorPin4); // read the current light levels


  //if lightVal is less than our initial reading withing a threshold then it is dark.
  //  Serial.print("Left: ");
  //  Serial.println(lightVal1);
  //  Serial.print("Right: ");
  //  Serial.println(lightVal2);

  //  Serial.print("Up: ");
  //  Serial.println(lightVal3);
  //  Serial.print("Down: ");
  //  Serial.println(lightVal4);

  horDif = lightVal1 - lightVal2;
  vertDif = lightVal3 - lightVal4;

  //  Serial.println(horDif);
  if (horizontalRead == false)
  {
    if (horDif > 5)
    {
      stepper.moveRelativeInSteps(-1);
      return;

    }
    if (horDif < -5)
    {
      stepper.moveRelativeInSteps(1);
      return;

    }
  }
  horizontalRead = true;
  Serial.print("Up: ");
  Serial.print(lightVal3);
  Serial.print(" ||| Down: ");
  Serial.print(lightVal4);

  Serial.print(" ||| Left: ");
  Serial.print(lightVal1);
  Serial.print(" ||| Right: ");
  Serial.print(lightVal2);


  Serial.print(" ||| Dif: ");
  Serial.println(vertDif);



  if (vertDif > 15)
  {
    pos -= 1;
    delay(30);
  }
  if (vertDif < -15)
  {

    pos += 1;
    delay(30);
  }

  if (pos < 70)
  {
    pos = 70;
  }
  if (pos > 80)
  {
    pos = 80;
  }
  Serial.print("Pos: ");
  Serial.println(pos);
  myservo.write(pos);

  //  mycontservo.write(105);


  delay(200);
}
