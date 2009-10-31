#include "OgreMesh.h"
#include <Common/LogManager.h>

namespace Flagship
{
	OgreMesh::OgreMesh()
	{
		m_iClassType = Base::Mesh_OgreMesh;
		m_uiChunkSize = 0;
	}

	OgreMesh::~OgreMesh()
	{

	}

	bool    OgreMesh::Create()
	{
		const int OGRE_STREAM_TEMP_SIZE = 128;
		char * pData = (char *) m_kFileBuffer.GetPointer();

		// Read header and determine the version
		unsigned short headerID;

		// Read header ID
		headerID = * ( (unsigned short *) pData );
		pData += sizeof( unsigned short );
		
		if ( headerID != M_HEADER )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "OgreMesh::Create Version Error! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		// Read version
		ReadString( &pData );

		unsigned short streamID;
		while( ! IsEnd( pData ) )
		{
			streamID = ReadChunk( &pData );
			
			if ( streamID == M_MESH )
			{
				bool bSkeleton = ReadBool( &pData );
				streamID = ReadChunk( &pData );
				while( ( ! IsEnd( pData ) ) &&
					( streamID == M_GEOMETRY ||
					streamID == M_SUBMESH ||
					streamID == M_MESH_SKELETON_LINK ||
					streamID == M_MESH_BONE_ASSIGNMENT ||
					streamID == M_MESH_LOD ||
					streamID == M_MESH_BOUNDS ||
					streamID == M_SUBMESH_NAME_TABLE ||
					streamID == M_EDGE_LISTS ||
					streamID == M_POSES ||
					streamID == M_ANIMATIONS ||
					streamID == M_TABLE_EXTREMES) )
				{
					if ( streamID == M_GEOMETRY )
					{
						ReadGeometry( &pData );
					}
					else if ( streamID == M_SUBMESH )
					{
						ReadString( &pData );

						bool bSharedVertex = ReadBool( &pData );
						unsigned int uiIndexCount = ReadInt( &pData );
						m_dwNumTrangle = uiIndexCount / 3;
						m_b32Index = ReadBool( &pData );

						m_pIndexData = pData;
						if ( m_b32Index )
						{
							pData += uiIndexCount * sizeof( unsigned int );
						}
						else
						{
							pData += uiIndexCount * sizeof( unsigned short );
						}

						if ( ! bSharedVertex )
						{
							streamID = ReadChunk( &pData );
							ReadGeometry( &pData );
						}
					}
					else
					{
						pData += m_uiChunkSize;
					}

					if ( ! IsEnd( pData ) )
					{
						streamID = ReadChunk( &pData );
					}
				}

				pData -= sizeof(unsigned short) + sizeof(unsigned int);
			}
			else
			{
				pData += m_uiChunkSize;
				break;
			}
		}

		return true;
	}

	void    OgreMesh::Destory()
	{
		m_dwFVF = 0;
		m_dwNumTrangle = 0;
		m_dwNumVertex = 0;
		m_dwVertexSize = 0;
	}

	unsigned short    OgreMesh::ReadChunk( char ** ppData )
	{
		unsigned int streamID;

		streamID = * ( (unsigned short *) *ppData );
		*ppData += sizeof( unsigned short );

		m_uiChunkSize = * ( (int *) *ppData );
		*ppData += sizeof( unsigned int );

		return streamID;
	}

	unsigned short    OgreMesh::ReadShort( char ** ppData )
	{
		unsigned short uiResult;

		uiResult = * ( (unsigned short *) *ppData );
		*ppData += sizeof( unsigned short );

		return uiResult;
	}

	unsigned int    OgreMesh::ReadInt( char ** ppData )
	{
		unsigned int uiResult;

		uiResult = * ( (unsigned int *) *ppData );
		*ppData += sizeof( unsigned int );

		return uiResult;
	}

	bool    OgreMesh::ReadBool( char ** ppData )
	{
		bool bResult;

		bResult = * ( (bool *) *ppData );
		*ppData += sizeof( bool );

		return bResult;
	}

	void    OgreMesh::ReadString( char ** ppData )
	{
		const int OGRE_STREAM_TEMP_SIZE = 128;
		char tmpBuf[OGRE_STREAM_TEMP_SIZE];
		string retString;
		size_t readCount = OGRE_STREAM_TEMP_SIZE-1;
		// Keep looping while not hitting delimiter
		while ( ! IsEnd( *ppData ) )
		{
			memcpy( tmpBuf, *ppData, OGRE_STREAM_TEMP_SIZE-1 );
			*ppData += OGRE_STREAM_TEMP_SIZE-1;

			// Terminate string
			tmpBuf[readCount] = '\0';

			char* p = strchr(tmpBuf, '\n');
			if (p != 0)
			{
				// Reposition backwards
				*ppData += (long)(p + 1 - tmpBuf - readCount);
				*p = '\0';
			}

			retString += tmpBuf;

			if (p != 0)
			{
				// Trim off trailing CR if this was a CR/LF entry
				if (retString.length() && retString[retString.length()-1] == '\r')
				{
					retString.erase(retString.length()-1, 1);
				}

				// Found terminator, break out
				break;
			}
		}
	}

	void    OgreMesh::ReadGeometry( char ** ppData )
	{
		unsigned int uiVertexCount = ReadInt( ppData );
		m_dwNumVertex = uiVertexCount;
		
		unsigned short streamID = ReadChunk( ppData );
		while( ( ! IsEnd( *ppData ) ) &&
			( streamID == M_GEOMETRY_VERTEX_DECLARATION 
			|| streamID == M_GEOMETRY_VERTEX_BUFFER ) )
		{
			if ( streamID == M_GEOMETRY_VERTEX_DECLARATION )
			{
				int iTexCount = 1;
				streamID = ReadChunk( ppData );
				while( ( ! IsEnd( *ppData ) ) &&
					streamID == M_GEOMETRY_VERTEX_ELEMENT )
				{
					if ( streamID == M_GEOMETRY_VERTEX_ELEMENT )
					{
						unsigned short source, offset, index, tmp;
						VertexElementType vType;
						VertexElementSemantic vSemantic;

						source = ReadShort( ppData );
						tmp = ReadShort( ppData );
						vType = static_cast<VertexElementType>(tmp);
						tmp = ReadShort( ppData );
						vSemantic = static_cast<VertexElementSemantic>(tmp);
						offset = ReadShort( ppData );
						index = ReadShort( ppData );

						switch ( vSemantic )
						{
						case VES_POSITION:
							{
								m_dwFVF |= Mesh_Position;
							}
							break;
						case VES_BLEND_WEIGHTS:
							{
								m_dwFVF |= Mesh_BlendWeights;
							}
							break;
						case VES_BLEND_INDICES:
							{
								m_dwFVF |= Mesh_BlendIndices;
							}
							break;
						case VES_NORMAL:
							{
								m_dwFVF |= Mesh_Normal;
							}
							break;
						case VES_TEXTURE_COORDINATES:
							{
								if ( iTexCount > 8 )
								{
									break;
								}

								m_dwFVF |= Mesh_Texcoord1 * iTexCount;
								iTexCount *= 2;
							}
							break;
						case VES_BINORMAL:
							{
								m_dwFVF |= Mesh_Binormal;
							}
							break;
						case VES_TANGENT:
							{
								m_dwFVF |= Mesh_Tangent;
							}
							break;
						}

					}
					else
					{
						*ppData += m_uiChunkSize;
					}

					if ( ! IsEnd( *ppData ) )
					{
						streamID = ReadChunk( ppData );
					}
				}

				*ppData -= sizeof(unsigned short) + sizeof(unsigned int);
			}
			else if ( streamID == M_GEOMETRY_VERTEX_BUFFER )
			{
				unsigned short bindIndex, vertexSize;

				bindIndex = ReadShort( ppData );
				vertexSize = ReadShort( ppData );

				unsigned short headerID = ReadChunk( ppData );
				m_dwVertexSize = vertexSize;
				m_pVertexData = *ppData;
				*ppData += m_dwVertexSize * m_dwNumVertex;
			}
			else
			{
				*ppData += m_uiChunkSize;
			}

			if ( ! IsEnd( *ppData ) )
			{
				streamID = ReadChunk( ppData );
			}
		}

		*ppData -= sizeof(unsigned short) + sizeof(unsigned int);
	}

	bool    OgreMesh::IsEnd( char * pData )
	{
		DWORD dwRead = (DWORD) ( pData - (char *) m_kFileBuffer.GetPointer() );
		if ( dwRead >= m_kFileBuffer.GetSize() - 1 )
		{
			return true;
		}
		
		return false;
	}
}