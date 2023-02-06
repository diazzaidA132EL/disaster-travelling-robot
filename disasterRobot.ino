#include <Wire.h>
#include "Adafruit_TCS34725.h"

//motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;
// Ultrasonic Sensor
//
int trigPin = 3;    // TRIG pin
int echoPin = 4;    // ECHO pin
int buzzer = 14;
#define commonAnode true
float duration_us, distance_cm;

byte gammatable[256];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // configure the trigger pin to output mode
  pinMode(trigPin, OUTPUT);
  // configure the echo pin to input mode
  pinMode(echoPin, INPUT);
  if (tcs.begin()) {
    //Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }


  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }
}



//For speed COntrol of Tank
void speedControl(){
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enB, 75);
  analogWrite(enA, 75);
  
}

void belokKiri(){

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enB, 111);
  analogWrite(enA, 111);
  
}
void stopTank(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {
  float red, green, blue;
    // generate 10-microsecond pulse to TRIG pin
    tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRGB(&red, &green, &blue);
  
  tcs.setInterrupt(true);  // turn off LED

  Serial.print("R:\t"); Serial.print(int(red)); 
  Serial.print("\tG:\t"); Serial.print(int(green)); 
  Serial.print("\tB:\t"); Serial.print(int(blue));
  

//  Serial.print("\t");
//  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\n");

  //buzzer turn on this is for human detetction when human RGB color is detected,
  //then buzzer wll turn on
  if ((int(red)> 135) && (int(green) < 80) && (int(blue) < 60)){
    digitalWrite(buzzer, HIGH);  
    stopTank();
  }
  else{
    digitalWrite(buzzer, LOW);
  }
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
    // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  delay(100);
  if (distance_cm < 20){
    
        stopTank();
        delay(2000);
        belokKiri();
        delay(1000);
        stopTank();
        
  }
  else{
    speedControl();
  }
}
