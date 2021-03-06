//P20301 Owner: Debra Kunz, TX REV 3
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define THRESH 2 //voltage threshold to dete

//nrf message definitions
int go[1];
int set[1];
//radio setup
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
const int sampleWindow = 250; // Sample window width in mS (250 mS = 4Hz)
unsigned int clap;

//interrupt definitions
//ISR capable pins 0[0], 1[1], 2[2], 3[3], 7[4]

int coachButton = 7;
int setSwitch = 3;
int BATvoltagePin = 2; 
int PracticeRaceSelectSwitch = 8; //practice mode is a logic 1

volatile int coachButtonState = 0; //included for possible later use. if not necessary - can be deleted
volatile int setSwitchState = 0;
volatile int BATvoltagePinState = 0;

int lowBATled = 5; //note this is a PWM
int powerONled = 6; //note this is a PWM

//setup---------------------------------------------------------
void setup(void){
  //radio setup
Serial.begin(9600);
radio.begin();
radio.openWritingPipe(pipe);
 //I-O
//pinMode(BATvoltagePin, INPUT);
pinMode(coachButton, INPUT);
pinMode(setSwitch, INPUT);
pinMode(PracticeRaceSelectSwitch, INPUT);
//pinMode(lowBATled, OUTPUT);
//pinMode(powerONled, OUTPUT);
 // Attach an interrupt to the ISR vector
//attachInterrupt(2, power_ISR, RISING);
attachInterrupt(digitalPinToInterrupt(7), coachButton_ISR, RISING);
attachInterrupt(digitalPinToInterrupt(3), set_ISR, RISING);
 // turn on power_on LED
//digitalWrite(powerONled, HIGH);
 //define message contents
go[0] = 111;
set[0] = 101;
}

//main loop ----------------------------------------------------------



void loop(void){
  //while select switch ON for race, listen for shot
  while(!digitalRead(PracticeRaceSelectSwitch))
  {
    listen();
  }
    while(digitalRead(PracticeRaceSelectSwitch))
  {
    
  }
} 




//Functions -----------------------------------------------------------

void listen(){
  
unsigned long start= millis(); // Start of sample window
unsigned int peakToPeak = 0; // peak-to-peak level
unsigned int signalMax = 0;
unsigned int signalMin = 1024;

// collect data for 250 miliseconds
 while (millis() - start < sampleWindow)
 {
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

//SEND command control if shot heard
  if (volts >=THRESH)
  {
    Serial.println(go[0]);
    radio.write(go, 1);
  }
}

//ISRs --------------------------------------------------------------

/*void power_ISR() {
  BATvoltagePinState = digitalRead(BATvoltagePin);
  //here we read the output of the comparator, if pressed turn led on.
  Serial.println("battery low");
  digitalWrite(lowBATled, HIGH);
}*/

void coachButton_ISR() {
  detachInterrupt(4);
  //coachButtonState = digitalRead(coachButton);
  Serial.println("coach button");
  //if for practice time (ie switch is OFF)
  if(digitalRead(PracticeRaceSelectSwitch)){
    Serial.println(go[0]);
    radio.write(go, 1); //write message size = 1
  }
    attachInterrupt(4, coachButton_ISR, RISING);
}

void set_ISR() {
    Serial.println("set switch");
    Serial.println(set[0]);
    radio.write(set, 1);
}
