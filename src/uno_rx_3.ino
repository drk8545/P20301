/*
LED on and msg set to 111 at button press from T
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int LED1 = 3;

void setup(void){
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(1,pipe);
radio.startListening();
pinMode(LED1, OUTPUT);}

void loop(void){
if (radio.available()){
bool done = false;
while (!done){
radio.read(msg, 1);
Serial.println(msg[0]);
if (msg[0] == 101){
    digitalWrite(LED1, HIGH);
}//removed delay here from rev0

else if (msg[0] == 111){
    digitalWrite(LED1, LOW);
  delay(300);
      digitalWrite(LED1, HIGH);
  delay(300);
      digitalWrite(LED1, LOW);
  delay(300);
      digitalWrite(LED1, HIGH);
  delay(300);
      digitalWrite(LED1, LOW);
}//removed delay here from rev0
    
//else {
//    digitalWrite(LED1, LOW);}//removed delay here from rev0
//delay(10);
  //msg[0] = 000; //remove for light stay on until reset
}}
else{Serial.println("No radio available");}}
