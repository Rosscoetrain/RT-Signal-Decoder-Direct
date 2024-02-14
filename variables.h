/*
 * variables.h
 */


NmraDcc  Dcc ;
DCC_MSG  Packet ;


struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};


/*

So for each address need 10 CVs for data storage

           address CV
           1   2   3   4   ...

data

mode       33  43  53  63  73  ... 
ontime     34  44  54  64
ontimeX    35  45  55  65
offtime    36  46  56  66
offtimeX   37  47  57  67
outputPin  38  48  58  68
outputPin2 39  49  59  69
outputPin3 40  50  60  70
fadein     41  51  61  71
fadeout    42  52  62  72

mode
  0 continuous (stays on until off command received)
  1 oneshot (on for ontime once) ms / 100
  2 flashing (alternate ontime offtime until off command received)
  3 flash/fade (alternate ontime offtime until off command received fade in/out
  4 flash alternate (requires two output pins)
  5 strobe double
  6 random brightness

ontime offtime
  time in ms / 10 for on or off functions

ontimeX offtimeX
  multiplier for on or off times

*/


#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},

                        // default address 1
  {33, 0},
  {34, 10},
  {35, 100},
  {36, 10},
  {37, 100},
  {38, 4},
  {39, 3},
  {40, 0},
  {41, 10},
  {42, 10},
                        // default address 2
  {43, 0},
  {44, 10},
  {45, 100},
  {46, 10},
  {47, 100},
  {48, 6},
  {49, 5},
  {50, 0},
  {51, 10},
  {52, 10},
                        // default address 3
  {53, 0},
  {54, 10},
  {55, 100},
  {56, 10},
  {57, 100},
  {58, 8},
  {59, 7},
  {60, 0},
  {61, 10},
  {62, 10},
                        // default address 4
  {63, 0},
  {64, 10},
  {65, 100},
  {66, 10},
  {67, 100},
  {68, 10},
  {69, 9},
  {70, 0},
  {71, 10},
  {72, 10},
                        // default address 5
  {73, 0},
  {74, 10},
  {75, 100},
  {76, 10},
  {77, 100},
  {78, 12},
  {79, 11},
  {80, 0},
  {81, 10},
  {82, 10},
                        // default address 6
  {83, 0},
  {84, 10},
  {85, 100},
  {86, 10},
  {87, 100},
  {88, 14},
  {89, 13},
  {90, 0},
  {91, 10},
  {92, 10},
                        // default address 7
  {93, 0},
  {94, 10},
  {95, 100},
  {96, 10},
  {97, 100},
  {98, 17},
  {99, 16},
  {100, 0},
  {101, 10},
  {102, 10},
                        // default address 8
  {103, 0},
  {104, 10},
  {105, 100},
  {106, 10},
  {107, 100},
  {108, 19},
  {109, 18},
  {110, 0},
  {111, 10},
  {112, 10},
};
#endif

uint8_t FactoryDefaultCVIndex = 0;

uint16_t BaseDecoderAddress;

#ifdef LEARNING
// for address learning mode
int LEARNINGBUTTON = 0;    // pin 0
#define LEDCONTROL LED_BUILTIN
int learningMode = LOW;
#endif

byte thisCommand = 0;

#ifdef ENABLE_SERIAL
// buffer to hold serial commands
String readString;
#endif

Lights lLights;

  
char* version;
uint8_t versionBuffer[3];
