/*
 * This is an accessory decoder to control coloured light signals.
 * 
 * 
 * 
 */

#include <NmraDcc.h>
#include "version.h"
#include "defines.h"

#include "Lights.h"

#include "variables.h"


#include "functions.h"



/**********************************************************************
 * setup
 */

void setup()
{
  pinMode(2, INPUT);

  setVersion();

#ifdef ENABLE_SERIAL
  Serial.begin(115200);
  uint8_t maxWaitLoops = 255;
  while(!Serial && maxWaitLoops--)
    delay(20);
#endif

  // Configure the DCC CV Programing ACK pin for an output
#ifdef ENABLE_DCC_ACK
  pinMode( DccAckPin, OUTPUT );
#endif

  lLights.init(Dcc);

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion. 

#ifdef digitalPinToInterrupt
  Dcc.pin(DCC_PIN, 0);
#else
  Dcc.pin(0, DCC_PIN, 1);
#endif

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, DCC_DECODER_VERSION_NUM, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
#ifdef ENABLE_SERIAL
  Serial.println("Resetting CVs to Factory Defaults");
#endif
  notifyCVResetFactoryDefault(); 
#endif

  BaseDecoderAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;

#ifdef DEBUG_MSG
  Serial.print("DCC Base Address: "); Serial.println(BaseDecoderAddress, DEC);
#endif

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(600);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(600);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(600);
  digitalWrite(LED_BUILTIN, LOW);

#ifdef ENABLE_SERIAL
//  Serial.println("NMRA DCC Example 1");
  Serial.print("Rosscoe Train Coloured Light Signal Decoder ");
  Serial.print(F("Version: "));
  Serial.print(versionBuffer[0]);
  Serial.print(F("."));
  Serial.print(versionBuffer[1]);
  Serial.print(F("."));
  Serial.println(versionBuffer[2]);

  Serial.print("base DCC address: ");
  Serial.println(BaseDecoderAddress, DEC);


  Serial.println("Init Done");
#endif
}

void loop()
{
#ifdef LEARNING
  static int learningbuttonOldval = 0,learningbuttonVal = 0;
#endif
  
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();

  lLights.process();
  
#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
  if( FactoryDefaultCVIndex && Dcc.isSetCVReady())
  {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array 
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
  }
#endif

#ifdef LEARNING
  learningbuttonVal = dr(LEARNINGBUTTON);

  if (learningbuttonOldval != learningbuttonVal) {
    learningMode = learningbuttonVal;
    if (learningMode == HIGH) showAcknowledge(3);
   }
  learningbuttonOldval = learningbuttonVal;
#endif

#ifdef ENABLE_SERIAL
    // see if there are serial commands
  readString="";              //empty for next input

  while (Serial.available())
   {
    char c = Serial.read();     //gets one byte from serial buffer
    readString += c;            //makes the string readString
    delay(10);                   //slow looping to allow buffer to fill with next character
   }

  // act on serial commands

  if (readString.length() >0)
   {
    doSerialCommand(readString);
   } 
#endif

}




  
