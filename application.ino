
#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include <services.h> 
#include <sstream>



int softPot = 0;  // this line selects the input pin a0 for the sensor
int ledPin = 13; // this line selects the pin 13 for the LED output
int tempPot = 0; // variable to store the value coming from the sensor
int timeSitting = 0;
int previousTimeSitting = 0;
unsigned char buf[16] = {0};
unsigned char len = 0;
int  displayedTime;

int prev=0;


//ostringstream convert;   // stream used for the conversion
void setup() {
  
    ble_set_name("BlendMicro");
  
  // Init. and start BLE library.
  ble_begin();
  
  // Enable serial debug
  Serial.begin(57600);
  pinMode(softPot, INPUT_PULLUP);
}
int numDigits(int number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}
void ble_write_integer(int number){
  int digits = numDigits(number);
  char * time;
  int digitsArray[digits];
  if(digits ==1 ){
     itoa(number,time,10);
     ble_write_bytes((unsigned char *)time, sizeof(time));
  }else{
    int tempDigits=digits;
    while(number>0){
      digitsArray[tempDigits-1]=number%10;
      number= number/10;
      tempDigits--;
    }
    for(int i = 0;i<digits;i++){
      itoa(digitsArray[i], time, 10);
      ble_write_bytes((unsigned char*)time, sizeof(time));
      
    }
  }
  
}
void ble_write_string(unsigned char *data, uint8_t len)
{
  
    for (int i = len-1; i >= 0; i--){
        //Serial.println(i+" "+data[i]);
        ble_write(data[i]);
    }
}

void loop() {
  
  
  tempPot = digitalRead(softPot);  
  
  if(tempPot==0 && prev==0){
    timeSitting++;
    displayedTime = timeSitting;
  }
  else if(tempPot==0 && prev==1){
    timeSitting = 1;
    displayedTime = timeSitting;
    prev=0;   
  }else if(tempPot ==1 && prev==0){
    displayedTime = timeSitting;
    prev=1;
  }else{
    displayedTime = timeSitting;
  }
  

  if(tempPot==0){
  digitalWrite(ledPin,HIGH);
  }else {
  digitalWrite(ledPin,LOW);
  }
  if( ble_connected() && tempPot==0){
    
    ble_write_integer(displayedTime);
    //ble_write_bytes(displayedTime, sizeof(displayedTime));
    //Serial.println(displayedTime);
    }
    else if( ble_connected() && tempPot==1){
    
    ble_write_integer(displayedTime);
   // ble_write_bytes(displayedTime, sizeof(displayedTime));
    //Serial.println(displayedTime);
    }


  ble_do_events();
 
  Serial.println(tempPot);
  Serial.println(displayedTime);
    delay(1000); 
    }
