// Flagship Effect File : D3D9Basic.fx
// Remark : Basic transform and texture mapping

#include "./Effect/D3D9Skin.fxh"

float4x4 g_matWorld : World;
float4x4 g_matWorldI : WorldInverse;
float4x4 g_matView : View;
float4x4 g_matProj : Projection;

float4   g_vLightDir : Direction;

texture g_Texture;

sampler TextureSampler = sampler_state 
{
    Texture   = <g_Texture>;
    MinFilter = Linear;
    MipFilter = Linear;
    MagFilter = Linear;
};

struct VS_OUTPUT
{
    float4 Position   : POSITION;
    float3 Normal     : TEXCOORD0;
    float2 Tex        : TEXCOORD1;
};

VS_OUTPUT MainVS( float4 vPos : POSITION, 
                  float3 vNormal : NORMAL,
                  float2 vTex : TEXCOORD0 )
{
    VS_OUTPUT Output;
        
    float4x4 matMVP;
    matMVP = mul( g_matWorld, g_matView );
    matMVP = mul( matMVP, g_matProj );
    
    float4x4 matWorldIT;
    matWorldIT = transpose( g_matWorldI );
    
    Output.Position = mul( vPos, matMVP );
    Output.Normal = mul( vNormal, matWorldIT ).xyz;
    Output.Tex = vTex;
    
    return Output;
}

VS_OUTPUT SkinVS( float4 vPos : POSITION, 
                  float3 BlendWeights : BLENDWEIGHT,
                  float4 BlendIndices : BLENDINDICES,
                  float3 vNormal : NORMAL,
                  float2 vTex : TEXCOORD0
                  )
{
    VS_OUTPUT Output;
    float4x4 matSkinned = ComputeSkinnedMap(BlendWeights, BlendIndices);
        
    float4x4 matMVP;
    matMVP = mul( matSkinned, g_matView );
    matMVP = mul( matMVP, g_matProj );
    
    float4x4 matWorldIT;
    matWorldIT = transpose( g_matWorldI );
    
    Output.Position = mul( vPos, matMVP );
    Output.Normal = mul( vNormal, matWorldIT ).xyz;
    Output.Tex = vTex;
    
    return Output;
}

struct PS_OUTPUT
{
    float4 Color : COLOR0;
};

float4 LightPS( VS_OUTPUT In ) : COLOR
{
    float3 L = normalize( - g_vLightDir.xyz );
    return max( 0, dot( In.Normal, L ));
}

float4 ColorPS( VS_OUTPUT In ) : COLOR
{
    return tex2D( TextureSampler, In.Tex );
}

technique Light
{
    pass P0
    {
        AlphaBlendEnable = True;
        SrcBlend = One;
        DestBlend = One;
        ZWriteEnable = False;
            
        VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 LightPS();
    }
}

technique LightS
{
    pass P0
    {
        AlphaBlendEnable = True;
        SrcBlend = One;
        DestBlend = One;
        ZWriteEnable = False;
            
        VertexShader = compile vs_3_0 SkinVS();
        PixelShader  = compile ps_3_0 LightPS();
    }
}

technique Color
{
    pass P0
    {
        AlphaBlendEnable = True;
        SrcBlend = DestColor;
        DestBlend = Zero;
        ZWriteEnable = False;
        
        VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 ColorPS();
    }
}

technique ColorS
{
    pass P0
    {
        AlphaBlendEnable = True;
        SrcBlend = DestColor;
        DestBlend = Zero;
        ZWriteEnable = False;
        
        VertexShader = compile vs_3_0 SkinVS();
        PixelShader  = compile ps_3_0 ColorPS();
    }
}
