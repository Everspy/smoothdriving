/*
 * GPS.c
 *
 * Created: 11/7/2017 10:56:00 PM
 * Author : matth
 */ 

#define F_CPU 16000000UL

#define MAXIDLETIME 60*5 // 5 minutes

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "GPSFunc/GPSFunctions.h"
#include "USART/USARTFunc.h"
#include "GPSDecoder/GPSDecoder.h"
#include "Display/16x2_Display.h"
#include "RecordLoader/RecordLoader.h"
#include "PowerManager/PowerManager.h"

volatile float syncTime = 0;
volatile float lastIntersectionChangeTime = 0;
void (*SendString)(char*) = &SendUARTString;

/**
    The WaitForGPSFix() function blocks until a gps fix is found.
 */
void WaitForGPSFix()
{
	while(!IsGPSFixed())
	{
		SendUARTString("WaitingData\n\r");
		while(!IsNewGPSData());
		SendUARTString("Got Data\n\r");
		_delay_ms(2000);
	}
}

/**
    The WaitForSDMount() function blocks until the SD card is mounted
 */
void WaitForSDMount()
{
	bool mounted = InitializeStorageDevice();
	while(!mounted)
	{
		_delay_ms(250);
		mounted = MountStorageDevice();
	}
}
/**
    The TimeSinceLastMovement() function is unimplemented.
 */
float TimeSinceLastMovement()
{
	return 0;
}

/**
    The CurrentlyMoving() function returns true if speed > ~10 mph, and false otherwise.
 */
bool CurrentlyMoving()
{
	float speed = GetSpeed(locationBuffer.prevLocations[0], locationBuffer.prevLocations[1]);
	return (speed > 4.8);
}
/**
    The WaitForIntersectionSync() function waits for the user to stop at an intersection, then sets the sync time.
 */
void WaitForIntersectionSync()
{
	bool intersectionSynced = false;
	while(!intersectionSynced)
	{
		gpsPoint currentLocation = locationBuffer.prevLocations[0];
		intersection nearestIntersection = LoadNearestIntersection(currentLocation.lat, currentLocation.lng);
		
		gpsPoint intersectionLocation = { .lat = nearestIntersection.lat, .lng = nearestIntersection.lng };
		if(100 > DistanceBetweenPoints(currentLocation, intersectionLocation) &&
			!CurrentlyMoving())
		{
			DeleteIntersection(currentIntersection);
			currentIntersection = nearestIntersection;
			LoadLinkedIntersections((intersection *)&currentIntersection);
			syncTime = currentTime;
			lastIntersectionChangeTime = currentTime;
			intersectionSynced = true;
		}
	}
}

void PrintIntersectionUART(intersection intSec)
{
	char temp[15];
	itoa(intSec.id, temp, 10);
	SendUARTString("ID: ");
	SendUARTString(temp);
	SendUARTString(", Name: ");
	SendUARTString(intSec.name);
	SendUARTString("Lat:");
	dtostrf(intSec.lat, -8,6, temp);
	SendUARTString(temp);
	SendUARTString(", Lng: ");
	dtostrf(intSec.lng, -8,6, temp);
	SendUARTString(temp);
	SendUARTString(", NearbyCount: ");
	itoa(intSec.nearbyIntersectionCount, temp, 10);
	SendUARTString(temp);
	SendUARTString("\r\n");
}
/**
    The SendStringToDispla() function sends the string through UART
 */
void SendStringToDisplay(char* str)
{
	SendUARTString(str);
}

int main(void)
{
	InitUART();
	InitGPS();
	InitIntersectionManager();
	sei();
	SendStringToDisplay("Waiting for GPS fix\r\n");
	WaitForGPSFix();
	SendStringToDisplay("Waiting for SD card init...\n\r");
	WaitForSDMount();
	SendStringToDisplay("Waiting for Intersection Sync..\n\r");
	WaitForIntersectionSync();
	SendStringToDisplay("Started.");
	
	char speedStr[10];
	while(1)
	{
		WaitForGPSFix();
		if(!CurrentlyMoving() || (currentTime - lastIntersectionChangeTime) > 300)
		{
			float idleTime = TimeSinceLastMovement();
			if(idleTime > MAXIDLETIME)
			{
				Sleep();
				continue;
			}
			WaitForIntersectionSync();
		}
		
		intersection nextInt = GetNextIntersection(locationBuffer.prevLocations[0], locationBuffer.velocity[0]);
		gpsPoint nextIntersectionLocation = { .lat = nextInt.lat, .lng = nextInt.lng };
		gpsPoint currentIntersectionLocation = { .lat = currentIntersection.lat, .lng = currentIntersection.lng };
			
		float distanceBetweenIntersections = DistanceBetweenPoints(nextIntersectionLocation, currentIntersectionLocation);
		
		short necessarySpeed = (short)ConvertSpeedToMPH((distanceBetweenIntersections/(syncTime + nextInt.time - currentTime)));
		itoa(necessarySpeed, speedStr, 10);
		SendStringToDisplay("Maintain at least ");
		SendStringToDisplay(speedStr);
		SendStringToDisplay("mph.\n\r");
		
		float distanceFromCurrentIntersection = DistanceBetweenPoints(locationBuffer.prevLocations[0], currentIntersectionLocation);
		float distanceFromNextIntersection = DistanceBetweenPoints(locationBuffer.prevLocations[0], nextIntersectionLocation);
		
		if(distanceFromNextIntersection < distanceFromCurrentIntersection)
		{
			DeleteIntersection(currentIntersection);
			currentIntersection = nextInt;
			LoadLinkedIntersections((intersection *)&currentIntersection);
			lastIntersectionChangeTime = currentTime;
		}
		else if (distanceFromCurrentIntersection < 100) // 100 metres
		{
			syncTime = currentTime;
		}
		else
			DeleteIntersection(nextInt);
	}

}

/**
    10ms period FatFs timer function.
 */
ISR(TIMER1_COMPA_vect)
{
	currentTime += 0.010; // add 10ms to current time.
	disk_timerproc();
}





