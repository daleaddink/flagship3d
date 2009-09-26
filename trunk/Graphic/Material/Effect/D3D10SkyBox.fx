// Flagship Effect File : D3D9SkyBox.fx
// Remark : Simple sky box

cbuffer cbResize
{
    float4x4 g_matProj : Projection;
}

cbuffer cbFrame
{
    float4x4 g_matView : View;
}

TextureCube g_Texture : TEXTURE;

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

struct VS_INPUT
{
    float4 Pos: POSITION;
};

struct VS_OUTPUT
{
    float4 Pos: SV_POSITION;
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

SamplerState LinearSamp0
{
	Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

float4 CubeMap(VS_OUTPUT In): SV_Target
{
    return g_Texture.Sample(LinearSamp0, In.vTex0);
}

technique10 Normal
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, CubeMap() ) );
        
        SetDepthStencilState( DisableDepth, 0 );
    }
}