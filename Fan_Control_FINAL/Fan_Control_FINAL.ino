/*Fan Control Code #9                   Taylor Sharpe & Jamie Wong
EAS199A, 004                            11/14/2012

Calibrates a temperature probe to the value "inputtemp", assuming the room
is at that temperature.  Uses this calibration process to develop a conversion
factor, which is applied to the temperature probe's readings in a linear 
fashion.

Reads a potentiometer during the "loop" function; three settings will either
turn the fan off, turn it to "auto" (at which point the fan will turn on when
the calibrated temperature sensor detectrs a temperature above "caltemp," or
turn the fan on manually.

When the fan is on, an LED will indicate this to be the case; the servo will
complete an arc of 100 degrees; and the fan will spin as the DC motor is 
activated.  The "on" function is called whenever the fan is set to on 
manually or when the fan is set to auto and the temperature sensor detects a
temperature above "caltemp."
*/

#include <Servo.h>                          ////Include the Servo Library

int potPin = 5;                             ////Potentiometer
int pot_value = 0;                          ////Potent.'s Value
int heat_value = 0;                            ////Thermal Value
int automaticPin = 3;                      ///LED which indicates Thermal mode
int onPin = 2;                             ///LED which indicates On mode

Servo theservo;                             ///Creates a servo object
int servo_pos = 0;                          ///Defines the servo's position

int temperaturePin = 0;                     ///Temperature Sensor
float fartemperature = 0;                  ///Converted temperature
float counter = 0;                          ///Used in calibration
float counter2 = 0;                         ///Also used in calibration
float inputtemp = 72;                       ///Calibrates to this value
float caltemp = 77;                       ///The temperature
                                            ///at which auto turns on
  float conversionfactor = .64;

int motor_pin = 5;
int motor_speed = 150;


void setup()
{
 Serial.begin(9600);
 pinMode(automaticPin, OUTPUT);
 pinMode(onPin, OUTPUT);
 theservo.attach(9);                        ////Attaches the servo to PIN 9
 theservo.write(servo_pos);                 ////Initialy sets servo to pos. 0
 pinMode(motor_pin, OUTPUT);  
 float conversionfactor = calibrate();
 Serial.println(conversionfactor);
 delay(50);
}


void loop()
{
  pot_value = analogRead(potPin);
    float temperature = (analogRead(temperaturePin)*.004882814);
  fartemperature = ((((temperature - .5) * 100) * 1.8) + 32)*conversionfactor;
  if(pot_value <= 360)
    {
      analogWrite(motor_pin, 0);
      off();
    }
  else if(pot_value > 360 && pot_value <= 730)
    {
      analogWrite(motor_pin, 0);
      Serial.println(fartemperature);
      automatic(fartemperature);
    }
  else
    {
   analogWrite(motor_pin, motor_speed);

      on();
    }
}

//////////////////////////This function is called when OFF//////////////////////////
void off()
{
      digitalWrite(automaticPin, LOW);
      digitalWrite(onPin, LOW);
    Serial.println("OFF");
  delay(100);
}

/////////////////////////This function is called when AUTO//////////////////////////
float automatic(float inputtemp)
{
  
     digitalWrite(automaticPin, HIGH);
     digitalWrite(onPin, LOW);
      delay(500);
      
      if(inputtemp > caltemp && abs(caltemp-inputtemp) < 10)  ///ignores huge/tiny #s
       { digitalWrite(motor_pin, motor_speed);
       on();}
      else
      {digitalWrite(motor_pin, 0);}
}

////////////////////////This function is called when ON/////////////////////////////
void on()
{
      digitalWrite(onPin, HIGH);
      digitalWrite(automaticPin, LOW);
  Serial.println("ON");
  
  for(servo_pos = 0; servo_pos < 100; servo_pos +=1)
  {
    theservo.write(servo_pos);
    delay(15);
  }
  
  for(servo_pos = 100; servo_pos>=1; servo_pos-=1)
  {
    theservo.write(servo_pos);
    delay(15);
  }
}

///////////This function calibrates the sensor to an assumed value ("inputtemp")//////
float calibrate()
{
  int i = 0;
      while(i<=30)
      {
  float temp = (analogRead(temperaturePin)*.004882814);
  float newtemp = ((((temp - .5) * 100) * 1.8) + 32);
  counter = counter + newtemp;
  counter2++;
  i++;
  Serial.println(counter);
  delay(500);
      }
      
 Serial.println(counter);
 delay(1000);
 float average = counter/30;
 conversionfactor = inputtemp/average;
 
  return conversionfactor;
}


