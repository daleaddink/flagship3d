// Flagship Effect File : D3D10Glass.fx
// Remark : Glass effect

cbuffer cbParam
{
    float reflectStrength = 1.0;
    float refractStrength = 1.0;
    float3 etas = { 0.80, 0.82, 0.84 };
}

cbuffer cbResize
{
    float4x4 g_matProj : Projection;
}

cbuffer cbFrame
{
    float4x4 g_matWorld : World;
    float4x4 g_matView : View;
    float4x4 g_matWorldI : WorldInverse;
    float4x4 g_matViewI : ViewInverse;
}

TextureCube g_CubeTexture : Environment;
Texture2D g_FresnelTexture : Environment;

SamplerState environmentMapSampler
{
	Filter = ANISOTROPIC;
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState fresnelSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

struct appdata {
    float4 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float3 Normal	: NORMAL;
};

struct vertexOutput {
    float4 HPosition	: SV_POSITION;
    float4 TexCoord		: TEXCOORD0;
    float3 WorldNormal	: TEXCOORD1;
    float3 WorldView	: TEXCOORD2;
};

vertexOutput mainVS( appdata IN )
{
    vertexOutput OUT;
    
    float4x4 matWorldIT;
    matWorldIT = transpose( g_matWorldI );
    float4x4 matMVP;
    matMVP = mul( g_matWorld, g_matView );
    matMVP = mul( matMVP, g_matProj );
    
    float3 normal = normalize( IN.Normal );
    OUT.WorldNormal = mul( normal, (float3x3) matWorldIT );
    float3 Pw = mul( IN.Position, g_matWorld ).xyz;
    OUT.TexCoord = IN.UV;
    OUT.WorldView = g_matViewI[3].xyz - Pw;
    OUT.HPosition = mul( IN.Position, matMVP );
    
    return OUT;
}

float3 refract2( float3 I, float3 N, float eta, out bool fail )
{
	float IdotN = dot( I, N );
	float k = 1 - eta * eta * ( 1 - IdotN * IdotN );
	fail = k < 0;
	
	return eta * I - ( eta * IdotN + sqrt( k ) ) * N;
}

float4 mainPS( vertexOutput IN ) : SV_Target
{
    half3 N = normalize( IN.WorldNormal );
    float3 V = normalize( IN.WorldView );
    
    half3 R = reflect(-V, N);
    half4 reflColor = g_CubeTexture.Sample( environmentMapSampler, R );
	half fresnel = g_FresnelTexture.Sample( fresnelSampler, dot( N, V ) );

	const half4 colors[3] = {
    	{ 1, 0, 0, 0 },
    	{ 0, 1, 0, 0 },
    	{ 0, 0, 1, 0 },
	};
        
 	half4 transColor = 0;
  	bool fail = false;
    for( int i=0; i<3; i++ ) 
    {
    	half3 T = refract2( -V, N, etas[i], fail );
    	transColor += g_CubeTexture.Sample( environmentMapSampler, T ) * colors[i];
	}

    return lerp( transColor * refractStrength, reflColor * reflectStrength, fresnel );
}

technique10 Normal
{
	pass P0
	{		
        SetVertexShader( CompileShader( vs_4_0, mainVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, mainPS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
	}
}

technique10 Skinned
{
	pass P0
	{		
        SetVertexShader( CompileShader( vs_4_0, mainVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, mainPS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
	}
}