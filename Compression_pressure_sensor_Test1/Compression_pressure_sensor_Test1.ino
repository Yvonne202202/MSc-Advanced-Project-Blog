#define DIR1 4 //方向信号
#define PUL1 5 //脉冲信号
int Step[12]={422,266,200,156,133,111,88,67,53,44,35,25};
bool Flag=true; 
int Count=1; 
void setup() 
{                                
    Serial.begin(9600);
    pinMode(DIR1, OUTPUT);
    pinMode(PUL1, OUTPUT);           
}

void loop()
{
    int Press=analogRead(A0);
    Serial.println(Press);
    if(Press>1 &&Flag==true)
    {
         if(Count>12)
         {
             Count=1;
         }
         Forward_1(Step[Count-1]);  
         Count++;
         Flag=false;
    }else{
         Flag=true;  
    }

}
//1600
//95+60+45+35+30+25+20+15+12+10+8+5
//160 36度
//80  18度
//40  9度



void Forward_1(int count) //正转
{  
    digitalWrite(DIR1, LOW);    
    for(int i=0;i<count;i++)
    {
        digitalWrite(PUL1, HIGH);
        delayMicroseconds(1000);
        digitalWrite(PUL1, LOW);
        delayMicroseconds(1000);    
    }
}
void Reversal_1(int count)       //反转
{  
    digitalWrite(DIR1, HIGH); 
    for(int i=0;i<count;i++)
    {
        digitalWrite(PUL1, HIGH);
        delayMicroseconds(1000);
        digitalWrite(PUL1, LOW);
        delayMicroseconds(1000);    
    }
}
