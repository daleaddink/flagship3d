// Flagship Effect File : D3D9SkyBox.fx
// Remark : Simple sky box

float4x4 g_matWorld : World;
float4x4 g_matView  : View;
float4x4 g_matProj  : Projection;
float4   g_vAmbientColor;

int g_TerrainSize = 2048.0f;
int g_TerrainScale = 255.0f;

texture g_HeightMap : TEXTURE;
texture g_TexcoordMap : TEXTURE;
texture g_NormalMap : TEXTURE;

sampler2D HeightMapSampler = sampler_state 
{
    Texture   = <g_HeightMap>;
    MinFilter = Linear;
    MipFilter = Linear;
    MagFilter = Linear;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

sampler2D TexcoordMapSampler = sampler_state 
{
    Texture   = <g_TexcoordMap>;
    MinFilter = Linear;
    MipFilter = Linear;
    MagFilter = Linear;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

sampler2D NormalMapSampler = sampler_state 
{
    Texture   = <g_NormalMap>;
    MinFilter = Linear;
    MipFilter = Linear;
    MagFilter = Linear;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

struct VS_INPUT
{
    float4 Pos: POSITION;
    float4 Tex: TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos: POSITION;
    float4 Tex: TEXCOORD0;
};

VS_OUTPUT MainVS(VS_INPUT v)
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;
    
//  float4 coord = float4( v.Pos.x / g_TerrainSize, v.Pos.z / g_TerrainSize, 0.0f, 1.0f );
//	float4 height = tex2Dlod( HeightMapSampler, coord );
		
//	v.Pos.y = ( height.x - 0.5f ) * g_TerrainScale;
    
    float4x4 matMVP;
    matMVP = mul( g_matWorld, g_matView );
    matMVP = mul( matMVP, g_matProj );
    Out.Pos = mul(v.Pos, matMVP);
//  Out.Tex = tex2Dlod( TexcoordMapSampler, coord );
    
    return Out;
}

float4 ColorPS(VS_OUTPUT In): COLOR
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 AmbientPS(VS_OUTPUT In): COLOR
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

technique Ambient
{
    pass P0
    {
        VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 AmbientPS();
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