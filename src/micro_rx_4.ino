//P20301 Owner: Debra Kunz, RXREV 3
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define LEDelay 20 //controls how long the GO led blinks for
#define BLINKdelay 30 //controls blinking frequency

#define MSG_GO 111  //control message contents for go
#define MSG_SET 101 //control message contents for set

//radio definitions
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

//LEDs
int goLED = 7;
//int goLED2 = 9;
int setLED = 8;
int lowBATled = 5;
int powerONled = 6;

//battery interrupt
int BATvoltagePin = 2; 
volatile int BATvoltagePinState = 0;

//function definitions
void checkRadio();

//setup ----------------------------------------------

void setup(void){
  
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(1,pipe);
radio.startListening();

pinMode(BATvoltagePin, INPUT);
pinMode(goLED, OUTPUT);
//pinMode(goLED2, OUTPUT);
pinMode(setLED, OUTPUT);
// Attach an interrupt to the ISR vector
//attachInterrupt(digitalPinToInterrupt(2), power_ISR, CHANGE);
// turn on power_on LED
digitalWrite(powerONled, HIGH);
}

//main loop ------------------------------------------

void loop(void){
  
checkRadio();

}


//functions ------------------------------------------

void checkRadio(){
    if (radio.available())
  {
    bool done = false;
      while (!done)
      {
        radio.read(msg, 1);
        if(msg[0]!= 0)
        {Serial.println(msg[0]);}
          if (msg[0] == MSG_GO)
          {
            for(int i=0; i < LEDelay; i++)
            {
              digitalWrite(goLED, HIGH);
              //digitalWrite(goLED2, HIGH);
              delay(BLINKdelay);
              digitalWrite(goLED, LOW);
              //digitalWrite(goLED2, LOW);
              delay(BLINKdelay);
            }
            digitalWrite(setLED, LOW);
          }
          
          if(msg[0] == MSG_SET)
          {
            digitalWrite(setLED, HIGH);
          } 
          
          else 
          {
            digitalWrite(goLED, LOW);
            //digitalWrite(goLED2, LOW);
          }
      }   
  }
  else
  {
    Serial.println("No radio available");
  }
}

//ISRs ------------------------------------------------------

/*void power_ISR() {
  //BATvoltagePinState = digitalRead(BATvoltagePin);
  //here we read the output of the comparator, if pressed turn led on.
  if (!digitalRead(BATvoltagePin))
  {
  Serial.println("battery low");
  digitalWrite(lowBATled, HIGH);
  }
  else { digitalWrite(lowBATled, LOW);}
}*/
