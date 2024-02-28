# RT-Signal-Decoder-Direct

A DCC accessory decoder for coloured light signals.

Code is based on the NMRA Accessory Decoder example.

Additions include:

-- programming parameters via serial port

-- programming address via learning mode

-- modes set by CV


It provides control of up to 8 2 aspect signals, or 4 3/4 aspect signals.


Aspects available are:

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


The hardware that this can be used with is here:

https://github.com/Rosscoetrain/RT-Hardware/tree/master/RT_Pulse_8_decoder

However there is a modification needed to the hardware if using incandescent lamps.

