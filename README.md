# sx1278Bug
possible hardware bug in sx1278 chips

 I have encountered a strange phenomenon with SX1278, in particular I am using Arduino Due and the RA01 module inside which there is sx1278.

 Pin 10 of Arduino Due connected to NSS of RA01 module
 Pin 9 connected to RST
 Pin 2 connected to DIO0
 Pin 3.3 volt of Arduino Due connected to 3v3 of RA01 module
 Pin GND connected to GND
 Pin SCK MISO and MOSI of Arduino Due SPI connector connected to the respective pins on RA01.

 I noticed problems with this low sensitivity SX1278 in reception and I investigated a lot.
 What I found leaves me perplexed but it is so striking that I share it here.
 I hope that someone with more equipment than I have (spectrum analyzer) can investigate
 better than me.
 
 In some libraries, instead of using an interrupt approach, a polling approach is used
 in which the RegIrqFlags register (0x12) is continuously polled in order to
 determine when a packet has been received.

 Well I realized that this continuous polling is the source of the problems.
 Incredibly I realized that just polling any register
 the problem appears!
 
 So I wrote this code that intentionally does not use any library and that limits itself to polling 
 and I chose to use the RegVersion register at address 0x42.

 If you look at the code you are led to think that nothing will happen.
 Instead with this code in operation the module transmits a weak RF signal
 receivable (with my limited equipment) up to ten meters.
 It transmits many carriers at different frequencies spaced about 70Khz apart.
 All this signal disturbs the other receivers placed in the vicinity including
 itself if the module were actually in reception.
 The polling of RegIrqFlags disturbs the reception of the packet you want to receive.

 You are invited to verify what I have discovered and to let me know if you also encounter this phenomenon.

 Antonio_prev@hotmail.com
