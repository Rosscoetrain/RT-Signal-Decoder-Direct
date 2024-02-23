/*
   defines.h
*/

// Un-Comment the line below to force CVs to be written to the Factory Default values
// defined in the FactoryDefaultCVs below on Start-Up
//#define FORCE_RESET_FACTORY_DEFAULT_CV 1


// Un-comment the following to allow address learning
//#define LEARNING

// Un-comment the following to allow serial communications
#define ENABLE_SERIAL

// to show all debug messages
//#define DEBUG_MSG

// to show lights debug messages
//#define DEBUG_MSG_LIGHTS
//#define DEBUG_MSG_LIGHTS_1
//#define DEBUG_MSG_LIGHTS_2
//#define DEBUG_MSG_LIGHTS_3

// Uncomment to print all DCC Packets
//#define  NOTIFY_DCC_MSG

// Uncomment to print all DCC turnout messages
//#define NOTIFY_TURNOUT_MSG

// Uncomment to print all DCC signal messages
//#define NOTIFY_SIGNAL_MSG

// Un-comment the following line to activate testing routines.
//#define TESTING

// Un-comment the following line to activate dcc testing routine.
//#define TESTING_DCC

// Un-Comment the line below to Enable DCC ACK for Service Mode Programming Read CV Capablilty
//#define ENABLE_DCC_ACK  15  // This is A1 on the Iowa Scaled Engineering ARD-DCCSHIELD DCC Shield


// Define the Arduino input Pin number for the DCC Signal
#define DCC_PIN     2

// Set the Decoder Version - Used by JMRI to Identify the decoder
#define DCC_DECODER_VERSION_NUM 11

//#define ACTION_ONOFF                        // on off by command
//#define ACTION_ONESHOT                      // one shot flash
//#define ACTION_FLASH                        // flash on single pin
//#define ACTION_FLASHFADER                   // flash with fade up/down
//#define ACTION_FLASHALTERNATE               // flash on alternate pins needs two output pins
//#define ACTION_STROBEDOUBLE                 // double strobe flash on single pin
//#define ACTION_RANDOMBRIGHT                 // random varying brightness
//#define ACTION_7                            // action 
//#define ACTION_8                            // action 
//#define ACTION_9                            // action 
//#define ACTION_10                           // action 
//#define ACTION_11                           // action 
//#define ACTION_12                           // action 
//#define ACTION_13                           // action 
//#define ACTION_14                           // action 
//#define ACTION_15                           // action 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fill in the number of accessories you want to control depends on board in use
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAXACCESSORIES 8

// this determines the number of queued commands.
#define TT_MOVER_MAX_TRACKS 16

#define LED_BUILTIN 13

#define TT_MOVER_SLOT_EMPTY 255

//#define STROBE_DELAY 100


// the number of CVs requried for each address
//#define NUMBER_OF_CVS 12
#define NUMBER_OF_CVS 10

// the first CV used to store configuration data
//#define CV_BASE_NUMBER 33
#define CV_DECODER_MODE CV_MANUFACTURER_START 
#define CV_BASE_NUMBER CV_MANUFACTURER_START + 1
  
