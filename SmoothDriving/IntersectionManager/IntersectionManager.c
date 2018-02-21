/*
 * IntersectionManager.c
 *
 * Created: 11/28/2017 8:28:25 PM
 *  Author: Matt
 */ 

 #include "IntersectionManager.h"
 #include "../RecordLoader/RecordLoader.h"

float GetAngleDifference(float angleA, float angleB);


void InitIntersectionManager()
{
	currentIntersection.name = NULL;
	currentIntersection.nearbyIntersections = NULL;
	currentIntersection.nearbyIntersectionsID = NULL;
}

 float GetAngleDifference(float angleA, float angleB)
 {
	 float result = fabs(angleA - angleB);
	 
	 // Ensuring that the correct distance is measured, since errors can happen around the +-pi area
	 if(result > PI)
		result = 2*PI - result;
		
	return result;
	 
 }
 intersection GetNextIntersection(gpsPoint currentLocation, velocity heading)
 {
	float closestAngleDifference = PI;
	int closestIndex = 0;


	for(short i = 0; i < currentIntersection.nearbyIntersectionCount; i++)
	{
		gpsPoint intersecLocation = {.lat = currentIntersection.nearbyIntersections[i].lat, .lng = currentIntersection.nearbyIntersections[i].lng };

		float currentAngle = GetDirection(currentLocation, intersecLocation); // Angle between user and the next intersection being analyzed
		currentAngle = GetAngleDifference(heading.direction, currentAngle); // Angle difference between user's current heading and angle between user and intersections

		if(currentAngle < closestAngleDifference)
		{
			closestAngleDifference = currentAngle;
			closestIndex = i;
		}
	}
	return currentIntersection.nearbyIntersections[closestIndex];
 }
 
 // update so all calls send refernce!
 void DeleteIntersection(intersection* interSect)
 {
	if(interSect->name != NULL)
	 	free(interSect->name);
	if(interSect->nearbyIntersectionsID != NULL)
		free(interSect->nearbyIntersectionsID);
	if(interSect->nearbyIntersections != NULL)
	{
		for(short i = 0; i < interSect->nearbyIntersectionCount; i++)
			DeleteIntersection(&(interSect->nearbyIntersections[i]));
		free(interSect->nearbyIntersections);
	}

 }

 // untested
 void IntersectionDeepCopy(intersection * intDest, intersection * intSrc)
 {
		intDest->id = intSrc->id;
		intDest->lat = intSrc->lat;
		intDest->lng = intSrc->lng;
		if(intSrc->name != NULL)
		{
			intDest->name = malloc(sizeof(BYTE) * strlen(intSrc->name));
			intDest->name = strcpy(intDest->name, intSrc->name);
		}
		else
			intDest->name = NULL;
			
		intDest->nearbyIntersectionCount = intSrc->nearbyIntersectionCount;
		
		if(intSrc->nearbyIntersections != NULL)
		{
			intDest->nearbyIntersections = malloc(sizeof(intersection)*intSrc->nearbyIntersectionCount);
			intDest->nearbyIntersections = memcpy(intDest->nearbyIntersections, intSrc->nearbyIntersections, sizeof(intersection)*intSrc->nearbyIntersectionCount);
		}
		else
			intDest->nearbyIntersections = NULL;
			
		if(intSrc->nearbyIntersectionsID != NULL)
		{
				intDest->nearbyIntersectionsID = malloc(sizeof(intSrc->nearbyIntersectionCount) * intSrc->nearbyIntersectionCount);
				intDest->nearbyIntersectionsID = memcpy(intDest->nearbyIntersectionsID, intSrc->nearbyIntersectionsID, sizeof(intSrc->nearbyIntersectionCount) * intSrc->nearbyIntersectionCount);
		}
		else
			intDest->nearbyIntersectionsID = NULL;
			
		intDest->time = intSrc->time;
 }
