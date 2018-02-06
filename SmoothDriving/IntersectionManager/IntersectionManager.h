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

typedef struct _intersection
{
	float lat;
	float lng;
	char* name;
	short id;
	short time;
	short nearbyIntersectionCount;
	short* nearbyIntersections;
} intersection ;


//intersection nearbyIntersections[] = {
//			{ .lat = 36.114572f, .lng = -115.137860f, .name = "Maryland/Flamingo" },
//			{ .lat = 36.114646f, .lng = -115.142249f, .name = "Claymont/Flamingo" },
//			{ .lat = 36.114691f, .lng = -115.146362f, .name = "Swenson/Flamingo" },
//			{ .lat = 36.107965f, .lng = -115.150402f, .name = "Swenson/Harmon" },
//			{ .lat = 36.107977f, .lng = -115.152008f, .name = "Paradise/Harmon" },
//};

intersection GetNextIntersection(gpsPoint currentLocation, velocity heading);
intersection LoadNearbyIntersection(gpsPoint currentLocation);
/**
    The DeleteIntersection() function frees the necessary memory taken by an intersection object
 */
void DeleteIntersection(intersection interSect);

#endif
 