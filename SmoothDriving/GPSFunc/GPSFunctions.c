/*
 * GPSFunctions.c
 *
 * Created: 11/18/2017 4:10:44 PM
 *  Author: Matt
 */ 

#include "GPSFunctions.h"

/**
    The DegreesToRadians() function converts degrees to radians anb returns as a float value.
 */
float DegreesToRadians(float degrees);

/**
    The RadiansToDegrees() function converts radians to degrees and returns as a float value.
 */
float RadiansToDegrees(float radians);

float DistanceBetweenPoints(gpsPoint a, gpsPoint b)
{
	float latDist = DegreesToRadians(a.lat - b.lat);
	float lngDist = DegreesToRadians(b.lng - b.lng);
	
	float lata = DegreesToRadians(a.lat);
	float latb = DegreesToRadians(b.lat);

	float aVal = sin(latDist / 2) * sin(latDist / 2) + sin(lngDist / 2) * sin(lngDist / 2) * cos(lata) * cos(latb);
	float scalar = 2 * atan2(sqrt(aVal), sqrt(1-aVal));

	return scalar*EARTHRADIUSKM*1000;
}

//float DistanceBetweenPoints(gpsPoint a, gpsPoint b)
//{
//	return DistanceBetweenPoints(a.lat, a.lng, b.lat, b.lng);
//}
//
//float DistanceBetweenPoints(float aLat, float aLng, float bLat, float bLng)
//{
//	float latDist = DegreesToRadians(aLat - bLat);
//	float lngDist = DegreesToRadians(aLng - bLng);
//	
//	float lata = DegreesToRadians(aLat);
//	float latb = DegreesToRadians(bLat);
//
//	float aVal = sin(latDist / 2) * sin(latDist / 2) + sin(lngDist / 2) * sin(lngDist / 2) * cos(lata) * cos(latb);
//	float scalar = 2 * atan2(sqrt(aVal), sqrt(1-aVal));
//
//	return scalar*EARTHRADIUSKM*1000;
//}

float GetSpeed(gpsPoint a, gpsPoint b)
{
	return (float)DistanceBetweenPoints(a,b)/abs(a.locatedTime - b.locatedTime);
}

velocity GetVelocity(gpsPoint start, gpsPoint end)
{
	velocity v;
	v.speed = GetSpeed(start,end);
	v.direction = GetDirection(start, end);
	return v;
}
float GetDirection(gpsPoint start, gpsPoint end)
{
	float lat = (end.lat - start.lat);
	float lng = (end.lng - start.lng);
	return RadiansToDegrees(atan2(lng,lat));
}
float DegreesToRadians(float degrees)
{
	return degrees*PI/180;
}
float RadiansToDegrees(float radians)
{
	return radians * 180 / PI;
}
float ConvertSpeedToMPH(float speed)
{
	return speed * 2.23694;
}