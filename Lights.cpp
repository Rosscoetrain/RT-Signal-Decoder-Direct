/*
 * Lights.cpp
 * 
 * A class to control light signal decoder
 * 
 */

#include <NmraDcc.h>

#include "Lights.h"
#include "defines.h"

void Lights::init(NmraDcc Dcc, byte* outputs)
{
  this->state = TT_IDLE;
  memset(this->commandQueue, TT_MOVER_SLOT_EMPTY, TT_MOVER_MAX_TRACKS + 1);

  for (byte i=0; i < MAXACCESSORIES; i++)
   {
#ifdef DEBUG_MSG_LIGHTS
     Serial.print("CV: ");Serial.print(CV_BASE_NUMBER + 0 + i * NUMBER_OF_CVS);
     Serial.print(" value: ");Serial.println(Dcc.getCV(CV_BASE_NUMBER + 0 + i * NUMBER_OF_CVS));

     Serial.print("mode: ");Serial.println(Dcc.getCV(CV_BASE_NUMBER + 0 + i * NUMBER_OF_CVS) >> 4 & 0x0F);
     Serial.print("outputpin: ");Serial.println(Dcc.getCV(CV_BASE_NUMBER + 0 + i * NUMBER_OF_CVS) & 0x0F);
#endif

     accessory[i].mode1 = Dcc.getCV(CV_BASE_NUMBER + 0 + i * NUMBER_OF_CVS) >> 4 & 0xF0;         // outputPin1 mode
     accessory[i].mode2 = Dcc.getCV(CV_BASE_NUMBER + 1 + i * NUMBER_OF_CVS) >> 4 & 0xF0;         // outputPin2 mode
     accessory[i].mode3 = Dcc.getCV(CV_BASE_NUMBER + 2 + i * NUMBER_OF_CVS) >> 4 & 0xF0;         // outputPin3 mode
     accessory[i].mode4 = Dcc.getCV(CV_BASE_NUMBER + 3 + i * NUMBER_OF_CVS) >> 4 & 0xF0;         // outputPin3 mode

     accessory[i].outputPin1 = outputs[int(Dcc.getCV(CV_BASE_NUMBER + 0 + i * NUMBER_OF_CVS) & 0x0F)];    // output pin 1 for signals
     accessory[i].outputPin2 = outputs[int(Dcc.getCV(CV_BASE_NUMBER + 1 + i * NUMBER_OF_CVS) & 0x0F)];    // output pin 2 for signals
     accessory[i].outputPin3 = (Dcc.getCV(CV_BASE_NUMBER + 2 + i * NUMBER_OF_CVS) & 0x0F) ? 0 : outputs[int(Dcc.getCV(CV_BASE_NUMBER + 2 + i * NUMBER_OF_CVS) & 0x0F)];    // output pin 3 for signals
     accessory[i].outputPin4 = (Dcc.getCV(CV_BASE_NUMBER + 3 + i * NUMBER_OF_CVS) & 0x0F) ? 0 : outputs[int(Dcc.getCV(CV_BASE_NUMBER + 3 + i * NUMBER_OF_CVS) & 0x0F)];    // output pin 3 for signals

     accessory[i].ontime = Dcc.getCV(CV_BASE_NUMBER + 4 + i * NUMBER_OF_CVS);        // ontime
     accessory[i].ontimeX = Dcc.getCV(CV_BASE_NUMBER + 5 + i * NUMBER_OF_CVS);       // ontime multiplier
     accessory[i].offtime = Dcc.getCV(CV_BASE_NUMBER + 6 + i * NUMBER_OF_CVS);       // offtime
     accessory[i].offtimeX = Dcc.getCV(CV_BASE_NUMBER + 7 + i * NUMBER_OF_CVS);      // offtime multiplier
     accessory[i].fadein = Dcc.getCV(CV_BASE_NUMBER + 8 + i * NUMBER_OF_CVS);        // fade in time for fader
     accessory[i].fadeout = Dcc.getCV(CV_BASE_NUMBER + 9 + i * NUMBER_OF_CVS);       // fade out time for fader

#ifdef DEBUG_MSG_LIGHTS

     Serial.print("accessory: ");Serial.print(i);Serial.print(" mode1: ");Serial.print(accessory[i].mode1);
     Serial.print(" outputPin1: ");Serial.println(accessory[i].outputPin1);

#endif

     accessory[i].dccstate = 0;                           // Internal use. DCC state of accessory: 1=on, 0=off
     accessory[i].onoff = 0;                              // Internal use. Output state of accessory: 1=on, 0=off
     accessory[i].onMilli = 0;                            // Internal use.
     accessory[i].offMilli = 0;                           // Internal use.

     accessory[i].fade = 0;                               // Internal use. Output state of accessory: 1=on, 0=off

     pinMode(accessory[i].outputPin1, OUTPUT);
     digitalWrite(accessory[i].outputPin1, LOW);
     pinMode(accessory[i].outputPin2, OUTPUT);
     digitalWrite(accessory[i].outputPin2, HIGH);

     if(accessory[i].outputPin3 != 0)
      {
       pinMode(accessory[i].outputPin3, OUTPUT);
       digitalWrite(accessory[i].outputPin3, HIGH);
      }
     if(accessory[i].outputPin4 != 0)
      {
       pinMode(accessory[i].outputPin4, OUTPUT);
       digitalWrite(accessory[i].outputPin4, HIGH);
      }
   }
}


void Lights::addCommand(uint8_t command)
{
  for(uint8_t i = 0; i < TT_MOVER_MAX_TRACKS; i++)
  {
    if(this->commandQueue[i] == command)
    {
      return;
    }

    else if(commandQueue[i] == TT_MOVER_SLOT_EMPTY)
    {
      this->commandQueue[i] = command;
      this->process();
      return;
    }
  }
  return;
}



void Lights::process(void)
{
    if ( this->commandQueue[0] != TT_MOVER_SLOT_EMPTY )
     {
      this->thisCommand = this->commandQueue[0];
      this->state = TT_MOVE;

#ifdef DEBUG_MSG_LIGHTS_1
      Serial.print("Process 1: commandQueue 0 = ");Serial.println(this->commandQueue[0], DEC);
      Serial.print("Process 1: state = ");Serial.println(this->state);
#endif

     }

/*
 * this starts and stops the applicable mode depending on the state of accessory[target].dccstate
 * for each mode there will be a #ifdef ACTION_ #endif section
 * 
 */


    if ( this->state == TT_MOVE )
     {

      this->target = byte( this->thisCommand / 10);
      this->direction = ( ( this->thisCommand ) - ( this->target * 10 ) - 1);

#ifdef DEBUG_MSG_LIGHTS_2
//      Serial.print("thisCommand = ");Serial.println(this->thisCommand);
      Serial.print("Process 2: target = ");Serial.println(this->target);
      Serial.print("Process 2: direction = ");Serial.println(this->direction);
      Serial.println();
      Serial.print("Process 2: mode1 = ");Serial.println(accessory[this->target].mode1);
      Serial.print("Process 2: outputPin1 = ");Serial.println(accessory[this->target].outputPin1);
      Serial.print("Process 2: mode2 = ");Serial.println(accessory[this->target].mode2);
      Serial.print("Process 2: outputPin2 = ");Serial.println(accessory[this->target].outputPin2);
      Serial.println();
#endif


      switch (this->direction)
       {
        case 0:                                                          // red signal
#ifdef DEBUG_MSG_LIGHTS_3
//      Serial.print("thisCommand = ");Serial.println(this->thisCommand);
      Serial.print("Process 3: target = ");Serial.println(this->target);
      Serial.print("Process 3: direction = ");Serial.println(this->direction);
      Serial.println();
#endif
          switch (accessory[this->target].mode1)
           {
            case 0:                                                         // on/off
              digitalWrite(accessory[this->target].outputPin1, LOW);
              digitalWrite(accessory[this->target].outputPin2, HIGH);
              if (accessory[this->target].outputPin3 != 0)
               {
                digitalWrite(accessory[this->target].outputPin3, LOW);
               }
              if (accessory[this->target].outputPin4 != 0)
               {
                digitalWrite(accessory[this->target].outputPin4, LOW);
               }
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
            break;
            case 1:                                                        // one shot
              accessory[this->target].dccstate = 2;
              accessory[this->target].onoff = 1;
            break;
            case 2:                                                        // flashing
              accessory[this->target].dccstate = 3;
              accessory[this->target].onoff = 1;
            break;
           }
        break;


        case 1:                                                            // green signal
          switch (accessory[this->target].mode1)
          {
           case 0:
             digitalWrite(accessory[this->target].outputPin1, HIGH);
             digitalWrite(accessory[this->target].outputPin2, LOW);
              if (accessory[this->target].outputPin3 != 0)
               {
                digitalWrite(accessory[this->target].outputPin3, LOW);
               }
              if (accessory[this->target].outputPin4 != 0)
               {
                digitalWrite(accessory[this->target].outputPin4, LOW);
               }
             accessory[this->target].dccstate = 1;
             accessory[this->target].onoff = 1;
             this->state = TT_STOP;
           break;
          }
        break;


        case 2:                                                             // amber signal
             digitalWrite(accessory[this->target].outputPin1, LOW);
             digitalWrite(accessory[this->target].outputPin2, LOW);
              if (accessory[this->target].outputPin3 != 0)
               {
                digitalWrite(accessory[this->target].outputPin3, HIGH);
               }
              if (accessory[this->target].outputPin4 != 0)
               {
                digitalWrite(accessory[this->target].outputPin4, LOW);
               }
             accessory[this->target].dccstate = 1;
             accessory[this->target].onoff = 1;
             this->state = TT_STOP;
        break;


        case 3:                                                             // aspect amber amber
             digitalWrite(accessory[this->target].outputPin1, LOW);
             digitalWrite(accessory[this->target].outputPin2, LOW);
              if (accessory[this->target].outputPin3 != 0)
               {
                digitalWrite(accessory[this->target].outputPin3, HIGH);
               }
              if (accessory[this->target].outputPin4 != 0)
               {
                digitalWrite(accessory[this->target].outputPin4, HIGH);
               }
             accessory[this->target].dccstate = 1;
             accessory[this->target].onoff = 1;
             this->state = TT_STOP;
        break;

        case 4:                                                  // aspect 4
        break;

        case 5:                                                  // aspect 5
        break;

        case 6:                                                  // aspect 6
        break;

        default:
          this->state = TT_STOP;
        break;
       }
     }

/*
 * this alters the output depending on mode, time and dccstate
 * 
 * 
 */


    for ( int i = 0; i < MAXACCESSORIES; i++)
     {
      if (accessory[i].dccstate > 1)
       {


#ifdef DEBUG_MSG_LIGHTS
        Serial.print("onMilli = ");Serial.println(accessory[i].onMilli);
        Serial.print("offMilli = ");Serial.println(accessory[i].offMilli);
        Serial.print("millis = ");Serial.println(millis());
#endif
       }
     }








/*
 * change state to TT_IDLE and move next command up in the queue
 */

    if (this->state == TT_STOP)
     {
      memmove(this->commandQueue, this->commandQueue + 1, TT_MOVER_MAX_TRACKS);
      this->state = TT_IDLE;
     }

}





