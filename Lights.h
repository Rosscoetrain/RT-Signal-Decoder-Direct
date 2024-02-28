/*
 * Lights.h
 * 
 * A class to control the light decoder
 * 
 */

#include <Arduino.h>
#include "defines.h"

enum TT_State
{  
  TT_IDLE = 0,
  TT_MOVE,
  TT_MOVING,
  TT_STOP,
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
//  int               address;          // User Configurable. DCC address to respond to
  byte              mode1;            // User Configurable. mode for outputPin1
  byte              mode2;            // User configurable, mode for outputPin2
  byte              mode3;            // User configurable, mode for outputPin3
  byte              mode4;            // User configurable, mode for outputPin4
  byte              outputPin1;       // User Configurable. Arduino pin where accessory is connected to
  byte              outputPin2;       // User Configurable. 2nd pin for AlternatingFlasher (e.g. railway crossing)
  byte              outputPin3;       // User Configurable. 3rd pin for e.g. amber aspect
  byte              outputPin4;       // User Configurable. 4th pin for e.g. white aspect
  byte              ontime;           // User Configurable. Oneshot or Flasher on time in ms X ontimeX
  byte              ontimeX;          // User Configurable. on time multiplier
  byte              offtime;          // User Configurable. Flasher off time in ms X offtimeX
  byte              offtimeX;         // User Configurable. off time multiplier
  byte              dccstate;         // Internal use. DCC state of accessory: 1=on, 0=off
  byte              onoff;            // Internal use. Output state of accessory: 1=on, 0=off
  byte              fadein;           // fade in time for fader
  byte              fadeout;          // fade out time for fader
  byte              fade;             // fade level
  byte              onoff2;           // Internal use. Output state of AlternatingFlasher 1=on, 0=off NOT USED
  byte              finished;         // Internal use. Memory that says the Oneshot is finished
  unsigned long     onMilli;          // Internal use.
  unsigned long     offMilli;         // Internal use.

} DCCAccessoryAddress;

class Lights
{
  private:
    TT_State state = TT_IDLE;
    uint8_t commandQueue[TT_MOVER_MAX_TRACKS + 1];
    byte thisCommand, newCommand;
    byte target;
    byte direction;
    
    DCCAccessoryAddress accessory[MAXACCESSORIES];

    void strobe(uint8_t pin);

  public:
    void init(NmraDcc Dcc, byte* outputs);
    void addCommand(uint8_t command);
    void process(void);

};


  
