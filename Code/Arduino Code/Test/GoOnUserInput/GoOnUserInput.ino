/* This example shows basic use of the AMIS-30543 stepper motor
driver.

It shows how to initialize the driver, set the current limit, set
the micro-stepping mode, and enable the driver.  It shows how to
send pulses to the NXT/STEP pin to get the driver to take steps
and how to switch directions using the DIR pin.  The DO pin is
not used and does not need to be connected.

Before using this example, be sure to change the
setCurrentMilliamps line to have an appropriate current limit for
your system.  Also, see this library's documentation for
information about how to connect the driver:

    http://pololu.github.io/amis-30543-arduino/
*/

#include <SPI.h>
#include <AMIS30543.h>
#include <Encoder.h>

const uint8_t amisDirPin = 5;//2;
const uint8_t amisStepPin = 6;//3;
const uint8_t amisSlaveSelect = 4;//SS;

AMIS30543 stepper;
int a = 500;
int b = 270;
int usr;

Encoder slidePos(2, 3); //Use pins 0 + 1 as they are both interupt pins.

void setup()
{
  SPI.begin();
  stepper.init(amisSlaveSelect);
  // Drive the NXT/STEP and DIR pins low initially.
  digitalWrite(amisStepPin, LOW);
  pinMode(amisStepPin, OUTPUT);
  digitalWrite(amisDirPin, LOW);
  pinMode(amisDirPin, OUTPUT);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings.
  stepper.resetSettings();

  // Set the current limit.  You should change the number here to
  // an appropriate value for your particular system.
  stepper.setCurrentMilliamps(a);

  // Set the number of microsteps that correspond to one full step.
  stepper.setStepMode(4);

  // Enable the motor outputs.
  stepper.enableDriver();
  Serial.begin(9600);
  stepper.sleep();
}

void loop()
{
  while(!Serial){}
  Serial.print("0 for zero, 6 prints current pos, any other # probes\n"); //Going to write a 
  while(!Serial.available()){}
  while(!Serial.available()){}
  usr = Serial.parseInt();
  if(usr == 0){
    slidePos.write(0);
  }else if(usr == 6){
    output();
  }else{
  // Step in the default direction 1000 times.
  stepper.sleepStop();
  setDirection(0);
  for (unsigned int x = 0; x < 100; x++)
  {
    step();
  }
  stepper.setCurrentMilliamps(b);
  for (unsigned int x = 0; x < 9900; x++)
  {
    step();
  }
  output();
  delay(500);
  
  stepper.setCurrentMilliamps(1500);
  
  setDirection(1);
  // Step in the other direction 1000 times.
  for (unsigned int x = 0; x < 10000; x++)//while(slidePos.read()>3)
  {
    step();
  }
  stepper.setCurrentMilliamps(a);
  Serial.println();
  stepper.sleep();
}
}

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void step()
{
  // The NXT/STEP minimum high pulse width is 2 microseconds.
  digitalWrite(amisStepPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(amisStepPin, LOW);
  delayMicroseconds(3);

  // The delay here controls the stepper motor's speed.  You can
  // increase the delay to make the stepper motor go slower.  If
  // you decrease the delay, the stepper motor will go fast, but
  // there is a limit to how fast it can go before it starts
  // missing steps.
  delayMicroseconds(600);
}

// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void setDirection(bool dir)
{
  // The NXT/STEP pin must not change for at least 0.5
  // microseconds before and after changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(amisDirPin, dir);
  delayMicroseconds(1);
}

void output(){
  Serial.print("mm: ");
  Serial.print((float)slidePos.read()/200.0, DEC);
  Serial.print("\npulses: ");
  Serial.print(slidePos.read(), DEC);
  Serial.print("\n");
}
