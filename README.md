# RT-Signal-Decoder-Direct

A DCC accessory decoder for coloured light signals.

Code is based on the NMRA Accessory Decoder example.

Additions include:

-- programming parameters via serial port

-- programming address via learning mode

-- modes set by CV


It provides control of up to 8 2 aspect signals, or 4 3 aspect signals.


Various modes can be set including:

  0 continuous (stays on until off command received)
  1 oneshot (on for ontime once)
  2 flashing (alternate ontime offtime until off command received)
  3 flash/fade (alternate ontime offtime until off command received fade in/out)
  4 flash alternate (requires two output pins)
  5 flash/face alternate (as 5 but fade in/out)
  6 strobe double
  7 random brightness

The hardware that this can be used with is here:

https://github.com/Rosscoetrain/RT-Hardware/tree/master/RT_Pulse_8_decoder

However there is a modification needed to the hardware if using incandescent lamps.

