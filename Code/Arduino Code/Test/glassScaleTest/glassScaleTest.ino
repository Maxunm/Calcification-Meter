#include <SPI.h>
#include <AMIS30543.h>
#include <Encoder.h>

const uint8_t amisDirPin = 2;
const uint8_t amisStepPin = 3;
const uint8_t amisSlaveSelect = SS;

AMIS30543 stepper;
int a = 1000;
int b = 1000;

int toTake, taken;

Encoder slidePos(0, 1);
volatile byte first = 0;


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
  taken = 0;
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
  while(!Serial){}
  slidePos.write(0);
}

void loop()
{
  // Step in the default direction 1000 times.
  stepper.sleepStop();
  setDirection(1);
  toTake = random(6000, 12000);
  for (unsigned int x = 0; x < toTake; x++)
  {
    step();
  }
  output();
  delay(500);
  // Step in the other direction 1000 times.
  setDirection(0);
  while(abs(slidePos.read()) > 5)
  {
    step();
    taken++;
  }
  output();
  Serial.print("Steps taken: ");
  Serial.println(taken, DEC);
  Serial.println();
  taken = 0;
  stepper.sleep();
  delay(500);
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
  Serial.print((float)slidePos.read()/100.0, DEC);
  Serial.print("\npulses: ");
  Serial.print(slidePos.read(), DEC);
  Serial.print("\n");
}
