#include <Wire.h>
#include "MMA7660.h"
#include <SoftwareSerial.h>
#define COUNT_OF_LATITUDE 3
#define COUNT_OF_LONGITUDE 5
#define COUNT_OF_TIME  1
MMA7660 acc;  //Объявление объектов и массива строк
SoftwareSerial gps(5,4);
SoftwareSerial sim(2,3);
String StringMass[12];
int prevTime= -300000;
bool trigger = false;
float ax1,ay1,az1,ax2,ay2,az2;
void setup()
{
    acc.init();  //инициализируем акселерометр
    pinMode(6, OUTPUT);
    Serial.begin(38400);   
    gps.begin(38400);
    sim.begin(19200);  
    delay(1000);
    acc.getAcceleration(&ax1,&ay1,&az1);
}

void loop()
{
    acc.getAcceleration(&ax2,&ay2,&az2);
    if((ax2-ax1>=0.5)||(ay2-ay1>=0.5)||(az2-az1>=0.5))
    {
        trigger=true;
    }
    if((ax2-ax1<=0.2)||(ay2-ay1<=0.2)||(az2-az1<=0.2))
    {
        if(millis()-prevTime>300000){       
            trigger=false;
            prevTime= -300000;
        }
    }
    char index = 0;
    char temp = 0;
    String dataString = "";
    int count=0;

    if(trigger==true){
        tone(6, 440);
        while(gps.available()) //в этой части кода мы читаем то, что отправляет нам GPS модуль
        {    
    
            temp = gps.read();
            if(temp==','){                          //разделяем данные на отдельные элементы
                StringMass[count]=dataString;
                temp=0;
                dataString="";
                count++;   
            }
            if(StringMass[2]=="V"){         //Проверяем приходят ли координаты
                /*digitalWrite(4, HIGH);
                delay(400);
                digitalWrite(4,LOW);*/
                Serial.print("nothin");
                break;
            }
            if(count==1&&StringMass[0]!="$GPRMC"){  //проверяем та ли строка нам приходит
                for(int i=0; i<2;i++){
                    StringMass[i]="";
                }
                while(dataString!="$GPRMC"){ //ждем нужной строки
                    temp = gps.read();
                    if(temp==','){
                        StringMass[count]=dataString;
                        temp=0;
                        dataString="";
                        count++;
                    }   
                    dataString += String(temp);
                }
            }
            dataString += String(temp); // заполняем часть строки
            index++;
            if(index>600){
                  break;
            }
        }
        tone(6, 831);
        if(StringMass[COUNT_OF_TIME]&&StringMass[COUNT_OF_LONGITUDE]&&StringMass[COUNT_OF_LATITUDE]){ //индикатор получения данных
            if(millis()-prevTime>300000){    
                sim.print("\r");
                delay(1000);                    
                sim.print("AT+CMGF=1\r");    
                sim.print("AT+CMGS=\"+79137977081\"\r");
                delay(1000);
                sim.print(StringMass[COUNT_OF_TIME]); //отправка текущего времени и координат в смс сообщении
                sim.print(",");
                sim.print(StringMass[COUNT_OF_LONGITUDE]); 
                sim.print(",");
                sim.print(StringMass[COUNT_OF_LATITUDE]);
                delay(1000); 
                sim.print(26,HEX);
                prevTime=millis();
            }
        }
    }
}




