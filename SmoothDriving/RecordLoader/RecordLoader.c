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

#include "RecordLoader.h"

FATFS fs;
bool initialized = false;

/**
    The GetCSVIndex() function returns the value stored in a CSV string at the passed index.
	The index is 0 based. Returned result will be NULL if no result is found.
	The returned result must be freed to prevent a memory leak
 */
char* GetCSVIndex(char* csvEntry, short index);

ISR(TIMER1_COMPA_vect)
{
	disk_timerproc();
}



bool IsStorageDetected()
{
	return !(disk_status(0) && STA_NODISK);
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
	FRESULT res = f_mount(&fs,"0:",1);
	
	return res == FR_OK;
}

intersection LoadRecordByID(unsigned short id)
{
	FIL file; // file object
	char line[LOAD_BYTE_COUNT];
	intersection intSec;
	intSec.name = NULL;
	
	
	FRESULT fr = f_open(&file, INTERSECTION_CSV_NAME, FA_READ);
	
	if(fr != FR_OK)
		return intSec;
		
	bool found = false;
	
	while(!f_error(&file) && !f_eof(&file))
	{
		f_gets(line, LOAD_BYTE_COUNT, &file);
		char* idString = GetCSVIndex(line, INDEX_ID);
		if(idString == NULL)
			continue;
		
		unsigned short currentID = atoi(idString);
		free(idString);
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

	intSec.id = id;
	char* temp;
	// Get Latitude
	temp = GetCSVIndex(line, INDEX_LAT);
	intSec.lat = atof(temp);
	free(temp);
	
	// Get Longitude
	temp = GetCSVIndex(line, INDEX_LNG);
	intSec.lng = atof(temp);
	free(temp);
	
	// Get Time
	temp = GetCSVIndex(line, INDEX_TIME);
	intSec.time = atoi(temp);
	free(temp);

	// Get Name
	temp = GetCSVIndex(line, INDEX_NAME);
	intSec.name = temp;

	// Get linked intersection count
	temp = GetCSVIndex(line, INDEX_LINKED_COUNT);
	intSec.nearbyIntersectionCount = atoi(temp);
	free(temp);

	if(intSec.nearbyIntersectionCount > 0)
	{
		intSec.nearbyIntersections = malloc(sizeof(typeof(intSec.id))*intSec.nearbyIntersectionCount);
	
		for(unsigned short i = 0; i < intSec.nearbyIntersectionCount; i++)
		{
			temp = GetCSVIndex(line, INDEX_LINKED_INTERSECTION_BASE + i);
			intSec.nearbyIntersections[i] = atoi(temp);
			free(temp);
		}
	}

	f_close(&file);
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

