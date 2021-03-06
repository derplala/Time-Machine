#include <EasyTransfer.h>
#include <X113647Stepper.h>

//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  int16_t lengthID; // An ID number 0, 1, 2, 3, 4
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

static const int STEPS_PER_REVOLUTION = 64 * 32;  // the number of steps per revolution for the motor

// initialize the stepper library on pins 2 through 5:
X113647Stepper myStepper(STEPS_PER_REVOLUTION, 3, 4, 5, 6);

int motorspeed = 5.5;

int sensorPin = A0;
int sensorValue = 0;

int previousID;

void setup() {
  // set the speed in rpm:
  Serial.begin(9600);
  myStepper.setSpeed(motorspeed);  
  ET.begin(details(mydata), &Serial);
  
  previousID = 5;
  mydata.lengthID = 5;
  ET.sendData(); // send to arduino buddy
  myStepper.step(-STEPS_PER_REVOLUTION/2); // reel in a little bit  
}

void loop() {
  sensorValue = analogRead(sensorPin);
  if (sensorValue < 190){ // 250 cm
    mydata.lengthID = 4;
  }
  if (sensorValue < 152){ // 200 cm
    mydata.lengthID = 3;
  }
  if (sensorValue < 114 ){ // 150 cm
    mydata.lengthID = 2;
  }
  if (sensorValue < 76 ){ // 100 cm
    mydata.lengthID = 1;
  }
  if (sensorValue < 39 ){ // 50 cm
    mydata.lengthID = 0;
  }
  else if (sensorValue > 190) {
    mydata.lengthID = 5;
  }
  
  if (previousID != mydata.lengthID) { // only if the value has 
    ET.sendData(); // send to arduino buddy
    int ID = previousID;
    previousID = mydata.lengthID; 
    while (ID > mydata.lengthID) {
      myStepper.step(-STEPS_PER_REVOLUTION/2); // reel in
      Serial.println(ID);
      ID--;
    } 
    while (ID < mydata.lengthID) {
      myStepper.step(STEPS_PER_REVOLUTION/2); // reel out
      Serial.println(ID);
      ID++;
    }
  }
  delay(2000); // wait for a bit
}
