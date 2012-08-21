#include "GameState.h"
#include "CoordinateConversions.h"
#include "FatalError.h"

using namespace std;

//constructor
GameState::GameState() :	pHWnd(0), m(80,80,20,4,"single.txt")
{
}
//destructor
GameState::~GameState()
{
}
//initialize function
bool GameState::Initialize( HWND* pWnd, int2 res )
{
	//set game settings
	pHWnd = pWnd;
	resolution = res;

	//initialize renderer
	if ( !renderer.Initialize(pWnd) ) return FatalError(*pWnd, "renderer init failed");		

	return true;
}
//load level
bool GameState::LoadLevel( const char* filename, int width, int height, int num_robots, int ratio_foragers_to_explorers )
{
	//load input cluster file
	if (!m.load(width,height,num_robots,ratio_foragers_to_explorers,filename)) FatalError(*pHWnd,"Grid was unsuccesfully loaded");

	//if render graph exists reset render graph 
	if ( trg.pTiles != 0 )
	{
		delete[] trg.pTiles;
		trg.numTiles = 0;
		trg.tileSize = float2(0,0);
	}

	//calculate tile size fit to window
	//TODO: Replace navgraph.height with gridsize.x and gridsize.y
	int v = (int) ( (float) resolution.y / m.size.y );
	int h = (int) ( (float) resolution.x / m.size.x );	
	int2 tileDimensions;

	if ( v < 1 || h < 1 ) tileDimensions = int2(1,1);
	else if ( v < h ) tileDimensions = int2(v,v);
	else tileDimensions = int2(h,h);

	//create tile render graph	
	trg.tileSize = ConvertDimensionsSSToCS(resolution.x, resolution.y, tileDimensions);
	trg.numTiles =  m.size.y*m.size.x;		
	trg.pTiles = new Tile[trg.numTiles];
	
	//fill with tiles
	unsigned int cnt = 0;
	unsigned int color = 0;
	float tx=-1, ty=1;

	//TODO: Replace navgraph with Mine.grid
	for ( int y=0; y <  m.size.y; y++ )
	{
		for ( unsigned int x=0; x < m.size.x; x++ )
		{
			trg.pTiles[cnt].topLeft = float2(tx,ty);
			trg.pTiles[cnt++].color = float3(0,0,0);
			tx += trg.tileSize.x;			
		}

		tx = -1;
		ty -= trg.tileSize.y;
	}

	//generate 255 random colors
	for ( unsigned int i=0; i < 255; i++)
	{
		float r = (float)rand() / RAND_MAX;
        float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;

		rndColors.push_back( float3( r,g,b));
	}

	//create vertex buffer
	if ( !renderer.CreateVertexBuffer(sizeof(Tile),m.size.y*m.size.x) ) return FatalError(*pHWnd, "Vertex Buffer creation failed!");

	return true;
}
//render grid map and visualizations
void GameState::RenderScene()
{
	int count =0;
	for ( unsigned int y=0; y <  m.size.y; y++ )
	{
		for ( unsigned int x=0; x < m.size.x; x++ )
		{
			switch ( m.grid[x][y].type ) {
				case ROBOT: trg.pTiles[count].color = float3(1,0,1); 
							
							switch ( m.robots[m.grid[x][y].index].activity ) {
								case CLUSTER: trg.pTiles[count].color = float3(1,1,0); break;
								case EXPLORE: trg.pTiles[count].color = float3(0,1,1); break;
								case FORAGE: trg.pTiles[count].color = rndColors[m.robots[m.grid[x][y].index].state]; break;
							}
							break;
				case GOLD: trg.pTiles[count].color= float3(0,0,1);break;
				case WASTE: trg.pTiles[count].color= float3(0,1,0);break;
				case SINK: trg.pTiles[count].color= float3(1,0,0);break;
				case EMPTY:  trg.pTiles[count].color= float3(1,1,1);break;
			}
			count++;
		}
	}
	/*
	if ( navgraph.pNodes )
	{	
		//render grid map
		for ( unsigned int i=0; i < trg.numTiles; i++ )
		{
			if ( navgraph.pNodes[i] == 0 ) trg.pTiles[i].color = float3(1,1,1);
			else trg.pTiles[i].color = float3(0,0,0);
		}

		//draw node visualizations	
		for ( unsigned int p=0; p < nodeVisualizations.size(); p++ )
		{		
			std::list<unsigned int>::iterator i = nodeVisualizations[p].nodes.begin();

			while ( i != nodeVisualizations[p].nodes.end() )
			{
				trg.pTiles[*i].color = nodeVisualizations[p].color;
				++i;
			}		
		}*/

	
		//render scene
		renderer.RenderScene(trg);
}