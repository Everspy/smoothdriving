/*
 * RecordLoader.h
 *
 * Created: 2/5/2018 3:39:57 PM
 *  Author: Matt
 */ 


#ifndef RECORDLOADER_H_
#define RECORDLOADER_H_

#include "../IntersectionManager/IntersectionManager.h"
#include "../FatFs/diskio.h"
#include "../FatFs/ff.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
    The InitializeStorageDevice() function prepares the necessary timers and attempts to mount the SD card.
	If the initialization fails, the function returns false. Otherwise true is returned
 */
bool InitializeStorageDevice();

/**
    The MountStorageDevice() function attempts to mount the SD card. returns true if successful. False otherwise
 */
bool MountStorageDevice();

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
    The LoadNearbyIntersections() function returns 'count' number of nearest intersections in the 'retArray' array
	retArray must be allocated by caller and have 'count' number of spaces.
 */
intersection* LoadNearbyIntersections(float currentLat, float currentLng, intersection retArray[], short count);

/**
    The LoadNearestIntersection() function returns the intersection closest to the passed coords.
	Note: This does not load linked intersections. Only the IDs of the linked intersections.
 */
intersection LoadNearestIntersection(float lat, float lng);

/**
	The LoadLinkedIntersections() function fills in the passed intersections nearbyIntersection field
 */
void LoadLinkedIntersections(intersection* intSec); 
#endif /* RECORDLOADER_H_ */