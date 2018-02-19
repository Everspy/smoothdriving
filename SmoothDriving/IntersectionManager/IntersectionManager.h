/*
 * IntersectionManager.h
 *
 * Created: 11/28/2017 8:28:33 PM
 *  Author: Matt
 */ 

#ifndef INTERSECTIONMANAGER_H_
#define INTERSECTIONMANAGER_H_

#define NEARBYINTERSECTIONCOUNT 5

#include <stdlib.h>

#include "../GPSFunc/GPSFunctions.h"
#include "../GPSDecoder/GPSDecoder.h"
#include "../IntersectionManager/IntersectionManager.h"

typedef struct intersection intersection;

struct intersection
{
	float lat;
	float lng;
	char* name;
	short id;
	short time;
	short nearbyIntersectionCount;
	short* nearbyIntersectionsID;
	intersection* nearbyIntersections;
};


volatile intersection currentIntersection;

/**
    The InitIntersectionManager() function initializes the currentIntersection and any other necessary objects for the intersection manager
 */
void InitIntersectionManager(); 

/**
    The GetNextIntersection() function calculates which intersection the driver is heading towers, and returns that intersection
 */
intersection GetNextIntersection(gpsPoint currentLocation, velocity heading);

/**
    The DeleteIntersection() function frees the necessary memory taken by an intersection object
 */
void DeleteIntersection(intersection interSect);

/**
    The IntersectionDeepCopy() function performs a deep copy from intSrc to intDest
 */
void IntersectionDeepCopy(intersection * intDest, intersection * intSrc);
#endif
 