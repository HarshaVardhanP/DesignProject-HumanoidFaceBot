#include <Servo.h>

Servo myservo;
int pos=0;

void setup()
{
    Serial.begin(9600);
    myservo.attach(9);
      Serial.println(100);
}

int input,num;
int buff[5];
int j=-1;
 int flag=0;
 int flag2=0;
int calc()
{
    int num=0,x=0;
 
    for(x;x<=j;x++)
          num=num+(buff[x]-48)*pow(10,j-x);
 
    return num;
}
 

 
void loop()
{
     
    if(Serial.available()>0)
    {
        input=Serial.read();
 
        if(input==',')
        {
            num=calc();
            j=-1;
            
            Serial.println(num);
            //if (num==0) Serial.println("Blinked");
            
            if (num<=1 && flag==0) flag=flag+1;
            if(num>=1) flag=0;            
            if (num<=1 && flag>=1) flag=flag+1;
            
            if (flag == 4) {
              
              flag2=1;
              for(pos = 0; pos < 90; pos += 1)  // goes from 0 degrees to 180 degrees 
              {                                  // in steps of 1 degree 
                   myservo.write(pos);              // tell servo to go to position in variable 'pos' 
                   delay(15);                       // waits 15ms for the servo to reach the position 
              } 
              
            //Serial.println("Blinked");  
            //Serial.println("flag");  
            //Serial.println(flag);    
            }
            if (flag ==0 && flag2 ==1){
            for(pos = 90; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
            {                                
                   myservo.write(pos);              // tell servo to go to position in variable 'pos' 
                   delay(15);                       // waits 15ms for the servo to reach the position 
             } 
             flag2=0;
            }
        }
        else
        {
            j++;
            buff[j]=input;
        }
    }
}
