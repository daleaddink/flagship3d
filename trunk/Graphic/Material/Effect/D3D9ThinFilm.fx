// Flagship Effect File : D3D9ThinFlim.fx
// Remark : ThinFlim effect

#include "./Effect/D3D9Skin.fxh"

float4x4 g_matWorld : World;
float4x4 g_matView : View;
float4x4 g_matProj : Projection;
float4x4 g_matWorldI : WorldInverse;
float4x4 g_matViewI : ViewInverse;
float4   g_vLightDir : Direction;

//////////////////////////////////////////////////////////////////////////
// tweakables ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
float3 LightColor = {1.0f, 1.0f, 1.0f};
float3 SurfColor = { 0.3f, 0.3f, 0.5f };
float SpecExpon = 50.0f;
float FilmDepth = 0.05f;
float Ka = 0.05f;
float Kd = 0.15f;
float Ks = 10.0f;

texture g_FringeMap;
sampler FringeMapSampler = sampler_state 
{
    Texture   = <g_FringeMap>;
    MinFilter = Linear;
    MipFilter = Linear;
    MagFilter = Linear;
    AddressU  = Wrap;
    AddressV  = Clamp;
};

//////////////////////////////////////////////////////////////////////////
// structs ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct appData
{
    float4 Position : POSITION;
    float4 Normal   : NORMAL;
};

struct skinData
{
    float4 Position        : POSITION;
    float3 BlendWeights    : BLENDWEIGHT;
	float4 BlendIndices    : BLENDINDICES;
    float4 Normal          : NORMAL;
};

struct filmyVertexOutput
{
    float4 HPosition	: POSITION;
    float3 LightVec		: TEXCOORD0;
    float3 WorldNormal	: TEXCOORD1;
    float3 WorldView	: TEXCOORD2;
    float2 FilmDepth    : TEXCOORD3;
};

//////////////////////////////////////////////////////////////////////////
// VERTEX SHADER /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

float calc_view_depth(float NDotV,float Thickness)
{
    // return (1.0 / NDotV) * Thickness;
    return (Thickness / NDotV);
}

filmyVertexOutput LightVS( appData IN )
{
    filmyVertexOutput OUT = ( filmyVertexOutput ) 0;
    
    float4x4 matWorldIT;
    matWorldIT = transpose( g_matWorldI );
    float4x4 matMVP;
    matMVP = mul( g_matWorld, g_matView );
    matMVP = mul( matMVP, g_matProj );
        
    float4 normal = normalize( IN.Normal );
    OUT.WorldNormal = mul( normal, matWorldIT ).xyz;
    float4 Po = float4( IN.Position.xyz, 1.0f );
    float3 Pw = mul( Po, g_matWorld ).xyz;
    OUT.LightVec = normalize( - g_vLightDir.xyz );
    OUT.WorldView = normalize( g_matViewI[3].xyz - Pw );
    OUT.HPosition = mul( Po, matMVP );
    
    // compute the view depth for the thin film
    float viewdepth = calc_view_depth( dot( OUT.WorldView, OUT.WorldNormal ), FilmDepth.x );
    OUT.FilmDepth = viewdepth.xx;
    return OUT;
}

filmyVertexOutput LightSkinnedVS( skinData IN )
{
    filmyVertexOutput OUT = ( filmyVertexOutput ) 0;
    float4x4 matSkinned = ComputeSkinnedMap(IN.BlendWeights, IN.BlendIndices);
    
    float4x4 matWorldIT;
    matWorldIT = transpose( g_matWorldI );
    float4x4 matMVP;
    matMVP = mul( matSkinned, g_matView );
    matMVP = mul( matMVP, g_matProj );
        
    float4 normal = normalize( IN.Normal );
    OUT.WorldNormal = mul( normal, matWorldIT ).xyz;
    float4 Po = float4( IN.Position.xyz, 1.0f );
    float3 Pw = mul( Po, matSkinned ).xyz;
    OUT.LightVec = normalize( - g_vLightDir.xyz );
    OUT.WorldView = normalize( g_matViewI[3].xyz - Pw );
    OUT.HPosition = mul( Po, matMVP );
    
    // compute the view depth for the thin film
    float viewdepth = calc_view_depth( dot( OUT.WorldView, OUT.WorldNormal ), FilmDepth.x );
    OUT.FilmDepth = viewdepth.xx;
    return OUT;
}

///////// /////////////////////////////////////////////////////////////////
// PIXEL SHADER //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void metal_refl_shared( filmyVertexOutput IN,
		                out float3 DiffuseContrib,
		                out float3 SpecularContrib )
{
    float3 Ln = normalize( IN.LightVec );
    float3 Nn = normalize( IN.WorldNormal );
    float3 Vn = normalize( IN.WorldView );
    float3 Hn = normalize( Vn + Ln );
    float4 litV = lit( dot( Ln,Nn ), dot( Hn,Nn ), SpecExpon );
    DiffuseContrib = litV.y * LightColor;
    SpecularContrib = litV.z * LightColor;
}

float4 LightPS( filmyVertexOutput IN ) : COLOR
{
    // lookup fringe value based on view depth
    float3 diffContrib;
    float3 specContrib;
	metal_refl_shared( IN, diffContrib, specContrib );
	float3 fringeCol = (float3) tex2D( FringeMapSampler, IN.FilmDepth );
    // modulate specular lighting by fringe color, combine with regular lighting
    float3 rgb = fringeCol * specContrib * Ks + diffContrib * Kd * SurfColor;
    return float4( rgb, 1 );
}

//////////////////////////////////////////////////////////////////////////
// TECHNIQUE /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

technique Light
{
    pass P0
    {
        AlphaBlendEnable = True;
        SrcBlend = One;
        DestBlend = One;
        ZWriteEnable = False;
        
        VertexShader = compile vs_3_0 LightVS();
        PixelShader  = compile ps_3_0 LightPS();
    }
}

technique LightSkinned
{
    pass P0
    {
        AlphaBlendEnable = True;
        SrcBlend = One;
        DestBlend = One;
        ZWriteEnable = False;
        
        VertexShader = compile vs_3_0 LightSkinnedVS();
        PixelShader  = compile ps_3_0 LightPS();
    }
}
