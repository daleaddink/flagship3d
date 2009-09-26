// Flagship Effect File : D3D9SkyBox.fx
// Remark : Simple sky box

float4x4 g_matView : View;
float4x4 g_matProj : Projection;
float4 g_vAmbientColor;

texture g_Texture : TEXTURE;

struct VS_INPUT
{
    float4 Pos: POSITION;
};

struct VS_OUTPUT
{
    float4 Pos: POSITION;
    float4 vTex0: TEXCOORD0;
};

VS_OUTPUT VS(const VS_INPUT v)
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;
    
    float4x4 matMVP;
    matMVP = mul( g_matView, g_matProj );
    
    float4 pos = mul(v.Pos, matMVP);
    Out.Pos = pos.xyww;
    
    Out.vTex0.xyz = normalize(v.Pos.xyz);
    Out.vTex0.w = 1.0f;
    
    return Out;
}

samplerCUBE LinearSamp0 = 
sampler_state
{
    texture = <g_Texture>;
    Mipfilter = linear;
    Minfilter = linear;
    Magfilter = linear;
};

float4 CubeMap(VS_OUTPUT In): COLOR
{
    return texCUBE(LinearSamp0, In.vTex0);
}

float4 AmbientPS(VS_OUTPUT In): COLOR
{
    return g_vAmbientColor;
}

technique Ambient
{
    pass P0
    {
        Cullmode = None;
        
        VertexShader = compile vs_3_0 VS();
        PixelShader  = compile ps_3_0 AmbientPS();
    }
}

technique Color
{
    pass P0
    {
        Cullmode = None;
        AlphaBlendEnable = False;
        ZWriteEnable = False;
        
        VertexShader = compile vs_3_0 VS();
        PixelShader  = compile ps_3_0 CubeMap();
    }
}