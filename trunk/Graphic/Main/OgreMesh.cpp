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
		int m_uiReadSize = 0;

		// Read header and determine the version
		unsigned short headerID;

		// Read header ID
		headerID = * ( (unsigned short *) pData );
		m_uiReadSize += sizeof( unsigned short );
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
		char * szVer[128];
		memcpy( szVer, pData, OGRE_STREAM_TEMP_SIZE * sizeof(char) );
		m_uiReadSize += OGRE_STREAM_TEMP_SIZE * sizeof(wchar_t);
		pData += OGRE_STREAM_TEMP_SIZE * sizeof(wchar_t);
		szVer[128] = '\0';

		unsigned short streamID;
		while( m_uiReadSize < (int) m_kFileBuffer.GetSize() )
		{
			streamID = ReadChunk( pData );
			
			if ( streamID == M_MESH )
			{
				bool bSkeleton = ReadBool( pData );
				while( m_uiReadSize < (int) m_kFileBuffer.GetSize() )
				{
					streamID = ReadChunk( pData );

					if ( streamID == M_GEOMETRY )
					{
						ReadGeometry( pData );
					}
					else if ( streamID == M_SUBMESH )
					{
						bool bSharedVertex = ReadBool( pData );
						int iIndexCount = ReadInt( pData );
						m_b32Index = ReadBool( pData );

						m_pIndexData = pData;
						if ( m_b32Index )
						{
							pData += iIndexCount * sizeof( int );
						}
						else
						{
							pData += iIndexCount * sizeof( unsigned short );
						}

						if ( ! bSharedVertex )
						{
							ReadGeometry( pData );
						}
					}
					else
					{
						m_uiReadSize += m_uiChunkSize; 
						pData += m_uiChunkSize;
					}
				}
			}
			else
			{
				m_uiReadSize += m_uiChunkSize; 
				pData += m_uiChunkSize;
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

	unsigned short    OgreMesh::ReadChunk( char * pData )
	{
		unsigned int streamID;

		streamID = * ( (unsigned short *) pData );
		m_uiReadSize += sizeof( unsigned short );
		pData += sizeof( unsigned short );

		m_uiChunkSize = * ( (int *) pData );
		m_uiReadSize += sizeof( unsigned int );
		pData += sizeof( unsigned int );

		return streamID;
	}

	unsigned short    OgreMesh::ReadShort( char * pData )
	{
		unsigned short uiResult;

		uiResult = * ( (unsigned short *) pData );
		m_uiReadSize += sizeof( unsigned short );
		pData += sizeof( unsigned short );

		return uiResult;
	}

	int    OgreMesh::ReadInt( char * pData )
	{
		int iResult;

		iResult = * ( (int *) pData );
		m_uiReadSize += sizeof( int );
		pData += sizeof( int );

		return iResult;
	}

	bool    OgreMesh::ReadBool( char * pData )
	{
		bool bResult

		bResult = * ( (bool *) pData );
		m_uiReadSize += sizeof( bool );
		pData += sizeof( bool );

		return bResult;
	}

	void    OgreMesh::ReadGeometry( char * pData )
	{
		unsigned int uiVertexCount = * ( (unsigned int *) pData );
		m_uiReadSize += sizeof( unsigned int );
		pData += sizeof( unsigned int );

		m_dwNumVertex = uiVertexCount;
		m_dwNumTrangle = uiVertexCount / 3;

		while( m_uiReadSize < (int) m_kFileBuffer.GetSize() )
		{
			streamID = ReadChunk( pData );

			if ( streamID == M_GEOMETRY_VERTEX_DECLARATION )
			{
				int iTexCount = 1;
				while( m_uiReadSize < (int) m_kFileBuffer.GetSize() )
				{
					streamID = ReadChunk( pData );

					if ( streamID == M_GEOMETRY_VERTEX_ELEMENT )
					{
						unsigned short source, offset, index, tmp;
						VertexElementType vType;
						VertexElementSemantic vSemantic;

						source = ReadShort( pData );
						tmp = ReadShort( pData );
						vType = static_cast<VertexElementType>(tmp);
						tmp = ReadShort( pData );
						vSemantic = static_cast<VertexElementSemantic>(tmp);
						offset = ReadShort( pData );
						index = ReadShort( pData );

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
						m_uiReadSize += m_uiChunkSize; 
						pData += m_uiChunkSize;
					}
				}
			}
			else if ( streamID == M_GEOMETRY_VERTEX_BUFFER )
			{
				unsigned short bindIndex, vertexSize;

				bindIndex = ReadShort( pData );
				vertexSize = ReadShort( pData );

				m_dwVertexSize = vertexSize;
				m_pVertexData = pData;
				pData += m_dwVertexSize * m_dwNumVertex;
			}
			else
			{
				m_uiReadSize += m_uiChunkSize; 
				pData += m_uiChunkSize;
			}
		}
	}
}