#include "DXRenderer.h"

/*******************************************************************
* Constructor
*******************************************************************/
DXRenderer::DXRenderer() :	pD3DDevice(0),
							pSwapChain(0),
							pRenderTargetView(0),
							pEffect(0),
							pVertexBuffer(0),
							pVertexLayout(0)
{}
/*******************************************************************
* Destructor
*******************************************************************/
DXRenderer::~DXRenderer()
{
	if ( pRenderTargetView ) pRenderTargetView->Release();
	if ( pSwapChain ) pSwapChain->Release();
	if ( pD3DDevice ) pD3DDevice->Release();	
	if ( pEffect ) pEffect->Release();
	if ( pVertexBuffer ) pVertexBuffer->Release();
	if ( pVertexLayout ) pVertexLayout->Release();
}
/*******************************************************************
* Initializes Direct3D Device
*******************************************************************/
bool DXRenderer::Initialize( HWND* hW )
{
	//window handle
	hWnd = hW;
	
	RECT rc;
    GetClientRect( *hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;			

	// Initialize all stages and components
	//*****************************************************************************
	if ( !CreateSwapChainAndDevice(width, height) ) return false;
	if ( !LoadShadersAndCreateInputLayouts() ) return false;			
	if ( !CreateViewports(width, height) ) return false;
	if ( !CreateRenderTargets() ) return false;
		
	//everything completed successfully
	return true;
}
/*******************************************************************
* Create Swap Chain and D3D device
*******************************************************************/
bool DXRenderer::CreateSwapChainAndDevice( UINT width, UINT height )
{
	//Set up DX swap chain
	//--------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	
	//set buffer dimensions and format
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;;
	
	//set refresh rate
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	
	//sampling settings
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;

	//output window handle
	swapChainDesc.OutputWindow = *hWnd;
	swapChainDesc.Windowed = true;    

	//Create the D3D device
	//--------------------------------------------------------------
	if ( FAILED( D3D10CreateDeviceAndSwapChain(		NULL, 
													D3D10_DRIVER_TYPE_HARDWARE, 
													NULL, 
													0, 
													D3D10_SDK_VERSION, 
													&swapChainDesc, 
													&pSwapChain, 
													&pD3DDevice ) ) ) return FatalError("D3D device creation failed");
	return true;
}
/*******************************************************************
* Set up Viewports
*******************************************************************/
bool DXRenderer::CreateViewports( UINT width, UINT height )
{	
	//create viewport structure	
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	pD3DDevice->RSSetViewports(1, &viewPort);
	return true;
}
/*******************************************************************
* Create Rendering Targets
*******************************************************************/
bool DXRenderer::CreateRenderTargets()
{
	//try to get the back buffer
	ID3D10Texture2D* pBackBuffer;	
	if ( FAILED( pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*) &pBackBuffer) ) ) return FatalError("Could not get back buffer");

	//try to create render target view
	if ( FAILED( pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView) ) ) return FatalError("Could not create render target view");
	
	pBackBuffer->Release();
	pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	return true;
}
/*******************************************************************
* Shader Loader
*******************************************************************/
bool DXRenderer::LoadShadersAndCreateInputLayouts()
{
	if ( FAILED( D3DX10CreateEffectFromFile(	"GS.fx", 
												NULL, NULL, 
												"fx_4_0", 
												D3D10_SHADER_ENABLE_STRICTNESS, 
												0, 
												pD3DDevice, 
												NULL, 
												NULL, 
												&pEffect, 
												NULL, 
												NULL	) ) ) return FatalError("Could not load effect file!");	

	pTechnique = pEffect->GetTechniqueByName("RENDER");
	if ( pTechnique == NULL ) return FatalError("Could not find specified technique!");	
	pTechnique->GetDesc( &techDesc );

	//define input layout
	D3D10_INPUT_ELEMENT_DESC layout[] = 
	{	
		{ "ANCHOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },	
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//create input layout
	D3D10_PASS_DESC PassDesc;
	pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	if ( FAILED( pD3DDevice->CreateInputLayout( layout, 
												numElements, 
												PassDesc.pIAInputSignature,
												PassDesc.IAInputSignatureSize, 
												&pVertexLayout ) ) ) return FatalError("Could not create Input Layout!");

	// Set the input layout
	pD3DDevice->IASetInputLayout( pVertexLayout );

	return true;
}
/*******************************************************************
* Create Vertex Buffer
*******************************************************************/
bool DXRenderer::CreateVertexBuffer(const unsigned int elementByteSize, const unsigned int numElements )
{
	//free old buffer
	if ( pVertexBuffer != 0 )
	{
		pVertexBuffer->Release();
	}
	
	//create new buffer
	D3D10_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D10_USAGE_DYNAMIC;
	bd.ByteWidth = elementByteSize * numElements;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	
	if ( FAILED( pD3DDevice->CreateBuffer( &bd, 0, &pVertexBuffer ) ) ) return FatalError("Could not create vertex buffer!");;

	// Set vertex buffer
	UINT stride = elementByteSize;
	UINT offset = 0;
	pD3DDevice->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );

	return true;
}
/*******************************************************************
* Scene Renderer
*******************************************************************/
void DXRenderer::RenderScene( TileRenderGraph &renderGraph )
{
	//clear scene
	pD3DDevice->ClearRenderTargetView( pRenderTargetView, D3DXCOLOR(0.3f,0.3f,0.3f,1) );		
	
	//set topology
	pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	//set tile size
	ID3D10EffectVectorVariable* pTileWidth = pEffect->GetVariableByName("tileWidth")->AsVector();
	pTileWidth->SetFloatVector( &renderGraph.tileSize.x );

	//fill vertex buffer with tile data
	Tile* pTile = 0;
	pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**) &pTile);

	for ( unsigned int i=0; i < renderGraph.numTiles; i++)
	{
		pTile[i] = renderGraph.pTiles[i];	
	}

	pVertexBuffer->Unmap();

	//render vertex buffer	
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{		
		//apply technique			
		pTechnique->GetPassByIndex( p )->Apply( 0 );	

		//draw billboard
		pD3DDevice->Draw(renderGraph.numTiles,0);
	}

	//flip buffers
	pSwapChain->Present(0,0);
}
/*******************************************************************
* Fatal Error Handler
*******************************************************************/
bool DXRenderer::FatalError(const LPCSTR msg)
{
	MessageBox(*hWnd, TEXT(msg), "Fatal Error!", MB_ICONERROR);
	return false;
}