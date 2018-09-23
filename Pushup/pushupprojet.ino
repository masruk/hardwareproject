byte seven_seg_digits[10][4]={
                  {0,0,0,0},
                  {0,0,0,1},
                  {0,0,1,0},
                  {0,0,1,1},
                  {0,1,0,0},
                  {0,1,0,1},
                  {0,1,1,0},
                  {0,1,1,1},
                  {1,0,0,0},
                  {1,0,0,1}
                };

void setup() {             
  pinMode(2, OUTPUT);   
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,INPUT);
  pinMode(15,INPUT);
  pinMode(16,INPUT);
  pinMode(17,INPUT);
  //pinMode(18,INPUT);
  pinMode(19,INPUT);

 

}

    

void sevenSegWrite(byte digit,byte pin) {
  for (byte segCount = 0; segCount < 4; ++segCount) {
    digitalWrite(pin,seven_seg_digits[digit][segCount]);
    ++pin;
  }

}



void loop() {
 
  byte count=0;
  int flag=0;
  sevenSegWrite(0,2);
  sevenSegWrite(0,6);

  while(1){

    if(digitalRead(16)==HIGH && (flag==0 || flag==2))
    {
       digitalWrite(10,HIGH);
        if(flag==0)
        {
          flag=1;
        }
        else if(flag==2)
        {
          flag=3;
        }
      
    }

    if(digitalRead(17)==HIGH && (flag==0 || flag==1))
    {
        digitalWrite(11,HIGH);
        if(flag==0)
        {
          flag=2;
        }
        else if(flag==1)
        {
          flag=3;
        }
    }

    if(digitalRead(15)==HIGH && (flag==3 || flag==4))
    {
      digitalWrite(12,HIGH);
        if(flag==3)
        {
          flag=5;
        }
        else if(flag==4)
        {
          flag=0;
          count++;
          if(count==100)
          {
            count=0;
          }
          sevenSegWrite(count/10,2);
          sevenSegWrite(count%10,6);
          
          delay(500);
          digitalWrite(10,LOW);
          digitalWrite(11,LOW);
          digitalWrite(12,LOW);
          digitalWrite(13,LOW);
          
         // digitalWrite(15,LOW);
        }
    }

    if(digitalRead(19)==HIGH && (flag==3 || flag==5))
    {
        digitalWrite(13,HIGH);
        if(flag==3)
        {
          flag=4;
        }
        else if(flag==5)
        {
          flag=0;
          count++;
          if(count==100)
          {
            count=0;
          } 
          sevenSegWrite(count/10,2);
          sevenSegWrite(count%10,6);
          delay(500);
          digitalWrite(10,LOW);
          digitalWrite(11,LOW);
          digitalWrite(12,LOW);
          digitalWrite(13,LOW);
  
        }
    }
    if(digitalRead(15)==HIGH && (flag==1 || flag==2))
    {
          digitalWrite(10,LOW);
          digitalWrite(11,LOW);
          digitalWrite(12,LOW);
          digitalWrite(13,LOW);
          flag=0;
      
    }
    if(digitalRead(19)==HIGH && (flag==1 || flag==2))
    {
          digitalWrite(10,LOW);
          digitalWrite(11,LOW);
          digitalWrite(12,LOW);
          digitalWrite(13,LOW);
          flag=0;
      
    }

    if(digitalRead(14)==HIGH)
    {
          digitalWrite(10,LOW);
          digitalWrite(11,LOW);
          digitalWrite(12,LOW);
          digitalWrite(13,LOW);
          count=0;
          sevenSegWrite(count/10,2);
          sevenSegWrite(count%10,6);
          
        
    }

  
  }

}
