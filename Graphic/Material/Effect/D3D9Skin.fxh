// Flagship Effect File : D3D9SkinnedMesh.fxh
// Remark : SkinnedMesh Matrix Founction

texture g_SkinnedMap;
sampler2D SkinnedMapSampler = sampler_state 
{
    Texture   = <g_SkinnedMap>;
    MinFilter = Point;
    MipFilter = Point;
    MagFilter = Point;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

float4x4 ComputeSkinnedMap(in float3 in_weights, in float4 indices)
{
	float4 weights = float4(in_weights,1.0f-in_weights.x - in_weights.y - in_weights.z);
   
    float  index;
    float4 coord;
    float4 row[4];
 	float4x4 matTemp[4];
   	float4x4 matResult = 0.0f;
   	
   	int4 IndexVector = D3DCOLORtoUBYTE4(indices);

	for ( int i = 0; i < 4; ++i )
	{
		index = IndexVector[i] * 4.0f;
		coord = float4( index / 1024.0f, 0.0f, 0.0f, 0.0f );
		row[0] = tex2Dlod( SkinnedMapSampler, coord );
		coord = float4( ( index + 1.0f ) / 1024.0f, 0.0f, 0.0f, 0.0f );
		row[1] = tex2Dlod( SkinnedMapSampler, coord );
		coord = float4( ( index + 2.0f ) / 1024.0f, 0.0f, 0.0f, 0.0f );
		row[2] = tex2Dlod( SkinnedMapSampler, coord );
		coord = float4( ( index + 3.0f ) / 1024.0f, 0.0f, 0.0f, 0.0f );
		row[3] = tex2Dlod( SkinnedMapSampler, coord );
		matTemp[i] = float4x4( row[0], row[1], row[2], row[3] );
	
		matResult += matTemp[i] * weights[i];
	}
	
	return matResult;
}

float4x4  g_matSkinnedMatrix[50];

float4x4 ComputeSkinnedMatrix(in float3 in_weights, in float4 indices)
{
	float4 weights = float4(in_weights,1.0f-in_weights.x - in_weights.y - in_weights.z);
	
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(indices);
   
	float4x4 mat = 0.0f;
	for (int i = 0; i < 4; ++i)
	{
		mat += g_matSkinnedMatrix[IndexVector[i]] * weights[i];
	}
	
	return mat;
}
