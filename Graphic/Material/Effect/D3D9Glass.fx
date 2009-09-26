// Flagship Effect File : D3D9Glass.fx
// Remark : Glass effect

float4x4 g_matWorld : World;
float4x4 g_matView : View;
float4x4 g_matProj : Projection;
float4x4 g_matWorldI : WorldInverse;
float4x4 g_matViewI : ViewInverse;

float reflectStrength = 1.0;
float refractStrength = 1.0;
float3 etas = { 0.80, 0.82, 0.84 };

texture g_CubeTexture : Environment;
texture g_FresnelTexture : Environment;

samplerCUBE environmentMapSampler = sampler_state
{
	Texture = <g_CubeTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler2D fresnelSampler = sampler_state
{
	Texture = <g_FresnelTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = None;
};

struct appdata {
    float4 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float3 Normal	: NORMAL;
};

struct vertexOutput {
    float4 HPosition	: POSITION;
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

float4 mainPS( vertexOutput IN ) : COLOR
{
    half3 N = normalize( IN.WorldNormal );
    float3 V = normalize( IN.WorldView );
    
    half3 R = reflect(-V, N);
    half4 reflColor = texCUBE( environmentMapSampler, R );
	half fresnel = tex2D( fresnelSampler, dot( N, V ) );

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
    	transColor += texCUBE( environmentMapSampler, T ) * colors[i];
	}

    return lerp( transColor * refractStrength, reflColor * reflectStrength, fresnel );
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