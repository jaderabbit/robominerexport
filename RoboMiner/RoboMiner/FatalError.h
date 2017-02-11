#ifndef _FATAL_ERROR
#define _FATAL_ERROR

#include <Windows.h>

inline bool FatalError(HWND hWnd, char* msg )
{
	MessageBox(hWnd,  LPCSTR(msg), LPCSTR("FATAL ERROR!"), MB_ICONERROR);
	return false;
}

#endif