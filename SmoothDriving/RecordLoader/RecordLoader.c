/*
 * RecordLoader.c
 *
 * Created: 2/5/2018 3:39:36 PM
 *  Author: Matt
 */ 

#define INTERSECTION_CSV_NAME "i"
#define LOAD_BYTE_COUNT 52
#define INDEX_ID 0
#define INDEX_LAT 1
#define INDEX_LNG 2
#define INDEX_TIME 3
#define INDEX_NAME 4
#define INDEX_LINKED_COUNT 5
#define INDEX_LINKED_INTERSECTION_BASE 6

#include <float.h>
#include "RecordLoader.h"
#include "../GPSFunc/GPSFunctions.h"

FATFS fs;
bool initialized = false;

/**
    The GetIntersectionFromCSV() function will generate an intersection object from the passed CSV
 */
intersection GetIntersectionFromCSV(char* csvEntry);
/**
    The GetCSVIndex() function returns the value stored in a CSV string at the passed index.
	The index is 0 based. Returned result will be NULL if no result is found.
	The returned result must be freed to prevent a memory leak
 */
char* GetCSVIndex(char* csvEntry, short index);
/**
    The StrLen() function attempts to read the length of a char array (stopping at NULL). The return value will be the detected length
	if length is greater than maxLength, maxLength will return/
 */
int StrLen(char* str, int maxLength);
/**
    The GetFloatFromCSV() function reads a value from a CSV entry at the indicated index and returns it as a float
 */
float GetFloatFromCSV(char* csvEntry, short index);
/**
    The GetIntFromCSV() function reads a value from a CSV entry at the indicated index and returns it as an int
 */
int GetIntFromCSV(char* csvEntry, short index);
/**
    The GetLinkedIntersections() function attempts to read the length of a char array (stopping at NULL). The return value will be the detected length
	if length is greater than maxLength, maxLength will return/
 */
short* GetLinkedIntersections(char* csvEntry, short count);


bool IsStorageDetected()
{
	return true;
	// Not functional (SD Card module doesn't have sd card detection)
	//return !(disk_status(0) && STA_NODISK);
}

bool InitializeStorageDevice()
{
	//if(!initialized)
	//{
		// TIMER1 initialization. Needs 10ms period
		TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); //CTC, clock div 64
		OCR1A = 2500;// 0x270F; //For 10 ms
		TIMSK1 = (1 << OCIE1A); //Output compare match A int enable
		sei();
	//}
	return MountStorageDevice();
}
bool MountStorageDevice()
{
	FRESULT res = f_mount(&fs,"0:",1);
	return res == FR_OK;
}

intersection LoadRecordByID(unsigned short id)
{
	FIL file; // file object
	char line[LOAD_BYTE_COUNT];
	intersection intSec;
	intSec.name = NULL;
	intSec.nearbyIntersections = NULL;
	intSec.nearbyIntersectionsID = NULL;
	
	FRESULT fr = f_open(&file, INTERSECTION_CSV_NAME, FA_READ);
	
	if(fr != FR_OK)
		return intSec;
		
	bool found = false;
	
	while(!f_error(&file) && !f_eof(&file))
	{
		f_gets(line, LOAD_BYTE_COUNT, &file);
		
		short currentID = GetIntFromCSV(line, INDEX_ID);

		if(currentID == id)
		{
			found = true;
			break;
		}
	}
	
	if(!found)
	{
		f_close(&file);
		return intSec;
	}
	intSec = GetIntersectionFromCSV(line);
	f_close(&file);
	return intSec;
}

/*
intersection* LoadNearbyIntersections(float currentLat, float currentLng, intersection retArray[], short count)
{
	
	intersection* intSecArray = malloc(sizeof(intersection)*count);
	
	for(short i = 0; i < count; i++)
		intSecArray[i].name = NULL;
		
	FIL file;
	char line[LOAD_BYTE_COUNT];
	

	gpsPoint current = {.lat = currentLat, .lng = currentLng};
	gpsPoint reading = {.lat = currentLat, .lng = currentLng};

	
	// Scan file looking for nearest intersection
	while(!f_error(&file) && !f_eof(&file))
	{
		f_gets(line, LOAD_BYTE_COUNT, &file);
		
		if(StrLen(line, 30) < 30)
			continue;
			
		reading.lat = GetFloatFromCSV(line, INDEX_LAT);
		reading.lng = GetFloatFromCSV(line, INDEX_LNG);
		
		//float distance = DistanceBetweenPoints(current, reading);
		//if(distance < lowestDistance)
		//{
		//	lowestDistance = distance;
		//	if(lowestDistance != FLT_MAX)
		//		DeleteIntersection(intSec);
		//	intSec = GetIntersectionFromCSV(line);
		//}
		
	}

	char example = 0xFF;
	example &= (1 << 4);
	
	return retArray;
}
*/
void LoadLinkedIntersections(intersection* intSec)
{
		intSec->nearbyIntersections = malloc(sizeof(intersection)*(intSec->nearbyIntersectionCount));
		short filled = 0;
		for(short i = 0; i < intSec->nearbyIntersectionCount; i++)
		{
			intSec->nearbyIntersections[i].name = NULL;
			intSec->nearbyIntersections[i].nearbyIntersections = NULL;
			intSec->nearbyIntersections[i].nearbyIntersectionsID = NULL;
		}
		
		FIL file;
		char line[LOAD_BYTE_COUNT];
		
		FRESULT fr = f_open(&file, INTERSECTION_CSV_NAME, FA_READ);
		if(fr != FR_OK)
			return;
		
		// Scan file looking for nearest intersection
		while(!f_error(&file) && !f_eof(&file))
		{
			f_gets(line, LOAD_BYTE_COUNT, &file);
			
			if(StrLen(line, 30) < 30)
				continue;
			
			short currentID = GetIntFromCSV(line, INDEX_ID);
			
			bool linkedIntersection = false;
			for(short i = 0; i < (intSec->nearbyIntersectionCount); i++)
				linkedIntersection |= (currentID == intSec->nearbyIntersectionsID[i]);
			if(!linkedIntersection)
				continue;
			intSec->nearbyIntersections[filled] = GetIntersectionFromCSV(line);
			filled++;
		}
		f_close(&file);	
}
intersection LoadNearestIntersection(float lat, float lng)
{
	FIL file; // file object
	char line[LOAD_BYTE_COUNT];
	intersection intSec;
	
	intSec.name = NULL;
	intSec.nearbyIntersectionsID = NULL;
	intSec.nearbyIntersections = NULL;
	
	gpsPoint current = {.lat = lat, .lng = lng, .locatedTime = 0};
	gpsPoint reading;
	
	float lowestDistance = FLT_MAX;
	
	FRESULT fr = f_open(&file, INTERSECTION_CSV_NAME, FA_READ);
	if(fr != FR_OK)
		return intSec;
	
	// Scan file looking for nearest intersection
	while(!f_error(&file) && !f_eof(&file))
	{
		f_gets(line, LOAD_BYTE_COUNT, &file);
		
		if(StrLen(line, 30) < 30)
			continue;
			
		reading.lat = GetFloatFromCSV(line, INDEX_LAT);
		reading.lng = GetFloatFromCSV(line, INDEX_LNG);
		
		float distance = DistanceBetweenPoints(current, reading);
		if(distance < lowestDistance)
		{
			lowestDistance = distance;
			DeleteIntersection(intSec);
			intSec = GetIntersectionFromCSV(line);
		}
		
	}
	f_close(&file);	
	return intSec;
}
intersection GetIntersectionFromCSV(char* csvEntry)
{
	intersection intSec;
	intSec.id = GetIntFromCSV(csvEntry, INDEX_ID);
	intSec.name = GetCSVIndex(csvEntry, INDEX_NAME);
	intSec.lat = GetFloatFromCSV(csvEntry, INDEX_LAT);
	intSec.lng = GetFloatFromCSV(csvEntry, INDEX_LNG);
	intSec.time = GetIntFromCSV(csvEntry, INDEX_TIME);
	intSec.nearbyIntersectionCount = GetIntFromCSV(csvEntry, INDEX_LINKED_COUNT);
	intSec.nearbyIntersectionsID = GetLinkedIntersections(csvEntry,intSec.nearbyIntersectionCount);
	intSec.nearbyIntersections = NULL;
	return intSec;
}
char* GetCSVIndex(char* csvEntry, short index)
{
	short commaCount = 0;
	index++;
	short base = 0;
	char* result = NULL;

	short i = 0;
	while (i > -1)
	{
		if (csvEntry[i] == ',' || csvEntry[i] == '\0')
		{
			commaCount++;
			if (commaCount == (index - 1))
			base = i+1;
		}
		if (commaCount == index || csvEntry[i] == '\0')
		break;
		i++;
	}
	int size = i - base + 1;
	
	if (commaCount < index || size < 0)
	return result;

	result = malloc(sizeof(char)*(size));
	memcpy(result, csvEntry + base, size);
	result[size-1] = '\0';
	return result;
}
int StrLen(char* str, int maxLength)
{
	for(int i = 0; i < maxLength; i++)
	{
		if(str[i] == '\0')
			return i;
	}
	return maxLength;
}
float GetFloatFromCSV(char* csvEntry, short index)
{
	char* temp = GetCSVIndex(csvEntry, index);
	float res = atof(temp);
	free(temp);
	return res;
}
int GetIntFromCSV(char* csvEntry, short index)
{
	char* temp = GetCSVIndex(csvEntry, index);
	int res = atoi(temp);
	free(temp);
	return res;
}
short* GetLinkedIntersections(char* csvEntry, short count)
{
	if(count > 0)
	{
		short* nearby = malloc(sizeof(count)*count);
		
		for(unsigned short i = 0; i < count; i++)
		{
			nearby[i] = GetIntFromCSV(csvEntry, INDEX_LINKED_INTERSECTION_BASE + i);
		}
		return nearby;
	}
	return NULL;
}

