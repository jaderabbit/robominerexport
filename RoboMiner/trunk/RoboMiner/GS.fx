//--------------------------------------------------------------------------------------
// Tile Rendering Effect - 2011 - Bobby Anguelov
//--------------------------------------------------------------------------------------

//global variable
float2 tileWidth;

//VERTEX AND PIXEL SHADER INPUTS
//--------------------------------------------------------------------------------------
struct TILE_INPUT
{
	float2 topLeft : ANCHOR;
	float3 c: COLOR;
};
//pixel shader inputs
struct PS_INPUT
{
	float4 p : SV_POSITION; 
	float4 c : COLOR;	
};
//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
TILE_INPUT VS( TILE_INPUT input )
{
	return input;  
}
//--------------------------------------------------------------------------------------
// GEOMETRY SHADER
//--------------------------------------------------------------------------------------
[maxvertexcount(4)]
void GS( point TILE_INPUT tile[1], inout TriangleStream<PS_INPUT> triStream )
{
	PS_INPUT v;
	v.c = float4(tile[0].c,1);
		
	//create tile quad
	//--------------------------------------------
	v.p = float4(tile[0].topLeft[0],tile[0].topLeft[1]-tileWidth[1],0,1);	
	triStream.Append(v);

	v.p = float4(tile[0].topLeft[0],tile[0].topLeft[1],0,1);	
	triStream.Append(v);

	v.p = float4(tile[0].topLeft[0]+tileWidth[0],tile[0].topLeft[1]-tileWidth[1],0,1);		
	triStream.Append(v);

	v.p = float4(tile[0].topLeft[0]+tileWidth[0],tile[0].topLeft[1],0,1);		
	triStream.Append(v);
}
//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{ 	
	return input.c;      
}
//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 RENDER
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );		
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );				
        SetPixelShader( CompileShader( ps_4_0, PS() ) );       
    }    
}