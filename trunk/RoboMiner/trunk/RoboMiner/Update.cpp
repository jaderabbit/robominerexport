#include "GameState.h"
#include <sstream>

using namespace std;

void GameState::Update()
{	
	
	//Perform updates
	m.recruitmentAlgorithmStep();

	/*if ( m.cnt >= m.MAX_ITERATIONS ) {
		//Output Message
		stringstream msg;
		msg << "End statistics" << endl;

		MessageBox( *pHWnd, LPCSTR(msg.str().c_str()), "Search Result", 0 );
	}*/
}

