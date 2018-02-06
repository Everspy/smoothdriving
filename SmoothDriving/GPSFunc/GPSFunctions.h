/*
 * GPSFunctions.h
 *
 * Created: 11/18/2017 4:13:47 PM
 *  Author: Matt
 */ 

#ifndef GPSFUNCTS_H_
#define GPSFUNCTS_H_

#ifndef PI
#define PI 3.14159f
#endif

#define EARTHRADIUSKM 6371

#include <math.h>

typedef struct _gpsPoint
{
	float lat;
	float lng;
	float locatedTime;
} gpsPoint ;

typedef struct _velocity
{
	float speed; // speed in m/s
	float direction; // direction in radians. goes east = 0 radians, increasing counter clockwise
} velocity ;

/**
    The DistanceBetweenPoints() function returns distances between point a and b in metres.
 */
float DistanceBetweenPoints(gpsPoint a, gpsPoint b);

/**
    The GetSpeed() function returns average speed travelled between point a and b in time seconds. Returned value is m/s
 */

float GetSpeed(gpsPoint a, gpsPoint b);

/**
	The GetVelocity() function returns a velocity based on the distance travelled between the two passed gps points
  */
velocity GetVelocity(gpsPoint a, gpsPoint b);

/**
    The DegreesToRadians() function converts degrees to radians as a float value.
 */
float DegreesToRadians(float degrees);

/**
    The ConvertSpeedToMPH() function converts speed in m/s to miles per hour;
 */
float ConvertSpeedToMPH(float speed);

#endif