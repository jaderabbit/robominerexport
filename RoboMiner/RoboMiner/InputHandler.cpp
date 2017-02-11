#include "GameState.h"
#include "CoordinateConversions.h"

/*******************************************************************
* Helper Functions
*******************************************************************/
bool GameState::TileClicked( int2 mousePos, unsigned int &tileIndex )
{
	float2 mp = ConvertDimensionsSSToCS(resolution.x, resolution.y, mousePos);
	
	//get tile indices
	mp.x /= trg.tileSize.x;
	mp.y /= trg.tileSize.y;

	if ( mp.x >= 0 && mp.y >= 0 && mp.x <= m.size.x && mp.y <= m.size.y )
	{
		tileIndex = (unsigned int) (floor(mp.y) * m.size.x + floor(mp.x));
		return true;
	}
	else 
	{
		tileIndex = -1;
		return false;	
	}
}

//input handler
void GameState::HandleInput( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message) 
	{
		case WM_LBUTTONUP :
		{
			//get mouse position and check for tile selection
			int2 mousePos( LOWORD(lParam), HIWORD(lParam) );
			
			//if traversable tile clicked
			unsigned int selectedTile = 0;
			if ( TileClicked( mousePos, selectedTile ))// && navgraph.pNodes[selectedTile] == 0 )
			{								
				//add to search problem
				if ( searchProblem.x == -1 ) searchProblem.x = selectedTile;
				else if ( searchProblem.x != selectedTile ) searchProblem.y = selectedTile;						
			}			
		}
		break;
	}
}