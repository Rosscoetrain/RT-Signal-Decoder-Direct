/*
 * functions.h
 */

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

/*
        if (readString.startsWith("<P"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
            Serial.print(F("Value = ")); Serial.println(addr);
#endif

            Dcc.setCV(CV_ACCESSORY_DECODER_WAIT_TIME, addr);

           }
          else
           {
            Serial.println(F("Invalid command: should be <P ms/10>"));
           }
          delete splitter;
          splitter = NULL;
          resetFunc();
         }
*/


/*
        if (readString.startsWith("<R"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if ( itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
            Serial.print(F("Value = ")); Serial.println(addr);
#endif

            Dcc.setCV(CV_ACCESSORY_DECODER_CDU_RECHARGE_TIME, addr);
           }
          else
           {
            Serial.println(F("Invalid command: should be <U ms/10>"));
           }
          delete splitter;
          splitter = NULL;
         }
*/

/*
        if (readString.startsWith("<S"))
         {
          StringSplitter *splitter = new StringSplitter(readString, ' ', 3);  // new StringSplitter(string_to_split, delimiter, limit)
          int itemCount = splitter->getItemCount();

          if (itemCount == 2)
           {
            int addr = splitter->getItemAtIndex(1).toInt();

#ifdef DEBUG_MSG
            Serial.print(F("Value = ")); Serial.println(addr);
#endif

             Dcc.setCV(CV_ACCESSORY_DECODER_ACTIVE_STATE, addr);
           }
          else
           {
            Serial.println(F("Invalid command: should be <S 0> or <S 1>"));
           }
          delete splitter;
          splitter = NULL;
         }
*/


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
//                    Dcc.setCV(addr, value);
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



/*
NMRA functions
originally in the main ino file
*/


//#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs_8x2)/sizeof(CVPair);
};
//#endif

#ifdef ENABLE_DCC_ACK
// This function is called by the NmraDcc library when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain on the power supply for 6ms to ACK a CV Read 
void notifyCVAck(void)
{
  Serial.println("notifyCVAck") ;
  
  digitalWrite( ENABLE_DCC_ACK, HIGH );
  delay( 10 );  
  digitalWrite( ENABLE_DCC_ACK, LOW );
}
#endif


#ifdef NOTIFY_DCC_MSG
void notifyDccMsg( DCC_MSG * Msg)
{
  if (Msg->Size > 3)
   {
    Serial.print("notifyDccMsg: ") ;
   for(uint8_t i = 0; i < Msg->Size; i++)
    {
     Serial.print(Msg->Data[i], HEX);
     Serial.write(' ');
    }
   Serial.println();
   }
}
#endif

/*
// This function is called whenever a normal DCC Turnout Packet is received and we're in Board Addressing Mode
void notifyDccAccTurnoutBoard( uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower )
{
  Serial.print("notifyDccAccTurnoutBoard: ") ;
  Serial.print(BoardAddr,DEC) ;
  Serial.print(',');
  Serial.print(OutputPair,DEC) ;
  Serial.print(',');
  Serial.print(Direction,DEC) ;
  Serial.print(',');
  Serial.println(OutputPower, HEX) ;
}
*/

// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{

#ifdef TESTING_DCC
  showAcknowledge(2);
#endif

#ifdef  NOTIFY_TURNOUT_MSG
  Serial.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  Serial.print(Addr,DEC) ;
  Serial.print(" Direction: ");
  Serial.print(Direction ? "Closed" : "Thrown") ;
  Serial.print(" Output: ");
  Serial.print(OutputPower ? "On" : "Off") ;
#endif

#ifdef LEARNING
// check to see if in learning mode and update address
  if (learningMode == HIGH) {

//    int H = (Addr - 1) / 64;
//    int L = Addr - (H * 64);
    byte L = (Addr + 3) / 4;
    byte H = (Addr + 3) / 1024;

#ifdef DEBUG_MSG
    Serial.println("");
    Serial.print(F("Value = ")); Serial.println(Addr,DEC);
    Serial.print(F(" H = ")); Serial.println(H,DEC);
    Serial.print(F(" L = ")); Serial.println(L,DEC);
#endif
                  
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);

   }
  else
#endif
   {

    if(( Addr >= BaseDecoderAddress ) && ( Addr < (BaseDecoderAddress + MAXACCESSORIES )) && OutputPower )
     {

      thisCommand = ( ( ( Addr - BaseDecoderAddress ) * COMMAND_OFFSET ) + 1 ) + Direction;

      lLights.addCommand(thisCommand);

#ifdef  DEBUG_MSG
      Serial.print("ndato thisCommand: ");
      Serial.println(thisCommand,DEC);
#endif
     }

   }

#ifdef  NOTIFY_TURNOUT_MSG
  Serial.println();
#endif

}


// This function is called whenever a DCC Signal Aspect Packet is received
void notifyDccSigOutputState( uint16_t Addr, uint8_t State)
{

#ifdef NOTIFY_SIGNAL_MSG
  if(State != 18)
   {
    Serial.print("notifyDccSigOutputState: ") ;
    Serial.print(Addr,DEC) ;
    Serial.print(',');
    Serial.println(State, DEC) ;
   }
#endif

  if(( Addr >= BaseDecoderAddress ) && ( Addr < (BaseDecoderAddress + MAXACCESSORIES )))
   {

    thisCommand = ( ( ( Addr - BaseDecoderAddress ) * COMMAND_OFFSET ) + 1 ) + State;

    lLights.addCommand(thisCommand);

#ifdef  DEBUG_MSG
//  if(State != 18)
//   {
    Serial.print("ndsos thisCommand: ");
    Serial.println(thisCommand,DEC);
//   }
#endif
   }

  
}
