#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
MAX30105 particleSensor;
const byte RATE_SIZE = 4; 
byte rates[RATE_SIZE]; 
byte rateSpot = 0;
long lastBeat = 0; 
float beatsPerMinute;
int beatAvg;
int PWM=0;

#define DIR1 4 //方向信号
#define PUL1 5 //脉冲信号
int Step[12]={422,266,200,156,133,111,88,67,53,44,35,25};
bool Flag=true; 
int Count=1; 
void setup() 
{                                
    Serial.begin(115200);
    Serial.println("Initializing...");
    
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }
    Serial.println("Place your index finger on the sensor with steady pressure.");
    particleSensor.setup();                      
    particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
    particleSensor.setPulseAmplitudeGreen(0); 
    pinMode(DIR1, OUTPUT);
    pinMode(PUL1, OUTPUT);  
    pinMode(6, OUTPUT);          
}

void loop()
{
    Ctrl_Motor();
}
//1600
//95+60+45+35+30+25+20+15+12+10+8+5
//160 36度
//80  18度
//40  9度
void Ctrl_Motor()
{
    long irValue = particleSensor.getIR();
    if (checkForBeat(irValue) == true)
    {
        long delta = millis() - lastBeat;
        lastBeat = millis();
        beatsPerMinute = 60 / (delta / 1000.0);
        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
            rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
            rateSpot %= RATE_SIZE; //Wrap variable
            //Take average of readings
            beatAvg = 0;
            for (byte x = 0 ; x < RATE_SIZE ; x++)
            {
                beatAvg += rates[x];
            }
            beatAvg /= RATE_SIZE;
        }
    }

    if (irValue < 50000)
    {
        Serial.print(" No finger?");
        analogWrite(6,0);
    }else{
        Serial.print("IR=");
        Serial.print(irValue);
        Serial.print(",   Avg BPM=");
        Serial.println(beatAvg);
        if(beatAvg>100)
        {
            PWM=100;
        }else{
            PWM=map(beatAvg,0,100,50,255);
        }
        analogWrite(6,PWM);
    }
}

void Forward_1(int count) //正转
{  
    digitalWrite(DIR1, LOW);    
    for(int i=0;i<count;i++)
    {
        digitalWrite(PUL1, HIGH);
        //delayMicroseconds(1000);
        //Ctrl_Motor();
        digitalWrite(PUL1, LOW);
        //delayMicroseconds(1000); 
        //Ctrl_Motor();   
    }
}
void Reversal_1(int count)       //反转
{  
    digitalWrite(DIR1, HIGH); 
    for(int i=0;i<count;i++)
    {
        digitalWrite(PUL1, HIGH);
        //delayMicroseconds(1000);
       // Ctrl_Motor();
        digitalWrite(PUL1, LOW);
        //delayMicroseconds(1000); 
       // Ctrl_Motor();     
    }
}
