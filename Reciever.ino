//Name: Andrew Balmakund
//Data: 06/03/2018
//Purpose: This program will be implemented for the receiving end of the system (robotic car).  
//What does program does is setting up a communication line (pipe address) to receive the information from the transmitting end. 
//The data collected by it will allow to determine the movement of the robotic car (is it stationary or not, what direction will it need to travel in). 
//There is also a ultrasonic sensor implemented which will calculate the distance from the robotic car to an object in front of it. 
//Based on the distance calculation, if the robotic car detects an object is within 5cm of it, it will disregard the movement given by the transmitting end (the hand gesture)
//and essentially making the car to reverse until it has reached a distance between the object of being greater than 5cm)

//Add the necessary libraries
#include <SPI.h>      //SPI library for communicate with the nRF24L01+
#include "RF24.h"     //The main library of the nRF24L01+

//Define enable pins of the Motors
const int enbA = 3;
const int enbB = 5;

const int buzzer = 41; //buzzer to arduino pin 45

//Define control pins of the Motors
const int IN1 = 4;    //Right Motor (-)
const int IN2 = 2;    //Right Motor (+)
const int IN3 = 6;    //Left Motor (+)
const int IN4 = 7;   // Left Motor (-)

int RightSpd = 150;
int LeftSpd = 150;

//Define pin numbers for the ultrasonic sensor
const int trigPin = 31;
const int echoPin = 29;

// defines variables
long duration;
int distance;

//Define packet for the direction (X axis and Y axis)
int data[3];

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
RF24 radio(9,53);

//Create a pipe addresses for the communicate
const uint64_t pipe = 0xE8E8F0F0E1LL;



void setup(){
  //Define the motor pins as OUTPUT
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output


  pinMode(53, OUTPUT);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.begin(9600);            //Sets the data rate in bits per second (baud) for serial data transmission.
  radio.begin();                    //Start the nRF24 communicate            
  radio.openReadingPipe(1, pipe);   //Sets the address of the transmitter to which the program will receive data.
  radio.startListening();             
  }

void loop(){

// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

//CONSTANT Y AXIS (FORWARD AND BACKWARD) - is around 145 to 148
//Forward, y axis > 165
//Backward, Y axis < 125

//Constant X AXIS (LEFTWARD AND RIGHTWARD)- 358 to 360
//Leftward - x axis < 330
//Rightward - x axis > 380

// Calculating the distance
distance= duration*0.034/2;  //Measuring in Cm
   Serial.println(radio.available());  
 if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &data, sizeof(data) );             // Get the payload
      }
 
       Serial.println(data[0]);         
  
if(data[2] == 1){
while(data[2]==1){
  tone(buzzer, 1000); // Send 1KHz sound signal...

  delay(5000);
  break;

}
}

  noTone(buzzer);     // Stop sound...

    if(data[1] > 165){
      //forward            
      
   
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

       analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);

      Serial.println("FORWARD");
  noTone(buzzer);     // Stop sound...

    }
    
    if(data[1] < 125){
      //backward              

      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

       analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);

 tone(buzzer, 1000); // Send 1KHz sound signal...



       Serial.println("BACKWARD");

      
    }
     
    if(data[0] <330){
      //left

      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

       analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
       Serial.println("LEFTWARD");
  noTone(buzzer);     // Stop sound...

    }

    if(data[0] > 380){
      //right

      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

       analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);

       Serial.println("RIGHTWARD");
  noTone(buzzer);     // Stop sound...

    }
if(data[0] > 330 && data[0] < 380 && data[1] > 125 && data[1] < 165){
      //stop car
      analogWrite(enbA, 0);
      analogWrite(enbB, 0);
  noTone(buzzer);     // Stop sound...

    }
    
  }
}
