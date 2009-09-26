// Flagship Effect File : D3D9Shadow.fxh
// Remark : Use Shadow Mapping

#include "./Effect/D3D9Skin.fxh"

#define SHADOWMAP_WIDTH 1024.0f

float4x4 g_matWorld : World;
float4x4 g_matView : View;
float4x4 g_matProj : Projection;

// ShadowMap
texture g_ShadowMap;

// Light Param
float    g_LightSize = 0.01f;
int      g_BlokerSamples = 8;
int      g_PCFSamples = 8;
float    g_BlockerScale = 0.15f;
float4x4 g_LightMatrix;

// Shadow Param
float    g_SearchLevel = 5.0f;
float    g_DepthBias = 0.01f;

sampler ShadowSampler = sampler_state
{
    texture = <g_ShadowMap>; 
    magfilter = LINEAR; 
    minfilter = LINEAR; 
    mipfilter = LINEAR;
    addressu  = CLAMP;
    addressv  = CLAMP;
};

float FindBlocker( float2 uv, float4 LP )
{
    // divide filter width by number of samples to use
    float searchWidth = g_LightSize / LP.z * g_BlockerScale;
    float stepSize = 2 * searchWidth / g_BlokerSamples;

    // compute starting point uv coordinates for search
    uv = uv - float2(searchWidth, searchWidth);

    // reset sum to zero
    float blockerSum = 0;
    float receiver = LP.z;
    float blockerCount = 0;
    float foundBlocker = 0;

    // iterate through search region and add up depth values
    for (int i=0; i<g_BlokerSamples; i++)
    {
        for (int j=0; j<g_BlokerSamples; j++)
        {
            float shadMapDepth = tex2D(ShadowSampler, uv + float2(i*stepSize,j*stepSize)).x;
            // found a blocker
            if (shadMapDepth < receiver)
            {
                blockerSum += shadMapDepth;
                blockerCount++;
                foundBlocker = 1;
            }
        }
    }

	float result;
	
	if (foundBlocker == 0) {
		// set it to a unique number so we can check
		// later to see if there was no blocker
		result = 999;
	}
	else {
	    // return average depth of the blockers
		result = blockerSum / blockerCount;
	}
	
	return result;
}

float EstimatePenumbra( float4 LP, float Blocker )
{
    // receiver depth
    float receiver = LP.z;
    // estimate penumbra using parallel planes approximation
    float penumbra = (receiver - Blocker) * g_LightSize / Blocker;
    return penumbra;
}

float PCF_Filter( float2 uv, float4 LP, float filterWidth )
{
    // compute step size for iterating through the kernel
    float stepSize = 2 * filterWidth / g_PCFSamples;

    // compute uv coordinates for upper-left corner of the kernel
    uv = uv - float2(filterWidth,filterWidth);

    float sum = 0;  // sum of successful depth tests
    // now iterate through the kernel and filter
    for (int i=0; i<g_PCFSamples; i++)
    {
        for (int j=0; j<g_PCFSamples; j++)
        {
            // get depth at current texel of the shadow map
            float shadMapDepth = 0;
            shadMapDepth = tex2D(ShadowSampler, uv + float2(i*stepSize,j*stepSize)).x;
            // test if the depth in the shadow map is closer than the eye-view point
            float shad = LP.z <= shadMapDepth;
            // accumulate result
            sum += shad;
        }
    }
   
    // return average of the samples
    return sum / (g_PCFSamples*g_PCFSamples);
}

struct VS_OUTPUT
{
    float4 Position   : POSITION;
    float4 LPosition  : TEXCOORD0;
};

VS_OUTPUT ShadowVS( float4 vPos : POSITION )
{
    VS_OUTPUT Output;

    float4x4 matMVP;
    matMVP = mul( g_matWorld, g_matView );
    matMVP = mul( matMVP, g_matProj );
    Output.Position = mul( vPos, matMVP );
    
    float4x4 matLightMVP;
    matLightMVP = mul( g_matWorld, g_LightMatrix );
    Output.LPosition = mul( vPos, matLightMVP );
    Output.LPosition.z -= g_DepthBias;
    
    return Output;    
}

VS_OUTPUT ShadowSkinedVS( float4 vPos : POSITION
                        , float3 BlendWeights    : BLENDWEIGHT
                        , float4 BlendIndices    : BLENDINDICES )
{
    VS_OUTPUT Output;
    float4x4 matSkinned = ComputeSkinnedMap(BlendWeights, BlendIndices);

    float4x4 matMVP;
    matMVP = mul( matSkinned, g_matView );
    matMVP = mul( matMVP, g_matProj );
    Output.Position = mul( vPos, matMVP );
    
    float4x4 matLightMVP;
    matLightMVP = mul( matSkinned, g_LightMatrix );
    Output.LPosition = mul( vPos, matLightMVP );
    Output.LPosition.z -= g_DepthBias;
    
    return Output;    
}

float4 ShadowPS( VS_OUTPUT IN ) : COLOR
{
    // The soft shadow algorithm follows:

   // Compute uv coordinates for the point being shaded
   // Saves some future recomputation.
   float2 uv = float2(.5,-.5)*(IN.LPosition.xy)/IN.LPosition.w + float2(.5,.5);
   
   if ( IN.LPosition.z > 1.0f )
   {
       IN.LPosition.z = 1.0f;
   }
            
//   return tex2D(ShadowSampler, uv);
//   return IN.LPosition.z < tex2D(ShadowSampler, uv);
   
   // ---------------------------------------------------------
   // Step 1: Find blocker estimate
//   float4 uvlod = float4(uv.x, uv.y, 0.0f, 4.0f);
//   float blocker = tex2Dlod(ShadowSampler, uvlod).x;
//   float blocker = FindBlocker(uv, IN.LPosition);
   
   //return float4( blocker, blocker, blocker, 1.0f );  // uncomment to visualize blockers
   
   // ---------------------------------------------------------
   // Step 2: Estimate penumbra using parallel planes approximation
//   float penumbra;  
//   penumbra = EstimatePenumbra(IN.LPosition, blocker);

   //return penumbra*32;  // uncomment to visualize penumbrae

   // ---------------------------------------------------------
   // Step 3: Compute percentage-closer filter
   // based on penumbra estimate

   // Now do a penumbra-based percentage-closer filter
   float shadowed; 
//   if (IN.LPosition.z <= blocker)
   {
//       shadowed = 1.0;
//       return float4(shadowed, shadowed, shadowed, 1.0f);
   }
   
   shadowed = PCF_Filter(uv, IN.LPosition, 0.003f);
   
   // If no blocker was found, just return 1.0
   // since the point isn't occluded
     
   return float4(shadowed, shadowed, shadowed, 1.0f);
}

technique PostShadow
{
    pass P0
    {        
        AlphaBlendEnable = True;
        SrcBlend = DestColor;
        DestBlend = Zero;
        ZWriteEnable = False;
        
        VertexShader = compile vs_3_0 ShadowVS();
        PixelShader  = compile ps_3_0 ShadowPS();
    }
}

technique PostShadowS
{
    pass P0
    {
        AlphaBlendEnable = True;
        SrcBlend = DestColor;
        DestBlend = Zero;
        ZWriteEnable = False;
        
        VertexShader = compile vs_3_0 ShadowSkinedVS();
        PixelShader  = compile ps_3_0 ShadowPS();
    }
}