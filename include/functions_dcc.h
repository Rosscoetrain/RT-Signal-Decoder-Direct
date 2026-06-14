/*
 * @file functions_dcc.h
 */

#ifndef FUNCTIONS_DCC_H
#define FUNCTIONS_DCC_H

#include <EEPROM.h>


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


void notifyDccMsg(DCC_MSG *Msg)
 {
// ignore idle messages and service mode reset

  if (Msg->Data[0] == 0xFF || Msg->Data[0] == 0x7F || Msg->Data[0] == 0x00)
   {
    return;
   }

  #ifdef NOTIFY_DCC_MSG
    MYSERIAL.print("notifyDccMsg: ") ;
    for(uint8_t i = 0; i < Msg->Size; i++)
     {
      MYSERIAL.print(Msg->Data[i], HEX);
      MYSERIAL.write(' ');
     }
    MYSERIAL.println();
  #endif

/*
// 1. Service Mode CV Write (Pattern 0x70)
  if ((Msg->Data[0] & 0xF0) == 0x70)
   {
    uint16_t cv = (((Msg->Data[0] & 0x03) << 8) | Msg->Data[1]) + 1;
    uint8_t val = Msg->Data[2];
    Dcc.setCV(cv, val);
    BaseDecoderAddress = Dcc.getAddr();
   }

// 2. POM (Programming on Main) (Pattern 0xE0)
  if (Msg->Size >= 4)
   {
    if (Msg->Data[0] == BaseDecoderAddress)
     {
      if ((Msg->Data[1] & 0xF0) == 0xE0)
       {
        uint16_t cv = (((Msg->Data[1] & 0x03) << 8) | Msg->Data[2]) + 1;
        uint8_t val = Msg->Data[3];
#ifdef NOTIFY_DCC_MSG
        MYSERIAL.print("CV : ");
        MYSERIAL.print(cv);
        MYSERIAL.print(" value : ");
        MYSERIAL.println(val);
#endif
        if ((cv == 8) && (val == 8))
         {
#ifdef ENABLE_SERIAL
          MYSERIAL.println(F("Reset factory default CVs"));
#endif
          notifyCVResetFactoryDefault();
          return;
         }
        Dcc.setCV(cv, val);
        BaseDecoderAddress = Dcc.getAddr();
       }
     }
   }
*/
 }


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



uint8_t notifyCVWrite (uint16_t CV, uint8_t Value)
 {

#if DEBUG == 6
  MYSERIAL.print("notifyCVWrite cv : ");
  MYSERIAL.print(CV);
  MYSERIAL.print(" value : ");
  MYSERIAL.println(Value);
#endif

  if ((CV == 8) && (Value == 8))  // factory reset return before writing to CV
   {
    notifyCVResetFactoryDefault();
    return Value;
   }
  if (CV == 8)                    // ignore any attempts to write to CV8
   {
    return Value;
   }
  if ((CV == 1) || (CV == 9))     // ignore writes to CV1 and CV9 causes problems with actual address
   {
    return Value;
   }

  if ((CV - CV_BASE_NUMBER) % 10 < 4)  // output pin CV needs to be less that 16 for CVs CV_BASE_NUMBER + 0 to + 3
   {
    if (Value > 15)
     {
      return Value;
     }
   }

  if (Dcc.getCV(CV) != Value)
   {

#if DEBUG == 6
  MYSERIAL.print("Storing value : ");
  MYSERIAL.println(Value);
#endif

    EEPROM.update(CV, Value);
   }
  return Value;
 }


#endif
