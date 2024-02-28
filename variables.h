/*
 * variables.h
 */


NmraDcc  Dcc ;
DCC_MSG  Packet ;


// This is the Arduino Pin Mapping to decoder addresses with 2 pins per address 
// A1 is missing in the sequence as it is used for the DCC ACK

//   base address 1T 1C 2T 2C 3T 3C  4T 4C  5T  5C  6T  6C  7T  7C  8T  8C
byte outputs[] = { 4, 3, 6, 5, 8, 7, 10, 9, 12, 11, 13, 14, 16, 17, 19, 18};
//   pins         D4 D3 D6 D5 D8 D7 D10 D9 D12 D11 D13  A0  A2  A3  A5  A4
// index hex       0  1  2  3  4  5   6  7   8   9   A   B   C   D   E   F
// index dec       0  1  2  3  4  5   6  7   8   9  10  11  12  13  14  15


struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};


/*

Mode and outputPin to one CV and have them organised as:

Bit 7 6 5 4 3 2 1 0
    m m m m p p p p

mmmm = mode default is 0 not currently used (2024-02-28)
pppp = outputPin

                  address CV
                  1   2   3   4   ...
outputPin1Mode1   33  43  53  63                    CV_BASE_NUMBER + 0
outputPin2Mode2   34  44  54  64                    CV_BASE_NUMBER + 1
outputPin3Mode3   35  45  55  65                    CV_BASE_NUMBER + 2 
outputPin4Mode4   36  46  56  66                    CV_BASE_NUMBER + 3
ontime            37  47  57  67                    CV_BASE_NUMBER + 4
ontimeX           38  48  58  68                    CV_BASE_NUMBER + 5
offtime           39  49  59  69                    CV_BASE_NUMBER + 6
offtimeX          40  50  60  70                    CV_BASE_NUMBER + 7
fadein            41  51  61  71                    CV_BASE_NUMBER + 8
fadeout           42  52  62  72                    CV_BASE_NUMBER + 9

accessory.outputPin1 = (CV & 0x0F)
accessory.mode1 = (CV & 0xF0) >> 4


mode as at 27th February 2024 these are not used
  0 continuous (stays on until off command received)
  1 oneshot (on for ontime once) ms / 100
  2 flashing (alternate ontime offtime until off command received)
  3 flash/fade (alternate ontime offtime until off command received fade in/out
  4 flash alternate (requires two output pins)
  5 flash/fade altternate
  6 strobe double
  7 random brightness

ontime offtime
  time in ms / 10 for on or off functions

ontimeX offtimeX
  multiplier for on or off times

*/


/*
aspect

0 = red
1 = green
2 = amber
3 = amber amber
4 = flashing red
5 = flashing green
6 = flashing amber
7 = alternate flash
8 = alternate flash fade
9 = double strobe
10 = single strobe

18 = dark
*/




//#ifdef FORCE_RESET_FACTORY_DEFAULT_CV

/*
* factory defaults for 8 x 2 aspect signal heads
*/

CVPair FactoryDefaultCVs_8x2 [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
  {CV_MANUFACTURER_ID, 0},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},


  {CV_DECODER_MODE, 0},                       // decoder mode 8 x 2 aspect signals
                         // default address 1
  {CV_BASE_NUMBER + 0, 0x00},                 // mode 0 outputs[0]  outputPin1
  {CV_BASE_NUMBER + 1, 0x01},                 // mode 0 outputs[1]  outputPin2
  {CV_BASE_NUMBER + 2, 0x00},                 //                    outputPin3
  {CV_BASE_NUMBER + 3, 0x00},                 //                    outputPin4                      
  {CV_BASE_NUMBER + 4, 10},
  {CV_BASE_NUMBER + 5, 100},
  {CV_BASE_NUMBER + 6, 10},
  {CV_BASE_NUMBER + 7, 100},
  {CV_BASE_NUMBER + 8, 10},
  {CV_BASE_NUMBER + 9, 10},

                        // default address 2
  {CV_BASE_NUMBER + 10, 0x02},                 // mode 0 outputs[2]
  {CV_BASE_NUMBER + 11, 0x03},                 // mode 0 outputs[3]
  {CV_BASE_NUMBER + 12, 0x00},
  {CV_BASE_NUMBER + 13, 0x00},
  {CV_BASE_NUMBER + 14, 10},
  {CV_BASE_NUMBER + 15, 100},
  {CV_BASE_NUMBER + 16, 10},
  {CV_BASE_NUMBER + 17, 100},
  {CV_BASE_NUMBER + 18, 10},
  {CV_BASE_NUMBER + 19, 10},

                        // default address 3
  {CV_BASE_NUMBER + 20, 0x04},                 // mode 0 outputs[4]
  {CV_BASE_NUMBER + 21, 0x05},                 // mode 0 outputs[5]
  {CV_BASE_NUMBER + 22, 0x00},
  {CV_BASE_NUMBER + 23, 0x00},
  {CV_BASE_NUMBER + 24, 10},
  {CV_BASE_NUMBER + 25, 100},
  {CV_BASE_NUMBER + 26, 10},
  {CV_BASE_NUMBER + 27, 100},
  {CV_BASE_NUMBER + 28, 10},
  {CV_BASE_NUMBER + 29, 10},

                        // default address 4
  {CV_BASE_NUMBER + 30, 0x06},                 // mode 0 outputs[6]
  {CV_BASE_NUMBER + 31, 0x07},                 // mode 0 outputs[7]
  {CV_BASE_NUMBER + 32, 0x00},
  {CV_BASE_NUMBER + 33, 0x00},
  {CV_BASE_NUMBER + 34, 10},
  {CV_BASE_NUMBER + 35, 100},
  {CV_BASE_NUMBER + 36, 10},
  {CV_BASE_NUMBER + 37, 100},
  {CV_BASE_NUMBER + 38, 10},
  {CV_BASE_NUMBER + 39, 10},

                        // default address 5
  {CV_BASE_NUMBER + 40, 0x08},                 // mode 0 outputs[8]
  {CV_BASE_NUMBER + 41, 0x09},                 // mode 0 outputs[9]
  {CV_BASE_NUMBER + 42, 0x00},
  {CV_BASE_NUMBER + 43, 0x00},
  {CV_BASE_NUMBER + 44, 10},
  {CV_BASE_NUMBER + 45, 100},
  {CV_BASE_NUMBER + 46, 10},
  {CV_BASE_NUMBER + 47, 100},
  {CV_BASE_NUMBER + 48, 10},
  {CV_BASE_NUMBER + 49, 10},

                        // default address 6
  {CV_BASE_NUMBER + 50, 0x0A},                 // mode 0 outputs[10]
  {CV_BASE_NUMBER + 51, 0x0B},                 // mode 0 outputs[11]
  {CV_BASE_NUMBER + 52, 0x00},
  {CV_BASE_NUMBER + 53, 0x00},
  {CV_BASE_NUMBER + 54, 10},
  {CV_BASE_NUMBER + 55, 100},
  {CV_BASE_NUMBER + 56, 10},
  {CV_BASE_NUMBER + 57, 100},
  {CV_BASE_NUMBER + 58, 10},
  {CV_BASE_NUMBER + 59, 10},

                        // default address 7
  {CV_BASE_NUMBER + 60, 0x0C},                 // mode 0 outputs[12]
  {CV_BASE_NUMBER + 61, 0x0D},                 // mode 0 outputs[13]
  {CV_BASE_NUMBER + 62, 0x00},
  {CV_BASE_NUMBER + 63, 0x00},
  {CV_BASE_NUMBER + 64, 10},
  {CV_BASE_NUMBER + 65, 100},
  {CV_BASE_NUMBER + 66, 10},
  {CV_BASE_NUMBER + 67, 100},
  {CV_BASE_NUMBER + 68, 10},
  {CV_BASE_NUMBER + 69, 10},

                        // default address 8
  {CV_BASE_NUMBER + 70, 0x0E},                 // mode 0 outputs[14]
  {CV_BASE_NUMBER + 71, 0x0F},                 // mode 0 outputs[15]
  {CV_BASE_NUMBER + 72, 0x00},
  {CV_BASE_NUMBER + 73, 0x00},
  {CV_BASE_NUMBER + 74, 10},
  {CV_BASE_NUMBER + 75, 100},
  {CV_BASE_NUMBER + 76, 10},
  {CV_BASE_NUMBER + 77, 100},
  {CV_BASE_NUMBER + 78, 10},
  {CV_BASE_NUMBER + 79, 10},
};



/*
* factory defaults for 4 x 3 or 4 aspect signal heads
*/

CVPair FactoryDefaultCVs_4x4 [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},

  {CV_DECODER_MODE, 1},                       // decoder mode 4 x 3 or 4 aspect signals

                        // default address 1
  {CV_BASE_NUMBER + 0, 0x00},                 // mode 0 outputs[0]  outputPin1
  {CV_BASE_NUMBER + 1, 0x01},                 // mode 0 outputs[1]  outputPin2
  {CV_BASE_NUMBER + 2, 0x02},                 // mode 0 outputs[2]  outputPin3
  {CV_BASE_NUMBER + 3, 0x03},                 // mode 0 outputs[3]  outputPin4                      
  {CV_BASE_NUMBER + 4, 10},
  {CV_BASE_NUMBER + 5, 100},
  {CV_BASE_NUMBER + 6, 10},
  {CV_BASE_NUMBER + 7, 100},
  {CV_BASE_NUMBER + 8, 10},
  {CV_BASE_NUMBER + 9, 10},

                        // default address 2
  {CV_BASE_NUMBER + 10, 0x04},                 // mode 0 outputs[4]
  {CV_BASE_NUMBER + 11, 0x05},                 // mode 0 outputs[5]
  {CV_BASE_NUMBER + 12, 0x06},                 // mode 0 outputs[6]
  {CV_BASE_NUMBER + 13, 0x07},                 // mode 0 outputs[7]
  {CV_BASE_NUMBER + 14, 10},
  {CV_BASE_NUMBER + 15, 100},
  {CV_BASE_NUMBER + 16, 10},
  {CV_BASE_NUMBER + 17, 100},
  {CV_BASE_NUMBER + 18, 10},
  {CV_BASE_NUMBER + 19, 10},

                        // default address 3
  {CV_BASE_NUMBER + 20, 0x08},                 // mode 0 outputs[8]
  {CV_BASE_NUMBER + 21, 0x09},                 // mode 0 outputs[9]
  {CV_BASE_NUMBER + 22, 0x0A},                 // mode 0 outputs[10]
  {CV_BASE_NUMBER + 23, 0x0B},                 // mode 0 outputs[11]
  {CV_BASE_NUMBER + 24, 10},
  {CV_BASE_NUMBER + 25, 100},
  {CV_BASE_NUMBER + 26, 10},
  {CV_BASE_NUMBER + 27, 100},
  {CV_BASE_NUMBER + 28, 10},
  {CV_BASE_NUMBER + 29, 10},

                        // default address 4
  {CV_BASE_NUMBER + 30, 0x0C},                 // mode 0 outputs[12]
  {CV_BASE_NUMBER + 31, 0x0D},                 // mode 0 outputs[13]
  {CV_BASE_NUMBER + 32, 0x0E},                 // mode 0 outputs[14]
  {CV_BASE_NUMBER + 33, 0x0F},                 // mode 0 outputs[15]
  {CV_BASE_NUMBER + 34, 10},
  {CV_BASE_NUMBER + 35, 100},
  {CV_BASE_NUMBER + 36, 10},
  {CV_BASE_NUMBER + 37, 100},
  {CV_BASE_NUMBER + 38, 10},
  {CV_BASE_NUMBER + 39, 10},
};

//#endif

/*
aspects

0 - red
1 - green
2 - amber
3 - flash red
4 - flash green
5 - flash amber

31 - dark
*/



uint8_t FactoryDefaultCVIndex = 0;

uint16_t BaseDecoderAddress;

#ifdef LEARNING
// for address learning mode
int LEARNINGBUTTON = 0;    // pin 0
#define LEDCONTROL LED_BUILTIN
int learningMode = LOW;
#endif

uint16_t thisCommand = 0;

#ifdef ENABLE_SERIAL
// buffer to hold serial commands
String readString;
#endif

Lights lLights;


char* version;
uint8_t versionBuffer[3];

const byte MANUF_ID = MAN_ID_DIY;                          // Manufacturer ID in CV 8.

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
const byte FORCE_CVS = FORCE_RESET_FACTORY_DEFAULT_CV;
#else
const byte FORCE_CVS = 0;
#endif
