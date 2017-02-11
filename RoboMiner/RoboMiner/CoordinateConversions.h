#ifndef _COORD_CONV
#define _COORD_CONV

#include "Types.h"

inline float2 ConvertDimensionsSSToCS( const int screenX, const int screenY, const int2 pixelValues )
{
	return float2( (float)pixelValues.x/screenX*2, (float)pixelValues.y/screenY*2);
}

inline float2 ConvertCoordsSSToCS( const int screenX, const int screenY, const int2 pixelCoordinates )
{
	return float2( (float)pixelCoordinates.x/screenX*2 - 1, (float)pixelCoordinates.y/screenY*2 );
}

inline float ConvertPixelsToClipSpaceDistance( const int pixelDimension, const int pixels )
{
	return (float)pixels/pixelDimension*2;
}

#endif