#ifndef _OGREMESH_H_
#define _OGREMESH_H_

#include <Common/Prerequisites.h>
#include "Mesh.h"

namespace Flagship
{
	enum MeshChunkID {
		M_HEADER                = 0x1000,
		// char*          version           : Version number check
		M_MESH                = 0x3000,
		// bool skeletallyAnimated   // important flag which affects h/w buffer policies
		// Optional M_GEOMETRY chunk
		M_SUBMESH             = 0x4000, 
		// char* materialName
		// bool useSharedVertices
		// unsigned int indexCount
		// bool indexes32Bit
		// unsigned int* faceVertexIndices (indexCount)
		// OR
		// unsigned short* faceVertexIndices (indexCount)
		// M_GEOMETRY chunk (Optional: present only if useSharedVertices = false)
		M_SUBMESH_OPERATION = 0x4010, // optional, trilist assumed if missing
		// unsigned short operationType
		M_SUBMESH_BONE_ASSIGNMENT = 0x4100,
		// Optional bone weights (repeating section)
		// unsigned int vertexIndex;
		// unsigned short boneIndex;
		// float weight;
		// Optional chunk that matches a texture name to an alias
		// a texture alias is sent to the submesh material to use this texture name
		// instead of the one in the texture unit with a matching alias name
		M_SUBMESH_TEXTURE_ALIAS = 0x4200, // Repeating section
		// char* aliasName;
		// char* textureName;

		M_GEOMETRY          = 0x5000, // NB this chunk is embedded within M_MESH and M_SUBMESH
		// unsigned int vertexCount
		M_GEOMETRY_VERTEX_DECLARATION = 0x5100,
		M_GEOMETRY_VERTEX_ELEMENT = 0x5110, // Repeating section
		// unsigned short source;  	// buffer bind source
		// unsigned short type;    	// VertexElementType
		// unsigned short semantic; // VertexElementSemantic
		// unsigned short offset;	// start offset in buffer in bytes
		// unsigned short index;	// index of the semantic (for colours and texture coords)
		M_GEOMETRY_VERTEX_BUFFER = 0x5200, // Repeating section
		// unsigned short bindIndex;	// Index to bind this buffer to
		// unsigned short vertexSize;	// Per-vertex size, must agree with declaration at this index
		M_GEOMETRY_VERTEX_BUFFER_DATA = 0x5210,
		// raw buffer data
		M_MESH_SKELETON_LINK = 0x6000,
		// Optional link to skeleton
		// char* skeletonName           : name of .skeleton to use
		M_MESH_BONE_ASSIGNMENT = 0x7000,
		// Optional bone weights (repeating section)
		// unsigned int vertexIndex;
		// unsigned short boneIndex;
		// float weight;
		M_MESH_LOD = 0x8000,
		// Optional LOD information
		// unsigned short numLevels;
		// bool manual;  (true for manual alternate meshes, false for generated)
		M_MESH_LOD_USAGE = 0x8100,
		// Repeating section, ordered in increasing depth
		// NB LOD 0 (full detail from 0 depth) is omitted
		// float fromSquaredDepth;
		M_MESH_LOD_MANUAL = 0x8110,
		// Required if M_MESH_LOD section manual = true
		// String manualMeshName;
		M_MESH_LOD_GENERATED = 0x8120,
		// Required if M_MESH_LOD section manual = false
		// Repeating section (1 per submesh)
		// unsigned int indexCount;
		// bool indexes32Bit
		// unsigned short* faceIndexes;  (indexCount)
		// OR
		// unsigned int* faceIndexes;  (indexCount)
		M_MESH_BOUNDS = 0x9000,
		// float minx, miny, minz
		// float maxx, maxy, maxz
		// float radius

		// Added By DrEvil
		// optional chunk that contains a table of submesh indexes and the names of
		// the sub-meshes.
		M_SUBMESH_NAME_TABLE = 0xA000,
		// Subchunks of the name table. Each chunk contains an index & string
		M_SUBMESH_NAME_TABLE_ELEMENT = 0xA100,
		// short index
		// char* name

		// Optional chunk which stores precomputed edge data					 
		M_EDGE_LISTS = 0xB000,
		// Each LOD has a separate edge list
		M_EDGE_LIST_LOD = 0xB100,
		// unsigned short lodIndex
		// bool isManual			// If manual, no edge data here, loaded from manual mesh
		// bool isClosed
		// unsigned long numTriangles
		// unsigned long numEdgeGroups
		// Triangle* triangleList
		// unsigned long indexSet
		// unsigned long vertexSet
		// unsigned long vertIndex[3]
		// unsigned long sharedVertIndex[3] 
		// float normal[4] 

		M_EDGE_GROUP = 0xB110,
		// unsigned long vertexSet
		// unsigned long triStart
		// unsigned long triCount
		// unsigned long numEdges
		// Edge* edgeList
		// unsigned long  triIndex[2]
		// unsigned long  vertIndex[2]
		// unsigned long  sharedVertIndex[2]
		// bool degenerate

		// Optional poses section, referred to by pose keyframes
		M_POSES = 0xC000,
		M_POSE = 0xC100,
		// char* name (may be blank)
		// unsigned short target	// 0 for shared geometry, 
		// 1+ for submesh index + 1
		M_POSE_VERTEX = 0xC111,
		// unsigned long vertexIndex
		// float xoffset, yoffset, zoffset
		// Optional vertex animation chunk
		M_ANIMATIONS = 0xD000, 
		M_ANIMATION = 0xD100,
		// char* name
		// float length
		M_ANIMATION_TRACK = 0xD110,
		// unsigned short type			// 1 == morph, 2 == pose
		// unsigned short target		// 0 for shared geometry, 
		// 1+ for submesh index + 1
		M_ANIMATION_MORPH_KEYFRAME = 0xD111,
		// float time
		// float x,y,z			// repeat by number of vertices in original geometry
		M_ANIMATION_POSE_KEYFRAME = 0xD112,
		// float time
		M_ANIMATION_POSE_REF = 0xD113, // repeat for number of referenced poses
		// unsigned short poseIndex 
		// float influence

		// Optional submesh extreme vertex list chink
		M_TABLE_EXTREMES = 0xE000,
		// unsigned short submesh_index;
		// float extremes [n_extremes][3];

		/* Version 1.2 of the .mesh format (deprecated)
		enum MeshChunkID {
		M_HEADER                = 0x1000,
		// char*          version           : Version number check
		M_MESH                = 0x3000,
		// bool skeletallyAnimated   // important flag which affects h/w buffer policies
		// Optional M_GEOMETRY chunk
		M_SUBMESH             = 0x4000, 
		// char* materialName
		// bool useSharedVertices
		// unsigned int indexCount
		// bool indexes32Bit
		// unsigned int* faceVertexIndices (indexCount)
		// OR
		// unsigned short* faceVertexIndices (indexCount)
		// M_GEOMETRY chunk (Optional: present only if useSharedVertices = false)
		M_SUBMESH_OPERATION = 0x4010, // optional, trilist assumed if missing
		// unsigned short operationType
		M_SUBMESH_BONE_ASSIGNMENT = 0x4100,
		// Optional bone weights (repeating section)
		// unsigned int vertexIndex;
		// unsigned short boneIndex;
		// float weight;
		M_GEOMETRY          = 0x5000, // NB this chunk is embedded within M_MESH and M_SUBMESH
		*/
		// unsigned int vertexCount
		// float* pVertices (x, y, z order x numVertices)
		M_GEOMETRY_NORMALS = 0x5100,    //(Optional)
		// float* pNormals (x, y, z order x numVertices)
		M_GEOMETRY_COLOURS = 0x5200,    //(Optional)
		// unsigned long* pColours (RGBA 8888 format x numVertices)
		M_GEOMETRY_TEXCOORDS = 0x5300,    //(Optional, REPEATABLE, each one adds an extra set)
		// unsigned short dimensions    (1 for 1D, 2 for 2D, 3 for 3D)
		// float* pTexCoords  (u [v] [w] order, dimensions x numVertices)
		/*
		M_MESH_SKELETON_LINK = 0x6000,
		// Optional link to skeleton
		// char* skeletonName           : name of .skeleton to use
		M_MESH_BONE_ASSIGNMENT = 0x7000,
		// Optional bone weights (repeating section)
		// unsigned int vertexIndex;
		// unsigned short boneIndex;
		// float weight;
		M_MESH_LOD = 0x8000,
		// Optional LOD information
		// unsigned short numLevels;
		// bool manual;  (true for manual alternate meshes, false for generated)
		M_MESH_LOD_USAGE = 0x8100,
		// Repeating section, ordered in increasing depth
		// NB LOD 0 (full detail from 0 depth) is omitted
		// float fromSquaredDepth;
		M_MESH_LOD_MANUAL = 0x8110,
		// Required if M_MESH_LOD section manual = true
		// String manualMeshName;
		M_MESH_LOD_GENERATED = 0x8120,
		// Required if M_MESH_LOD section manual = false
		// Repeating section (1 per submesh)
		// unsigned int indexCount;
		// bool indexes32Bit
		// unsigned short* faceIndexes;  (indexCount)
		// OR
		// unsigned int* faceIndexes;  (indexCount)
		M_MESH_BOUNDS = 0x9000
		// float minx, miny, minz
		// float maxx, maxy, maxz
		// float radius

		// Added By DrEvil
		// optional chunk that contains a table of submesh indexes and the names of
		// the sub-meshes.
		M_SUBMESH_NAME_TABLE,
		// Subchunks of the name table. Each chunk contains an index & string
		M_SUBMESH_NAME_TABLE_ELEMENT,
		// short index
		// char* name

		*/
	};

	/// Vertex element semantics, used to identify the meaning of vertex buffer contents
	enum VertexElementSemantic {
		/// Position, 3 reals per vertex
		VES_POSITION = 1,
		/// Blending weights
		VES_BLEND_WEIGHTS = 2,
		/// Blending indices
		VES_BLEND_INDICES = 3,
		/// Normal, 3 reals per vertex
		VES_NORMAL = 4,
		/// Diffuse colours
		VES_DIFFUSE = 5,
		/// Specular colours
		VES_SPECULAR = 6,
		/// Texture coordinates
		VES_TEXTURE_COORDINATES = 7,
		/// Binormal (Y axis if normal is Z)
		VES_BINORMAL = 8,
		/// Tangent (X axis if normal is Z)
		VES_TANGENT = 9

	};

	/// Vertex element type, used to identify the base types of the vertex contents
	enum VertexElementType
	{
		VET_FLOAT1 = 0,
		VET_FLOAT2 = 1,
		VET_FLOAT3 = 2,
		VET_FLOAT4 = 3,
		/// alias to more specific colour type - use the current rendersystem's colour packing
		VET_COLOUR = 4,
		VET_SHORT1 = 5,
		VET_SHORT2 = 6,
		VET_SHORT3 = 7,
		VET_SHORT4 = 8,
		VET_UBYTE4 = 9,
		/// D3D style compact colour
		VET_COLOUR_ARGB = 10,
		/// GL style compact colour
		VET_COLOUR_ABGR = 11
	};

	class _DLL_Export OgreMesh : public Mesh
	{
	public:
		OgreMesh();
		virtual ~OgreMesh();

	protected:
		// 读取资源
		virtual bool          Create();

		// 释放资源
		virtual void          Destory();

		// 获取顶点数据
		virtual Vector3f *    GetVertexData() { return NULL; }

		// 获取索引数据
		virtual WORD *        GetIndexData() { return NULL; }

		// 获取法线数据
		virtual Vector3f *    GetNormalData() { return NULL; }

	private:
		// 读取数据头
		unsigned short        ReadChunk( char * pData );

		// 读取数据
		unsigned short        ReadShort( char * pData );

		// 读取数据
		int                   ReadInt( char * pData );

		// 读取数据
		bool                  ReadBool( char * pData );

		// 读取模型数据
		void                  ReadGeometry( char * pData );

	private:
		// 数据长度
		unsigned int          m_uiChunkSize;

		// 当前已读数据
		unsigned int          m_uiReadSize;

		// 索引格式
		bool                  m_b32Index;

		// 顶点数据
		char *                m_pVertexData;

		// 索引数据
		char *                m_pIndexData;				
	};
}

#endif