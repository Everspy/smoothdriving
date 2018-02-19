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
#include <stdlib.h>

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
    The DistanceBetweenPoints() function returns distances between point a and b in metres.
	doesn't work because i forgot C doesn't allow function overloading
 */
//float DistanceBetweenPoints(float alat, float alng, float bLat, float bLng);

/**
    The ConvertSpeedToMPH() function converts speed in m/s to miles per hour;
 */
float ConvertSpeedToMPH(float speed);

/**
    The GetSpeed() function returns average speed travelled between point a and b in time seconds. Returned value is m/s
 */

float GetSpeed(gpsPoint a, gpsPoint b);

/**
	The GetVelocity() function returns a velocity based on the distance travelled between the two passed gps points
	For velocity:
	Note: -pi < return value < pi
	North ~= 0, East ~= pi/2, West ~= -pi/2, south ~= +-pi
	This is an approximation for small values. Larger distances may be effected by curvature of earth etc
  */
velocity GetVelocity(gpsPoint a, gpsPoint b);
/**
	The GetDirection() function returns a direction based on the the passed pointed.
	Note: -pi < return value < pi
	North ~= 0, East ~= pi/2, West ~= -pi/2, south ~= +-pi
	This is an approximation for small values. Larger distances may be effected by curvature of earth etc
  */
float GetDirection(gpsPoint start, gpsPoint end);
#endif