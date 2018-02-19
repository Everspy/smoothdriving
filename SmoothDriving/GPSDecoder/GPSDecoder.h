/*
 * GPSDecoder.h
 *
 * Created: 11/25/2017 4:35:08 PM
 *  Author: Matt
 */ 

#ifndef GPSDECODER_H_
#define GPSDECODER_H_

#define GPS_DECODE_DEBUG 0

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef SREG
#define SREG _SFR_IO8(0x3f)
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../GPSFunc/GPSFunctions.h"
#include "../USART/USARTFunc.h"

#define UTCTIME 0
#define LATITUDE 1
#define LATITUDEDIRECTION 2
#define LONGITUDE 3
#define LONGITUDEDIRECTION 4
#define GPSQUALITY 5
#define NUMSATELLITES 6
#define HORIZONTALDILUTION 7
#define ALTITUDE 8
#define ALTITUDEUNITS 9

#define LOCATIONBUFFERSIZE 5

#define GGABUFFERSIZE 90

/**	
	locationBuffer holds the last LOCATIONBUFFERSIZE number of locations in prevLocations,
	and the last velocities in velocity              
 */
volatile struct _locationBuffer {
	gpsPoint prevLocations[LOCATIONBUFFERSIZE];
	velocity velocity[LOCATIONBUFFERSIZE];
	short bottom;
} locationBuffer;

volatile float currentTime;
/**	
	NewGPSData is a boolean that will only be set to true by the 
    usart interrupt when new location data is received.                
 */
bool NewGPSData;

/** 
	IsNewGPSData() returns true if there is unread GGA data, and false otherwise
 */
bool IsNewGPSData();
/** 
	IsGPSFixed() returns true if there GPS has a fix. False otherwise.
 */
bool IsGPSFixed();
/**
	PrintGGABuffer() attempts to send the GGA Buffer over USART
*/
void PrintGGABuffer();

/**
    InitGPS() prepares the variables used by the other GPS function and should be run once before using other functions
 */
void InitGPS(void);

/**
    SetGPSBuffer() sets the GPS buffer to the passed string. This is only used for testing
 */
void SetGPSBuffer(char*);

/**
   GetGGAItem() returns the string value stored in the GGA buffer for the desired item 
 */
char * GetGGAItem(uint8_t);

/**
    GetGPSTime() returns the time of the last GPS fix info in second since midnight.
 */
float GetGPSTime(void);

/**
    GetLatitude() returns the latitude of the last GPS fix info as a float
 */
float GetLatitude(void);

/**
    GetLongitude() returns the longitude of the last GPS fix info as a float
 */
float GetLongitude(void);

/**
    DegreeMinutesToDecimal() returns the decimal value of the degrees and minutes (format dddmm.mmmmm) and direction ('N','S','E','W')
 */
float DegreeMinutesToDecimal(float, char);

/**
    GetAltitude() returns the altitude of the last GPS fix info as a float
 */
float GetAltitude(void);

/**
    PushGPSPoint() inserts a point and velocity to the location buffer
 */
void PushGPSPoint(gpsPoint, velocity);

/**
    CreateGPSPoint() creates a gps point from the current GGA buffer and inserts it into the gps point buffer
 */
void CreateGPSPoint(void);

/**
    SetGPSBuffer() sets the GPS buffer to the passed string. This is only used for testing purposes
 */
void SetGPSBuffer(char*);

ISR (USART0_RX_vect);

#endif
