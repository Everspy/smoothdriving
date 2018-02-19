/*
 * GPSDecoder.c
 *
 * Created: 11/25/2017 4:34:54 PM
 *  Author: Matt
 */ 
 #include "GPSDecoder.h"
 //#include "../USART/USARTFunc.h"

char GGA_Buffer[GGABUFFERSIZE];             // to save GGA string
uint8_t CommaLocations[20];                 // to store instances of ','
char GGA_CODE[3];
bool ReceivingGGAString	= false;

volatile uint16_t GGA_Index, CommaCount;

void UpdateCurrentTime();

void InitGPS()
{
	NewGPSData = false;
    GGA_Index=0;
	for(uint8_t i = 0; i < 20; i++)
		CommaLocations[i] = 0;
    memset(GGA_Buffer, 0, GGABUFFERSIZE);
	locationBuffer.bottom = -1;
}
bool IsNewGPSData()
{
	if(NewGPSData && !ReceivingGGAString)
	{
		NewGPSData = false;
		return true;
	}
	return false;
}

bool IsGPSFixed()
{
	char* gpsquality = GetGGAItem(GPSQUALITY);
	char quality = gpsquality[0];
	free(gpsquality);

	// Detect no fix. Quality is a value between 0 and 8. '0' means no fix
	if(quality > '0' && quality < '8')
		return true;
		
	return false;
}

void DecodeBuffer()
{
	CommaCount = 1;
	for(uint8_t i = 0; GGA_Buffer[i] != '\0'; i++)
	{
		if(GGA_Buffer[i] == ',')
		{
			CommaLocations[CommaCount++] = i;
			memmove(&GGA_Buffer[i], &GGA_Buffer[i+1], strlen(GGA_Buffer)-i);
			i--;
		}
	}
}
void SetGPSBuffer(char* str)
{
	//SendString("Setting GGA buffer to :\t");
	uint8_t i = 0;
	do
	{
		//SendChar(str[i]);
		GGA_Buffer[i] = str[i];
		i++;
	} while(str[i] != 0);
	DecodeBuffer();

	#if GPS_DECODE_DEBUG 
	SendString("\n\rAdjusted: \t\t");
	SendString(GGA_Buffer);
	SendUARTChar('\n');
	SendUARTChar('\r');
	#endif

}

void PrintGGABuffer()
{
	uint8_t i = 0;
	uint8_t commas = 0;
	SendUARTString("$GPGGA");

	for(; GGA_Buffer[i] != '\0'; i++)
	{
		while(i == CommaLocations[commas])
		{
			commas++;
			SendUARTChar(',');
		}
		SendUARTChar(GGA_Buffer[i]);
	}
}


float GetGPSTime(void)
{
	char *Time_Buffer = GetGGAItem(UTCTIME);

	float time = 0;
	time = (Time_Buffer[0] - 48) * 10 * 60 * 60;	// 10s of hours
	time += (Time_Buffer[1] - 48) * 60 * 60;		// individual hours
	time += (Time_Buffer[2] - 48) * 10 * 60;		// 10s of minutes
	time += (Time_Buffer[3] - 48) * 60;				// minutes
	time += (Time_Buffer[4] - 48) * 10;				// 10s of seconds
	time += (Time_Buffer[5] - 48);					// seconds;

	// Handling decimal values (if any)
	for(uint8_t i = 6; Time_Buffer[i] != '\0'; i++)
	{
		if(Time_Buffer[i] == '.')
			continue;
		time += pow(10, 6 - i) * (Time_Buffer[i]-48);
	}

	free(Time_Buffer);
	return time;
}
char* GetGGAItem(uint8_t commaIndex)
{
	uint8_t index = CommaLocations[commaIndex];
	uint8_t endIndex = CommaLocations[commaIndex+1];

	char* returnValue = (char*)malloc(sizeof(char) * (endIndex - index + 1));

	// Null terminate the string
	returnValue[endIndex-index] = '\0';

	// Fill in all the characters
	for(uint8_t i = 0; i < (endIndex - index); i++)
	{
		returnValue[i] = GGA_Buffer[index + i];
	}
	return returnValue;
}

// Note:
// decimal degrees = (Degrees + (minutes / 60)) * -1 if South
float GetLatitude(void)
{
	char* latitudeBuffer = GetGGAItem(LATITUDE);
	char* latitudeDirection = GetGGAItem(LATITUDEDIRECTION);
	
	char direction = latitudeDirection[0];
	double value = atof(latitudeBuffer);

	free(latitudeBuffer);
	free(latitudeDirection);

	return DegreeMinutesToDecimal(value, direction);
}

float GetLongitude(void)
{
	char* longitudeBuffer = GetGGAItem(LONGITUDE);
	char* longitudeDirection = GetGGAItem(LONGITUDEDIRECTION);

	char direction = longitudeDirection[0];
	double value = atof(longitudeBuffer);

	free(longitudeBuffer);
	free(longitudeDirection);

	return DegreeMinutesToDecimal(value, direction);
}

float DegreeMinutesToDecimal(float value, char direction)
{
	int8_t multiplier;

	int32_t degrees = floor(value/100);
	double minutes = value - (double)degrees * 100;

	if(direction == 'W' || direction == 'S')
		multiplier = -1;
	else
		multiplier = 1;

	return (float)(degrees + (minutes/60)) * multiplier;
}
float GetAltitude(void)
{
	char* altitudeBuffer = GetGGAItem(ALTITUDE);
	float value = atof(altitudeBuffer);
	free(altitudeBuffer);

	return value;
}

void PushGPSPoint(gpsPoint point, velocity vel)
{
	// Shifting the buffer downwards
	for(uint8_t i = 0; i < LOCATIONBUFFERSIZE - 1; i++)
	{
		locationBuffer.prevLocations[i] = locationBuffer.prevLocations[i + 1];
		locationBuffer.velocity[i] = locationBuffer.velocity[i + 1];
	}
	// Inserting new point
	locationBuffer.prevLocations[0] = point;
	locationBuffer.velocity[0] = vel;
	
	if(locationBuffer.bottom < 5)
		locationBuffer.bottom++;
}

void CreateGPSPoint(void)
{
	// Generate point, fill in data
	gpsPoint point;
	point.lat = GetLatitude();
	point.lng = GetLongitude();
	point.locatedTime = GetGPSTime();

	if(!IsGPSFixed())
		return;

	// Calculate speed. set to zero if the buffer has no comparison
	velocity vel;
	if(locationBuffer.bottom > -1)
	{
		vel = GetVelocity(point, locationBuffer.prevLocations[0]);
	}
	else
	{
		vel.speed = 0;
		vel.direction = 0;
	}

	PushGPSPoint(point, vel);
}

void UpdateCurrentTime()
{
	if(!IsGPSFixed())
		return;
	currentTime = GetGPSTime();
}

/* 
 * Comma locations
 * [0] : Time
 * [1] : Latitude
 * [2] : N or S
 * [3] : Long
 * [4] : E or W
 * [5] : GPS Quality
 * [6] : # of satellites in use
 * [7] : Horizontal
 * [8] : Altitude
 * [9] : Units of altitude [m]
 */
// Example of desired data:
// $GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
// $GPGGA,230600.501,4543.8895,N,02112.7238,E,1,03,3.3,96.7,M,39.0,M,,0000*6A,
// hhmmss.ss = UTC of position
// llll.ll = latitude of position
// a = N or S
// yyyyy.yy = Longitude of position
// a = E or W
// x = GPS Quality indicator (0=no fix, 1=GPS fix, 2=Dif. GPS fix)
// xx = number of satellites in use
// x.x = horizontal dilution of precision
// x.x = Antenna altitude above mean-sea-level
// M = units of antenna altitude, meters
// x.x = Geoidal separation
// M = units of geoidal separation, meters
// x.x = Age of Differential GPS data (seconds)
// xxxx = Differential reference station ID


//$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47


ISR (USART_RX_vect)
{
	uint8_t oldSREG = SREG; // Preserve status register
	cli();
	char received_char = UDR0;		// Read char, clear register

	if(ReceivingGGAString && received_char == '$')
	{
		CreateGPSPoint();
		NewGPSData = true;
	}

	if(received_char =='$') // '$' starts the input
	{        
		// Reset data for new string   
		GGA_Buffer[GGA_Index] = '\0';      
		GGA_Index = 0;						
		CommaCount = 1;
		ReceivingGGAString = false;
	}
	else if(ReceivingGGAString == true)  // if true save GGA info into buffer
	{
		if(received_char == ',' )
			CommaLocations[CommaCount++] = GGA_Index;     // store instances of ',' divider location
		else
			GGA_Buffer[GGA_Index++] = received_char;	  // Store char in it's buffer
	}
	else if(GGA_CODE[0] == 'G' && GGA_CODE[1] == 'G' && GGA_CODE[2] == 'A') // End of initial sentence identifier ("$GPGGA").
	{
		// If GGA string, reset GGA data. Data should begin streaming in.
		ReceivingGGAString = true;
		GGA_CODE[0] = 0; GGA_CODE[1] = 0; GGA_CODE[2] = 0;
	}
	else // Store data in the GGA code scanner
	{
		GGA_CODE[0] = GGA_CODE[1];  GGA_CODE[1] = GGA_CODE[2]; GGA_CODE[2] = received_char;
	}
	SREG = oldSREG;
}