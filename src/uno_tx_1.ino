/*

*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int SW1 = 7;
const int sampleWindow = 250; // Sample window width in mS (250 mS = 4Hz)
unsigned int clap;


void setup(void){
Serial.begin(9600);
radio.begin();
radio.openWritingPipe(pipe);}

void loop(void){
unsigned long start= millis(); // Start of sample window
unsigned int peakToPeak = 0; // peak­to­peak level
unsigned int signalMax = 0;
unsigned int signalMin = 1024;

// collect data for 250 miliseconds
while (millis() - start < sampleWindow)
 {
  // Serial.println(clap);
 clap = analogRead(0);
if (clap < 1024) //This is the max of the 10­bit ADC so this loop will include all readings
 {
if (clap > signalMax)
 {
 signalMax = clap; // save just the max levels
 }
else if (clap < signalMin)
 {
 signalMin = clap; // save just the min levels
 }
 } 
  }
  //Serial.println(peakToPeak);
peakToPeak = signalMax - signalMin; // max ­ min = peak­peak amplitude
double volts = (peakToPeak * 3.3) / 1024; // convert to volts
Serial.println(volts);

//Serial.println(signalMax);
//Serial.println(signalMin);
if (digitalRead(SW1) == HIGH || volts >=2){//
msg[0] = 111;Serial.println(msg[0]);
radio.write(msg, 1);}
  else {
 }
} //write message size = 1
