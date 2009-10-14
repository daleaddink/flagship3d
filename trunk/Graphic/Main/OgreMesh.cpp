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
		while( m_uiReadSize < m_kFileBuffer.GetSize() )
		{
			streamID = ReadChunk( pData );
			
			if ( streamID == M_MESH )
			{
				bool bSkeleton;
				bSkeleton = * ( (bool *) pData );
				m_uiReadSize += sizeof( bool );
				pData += sizeof( bool );

				while( m_uiReadSize < m_kFileBuffer.GetSize() )
				{
					streamID = ReadChunk( pData );

					if ( streamID == M_GEOMETRY )
					{
						unsigned int uiVertexCount = * ( (unsigned int *) pData );
						m_uiReadSize += sizeof( unsigned int );
						pData += sizeof( unsigned int );

						while( m_uiReadSize < m_kFileBuffer.GetSize() )
						{
							streamID = ReadChunk( pData );

							if ( streamID == M_GEOMETRY_VERTEX_DECLARATION )
							{
								while( m_uiReadSize < m_kFileBuffer.GetSize() )
								{
									streamID = ReadChunk( pData );

									if ( streamID == M_GEOMETRY_VERTEX_ELEMENT )
									{

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
	}
}