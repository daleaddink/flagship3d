#include "Animation.h"
#include <Common/LogManager.h>

namespace Flagship
{
	Animation::Animation()
		: PAF_MAGIC( MAKEFOURCC('P', 'A', 'F', '\0') ), PAF_VERSION( 100 )
	{
		m_iClassType = Base::Resource_Animation;
	}

	Animation::~Animation()
	{
	
	}

	bool    Animation::Create()
	{
//		char szFile[256];
//		wcstombs( szFile, m_szPathName.c_str(), 256 );
//		FILE * pFile = fopen( szFile, "rb" );

		char * pData = (char *) m_kFileBuffer.GetPointer();

		int magic;
		int version;
		magic = * ( (int *) pData );
		pData += sizeof(int);
		version = * ( (int *) pData );
		pData += sizeof(int);
//		fread( &magic, sizeof(int), 1, pFile );
//		fread( &version, sizeof(int), 1, pFile );

		if (magic != PAF_MAGIC)
		{
			return false;
		}
		bool compress;
		if (version == PAF_VERSION)
		{
			compress = false;
		}else if (version == PAF_VERSION+1)
		{
			compress = true;
		}else
		{	
			return false;
		}

		// Read animation sample rate
		m_iSampleFps = * ( (int *) pData );
		pData += sizeof(int);
//		fread( &m_iSampleFps, sizeof(int), 1, pFile );

		// Read animation duration
		m_fDuration = * ( (float *) pData );
		pData += sizeof(float);
//		fread( &m_fDuration, sizeof(float), 1, pFile );

		// Read number of animation track
		unsigned int numAnimTrack = 0;
		numAnimTrack = * ( (unsigned int *) pData );
		pData += sizeof(unsigned int);
//		fread( &numAnimTrack, sizeof(unsigned int), 1, pFile );

		vector< Vector3f > transKey;
		m_pAnimTrack.resize(numAnimTrack);
		for (size_t i = 0; i < numAnimTrack; ++i)
		{
			AnimationTrack * pAnimTrack = &( m_pAnimTrack[i] );

			// Read bone id
			pAnimTrack->m_iBoneID = * ( (int *) pData );
			pData += sizeof(int);
//			fread( &(pAnimTrack->m_iBoneID), sizeof(int), 1, pFile );

			// Read all rotation key frames
			int numKeys = 0;
			numKeys = * ( (int *) pData );
			pData += sizeof(int);
//			fread( &numKeys, sizeof(int), 1, pFile );
			pAnimTrack->m_iNumRotKey = (numKeys);
			pAnimTrack->m_pRotKey    = new ROTATION_KEY[ numKeys ];

			if (compress)
			{
				memcpy( pAnimTrack->m_pRotKey, pData, numKeys * sizeof(ROTATION_KEY) );
				pData += numKeys * sizeof(ROTATION_KEY);
//				fread( pAnimTrack->m_pRotKey, sizeof(ROTATION_KEY), numKeys, pFile );
			}else
			{
				int k;
				for (k = 0; k < numKeys; ++k)
				{
					float fQuad[4];
					memcpy( fQuad, pData, 4 * sizeof(float) );
					pData += 4 * sizeof(float);
//					fread( fQuad, sizeof(float), 4, pFile );
					Quaternionf rotKey;
					rotKey = Quaternionf( fQuad[3], fQuad[0], fQuad[1], fQuad[2] );
					pAnimTrack->m_pRotKey[k].FromQuat( rotKey );
				}
			}

			// Read all position key frames
			DWORD dwLength;
			dwLength = * ( (DWORD *) pData );
			pData += sizeof(DWORD);
//			fread( &dwLength, sizeof(DWORD), 1, pFile );
			transKey.reserve( dwLength );
			transKey.resize( dwLength );
			for( DWORD t = 0; t < dwLength; t++ )
			{
				float fVector[3];
				memcpy( fVector, pData, 3 * sizeof(float) );
				pData += 3 * sizeof(float);
//				fread( fVector, sizeof(float), 3, pFile );
				transKey[t] = Vector3f( fVector );
			}

			if (!compress)
			{
				size_t k;
				for (k = 1; k < transKey.size(); ++k)
				{
					Vector3f diff = transKey[k] - transKey[k-1];
					if ( fabs(diff.X()) > 0.0001f ||
						fabs(diff.Y()) > 0.0001f ||
						fabs(diff.Z()) > 0.0001f )
					{
						break;
					}
				}
				if (k == transKey.size())
				{
					numKeys = 1;
				}else
				{
					numKeys = (int)transKey.size();
				}
			}else
			{
				numKeys = (int)transKey.size();
			}
			if (numKeys == 1)
			{
				pAnimTrack->m_iNumPosKey = 1;
				pAnimTrack->m_pTransKey  = NULL;
				pAnimTrack->m_vOnePosKey  = transKey[0];
			}else
			{
				pAnimTrack->m_iNumPosKey = numKeys;
				pAnimTrack->m_pTransKey  = new Vector3f[numKeys];
				memcpy( pAnimTrack->m_pTransKey, &transKey[0], sizeof(Vector3f) * numKeys);
			}
		}

		// Calculate number frames of this animation
		if ( m_pAnimTrack.size() > 0 )
		{
			m_iNumFrame = m_pAnimTrack[0].m_iNumRotKey;
		}else
		{
			m_iNumFrame = (int)(m_fDuration * m_iSampleFps) + 1;
		}

//		fclose( pFile );
		return true;
	}
}