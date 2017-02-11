#ifndef _TILE
#define _TILE

#include "Types.h"

struct Tile
{
	float2 topLeft;
	float3 color;

	void operator= (Tile &rhs)
	{
		topLeft = rhs.topLeft;		
		color = rhs.color;
	}
};

struct TileRenderGraph
{
	Tile* pTiles;
	unsigned int numTiles;
	float2 tileSize;

	TileRenderGraph():pTiles(0),numTiles(0),tileSize(float2(0,0)){}
};

#endif
