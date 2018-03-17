#include <EasyTransfer.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

//create object
EasyTransfer ET; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t lengthID; // An ID number 0, 1, 2, 3, 4
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

int pos = 0;    // variable to store the servo position
float P = 0;
//float g = 9.8 // gravitational acceleration
float len = 0; // length of the pendulum -- from servo turning point to center of ball.

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  Serial.begin(9600);
  ET.begin(details(mydata), &Serial);
}

float period(float L) {
  L = L/100.0; //length in m
  P = 2*PI*sqrt(L/9.8);
  return P*1000; //period in ms
}

void loop() {
  //check and see if a data packet has come in. 
  if(ET.receiveData()){
    //this is how you access the variables. [name of the group].[variable name]
    switch (mydata.lengthID) {
      case 0:
        P = period(10); // cm
        break;
      case 1:
        P = period(13); // cm
        break;
      case 2:
        P = period(16); // cm
        break;
      case 3:
        P = period(19); // cm
        break;
      case 4:
        P = period(22); // cm
        break;
      case 5:
        P = period(25); 
        break;
    }    
    Serial.println(P);
  }
  for (pos = 50; pos <= 130; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(P/80);                       // waits for the servo to reach the position
  }
  for (pos = 130; pos >= 50; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(P/80);                       // waits for the servo to reach the position
  }
}
