#include "OgreMesh.h"
#include <Common/LogManager.h>

namespace Flagship
{
	OgreMesh::OgreMesh()
		: HEADER_CHUNK_ID(  )
	{
		m_iClassType = Base::Mesh_OgreMesh;
	}

	OgreMesh::~OgreMesh()
	{

	}

	bool    OgreMesh::Create()
	{
		const int HEADER_CHUNK_ID = 0x1000;

		// Read header and determine the version
		unsigned short headerID;

		// Read header ID
		readShorts(stream, &headerID, 1);

		if ( headerID != HEADER_CHUNK_ID )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "OgreMesh::Create Version Error! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		// Read version
		String ver = readString(stream);
		// Jump back to start
		stream->seek(0);

		// Check header
		readFileHeader(stream);

		unsigned short streamID;
		while(!stream->eof())
		{
			streamID = readChunk(stream);
			switch (streamID)
			{
			case M_MESH:
				readMesh(stream, pMesh, listener);
				break;
			}

		}

		return true;
	}

	void    OgreMesh::Destory()
	{

	}
}