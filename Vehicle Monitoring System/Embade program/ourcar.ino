#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
int  res,level2,level;

String url,data,req;
String msg="",val="";
int flag=0;
int cleartosendmsg=0;

const int soundSensor = A0;
const int horn=11;

char ss,sps,v;
String maxspeed="",maxsound="";

String sflat,sflon,sspd;
double sdb;

String _buffer;
int _timeout;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);

void setup()
{
  
  Serial.begin(9600);
  pinMode(horn,INPUT);
  pinMode(A1, INPUT); 
  level=analogRead(A1); 
   
}


void loop()
{

     delay(3500);
      level2=analogRead(A1);
      res=level-level2;
      level=level2;  
    //  Serial.println(res); 
      if(res>=45)
       {
          Serial.println("Over rate");    
       }
       

//    usegps();
//
//    if(flag==1)
//    {
//       sendData(sflat,sflon,sspd,sdb);
//       flag=0;
//    }
    
}

void usegps()
{
   
   SoftwareSerial mySerial(10, 9);
   mySerial.begin(9600);
  
    bool newdata = false;
    unsigned long start = millis();
    // Every 5 seconds we print an update
     
    while (millis() - start < 100) 
    {
      if (mySerial.available()) 
      
      {
        char c = mySerial.read();
        //Serial.print(c);  
        if (gps.encode(c)) 
        {
          newdata = true;
          break;  
        }
      }
    }
 // Serial.println("in usegps");
  if (newdata) 
  {
    Serial.println("Acquired Data");
    Serial.println("-------------");
    gpsdump(gps);
    Serial.println("-------------");
    Serial.println();
  }
}


void gpsdump(TinyGPS &gps)
{
  
  long sum = 0;
  long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;

  gps.get_position(&lat, &lon, &age);

  gps.f_get_position(&flat, &flon, &age);
  Serial.print("Lat/Long(float): "); printFloat(flat, 5); sflat=val; Serial.print(", "); printFloat(flon, 5); sflon=val;

  gps.get_datetime(&date, &time, &age);

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  Serial.println();
  Serial.print("Date: ");  
    Serial.print(static_cast<int>(day)); Serial.print("/");Serial.print(static_cast<int>(month)); Serial.print("/"); Serial.println(year);
  Serial.print("Time: "); Serial.print(static_cast<int>(hour+6)%24);  Serial.print(":"); //Serial.print("UTC +08:00 Malaysia");
    Serial.println(static_cast<int>(minute));// Serial.print(":"); Serial.print(static_cast<int>(second));

  float spd= gps.f_speed_kmph();
  Serial.print("(kmph): ");  printFloat(spd); Serial.println(); sspd=val;

  for(int i=0; i<32; i++)
    {
        sum += analogRead(soundSensor);
    }

    sum = sum/32;

    double db =  20.0*log10(sum  +1.);
      
      if(digitalRead(horn)==HIGH)
      {
         Serial.print("Sound: ");
         Serial.println(db);
      }
      else{
        db=0;
      }
      sdb=db;
   // sendData(sflat,sflon,sspd,db);
   flag=1;
}

void printFloat(double number, int digits)
{
  val="";
  if (number < 0.0) 
  {
     Serial.print('-');
     number = -number;
     val.concat('-');
  }

  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);
  val.concat(int_part);
  
  if (digits > 0)
   {
      Serial.print("."); 
      val.concat('.');
   }

  while (digits-- > 0) 
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    val.concat(toPrint);
    remainder -= toPrint;
  }

}

void sendData( String lat,String lon,String spd,double sound)
{

    SoftwareSerial gprsSerial(2, 3);
    gprsSerial.begin(9600);
    Serial.println("Config SIM808...");
    delay(2000);
    Serial.println("Done!...");
    gprsSerial.flush();
    Serial.flush();
  
    // attach or detach from GPRS service 
     Serial.println("attach or detach from GPRS service ");
    gprsSerial.println("AT+CGATT?");
    delay(100);
    toSerial( gprsSerial);
      
      Serial.println("Set the connection type to GPRS=OK");
    
    gprsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    delay(2000);
   toSerial( gprsSerial);
   
        Serial.println("APN");
    gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"GP-INTERNET\"");
    delay(2000);
    toSerial( gprsSerial);
     
      //Enable the GPRS
      Serial.println("Enable the GPRS"); 
    gprsSerial.println("AT+SAPBR=1,1");
    delay(2000);
    toSerial( gprsSerial);
    
    // initialize http service
    Serial.println("initialize http service"); 
   gprsSerial.println("AT+HTTPINIT");
   delay(1000); 
   toSerial( gprsSerial);
    
   Serial.println(" set http param value"); 
   // set http param value
   url=String("AT+HTTPPARA=\"URL\",\"http://shaila.stereoserver.com/vehicle/interface.php?");
   data=String("lat=")+lat+String("&lon=")+lon+String("&speed=")+spd+String("&sound=")+sound+String("\"");
   req=url+data;
   gprsSerial.println(req); 
  // gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://shaila.stereoserver.com/vehicle/justecho.php\""); 

   delay(1000);
   toSerial( gprsSerial);
   
   Serial.println("set http action type 0 = GET"); 
   gprsSerial.println("AT+HTTPACTION=0");
   delay(6000);
   toSerial( gprsSerial);

   Serial.println(" // read server response"); 
   // read server response
   gprsSerial.println("AT+HTTPREAD"); 
   delay(1000);
   toSerial( gprsSerial);
 
   Serial.println(" // read server response"); 
   gprsSerial.println("");
   gprsSerial.println("AT+HTTPTERM");
   toSerial( gprsSerial);
 
   delay(300);

   gprsSerial.println("");
   //delay(1000);

   if(cleartosendmsg==1)
   {

       //SoftwareSerial sim(2, 3);
        String number = "+8801701860875";
       _buffer.reserve(50);
       // sim.begin(9600);
       delay(1000);
      
      //Serial.println ("Sending Message");
      gprsSerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);
      //Serial.println ("Set SMS Number");
      gprsSerial.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
      delay(1000);
     // String SMS = "i'm alive";
      gprsSerial.println(msg);
      delay(100);
      gprsSerial.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      cleartosendmsg=0;
     // _buffer = _readSerial(gprsSerial);
    }
 
 
  
}

//   String _readSerial(SoftwareSerial sim) {
//      _timeout = 0;
//      while  (!sim.available() && _timeout < 12000  )
//      {
//        delay(13);
//        _timeout++;
//      }
//      if (sim.available()) {
//        return sim.readString();
//      }
//   }

//void toSerial( SoftwareSerial &gprsSerial)
//{
//  
//  while(gprsSerial.available()!=0)
//  {
//    Serial.write(gprsSerial.read());
//  }
//}


void toSerial(SoftwareSerial &gprsSerial)
{
  char a[2];
  int sign=0;
  
  while(gprsSerial.available()!=0)
  {
     //Serial.write(gprsSerial.read());
     v=gprsSerial.read();
     if(v == '[')
     {
         msg="";
          sps=gprsSerial.read();
        //  Serial.println(sps);
          if(sps=='0')
          {
              v=gprsSerial.read();
              v=gprsSerial.read(); 
              while(v!= ',')
              {
                 maxspeed.concat(v);
                 v= gprsSerial.read();
                
              }
              msg.concat("You have crossed the speed limit. Max speed ");
              msg.concat(maxspeed);
              sign=1;
              cleartosendmsg=1;
           //  Serial.println(maxspeed);
          }
          ss=gprsSerial.read();
          if(ss=='0')
          {
              v=gprsSerial.read();
              v=gprsSerial.read();
              while(v!= ']')
              {
                 maxsound.concat(v);
                 v= gprsSerial.read();
              }
              if(sign==1)
               {
                    msg.concat(".");
               }
              msg.concat("You have crossed the sound limit. Max sound ");
              msg.concat(maxsound);
              cleartosendmsg=1;
          }
          Serial.print(maxsound);
          Serial.print("  ");
          Serial.println(maxspeed);
         
     }
     else{
          Serial.print(v);
     }

       
  }

  Serial.println("");
}

//void SendMessage()
//{
//      SoftwareSerial sim(2, 3);
//      String number = "+8801751704358";
//       _buffer.reserve(50);
//        sim.begin(9600);
//       delay(1000);
//      
//      //Serial.println ("Sending Message");
//      sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//      delay(1000);
//      //Serial.println ("Set SMS Number");
//      sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
//      delay(1000);
//     // String SMS = "i'm alive";
//      sim.println(msg);
//      delay(100);
//      sim.println((char)26);// ASCII code of CTRL+Z
//      delay(1000);
//      _buffer = _readSerial(sim);
//    }
//    String _readSerial(SoftwareSerial sim) {
//      _timeout = 0;
//      while  (!sim.available() && _timeout < 12000  )
//      {
//        delay(13);
//        _timeout++;
//      }
//      if (sim.available()) {
//        return sim.readString();
//      }
//}


//void toSerial(SoftwareSerial &gprsSerial)
//{
//  while(gprsSerial.available()!=0) {
//    
//        Serial.write(gprsSerial.read());
//  }
//
//  
//}
//

