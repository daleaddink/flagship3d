// Flagship Effect File : D3D9Satin.fx
// Remark : Satin effect

float4x4 g_matWorld : World;
float4x4 g_matView : View;
float4x4 g_matProj : Projection;
float4   g_vLightDir : Direction;

float noiseRate = 0.02;
float noiseScale = 0.50;
float4 color = { 1.0f, 0.32f, 0.0f, 1.0f };
float4 gloss = { 0.77f, 0.59f, 0.4f, 1.0f };
float Kd = 0.5f;
float Ks = 1.0f;

struct VS_OUTPUT 
{
   float4 Pos:      POSITION;
   float3 normal:   TEXCOORD0;
   float3 tangent:  TEXCOORD1;
   float3 binormal: TEXCOORD2;
   float3 viewVec:  TEXCOORD3;
   float3 texCoord: TEXCOORD4;
};

VS_OUTPUT MainVS( float4 Pos: POSITION, float3 tangent: TANGENT, float3 binormal: BINORMAL, float3 normal: NORMAL )
{
   VS_OUTPUT Out;

   float4x4 matMVP;
   matMVP = mul( g_matWorld, g_matView );
   matMVP = mul( matMVP, g_matProj );
    
   Out.Pos = mul( Pos, matMVP );
   // Transform our tangent-space into eye-space
   Out.normal   =  mul( normal, g_matView );
   Out.binormal =  mul( binormal, g_matView );
   Out.tangent  =  mul( tangent, g_matView );
   // The view-vector in eye-space is -eyePos
   Out.viewVec  = - mul( Pos, g_matView );
   // Texture coords for the noise
   Out.texCoord = Pos.xyz * noiseRate;

   return Out;
}

texture g_NoiseTexture;
sampler3D Noise = sampler_state
{
   Texture = <g_NoiseTexture>;
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

float4 MainPS( float3 normal: TEXCOORD0, float3 tangent: TEXCOORD1, float3 binormal: TEXCOORD2, float3 viewVec: TEXCOORD3, float3 texCoord: TEXCOORD4 ) : COLOR
{
   viewVec = normalize( viewVec );

   // Grab an angle to rotate the tangent around the normal
   // This is done to create a slightly bumpy feeling
   float angle = noiseScale * ( tex3D( Noise, texCoord ) - 0.5 );

   // Our angle is within the [-PI, PI] range and we need both
   // the sine and cosine. Perfect for the sincos function,
   // which will save us some hardware instructions over separate
   // sin and cos calls.
   float cosA, sinA;
   sincos( angle, sinA, cosA );

   // Rotate tangent around the normal
   float3 tang = sinA * tangent + cosA * binormal;

   // Do the anisotropic lighting
   float diffuse = saturate( dot( g_vLightDir.xyz, normal ) );
   float cs = - dot( viewVec, tang );
   float sn = sqrt( 1 - cs * cs );
   float cl = dot( g_vLightDir.xyz, tang );
   float sl = sqrt( 1 - cl * cl );
   float specular = pow( saturate( cs * cl + sn * sl ), 50 );

   // Output the results
   return diffuse * Kd * color + gloss * specular * Ks;
}

//--------------------------------------------------------------//
// Technique Section for Anisotropic Effect Group
//--------------------------------------------------------------//
technique Normal
{
   pass P0
   {
      VertexShader = compile vs_3_0 MainVS();
      PixelShader = compile ps_3_0 MainPS();
   }

}

technique Skinned
{
   pass P0
   {
      VertexShader = compile vs_3_0 MainVS();
      PixelShader = compile ps_3_0 MainPS();
   }

}