// Flagship Effect File : D3D9Metal.fx
// Remark : Metal effect

float4x4 g_matWorld : World;
float4x4 g_matView : View;
float4x4 g_matProj : Projection;
float4x4 g_matWorldI : WorldInverse;
float4x4 g_matViewI : ViewInverse;
float4   g_vLightDir : Direction;

float3 LightPos = {100.0f, 100.0f, -100.0f};
float3 LightColor = {1.0f, 1.0f, 1.0f};
float3 AmbiColor = {0.07f, 0.07f, 0.07f};

float3 SurfColor = {1.0f, 1.0f, 1.0f};
float SpecExpon = 50.0f;
float Kd = 0.01f;
float Ks = 10.0f;
float Kr = 0.3f;

texture g_CubeTexture : Environment;

samplerCUBE EnvSampler = sampler_state
{
	Texture = <g_CubeTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

struct appdata
{
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
};

struct vertexOutput
{
    float4 HPosition	: POSITION;
    float4 TexCoord		: TEXCOORD0;
    float3 LightVec		: TEXCOORD1;
    float3 WorldNormal	: TEXCOORD2;
    float3 WorldView	: TEXCOORD3;
};

vertexOutput mainVS( appdata IN )
{
    vertexOutput OUT;
    
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
    OUT.TexCoord = IN.UV;
    OUT.WorldView = normalize( g_matViewI[3].xyz - Pw );
    OUT.HPosition = mul( Po, matMVP );
    
    return OUT;
}

void metal_refl_shared( vertexOutput IN,
		                out float3 DiffuseContrib,
		                out float3 SpecularContrib,
		                out float3 ReflectionContrib )
{
    float3 Ln = normalize( IN.LightVec );
    float3 Nn = normalize( IN.WorldNormal );
    float3 Vn = normalize( IN.WorldView );
    float3 Hn = normalize( Vn + Ln );
    float4 litV = lit( dot( Ln,Nn ), dot( Hn,Nn ), SpecExpon );
    DiffuseContrib = litV.y * LightColor + AmbiColor;
    SpecularContrib = litV.z * LightColor;
    float3 reflVect = - reflect( Vn,Nn );
    ReflectionContrib = texCUBE( EnvSampler,reflVect ).xyz;
}

float4 mainPS( vertexOutput IN ) : COLOR
{
    float3 diffContrib;
    float3 specContrib;
    float3 reflColor;
	metal_refl_shared( IN, diffContrib, specContrib, reflColor);
    float3 result = diffContrib * Kd + ( SurfColor * ( specContrib * Ks + reflColor * Kr ) );
    return float4( result,1 );
}

technique Normal
{
	pass P0
	{
		VertexShader = compile vs_3_0 mainVS();
        PixelShader = compile ps_3_0 mainPS();
	}
}

technique Skinned
{
	pass P0
	{
		VertexShader = compile vs_3_0 mainVS();
        PixelShader = compile ps_3_0 mainPS();
	}
}