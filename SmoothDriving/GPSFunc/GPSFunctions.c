/*
 * GPSFunctions.c
 *
 * Created: 11/18/2017 4:10:44 PM
 *  Author: Matt
 */ 

#include "GPSFunctions.h"


float DistanceBetweenPoints(gpsPoint a, gpsPoint b)
{
	float latDist = DegreesToRadians(a.lat - b.lat);
	float lngDist = DegreesToRadians(a.lng - b.lng);
	
	float lata = DegreesToRadians(a.lat);
	float latb = DegreesToRadians(b.lat);

	float aVal = sin(latDist / 2) * sin(latDist / 2) + sin(lngDist / 2) * sin(lngDist / 2) * cos(lata) * cos(latb);
	float scalar = 2 * atan2(sqrt(aVal), sqrt(1-aVal));

	return scalar*EARTHRADIUSKM*1000;
}

float GetSpeed(gpsPoint a, gpsPoint b)
{
	return (float)DistanceBetweenPoints(a,b)/abs(a.locatedTime - b.locatedTime);
}

velocity GetVelocity(gpsPoint a, gpsPoint b)
{
	velocity v;
	v.speed = GetSpeed(a, b);
	
	float latMagnitude = fabs(a.lat - b.lat);
	float lngMagnitude = fabs(a.lng - b.lng);
	v.direction = atan(latMagnitude/lngMagnitude);
	return v;

}

float DegreesToRadians(float degrees)
{
	return degrees*PI/180;
}

float ConvertSpeedToMPH(float speed)
{
	return speed * 2.23694;
}