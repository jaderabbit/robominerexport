#ifndef _GAME_STATE
#define _GAME_STATE

//system includes
#include <Windows.h>
#include "DXRenderer.h"
#include "TileRenderGraph.h"
#include "Types.h"
#include "Mine.h"

//include STL
#include <vector>
#include <list>

//visualization structs
struct NodeVisualization
{
	float3 color;	
	std::list<unsigned int> nodes;
};

//game state
class GameState
{

private:

	//system settings
	HWND* pHWnd;	
	int2 resolution;

	//Mine
	Mine m; 

	//pathfinding search algorithm
	int2 searchProblem;

	//Renderer
	DXRenderer renderer;
	TileRenderGraph trg;

	//Random colours
	vector<float3> rndColors;
	vector<string> colourNames;

public:

	//constructor
	GameState();
	~GameState();

	//initialization and shutdown
	bool Initialize( HWND* pWnd, int2 resolution);
	void Shutdown() {};

	//level loader
	bool LoadLevel( const char* filename, int width, int height, int num_robots, int ratio_foragers_to_explorers );

	//input handling
	void HandleInput( UINT message, WPARAM wParam, LPARAM lParam );
		
	//game updates function
	void Update();	

	//render graph functions
	void RenderScene();

	//Get colours from file
	void GenerateColoursFromFile( const char* filename);

private:
	
	bool TileClicked( int2 mousePos, unsigned int &tileIndex );	
};

#endif