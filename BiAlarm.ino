#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define COUNT_OF_LATITUDE 3
#define COUNT_OF_LONGITUDE 5
#define COUNT_OF_TIME  1
const int chipSelect = 10;
SoftwareSerial ss(3,2);
String StringMass[12];
void setup()
{
    pinMode(13, OUTPUT);
    Serial.begin(38400);   
    ss.begin(38400);
    pinMode(10, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
}

void loop()
{
    char index = 0;
    char temp = 0;
    String dataString = "";
    int count=0;
    bool trigger = false;
    while(ss.available()) //в этой части кода мы читаем то, что отправляет нам GPS модуль
    {    

        temp = ss.read();
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
                temp = ss.read();
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
    if(StringMass[COUNT_OF_TIME]&&StringMass[COUNT_OF_LONGITUDE]&&StringMass[COUNT_OF_LATITUDE]){ //индикатор получения данных
        digitalWrite(5, HIGH);
        delay(400);
        digitalWrite(5,LOW);
    }
    Serial.print(StringMass[COUNT_OF_TIME]); //вывод данных в файл на SD карте
    Serial.print(",");
    Serial.print(StringMass[COUNT_OF_LONGITUDE]); 
    Serial.print(",");
    Serial.print(StringMass[COUNT_OF_LATITUDE]);
    Serial.print("\n");
    delay(100);


}


