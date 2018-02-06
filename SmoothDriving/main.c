/*
 * GPS.c
 *
 * Created: 11/7/2017 10:56:00 PM
 * Author : matth
 */ 

#define F_CPU 16000000UL

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

void (*SendString)(char*) = &SendUARTString;

int main(void)
{
	InitUART();
	InitializeStorageDevice();
	_delay_ms(3000);

	SendString("Started!\n\r");
	while(1)
	{
		bool result = InitializeStorageDevice();
		if(result)
			break;
		else
			SendString("Failed to mount... Retrying\n\r");
	}
	SendString("Successfully mounted card.\n\r");
	
	char str[13];
	char newLine[] = "\r\n";
	intersection intSec;
	while(1){
		_delay_ms(3000);
		SendString("------------\r\n");
		bool valid = true;
		short i = 0;
		while(valid)
		{
			intSec = LoadRecordByID(i);
			valid = intSec.name != NULL;
			if(valid)
			{
				SendString("ID:");
				SendString(itoa(intSec.id, str, 10));
				
				SendString(", Name:");
				SendString(intSec.name);
				
				SendString(", Lat:");
				SendString(dtostrf(intSec.lat, -6,4, str));
				SendString(", Lng:");
				SendString(dtostrf(intSec.lng, -6,4, str));

				SendString(", Nearby Count:");
				SendString(itoa(intSec.nearbyIntersectionCount, str, 10));

				for(short j = 0; j < intSec.nearbyIntersectionCount; j++)
				{
					SendString(",");
					SendString(itoa(intSec.nearbyIntersections[j],str,10));
				}

				SendString(newLine);
				DeleteIntersection(intSec);
			}
			i++;
		}
		_delay_ms(3000);
	}
	/*
	cli();
	LCDInit();
	InitUART();
	InitGPS();
	SendString("\n\rStarted GPS Test.\n\r");
	_delay_ms(3000); // wait for GPS receiver to initialize 
	sei();
	char lng[15];
	char lat[15];
	char alt[15];
	char fixTime[9];
	char* time;
	float latf, lngf, altf;
	LCDSet("Started\n:)");

	fixTime[2] = ':';
	fixTime[5] = ':';
	fixTime[8] = '\0';
    while (1)
    {	
		if(IsNewGPSData()){
			cli();
			SendString("S>");
			SendString("\n\rGGA: ");
			PrintGGABuffer();

			time = GetGGAItem(UTCTIME);
			uint8_t i = 0;
			fixTime[0] = time[0];
			fixTime[1] = time[1];
			fixTime[3] = time[2];
			fixTime[4] = time[3];
			fixTime[6] = time[4];
			fixTime[7] = time[5]; 
			free(time);

			latf = GetLatitude();
			lngf = GetLongitude();
			altf = GetAltitude();

			// Convert lat, lng and alt to strings
			dtostrf(latf, -8,6, lat);
			dtostrf(lngf, -8,6, lng);
			dtostrf(altf, 5,2, alt);

			SendString("Time: ");
			SendString(fixTime);
			SendString("\n\rLatitude: ");
			SendString(lat);
			SendString("\n\rLongitude: ");
			SendString(lng);
			SendString("\n\rAltitude: ");
			SendString(alt);
			SendString(" meters\n\r");

			LCDSet("Time: ");
			LCDAppend(fixTime);
			_delay_ms(2000);
			LCDSet("Lat: ");
			LCDAppend(lat);
			_delay_ms(2000);
			LCDSet("Lng: ");
			LCDAppend(lng);
			_delay_ms(2000);
			LCDSet("Alt: ");
			LCDAppend(alt);
			LCDAppend("m");
			_delay_ms(2000);

			SendString("<D\n\r");
			sei();
		}


	}
	*/
}






