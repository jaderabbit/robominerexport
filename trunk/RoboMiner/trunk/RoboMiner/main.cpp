#include <windows.h>
#include <tchar.h>

//include DX renderer
#include "CoordinateConversions.h"
#include "DXRenderer.h"

//include game state
#include "GameState.h"
#include "FatalError.h"

//include STL libraries
#include <vector>

/*******************************************************************
* Global Variables
*******************************************************************/

//window variables
HWND hWnd;					//window handle
int windowWidth = 900;	
int windowHeight = 900;	

//gamestate and renderer
GameState game;

/*******************************************************************
* Main Window Procedure - handles application events
*******************************************************************/
LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		// Allow the user to press the escape key to end the application
        case WM_KEYDOWN	:	switch(wParam)
							{								
								case VK_ESCAPE : PostQuitMessage(0);
								break;
							}
		
        break;		

		case WM_DESTROY	: PostQuitMessage(0); break;	

		default :
		{			
			game.HandleInput( message, wParam, lParam );
		}	
		break;	
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*******************************************************************
* Initialize Main Window
********************************************************************/
bool InitWindow(HWND &hWnd, HINSTANCE hInstance, int width, int height)
{
	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)wndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= TEXT("PFVISTOOLKIT");
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);
	
	//Resize the window
	RECT rect = { 0, 0, width, height };
    AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);

	//create the window from the class defined above	
	hWnd = CreateWindow( "PFVISTOOLKIT", 
						 "NOMAD - Pathfinding Visualization Toolkit", 
						 WS_OVERLAPPED |  WS_SYSMENU | WS_MINIMIZEBOX,
						 0, 
						 0, 
						 rect.right - rect.left, 
						 rect.bottom - rect.top, 
						 NULL, 
						 NULL, 
						 hInstance, 
						 NULL);
   
	//window handle not created
	if (!hWnd) return false;

	//if window creation was successful
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);	
	
	return true;
}

/*******************************************************************
* WinMain
*******************************************************************/
int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	//set up the application window
	if ( !InitWindow(hWnd, hInstance, windowWidth, windowHeight)) return FatalError(hWnd, "Could not Create Window!");
			
	//Initialize experiment + environment
	
	EXPERIMENT_DESC d;
	d.width = 50; d.height = 50;
	d.number_objects = 700;
	d.number_robots = 10;
	d.gold_waste_ratio = 1;
	d.forager_explorer_ratio = 0.7;
	d.total_iterations = 4000;
	d.gold_waste_division_ratio = 0.9;
	d.max_path = 50;
	d.samples = 19;

	ENVIRONMENT_DESC e;
	e.grid_size = 50;
	e.num_objects = 20;
	e.ratio_gold = 0.5;
	e.type = "clustered";
	e.sink_boundary = 5;

	//intialize game
	if ( !game.Initialize(&hWnd, int2(windowWidth, windowHeight),e,d) ) return FatalError(hWnd, "Game init failed!");
	
	//load map
	if ( !game.LoadLevel("clustered_test.txt",80,80,20,3) ) return FatalError(hWnd, "Level Load failed!");
	
	//main message loop
    MSG msg = {0};
	long t = 0;

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);			
		}
		else
		{		
			if ( t % 50 ==0 ) {
				game.Update();
			}
			t++;
			game.RenderScene();
		}
    }

	//cleanup	
	return (int) msg.wParam;
}


/*#include "Mine.h" 
#include <iostream>
#include "FrequencyHistogramTool.h"
#include <sstream>

using namespace std;

//18/06/2012 - restarted working on RoboMiner. Attempting to locate where I left it off. Will document such things from now on. 

int main() {
	//Mine mine(27,41,8,4,"goldonly.txt");
	//Mine mine(80,80,20,4,"single.txt");
	Mine mine(80,80,20,4,"clustered_test.txt");
	mine.outputAdvanced();
	mine.recruitmentAlgorithm();
	//mine.outputAdvanced(STATE_OUTPUT);

	int s = 0;
	while ( s < 4 ) {
		cout << "Enter display type: 0 = STATE, 1 = INDEX, 2 = ACTIVITY, 3 = LOADED" << endl;
		cin >> s;
		mine.outputAdvanced(s);
	}

	mine.robotTrackerOutput();

	FrequencyHistogramTool f(80,80);
	for (int i=0; i < 20; i++) {
		stringstream sIn;
		stringstream sOut;
		sIn << "tracker_robot_" << i << ".csv";
		sOut << "histogram_robot" << i << ".csv";
		f.createFrequencyHistogramData(sIn.str(),sOut.str());
	}

	int k=0;

}*/
