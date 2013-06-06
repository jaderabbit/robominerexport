#include "GameState.h"
#include "CoordinateConversions.h"
#include "FatalError.h"
#include <fstream>
#include <string> 
#include "DesertAntForage.h"

using namespace std;

//constructor
GameState::GameState() :	pHWnd(0)
{
}
//destructor
GameState::~GameState()
{
}
//initialize function
bool GameState::Initialize( HWND* pWnd, int2 res, ENVIRONMENT_DESC e, EXPERIMENT_DESC d )
{
	//set game settings
	pHWnd = pWnd;
	resolution = res;

	//initialize renderer
	if ( !renderer.Initialize(pWnd) ) return FatalError(*pWnd, "renderer init failed");		
	experiment = new BeeForage(d,e);

	return true;
}
//load level
bool GameState::LoadLevel( const char* filename, int width, int height, int num_robots, int ratio_foragers_to_explorers )
{
	//load input cluster file
	//if (!m.load(width,height,num_robots,ratio_foragers_to_explorers,filename,0)) FatalError(*pHWnd,"Grid was unsuccesfully loaded");
	experiment->initialize();

	//if render graph exists reset render graph 
	if ( trg.pTiles != 0 )
	{
		delete[] trg.pTiles;
		trg.numTiles = 0;
		trg.tileSize = float2(0,0);
	}

	//calculate tile size fit to window
	//TODO: Replace navgraph.height with gridsize.x and gridsize.y
	int v = (int) ( (float) resolution.y / experiment->mine.size.x );
	int h = (int) ( (float) resolution.x / experiment->mine.size.y );	
	int2 tileDimensions;

	if ( v < 1 || h < 1 ) tileDimensions = int2(1,1);
	else if ( v < h ) tileDimensions = int2(v,v);
	else tileDimensions = int2(h,h);

	//create tile render graph	
	trg.tileSize = ConvertDimensionsSSToCS(resolution.x, resolution.y, tileDimensions);
	trg.numTiles =  experiment->mine.size.x*experiment->mine.size.y;		
	trg.pTiles = new Tile[trg.numTiles];
	
	//fill with tiles
	unsigned int cnt = 0;
	unsigned int color = 0;
	float tx=-1, ty=1;

	//TODO: Replace navgraph with Mine.grid
	for ( int y=0; y <  experiment->mine.size.y; y++ )
	{
		for ( unsigned int x=0; x < experiment->mine.size.x; x++ )
		{
			trg.pTiles[cnt].topLeft = float2(tx,ty);
			trg.pTiles[cnt++].color = float3(0,0,0);
			tx += trg.tileSize.x;			
		}

		tx = -1;
		ty -= trg.tileSize.y;
	}

	//generate 255 random colors
	/*for ( unsigned int i=0; i < 255; i++)
	{
		float r = (float)rand() / RAND_MAX;
        float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;

		rndColors.push_back( float3( r,g,b));
	}*/

	//generate colours from file for debugging
	GenerateColoursFromFile("Colour Chart.txt");

	//create vertex buffer
	if ( !renderer.CreateVertexBuffer(sizeof(Tile),experiment->mine.size.y*experiment->mine.size.x) ) return FatalError(*pHWnd, "Vertex Buffer creation failed!");

	return true;
}

//render grid map and visualizations
void GameState::RenderScene()
{
	int count =0;
	for ( unsigned int y=0; y <  experiment->mine.size.y; y++ )
	{
		for ( unsigned int x=0; x < experiment->mine.size.x; x++ )
		{
			switch ( experiment->mine.grid[x][y].type ) {
				case ROBOT:	{
							int ind = experiment->mine.grid[x][y].index;
							switch (  experiment->robots[ind].state ) {
								case HOMING: trg.pTiles[count].color = float3(220.0/255.0,20.0/255.0,60.0/255.0); break; //RED
								case BEACON_HOMING: trg.pTiles[count].color = float3(225.0/255.0,97.0/255.0,3.0/255.0); break; //ORANGE
								case RECRUITING: trg.pTiles[count].color = float3(238.0/255.0,238.0/255.0,0.0/255.0); break; //YELLOW
								case EXPLORING: trg.pTiles[count].color = float3(0,238.0/255.0,0); break; //GREEN
								case WAITING: trg.pTiles[count].color = float3(0,0,1); break; //BLUE
								case LOCATING: trg.pTiles[count].color = float3(0,245.0/255.0,1); break; //CYAN
								case LOCAL_CLUSTER_SEARCH: trg.pTiles[count].color = float3(128.0/255.0,0,128.0/255.0); break; //PURPLE
								case LOADING: trg.pTiles[count].color = float3(0,0,0); break; //BLACK
								case UNLOADING: trg.pTiles[count].color = float3(1,0,1); break; //MAGENTA
								default :trg.pTiles[count].color = float3(0,0,0); break;
							};
							/*int ind = experiment->mine.grid[x][y].index;
							if ( experiment->robots[ind].activity == EXPLORE) {
									trg.pTiles[count].color = float3(139.0/255.0,117.0/255.0,0);break;
							} else {
								trg.pTiles[count].color= float3(70.0/255.0,40.0/255.0,70.0/255.0);break;	
							}
							trg.pTiles[count].color = rndColors[ind];
							*/
							/*switch (experiment->robots[ind].activity ) {
								case DESERTANT: trg.pTiles[count].color = float3(128.0/255.0,0,128.0/255.0); break;
								case EXPLORE: trg.pTiles[count].color = float3(0,1,1); break;
								case FORAGE: trg.pTiles[count].color = float3(1,0,1);; 
											if (experiment->robots[ind].state == WAITING) {
												 trg.pTiles[count].color = float3(0,0,1);
											}
											break;
								case BASICFORAGE:  trg.pTiles[count].color = float3(225.0/255.0,97.0/255.0,3.0/255.0); break;
							}*/

							
							break;}
				case GOLD: trg.pTiles[count].color= float3(139.0/255.0,117.0/255.0,0);break; //gold colour
				case WASTE: trg.pTiles[count].color= float3(40/255,40/255,40/255);break;				//waste colour
				case SINK: trg.pTiles[count].color= float3(1,0,0);break;
				case G_SINK: 
					trg.pTiles[count].color= float3(1,215.0/255,0);break;
				case W_SINK: trg.pTiles[count].color= float3(170/255,170/255,170/255);break;
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


void GameState::GenerateColoursFromFile( const char* filename) {
	ifstream in(filename);

	if ( in.is_open() ) {
		while ( in.good() ) {
			//get colour name for debugging purposes
			string colourName;
			getline(in,colourName,'\t');
			colourNames.push_back(colourName);

			//discard hex value
			string hex;
			getline(in,hex,'\t');

			//RGB
			int r,g,b;
			in >> r; in.ignore(1);in >> g; in.ignore(1);in >> b ;in.ignore(1);

			//ignore rest of line
			getline(in,colourName);

			//save color
			rndColors.push_back(float3(r/100.0,g/100.0,b/100.0));
		}
	}
}