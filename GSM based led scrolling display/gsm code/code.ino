
#include <SPI.h>
#include <bitBangedSPI.h>
#include <MAX7219_Dot_Matrix.h>
const byte chips = 8;
#include <GSM.h>
#define PINNUMBER ""
GSM gsmAccess;
GSM_SMS sms;
char senderNumber[20];

MAX7219_Dot_Matrix display (chips, 10);  // Chips / LOAD 


  char c;
  String in;
 char smsData[200];
  byte smsIndex = 0;
void setup ()
  {
     Serial.begin(9600);
  display.begin ();
  display.setIntensity (0);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("SMS Messages Receiver");
  // connection state
  boolean notConnected = true;

  // Start GSM connection
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
     // delay(1000);
    }
  }
  
  Serial.println("GSM initialized");
  Serial.println("Waiting for messages");
  }  // end of setup

unsigned long lastMoved = 0;
unsigned long MOVE_INTERVAL = 40;  // mS
int  messageOffset;



void loop () 
  {
    
     if (sms.available()) {
    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    if (sms.peek() == '#') {
      Serial.println("Discarded SMS");
      sms.flush();
    }
   smsIndex = 0;
       while ( c = sms.read()) {
//      Serial.print(c);
   smsData[smsIndex++] = c;
      smsData[smsIndex] = '\0';
      //display.sendString (& in[0]);
       //delay(1000); 
    //  Serial.print(in);
   
    }
    in = smsData;
  Serial.print(smsData);
  
      Serial.println("\nEND OF MESSAGE");

    // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");
  }
  if (millis () - lastMoved >= MOVE_INTERVAL)
    {
display.sendSmooth (smsData, messageOffset);
  
  // next time show one pixel onwards
 if (messageOffset++ >= (int) (strlen (smsData) * 8))
    messageOffset = - chips * 8;
    lastMoved = millis ();
    }
  //delay(100); 
  //sms.flush();
  }  // end of loop
  
  

