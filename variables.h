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

mode1      33  45  57  69  81  ... 
mode2      34  46  58  70
mode3      35  47  59  71
ontime     36  48  60  72
ontimeX    37  49  61  73
offtime    38  50  62  74
offtimeX   39  51  63  75
outputPin1 40  52  64  76
outputPin2 41  53  65  77
outputPin3 42  54  66  78
fadein     43  55  67  79
fadeout    44  56  68  30

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
  {34, 0},
  {35, 0},
  {36, 10},
  {37, 100},
  {38, 10},
  {39, 100},
  {40, 4},
  {41, 3},
  {42, 0},
  {43, 10},
  {44, 10},
                        // default address 2
  {45, 0},
  {46, 0},
  {47, 0},
  {48, 10},
  {49, 100},
  {50, 10},
  {51, 100},
  {52, 6},
  {53, 5},
  {54, 0},
  {55, 10},
  {56, 10},
                        // default address 3
  {57, 0},
  {58, 0},
  {59, 0},
  {60, 10},
  {61, 100},
  {62, 10},
  {63, 100},
  {64, 8},
  {65, 7},
  {66, 0},
  {67, 10},
  {68, 10},
                        // default address 4
  {69, 0},
  {70, 0},
  {71, 0},
  {72, 10},
  {73, 100},
  {74, 10},
  {75, 100},
  {76, 10},
  {77, 9},
  {78, 0},
  {79, 10},
  {80, 10},
                        // default address 5
  {81, 0},
  {82, 0},
  {83, 0},
  {84, 10},
  {85, 100},
  {86, 10},
  {87, 100},
  {88, 12},
  {89, 11},
  {90, 0},
  {91, 10},
  {92, 10},
                        // default address 6
  {93, 0},
  {94, 0},
  {95, 0},
  {96, 10},
  {97, 100},
  {98, 10},
  {99, 100},
  {100, 14},
  {101, 13},
  {102, 0},
  {103, 10},
  {104, 10},
                        // default address 7
  {105, 0},
  {106, 0},
  {107, 0},
  {108, 10},
  {109, 100},
  {110, 10},
  {111, 100},
  {112, 17},
  {113, 16},
  {114, 0},
  {115, 10},
  {116, 10},
                        // default address 8
  {117, 0},
  {118, 0},
  {119, 0},
  {120, 10},
  {121, 100},
  {122, 10},
  {123, 100},
  {124, 19},
  {125, 18},
  {126, 0},
  {127, 10},
  {128, 10},
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
