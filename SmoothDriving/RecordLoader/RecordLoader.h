/*
 * RecordLoader.h
 *
 * Created: 2/5/2018 3:39:57 PM
 *  Author: Matt
 */ 


#ifndef RECORDLOADER_H_
#define RECORDLOADER_H_

#include <stdbool.h>

#include "../IntersectionManager/IntersectionManager.h"
#include "../FatFs/diskio.h"
#include "../FatFs/ff.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
/**
    The InitializeStorageDevice() function prepares the SD card for reading.
	If the initialization fails, the function returns false. Otherwise true is returned
 */
bool InitializeStorageDevice();

/**
    The IsStorageDetected() function returns true if a SD card is detected. False otherwise
	Currently not functional
 */
bool IsStorageDetected();
/**
    The LoadRecordByID() function returns the intersection object with the passed ID.
	If the intersection does not exists, the name will be NULL
 */
intersection LoadRecordByID(unsigned short id);

/**
    The LoadNearestIntersection() function returns the intersection closest to the passed coords
 */
intersection LoadNearestIntersection(float lat, float lng);



#endif /* RECORDLOADER_H_ */