/*


Pet Feeder

 */
#include <string.h>
#include <time.h>
//#include <DateTime.h>
//#include <DateTimeStrings.h>

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
double iScaleZero = 0;
short LF = 10;


int par_pets;

typedef struct 
{
  int par_gram;
  int par_hour;
  int par_min;
} petfile;

petfile profile[3];




void setup() {
  Serial.begin(9600);
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
}

double GetWeight(void)
{
    unsigned char i;
    double iAveReading;
   
    iAveReading = 0;
    
    for (i = 0; i <= 9; i++)
    {
        delay(20);
        iAveReading = iAveReading + analogRead(sensorPin);      //Store the results
    }

    return(((iAveReading/10) - iScaleZero)*4);
}


//Zeros the scale and upates the iScaleZero int to reflect
void ScaleZero(void)
{
    unsigned char i;
    double iAveReading;
    
    Serial.println("Zeroing Scale.... ");
    
    iAveReading = 0;
    delay(2000);
    for (i = 0; i <= 9; i++)
    {
      delay(100);
      iAveReading = iAveReading + GetWeight();  //Store the results
    }

    iScaleZero = (iAveReading/10)/4;
    return;
}

String SerialRead(void)
{
  unsigned char cBuff[20],*buffPtr;
  buffPtr = &cBuff[0];
  while (*buffPtr != '\r')
  {
    if (Serial.available())
    {
      delay(5000);
    //Read byte from UART and write it to RAM buffer
      *buffPtr = Serial.read();
      Serial.println(*buffPtr);
    //Point to next available byte in RAM buffer
      buffPtr++;
    }
  return String(*buffPtr);
  }
}


void Configuration(void)
{

  //Serial.print(DateTimeStrings.dayStr(DateTime.DayofWeek));
  
  Serial.println("Please enter the number of pets you are planning to feed:");
  par_pets = 2; 
  Serial.println(String(par_pets));

  Serial.println("Feeding Time 1:");
  profile[1].par_hour = 2; 
  profile[1].par_min = 00; 
  Serial.println(String( profile[1].par_hour ) + ":" + String( profile[1].par_min ));
  
  Serial.println("Feeding Time 2:");
  profile[2].par_hour = 4; 
  profile[2].par_min = 00; 
  Serial.println(String( profile[2].par_hour ) + ":" + String( profile[2].par_min ));
  
  Serial.println("Gram for pet 1:");
  profile[1].par_gram = 500; 
  Serial.println(String( profile[1].par_gram ));

  Serial.println("Gram for pet 2:");
  profile[2].par_gram = 650; 
  Serial.println(String( profile[2].par_gram ));

}

void FeedTime(int profileID)
 {
  bool feeddone = false;
  while (!feeddone)
  {
    
    while (GetWeight() < profile[profileID].par_gram)
    {
      digitalWrite(13, HIGH); //turn on the vibro feeder
      Serial.println(GetWeight());
      delay(100);
    }
    digitalWrite(13, LOW);  //turn off the vibro feeder
    feeddone = true;
  }
  return;
 }

void loop() {

  ScaleZero();
  Serial.println(iScaleZero);
  delay(1000);

  Configuration();
  
  while (1)
  {
    FeedTime(1);
    //Serial.println(GetWeight());
    delay(500);

  }
  
}
