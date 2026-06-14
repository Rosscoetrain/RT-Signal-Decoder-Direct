/*
 * @file functions.h
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "defines.h"

#ifdef LEARNING
bool dr (int pin)
 {
  int val = analogRead(pin);
  return ( val > 512 ) ;
 }
#endif

/*
 * this is a function to show via the onboard PCB led, the state of the decoder
 */

#ifdef TESTING
void showAcknowledge(int nb) {
  for (int i=0;i<nb;i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500);               // wait for a second
  }
}
#endif

/*
 * 
 */

/*
 * setup the version number
 */


void setVersion() {
  const String versionString = VERSION;
  char versionArray[versionString.length() + 1];
  versionString.toCharArray(versionArray, versionString.length() + 1);
  version = strtok(versionArray, "."); // Split version on .
  versionBuffer[0] = atoi(version);  // Major first
  version = strtok(NULL, ".");
  versionBuffer[1] = atoi(version);  // Minor next
  version = strtok(NULL, ".");
  versionBuffer[2] = atoi(version);  // Patch last
}


void showUserCVs()
 {
  int CV = 0;
  for (int i = 0; i < MAXACCESSORIES; i++)
   {
    for (int j = 0; j <= (NUMBER_OF_CVS - 1); j++)
     {
      CV = (CV_BASE_NUMBER + j) + (i * NUMBER_OF_CVS);
      Serial.print("CV ");
      Serial.print(CV);
      Serial.print(F(" = "));
      Serial.print(Dcc.getCV(CV));
      Serial.print("    ");
     }
    Serial.println();
   }
 }



#ifdef ENABLE_SERIAL

/*
 * soft reset function
 */

void(* resetFunc) (void) = 0;//declare reset function at address 0

/*
 * initialize the sensors
 */


/*
 * process serial commands
 */

#include "StringSplitter.h"

void doSerialCommand(String readString)
 {
//  byte p = 0;

  readString.trim();

  Serial.println(readString);  //so you can see the captured string

  if (readString == "<Z>")
   {

    Serial.println(F("Resetting"));

    resetFunc();
   }

  if (readString == "<D>")
   {
    Serial.println(F("Reset factory default CVs"));

    notifyCVResetFactoryDefault();
   }

  if (readString == "<?>")
   {
    Serial.println(F("Help Text"));

/*
    Serial.println(F("Close a turnout: <C address>"));
    Serial.println(F("Throw a turnout: <T address>"));
*/

    Serial.println(F("Set decoder base address: <A address>"));

    Serial.println(F("Show current CVs: <>"));
                     
    Serial.println(F("Soft Reset: <Z>"));

   }
  else
   {
    if (readString.startsWith("<>"))
     {


      Serial.println(F("CVs are:"));
      Serial.print(F("CV "));
      Serial.print(CV_ACCESSORY_DECODER_ADDRESS_LSB);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB));
      Serial.print(F("CV "));
      Serial.print(CV_ACCESSORY_DECODER_ADDRESS_MSB);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB));
/*      
      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_WAIT_TIME);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_WAIT_TIME));
*/
      Serial.print(F("CV "));
      Serial.print(CV_MANUFACTURER_ID);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_MANUFACTURER_ID));


      Serial.print(F("CV "));
      Serial.print(CV_29_CONFIG);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_29_CONFIG));

      Serial.print(F("CV "));
      Serial.print(CV_DECODER_MODE);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_DECODER_MODE));
      
      showUserCVs();
/*
      int CV = 0;
      for (int i = 0; i < MAXACCESSORIES; i++)
       {
        for (int j = 0; j <= (NUMBER_OF_CVS - 1); j++)
         {
          CV = (CV_BASE_NUMBER + j) + (i * NUMBER_OF_CVS);
          Serial.print("CV ");
          Serial.print(CV);
          Serial.print(F(" = "));
          Serial.println(Dcc.getCV(CV));
          
         }
       }
*/

/*
      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME));

      Serial.print(F("CV"));
      Serial.print(CV_ACCESSORY_DECODER_ACTIVE_STATE);
      Serial.print(F(" = "));
      Serial.println(Dcc.getCV(CV_ACCESSORY_DECODER_ACTIVE_STATE));
*/

     }
    else
     {
      if (readString.startsWith("<"))
       {
//        int pos = 0;
        // this is where commands are completed

        if (readString.startsWith("<U>"))
         {
          showUserCVs();
          readString = "";
         }

        // command to turn on a light circuit <C address>

        if (readString.startsWith("<C"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();


          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            notifyDccAccTurnoutOutput( addr, 1, 1 );

           }
          else
           {
            Serial.println(F("Invalid command: should be <C address>"));
           }
          delete splitter;
          splitter = NULL;
         }


         // command to turn off a light circuit <T address>

        if (readString.startsWith("<T"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            notifyDccAccTurnoutOutput( addr, 0, 1 );

           }
          else
           {
            Serial.println(F("Invalid command: should be <T address>"));
           }
          delete splitter;
          splitter = NULL;
         }

        if (readString.startsWith("<A>"))
         {
          Serial.println(BaseDecoderAddress);
          readString = "";
         }


         // command to set address <A address>
         // address will be adjusted to the correct base turnout address
         // eg if address is 2 this will be corrected to 1 as the address are groups of 8 with an offset of 4
         // ie 1..8, 5..12, ...

        if (readString.startsWith("<A"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

            byte L = (addr + 3) / 4;
            byte H = (addr + 3) / 1024;

#ifdef DEBUG_MSG
            Serial.print(F("Value = ")); Serial.println(addr);
            Serial.print(F(" H = ")); Serial.println(H);
            Serial.print(F(" L = ")); Serial.println(L);
#endif


            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
            Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);


           }
          else
           {
            Serial.println(F("Invalid command: should be <A address>"));
           }
          delete splitter;
          splitter = NULL;
         }

        if (
// update outputPin1 CV
           (readString.startsWith("<M"))
// update outputPin2 CV
        || (readString.startsWith("<N"))
// update outputPin3 CV
        || (readString.startsWith("<O"))
// update outputPin4 CV
        || (readString.startsWith("<P"))
// update ontime CV
        || (readString.startsWith("<Q"))
// update ontimeX CV
        || (readString.startsWith("<R"))
// update offtime CV
        || (readString.startsWith("<S"))
// update offtimeX CV
        || (readString.startsWith("<T"))
// update fadein CV
        || (readString.startsWith("<U"))
// update fadeout CV
        || (readString.startsWith("<V")))

         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 3)
           {
            byte addr = splitter->getItemAtIndex(1).toInt();
            int value = splitter->getItemAtIndex(2).toInt();

#ifdef DEBUG_MSG_1
            Serial.print(F("addr = ")); Serial.println(addr);
            Serial.print(F("value = ")); Serial.println(value);
#endif

            Dcc.setCV(addr, value);

           }
          else
           {
            Serial.println(F("Invalid command: should be <cmd addr value>"));
           }
          delete splitter;
          splitter = NULL;
         }





        if (readString.startsWith("<W"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 3)
           {
            byte addr = splitter->getItemAtIndex(1).toInt();
            int value = splitter->getItemAtIndex(2).toInt();

            switch (addr) {
/*
              case CV_ACCESSORY_DECODER_ADDRESS_LSB:                  // CV1

                    byte L = (value + 3) / 4;
                    byte H = (value + 3) / 1024;

#ifdef DEBUG_MSG
                  Serial.print(F("Value = ")); Serial.println(value);
                  Serial.print(F(" H = ")); Serial.println(H);
                  Serial.print(F(" L = ")); Serial.println(L);
#endif

                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);
              break;
              case CV_ACCESSORY_DECODER_ADDRESS_MSB:                  // CV9
                  Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, value);
              break;
*/
              case 8:
                if (value == 8)
                 {
//                  Dcc.setCV(addr, value);
                     notifyCVResetFactoryDefault();
                 }
              break;

              case 33:
                  if((value >= 0) && (value <=2))
                   {
                    Serial.print(addr);Serial.print(" = ");Serial.println(value);
                    Dcc.setCV(addr, value);
                   }
                  else
                   {
                    Serial.println("Invalid mode");
                   }
              break;

              case CV_BASE_NUMBER ... CV_BASE_NUMBER + (MAXACCESSORIES * NUMBER_OF_CVS):
                  if((value >= 0) && (value <= 255))
                   {
                    Serial.print(addr);Serial.print(" = ");Serial.println(value);
                    Dcc.setCV(addr, value);
                   }
                  else
                   {
                    Serial.println("Invalid value");
                   }
              break;


              default:

                 Serial.println(F("Invalid cv number: should be <W cv value> "));
              break;
             }
           }
          else
           {
            Serial.println(F("Invalid command: should be <W cv value>"));
           }
          delete splitter;
          splitter = NULL;
         }


       }
      else
       {
        Serial.println(F("ERROR: Unknown command"));
       }
     }
   }
 }
#endif



uint8_t hbval = 128;
int8_t hbdelta = 8;
void heartbeat(byte pin) {
  static unsigned long last_time = 0;
  unsigned long now = millis();
  if ((now - last_time) < 40) {
    return;
  }
  last_time = now;
  if (hbval > 192) {
    hbdelta = -hbdelta;
  }
  if (hbval < 32) {
    hbdelta = -hbdelta;
  }
  hbval += hbdelta;
  analogWrite(pin, hbval);
}



#endif
