//P20301 Owner: Debra Kunz, RXREV 3 BAT Test
//will start printing to serial terminal time that battery went below threshold (at the time that the battery goes below threshold) //PT
//PT elements denote Battery testing lines
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define LEDelay 1000 //controls how long the GO led blinks for
#define BLINKdelay 30 //controls blinking frequency

#define MSG_GO 111  //control message contents for go
#define MSG_SET 101 //control message contents for set

//radio definitions
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

//LEDs
int goLED = 3;
int goLED2 = 9;
int setLED = 8;
int lowBATled = 5;
int powerONled = 6;

//battery interrupt
int BATvoltagePin = 2; 
volatile int BATvoltagePinState = 0;

//function definitions
void checkRadio();

unsigned long power_below_thresh_time; //PT
unsigned long time_in_min; //PT
int printed_time = 0;

//setup ----------------------------------------------

void setup(void){
  
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(1,pipe);
radio.startListening();

pinMode(BATvoltagePin, INPUT);
pinMode(goLED, OUTPUT);
pinMode(goLED2, OUTPUT);
pinMode(setLED, OUTPUT);
// Attach an interrupt to the ISR vector
attachInterrupt(2, power_ISR, RISING);
// turn on power_on LED
digitalWrite(powerONled, HIGH);
}

//main loop ------------------------------------------

void loop(void){
  
checkRadio();
if(printed_time == 1){
  Serial.println(power_below_thresh_time/60000); //PT //prints time in minutes after thresh is met
}
}



//functions ------------------------------------------

void checkRadio(){
    if (radio.available())
  {
    bool done = false;
      while (!done)
      {
        radio.read(msg, 1);
        //Serial.println(msg[0]);
          if (msg[0] == MSG_GO)
          {
            for(int i=0; i < LEDelay; i++)
            {
              digitalWrite(goLED, HIGH);
              digitalWrite(goLED2, HIGH);
              delay(BLINKdelay);
              digitalWrite(goLED, LOW);
              digitalWrite(goLED2, LOW);
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
            digitalWrite(goLED2, LOW);
          }
      }   
  }
  else
  {
    //Serial.println("No radio available");
  }
}

//ISRs ------------------------------------------------------

void power_ISR() {
  BATvoltagePinState = digitalRead(BATvoltagePin);
  //here we read the output of the comparator, if pressed turn led on.
  Serial.println("battery low");
  digitalWrite(lowBATled, HIGH);
  power_below_thresh_time = millis(); //PT
  printed_time = 1; //PT
}
