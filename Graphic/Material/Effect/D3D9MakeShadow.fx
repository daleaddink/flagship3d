// Flagship Effect File : D3D9Shadow.fx
// Remark : ShadowMap effect

#include "./Effect/D3D9Skin.fxh"

float4x4 g_matWorld : World;
float4x4 g_matView : View;
float4x4 g_matProj : Projection;

struct VertexInput
{
    float4 Position : POSITION;
};

struct SkinVertexInput
{
    float4 Position : POSITION;
    float3 BlendWeights    : BLENDWEIGHT;
    float4 BlendIndices    : BLENDINDICES;
};

struct VertexOutput
{
    float4 HPosition  : POSITION;
    float4 Depth      : TEXCOORD0;
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

VertexOutput SkinVS( SkinVertexInput IN )
{
    VertexOutput OUT = ( VertexOutput ) 0;
    float4x4 matSkinned = ComputeSkinnedMap(IN.BlendWeights, IN.BlendIndices);
    
    float4x4 matMVP;
    matMVP = mul( matSkinned, g_matView );
    matMVP = mul( matMVP, g_matProj );
        
    float4 Po = float4( IN.Position.xyz, 1.0f );
    OUT.HPosition = mul( Po, matMVP );
    OUT.Depth = OUT.HPosition;
    
    return OUT;
}

///////// /////////////////////////////////////////////////////////////////
// PIXEL SHADER //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

float4 MainPS( VertexOutput IN ) : COLOR
{
    return float4( IN.Depth.zzz, 1 );
}

//////////////////////////////////////////////////////////////////////////
// TECHNIQUE /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

technique MakeShadow
{
    pass P0
    {
        VertexShader = compile vs_3_0 MainVS();
        PixelShader = compile ps_3_0 MainPS();
    }
}

technique MakeShadowS
{
    pass P0
    {
        VertexShader = compile vs_3_0 SkinVS();
        PixelShader = compile ps_3_0 MainPS();
    }
}
