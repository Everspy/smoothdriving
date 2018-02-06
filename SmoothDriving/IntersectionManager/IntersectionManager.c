/*
 * IntersectionManager.c
 *
 * Created: 11/28/2017 8:28:25 PM
 *  Author: Matt
 */ 

 #include "IntersectionManager.h"

 intersection GetNextIntersection(gpsPoint currentLocation, velocity heading)
 {
	intersection nextIntersection;

	return nextIntersection;
 }

 void DeleteIntersection(intersection interSect)
 {
	if(interSect.name != NULL)
		free(interSect.name);
	if(interSect.nearbyIntersections != NULL)
		free(interSect.nearbyIntersections);
 }