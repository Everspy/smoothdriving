/*
 * NOKIADisplay.h
 *
 * Created: 11/19/2017 2:53:43 PM
 *  Author: Matt
 */ 

 /*
 * Pins on NOKIA Display:
 * 1: VCC		Power supply			between 2.7 and 3.3V
 * 2: GND		Ground
 * 3: SCE		Chip Select				Active Low
 * 4: RST		Reset					Active Low
 * 5: D/C		Mode Select				Select between command mode (low) and data mode (high)
 * 6: DN(MOSI)	Serial Data In	
 * 7: SCLK		Serial Clock
 * 8: LED		LED Backlight supply	Max voltage 3.3V
 */

#ifndef NOKIADISPLAY_H_
#define NOKIADISPLAY_H_

#define NOKIAPORT PORTB
#define NOKIASCLK PORTB0
#define NOKIARESET PORTB1




void SetupDisplay();
void SendNokiaCommand(char command);
void SendNokiaLetter(char letter);
#endif