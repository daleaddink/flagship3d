// Flagship Effect File : D3D10Shadow.fx
// Remark : ShadowMap effect

cbuffer cbResize
{
    float4x4 g_matProj : Projection;
}

cbuffer cbFrame
{
    float4x4 g_matWorld : World;
    float4x4 g_matView : View;
}

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

struct VertexInput
{
    float4 Position : POSITION;
};

struct VertexOutput
{
    float4 HPosition	: SV_POSITION;
    float4 Depth        : TEXCOORD0;
};

//////////////////////////////////////////////////////////////////////////
// VERTEX SHADER /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VertexOutput MainVS( VertexInput IN )
{
    VertexOutput OUT = ( VertexOutput ) 0;
    
    float4x4 matMVP;
    matMVP = mul( g_matWorld, g_matView );
    matMVP = mul( matMVP, g_matProj );
        
    float4 Po = float4( IN.Position.xyz, 1.0f );
    OUT.HPosition = mul( Po, matMVP );
    OUT.Depth = OUT.HPosition;
    
    return OUT;
}

///////// /////////////////////////////////////////////////////////////////
// PIXEL SHADER //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

float4 MainPS( VertexOutput IN ) : SV_Target
{
    return float4( IN.Depth.zzz, 1 );
}

//////////////////////////////////////////////////////////////////////////
// TECHNIQUE /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

technique10 Normal
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, MainVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, MainPS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}
