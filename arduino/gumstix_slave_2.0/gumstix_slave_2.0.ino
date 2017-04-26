/*
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 16 servos, one after the other
  
  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  The Arduino as an I2C slave to power PWM channels for servos.
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


#define SERVOMIN  120 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

#define I2C_SLAVE_ADDRESS 0x10  // Set Arduino's address on I2C bus

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // default address (0x40)

uint8_t BlueArmLeft = 10;  
uint8_t BlueArmRight = 9; 
uint8_t RedArmLeft = 8; 
uint8_t RedArmRight = 7; 

// RIGHT ARMS GO POSITIVE ANGLES, LEFT ARMS NEGATIVE (relative to 90 degrees)

/*
 * Create callback function when I2C data sent from Gumstix
 */
 void onRecieveHandler(int numBytes)
{
  if (Wire.available() == 3) {
    int player_id = Wire.read();      // receive byte as an integer  (arm number to move)
    int leftAngle = Wire.read();      // receive byte as an integer  (left arm position)
    int rightAngle = Wire.read();     // receive byte as an integer  (right arm position)

    int pulseLeft = map(leftAngle, 0, 180, SERVOMIN, SERVOMAX);
    int pulseRight = map(rightAngle, 0, 180, SERVOMIN, SERVOMAX);
      
    if (player_id == 1) 
    {
      pwm.setPWM(BlueArmLeft, 0, pulseLeft);
      pwm.setPWM(BlueArmRight, 0, pulseRight);
    } 
    else 
    { 
      pwm.setPWM(RedArmLeft, 0, pulseLeft);
      pwm.setPWM(RedArmRight, 0, pulseRight);
    }
  } 
  else 
  {
    for (int i = 0; i < numBytes; i++) 
    {
      Wire.read();
    }
  }
}

/*
 * Setup the I2C communication
 */
void begin_i2c()
{
    Wire.begin(I2C_SLAVE_ADDRESS);    // Connect Arduino to I2C bus at I2C_SLAVE_ADDRESS
    Wire.onReceive(onRecieveHandler); // Create callback function for receiving data
}

void setup() {
  begin_i2c();
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();
}

void loop() 
{
 
}
