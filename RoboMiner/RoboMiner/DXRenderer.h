#ifndef _DX_RENDERER
#define _DX_RENDERER

#include <windows.h>
#include <D3D10.h>
#include <D3DX10.h>

#include "TileRenderGraph.h"

class DXRenderer
{
	//window handle shortcut
	HWND*						hWnd;
	
	//device vars
	ID3D10Device*				pD3DDevice;
	IDXGISwapChain*				pSwapChain;
	ID3D10RenderTargetView*		pRenderTargetView;
	D3D10_VIEWPORT				viewPort;

	//input layout and vertex buffer
	ID3D10Buffer*				pVertexBuffer;
	ID3D10InputLayout*			pVertexLayout;

	//effects and techniques
	ID3D10Effect*				pEffect;
	ID3D10EffectTechnique*		pTechnique;
	D3D10_TECHNIQUE_DESC		techDesc;
				
	/*******************************************************************
	* Methods
	********************************************************************/	
public:

	//constructor and destructor
	DXRenderer();
	~DXRenderer();

	//initialize directx device
	bool Initialize(HWND* hWnd);	

	//create vertex buffer
	bool CreateVertexBuffer(const unsigned int elementByteSize, const unsigned int numElements);
	
	//scene function
	void RenderScene( TileRenderGraph &renderGraph );	
	
private:

	//initialization methods
	bool CreateSwapChainAndDevice( UINT width, UINT height );
	bool LoadShadersAndCreateInputLayouts();
	bool CreateViewports( UINT width, UINT height );	
	bool CreateRenderTargets();	

	//fatal error handler
	bool FatalError(const LPCSTR msg); 
};

#endif