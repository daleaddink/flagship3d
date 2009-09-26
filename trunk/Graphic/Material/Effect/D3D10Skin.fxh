// Flagship Effect File : D3D10SkinnedMesh.fxh
// Remark : SkinnedMesh Matrix Founction

Texture1D g_SkinnedMap;

float4x4 ComputeSkinnedMap(in float3 in_weights, in uint4 indices)
{
	float4 weights = float4(in_weights,1.0f-in_weights.x - in_weights.y - in_weights.z);
   
    uint   index;
    float2 coord;
    float4 row[4];
 	float4x4 matTemp;
   	float4x4 matResult = 0.0f;
   	
   	index = indices.x * 4;
	row[0] = g_SkinnedMap.Load( float2(index,     0) );
    row[1] = g_SkinnedMap.Load( float2(index + 1, 0) );
    row[2] = g_SkinnedMap.Load( float2(index + 2, 0) );
    row[3] = g_SkinnedMap.Load( float2(index + 3, 0) );
    matTemp = float4x4( row[0], row[1], row[2], row[3] );
    matResult += matTemp * weights[0];
    
    index = indices.y * 4;
	row[0] = g_SkinnedMap.Load( float2(index,     0) );
    row[1] = g_SkinnedMap.Load( float2(index + 1, 0) );
    row[2] = g_SkinnedMap.Load( float2(index + 2, 0) );
    row[3] = g_SkinnedMap.Load( float2(index + 3, 0) );
    matTemp = float4x4( row[0], row[1], row[2], row[3] );
    matResult += matTemp * weights[1];
    
    index = indices.z * 4;
	row[0] = g_SkinnedMap.Load( float2(index,     0) );
    row[1] = g_SkinnedMap.Load( float2(index + 1, 0) );
    row[2] = g_SkinnedMap.Load( float2(index + 2, 0) );
    row[3] = g_SkinnedMap.Load( float2(index + 3, 0) );
    matTemp = float4x4( row[0], row[1], row[2], row[3] );
    matResult += matTemp * weights[2];
    
    index = indices.w * 4;
	row[0] = g_SkinnedMap.Load( float2(index,     0) );
    row[1] = g_SkinnedMap.Load( float2(index + 1, 0) );
    row[2] = g_SkinnedMap.Load( float2(index + 2, 0) );
    row[3] = g_SkinnedMap.Load( float2(index + 3, 0) );
    matTemp = float4x4( row[0], row[1], row[2], row[3] );
    matResult += matTemp * weights[3];
		
	return matResult;
}

cbuffer cbSkinned
{
    float4x4  g_matSkinnedMatrix[255];
}

float4x4 ComputeSkinnedMatrix(in float3 in_weights, in float4 indices)
{
	float4 weights = float4(in_weights,1.0f-in_weights.x - in_weights.y - in_weights.z);
	
	uint index;
	float4x4 mat = 0.0f;
	
    index = indices.x;
    mat += g_matSkinnedMatrix[index] * weights[0];
    index = indices.y;
    mat += g_matSkinnedMatrix[index] * weights[1];
    index = indices.z;
    mat += g_matSkinnedMatrix[index] * weights[2];
    index = indices.w;
    mat += g_matSkinnedMatrix[index] * weights[3];
	
	return mat;
}
