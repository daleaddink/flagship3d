// Flagship Effect File : D3D9Ambient.fx
// Remark : Ambient Pass

#include "./Effect/D3D9Skin.fxh"

float4x4 g_matWorld : World;
float4x4 g_matView : View;
float4x4 g_matProj : Projection;
float4   g_vAmbientColor;

struct VS_OUTPUT
{
    float4 Position   : POSITION;
};

VS_OUTPUT MainVS( float4 vPos : POSITION )
{
    VS_OUTPUT Output;
    
    float4x4 matMVP;
    matMVP = mul( g_matWorld, g_matView );
    matMVP = mul( matMVP, g_matProj );
    
    Output.Position = mul( vPos, matMVP );
    return Output;
}

struct PS_OUTPUT
{
    float4 Color : COLOR0;
};

PS_OUTPUT MainPS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;
    Output.Color = g_vAmbientColor;
    return Output;
}

VS_OUTPUT SkinVS( float4 vPos : POSITION
                , float3 BlendWeights    : BLENDWEIGHT
                , float4 BlendIndices    : BLENDINDICES )
{
    VS_OUTPUT Output;
    float4x4 matSkinned = ComputeSkinnedMap(BlendWeights, BlendIndices);
    
    float4x4 matMVP;
    matMVP = mul( matSkinned, g_matView );
    matMVP = mul( matMVP, g_matProj );
    
    Output.Position = mul( vPos, matMVP );
    return Output;
}

technique Ambient
{
    pass P0
    {
        VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();
    }
}

technique AmbientS
{
    pass P0
    {
        VertexShader = compile vs_3_0 SkinVS();
        PixelShader  = compile ps_3_0 MainPS();
    }
}

