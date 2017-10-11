#include <OneWire.h>

// const int led = 1;
 const uint8_t digit1 = 10;  //PB1
 const uint8_t digit2 = 11;  //PB2
const uint8_t digit3 = 12;  //PB3

 //Pin connected to ST_CP of 74HC595
const uint8_t latchPin = 7;  //PD5
//Pin connected to SH_CP of 74HC595
const uint8_t clockPin = 6;   //PD4
////Pin connected to DS of 74HC595
const uint8_t dataPin = 5;   //PD3

// data pin for DS18B20 
const uint8_t termo_pin = 0; // PD0

int16_t raw;
int t;
int data_buf;
int sum;
uint8_t counter;

OneWire TemperatureSensor(termo_pin);

byte data[12]; 
byte dataArray[10]= {~0b00111111, 0b11111001, ~0b01011011, ~0b01001111,~0b01100110 , ~0b01101101, ~0b01111101, ~0b00000111, ~0b01111111, ~0b01101111}; //tablica z numerami do 7 seg

    void setup() {                
      pinMode(latchPin, OUTPUT);
      //pinMode(led, OUTPUT);  
      pinMode(digit1, OUTPUT);   
      pinMode(digit2, OUTPUT);
      pinMode(digit3, OUTPUT);

      //digitalWrite(digit1, 1);
     // digitalWrite(digit2, 1);
      //digitalWrite(digit3, 0);
  
    }


    void loop() {
      
      TemperatureSensor.reset(); // reset one wire buss
      TemperatureSensor.skip(); // select only device

      TemperatureSensor.write(0x44); // start conversion

      delay(1000); // wait for the conversion
      TemperatureSensor.reset();
      TemperatureSensor.skip();
      TemperatureSensor.write(0xBE); // Read Scratchpad
      for (int j = 0; j < 9; ++j) { // 9 bytes
      data[j] = TemperatureSensor.read();
      }
      raw = (data[1] << 8) + data[0];   // plus czy |, oto jest pytanie 
      raw = raw>>4;

      
    int maska=0b00001000;
    int wartosc=50;
    for(int i=0; i<4; i++)
    {
        if((data[0]&maska))
            sum=sum+wartosc;
        maska=maska>>1;
        wartosc=wartosc/2;
    }

      while(sum>0 &&counter<9){
        sum=sum-10;
        ++counter;
      }
      sum=counter;
      


      for(int i=0; i<5000; i++)
      temp_display(raw,sum);  // jestem idiotą, funkcja wyswietlajaca jest dostosowana tylko do temperatury pełnej
      sum=0;
      counter=0;
      //digitalWrite(led, HIGH);

       /*
      for(int i=0; i<10; i++)
      {

      //data=data>>1;

      // functions below are essential for shifting out data, dont touch it
      digitalWrite(latchPin, 0);
      shiftOut( dataPin, clockPin, dataArray[raw/10]);
      digitalWrite(latchPin, 1);

      // casual delay
      delay(1000);
      }
      */
      
      //digitalWrite(led, LOW);
      //delay(1000);
     
      
    }


void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOutďż˝
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}

void temp_display(int temp, int decimal)
{
  
  digitalWrite(digit1, 1);
  digitalWrite(digit2, 0);
  digitalWrite(digit3, 0);

  digitalWrite(latchPin, 0);
  shiftOut( dataPin, clockPin, dataArray[temp%10]);    //bylo temp%10
  digitalWrite(latchPin, 1);


  digitalWrite(digit1, 0);
  digitalWrite(digit2, 1);

  digitalWrite(latchPin, 0);
  shiftOut( dataPin, clockPin, dataArray[decimal]);   //bylo temp/10
  digitalWrite(latchPin, 1);  


  digitalWrite(digit1, 0);
  digitalWrite(digit2, 0);
  digitalWrite(digit3, 1);
  
  digitalWrite(latchPin, 0);
  shiftOut( dataPin, clockPin, dataArray[temp/10]);
  digitalWrite(latchPin, 1);

  //delay(50);

  digitalWrite(digit1, 0);
  digitalWrite(digit2, 0);
  digitalWrite(digit3, 0);


}


