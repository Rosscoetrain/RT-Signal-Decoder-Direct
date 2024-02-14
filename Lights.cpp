/*
 * Lights.cpp
 * 
 * A class to control light signal decoder
 * 
 */

#include <NmraDcc.h>

#include "Lights.h"
#include "defines.h"

void Lights::init(NmraDcc Dcc)
{
  this->state = TT_IDLE;
  memset(this->commandQueue, TT_MOVER_SLOT_EMPTY, TT_MOVER_MAX_TRACKS + 1);

  for (byte i=0; i < MAXACCESSORIES; i++)
   {
     accessory[i].mode1 = Dcc.getCV(33 + i * NUMBER_OF_CVS);         // outputPin1 mode
     accessory[i].mode2 = Dcc.getCV(34 + i * NUMBER_OF_CVS);         // outputPin2 mode
     accessory[i].mode2 = Dcc.getCV(35 + i * NUMBER_OF_CVS);         // outputPin3 mode

     accessory[i].ontime = Dcc.getCV(36 + i * NUMBER_OF_CVS);        // ontime
     accessory[i].ontimeX = Dcc.getCV(37 + i * NUMBER_OF_CVS);       // ontime multiplier
     accessory[i].offtime = Dcc.getCV(38 + i * NUMBER_OF_CVS);       // offtime
     accessory[i].offtimeX = Dcc.getCV(39 + i * NUMBER_OF_CVS);      // offtime multiplier

     accessory[i].outputPin1 = Dcc.getCV(40 + i * NUMBER_OF_CVS);    // output pin 1 for signals
     accessory[i].outputPin2 = Dcc.getCV(41 + i * NUMBER_OF_CVS);    // output pin 2 for signals
     accessory[i].outputPin3 = Dcc.getCV(42 + i * NUMBER_OF_CVS);    // output pin 3 for signals

     accessory[i].fadein = Dcc.getCV(43 + i * NUMBER_OF_CVS);        // fade in time for fader
     accessory[i].fadeout = Dcc.getCV(44 + i * NUMBER_OF_CVS);       // fade out time for fader

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

#ifdef DEBUG_MSG_LIGHTS
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

#ifdef DEBUG_MSG_LIGHTS
//      Serial.print("thisCommand = ");Serial.println(this->thisCommand);
      Serial.print("Process 2: target = ");Serial.println(this->target);
      Serial.print("Process 2: direction = ");Serial.println(this->direction);
      Serial.print("Process 2: outputPin1 = ");Serial.println(accessory[this->target].outputPin1);
      Serial.print("Process 2: outputPin2 = ");Serial.println(accessory[this->target].outputPin2);
      Serial.println();
#endif

      if ( !accessory[this->target].dccstate && this->direction)         // if turned off and direction turn on
       {
        switch (accessory[this->target].mode1)
         {
#ifdef ACTION_ONOFF
          case 0:                                                        // on/off

            digitalWrite(accessory[this->target].outputPin1, HIGH);
            digitalWrite(accessory[this->target].outputPin2, LOW);

// outputPin3 details here

            accessory[this->target].dccstate = 1;
            accessory[this->target].onoff = 1;

            this->state = TT_STOP;
          break;
#endif

/*
#ifdef ACTION_ONESHOT
          case 1:                                                        // oneshot
            accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
            digitalWrite(accessory[this->target].outputPin1, HIGH);
            accessory[this->target].dccstate = 1;
            accessory[this->target].onoff = 1;
            this->state = TT_STOP;
          break;
#endif
#ifdef ACTION_FLASH
          case 2:                                                        // flashing
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
          break;
#endif



#ifdef ACTION_FLASHFADER
          case 3:                                                        // flashing fade turn on
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);

              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;

              accessory[this->target].fade = 0;
              analogWrite(accessory[this->target].outputPin1, accessory[this->target].fade);

              this->state = TT_STOP;
             }
          break;
#endif



#ifdef ACTION_FLASHALTERNATE
          case 4:
            if (!accessory[this->target].dccstate)                     // flash alternate turn on
             {
              accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              digitalWrite(accessory[this->target].outputPin2, LOW);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
//              accessory[this->target].onoff2 = 0;
              this->state = TT_STOP;
             }
          break;
#endif

#ifdef ACTION_STROBEDOUBLE
          case 5:                                                        // strobe double turn on
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
              this->strobe(accessory[this->target].outputPin1);
              delay(100);
              this->strobe(accessory[this->target].outputPin1);

              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
          break;
#endif

#ifdef ACTION_RANDOMBRIGHT
          case 6:                                                        // random bright turn on  changes random brightness after ontime * ontimeX
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].onMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;

              accessory[this->target].fade = random(255);
              analogWrite(accessory[this->target].outputPin1, accessory[this->target].fade);

              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_7
          case 7:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_8
          case 8:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_9
          case 9:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_10
          case 10:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_11
          case 11:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_12
          case 12:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_13
          case 13:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_14
          case 14:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_15
          case 15:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif
*/

         }
       }
      else
       {
        if ( accessory[this->target].dccstate && !this->direction )         // if turned on and direction turn off
         {

#ifdef DEBUG_MSG_LIGHTS
//      Serial.print("thisCommand = ");Serial.println(this->thisCommand);
      Serial.print("Process 3: target = ");Serial.println(this->target);
      Serial.print("Process 3: direction = ");Serial.println(this->direction);
      Serial.println();
#endif


          switch (accessory[this->target].mode1)
           {
#ifdef ACTION_ONOFF
            case 0:
              digitalWrite(accessory[this->target].outputPin1, LOW);
              digitalWrite(accessory[this->target].outputPin2, HIGH);

// outputPin3 here

              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
            break;
#endif
/*
#ifdef ACTION_ONESHOT
            case 1:
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
            break;
#endif
#ifdef ACTION_FLASH
            case 2:
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
            break;
#endif


#ifdef ACTION_FLASHFADER
            case 3:
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;

              accessory[this->target].fade = 0;
              analogWrite(accessory[this->target].outputPin1, accessory[this->target].fade);

              this->state = TT_STOP;
            break;
#endif


#ifdef ACTION_FLASHALTERNATE
            case 4:
              digitalWrite(accessory[this->target].outputPin1, LOW);
              digitalWrite(accessory[this->target].outputPin2, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
//              accessory[this->target].onoff2 = 0;
              this->state = TT_STOP;
            break;
#endif

#ifdef ACTION_STROBEDOUBLE
            case 5:
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
            break;
#endif

#ifdef ACTION_RANDOMBRIGHT
            case 6:
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;

              accessory[this->target].fade = 0;
              analogWrite(accessory[this->target].outputPin1, accessory[this->target].fade);

              this->state = TT_STOP;
            break;
#endif


#ifdef ACTION_7
          case 7:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_8
          case 8:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_9
          case 9:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_10
          case 10:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_11
          case 11:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_12
          case 12:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_13
          case 13:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_14
          case 14:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_15
          case 15:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin1, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif
*/


           }
         }
        else
         {
          this->state = TT_STOP;
         }

       }
     }

/*
 * this alters the output depending on mode, time and dccstate
 * 
 * 
 */

    for ( int i = 0; i < MAXACCESSORIES; i++)
     {
      if (accessory[i].dccstate)
       {

/*
#ifdef DEBUG_MSG_LIGHTS
        Serial.print("onMilli = ");Serial.println(accessory[i].onMilli);
        Serial.print("offMilli = ");Serial.println(accessory[i].offMilli);
        Serial.print("millis = ");Serial.println(millis());
#endif
*/

        switch (accessory[i].mode1)
         {
#ifdef ACTION_ONOFF
          case 0:                                         // oneshot
          break;
#endif

/*
#ifdef ACTION_ONESHOT
          case 1:                                         // oneshot
            if (millis() > accessory[i].offMilli)
             {
              digitalWrite(accessory[i].outputPin1, LOW);
              accessory[i].dccstate = 0;
              accessory[i].onoff = 0;
              this->state = TT_STOP;
             }
          break;
#endif

#ifdef ACTION_FLASH
          case 2:                                         // flashing
            if (accessory[i].onoff && millis() > accessory[i].offMilli)
             {
              digitalWrite(accessory[i].outputPin1, LOW);
              accessory[i].onMilli = millis() + (accessory[i].offtime * accessory[i].offtimeX);
              accessory[i].onoff = 0;
              this->state = TT_STOP;
             }
            else
             {
              if (!accessory[i].onoff && millis() > accessory[i].onMilli)
               {
                digitalWrite(accessory[i].outputPin1, HIGH);
                accessory[i].offMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
                accessory[i].onoff = 1;
                this->state = TT_STOP;
               }
             }
          break;
#endif

#ifdef ACTION_FLASHFADER
          case 3:                                        // flashing fade
            if ( (accessory[i].onoff) && (millis() > accessory[i].offMilli) )
             {

              if ( accessory[i].fade <= 255 )
               {
                if ( millis() > accessory[i].offMilli + accessory[i].fadein )
                 {
                  analogWrite(accessory[i].outputPin1, accessory[i].fade);
                  accessory[i].offMilli += accessory[i].fadein;

                  accessory[i].fade += 5;

                 }
               }
              else
               {
                accessory[i].onMilli = millis() + (accessory[i].offtime * accessory[i].offtimeX);
                accessory[i].onoff = 0;
                accessory[i].fade = 255;

                this->state = TT_STOP;
               }

             }
            else
             {
              if ( (!accessory[i].onoff) && (millis() > accessory[i].onMilli))
               {

                if ( accessory[i].fade >= 0 )
                 {
                  if ( millis() > accessory[i].onMilli + accessory[i].fadeout )
                   {
                    analogWrite(accessory[i].outputPin1, accessory[i].fade);
                    accessory[i].onMilli += accessory[i].fadeout;

                    accessory[i].fade -= 5;

                   }
                 }
                else
                 {
                  accessory[i].offMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
                  accessory[i].onoff = 1;
                  accessory[i].fade = 0;

                  this->state = TT_STOP;
                 }

               }
             }
          break;
#endif

#ifdef ACTION_FLASHALTERNATE
          case 4:                                         // flashing alternate
            if (accessory[i].onoff && millis() > accessory[i].offMilli)
             {
              digitalWrite(accessory[i].outputPin1, LOW);
              digitalWrite(accessory[i].outputPin2, HIGH);
              accessory[i].onMilli = millis() + (accessory[i].offtime * accessory[i].offtimeX);
              accessory[i].onoff = 0;
              this->state = TT_STOP;
             }
            else
             {
              if (!accessory[i].onoff && millis() > accessory[i].onMilli)
               {
                digitalWrite(accessory[i].outputPin1, HIGH);
                digitalWrite(accessory[i].outputPin2, LOW);
                accessory[i].offMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
                accessory[i].onoff = 1;
                this->state = TT_STOP;
               }
             }
          break;
#endif

#ifdef ACTION_STROBEDOUBLE
          case 5:                                         // strobe double
            if (accessory[i].onoff && millis() > accessory[i].offMilli)
             {
              accessory[i].onMilli = millis() + (accessory[i].offtime * accessory[i].offtimeX);
              accessory[i].onoff = 0;
              this->state = TT_STOP;
             }
            else
             {
              if (!accessory[i].onoff && millis() > accessory[i].onMilli)
               {
                this->strobe(accessory[i].outputPin1);
                delay(STROBE_DELAY);
                this->strobe(accessory[i].outputPin1);
                accessory[i].offMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
                accessory[i].onoff = 1;
                this->state = TT_STOP;
               }
             }
          break;
#endif

#ifdef ACTION_RANDOMBRIGHT
          case 6:                                         // random bright
            if (accessory[i].onoff && millis() > accessory[i].onMilli)
             {
              accessory[i].fade = random(255);
              analogWrite(accessory[i].outputPin1, accessory[i].fade);
              accessory[i].onMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
              this->state = TT_STOP;
             }
          break;
#endif


#ifdef ACTION_7
          case 7:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_8
          case 8:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_9
          case 9:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_10
          case 10:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_11
          case 11:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_12
          case 12:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_13
          case 13:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_14
          case 14:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_15
          case 15:
            this->state = TT_STOP;
          break;
#endif
*/

         }
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



/*
#ifdef ACTION_STROBEDOUBLE
void Lights::strobe(byte pin)
 {
  digitalWrite(pin, HIGH);
  delay(10);
  digitalWrite(pin, LOW);
 }
#endif
*/


