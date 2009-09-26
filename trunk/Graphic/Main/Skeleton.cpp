#include "Skeleton.h"
#include "Animation.h"
#include <Common/LogManager.h>

namespace Flagship
{
	Bone::Bone()
	{
		m_pParentBone = NULL;
	}

	Bone::~Bone()
	{

	}

	Bone&    Bone::operator = ( Bone& Right )
	{
		m_szName = Right.m_szName.c_str();
		m_iBoneID = Right.m_iBoneID;
		m_iParentID = Right.m_iParentID;

		m_pChild.reserve( Right.m_pChild.size() );
		m_pChild.resize( Right.m_pChild.size() );
		for ( int i = 0; i < (int) Right.m_pChild.size(); i++ )
		{
			m_pChild[i] = Right.m_pChild[i];
		}

		m_vRelTrans = Right.m_vRelTrans;
		m_qRelRot = Right.m_qRelRot;
		m_matInv = Right.m_matInv;
		m_qInvRot = Right.m_qInvRot;

		m_vRotLimitMin = Right.m_vRotLimitMin;
		m_vRotLimitMax = Right.m_vRotLimitMax;

		for ( int i = 0; i < 4; i++ )
		{
			m_pAxisLocked[i] = Right.m_pAxisLocked[i];
		}

		return *this;
	}

	void    Bone::UpdateBone()
	{
		m_vAbsTrans = m_vRelTrans;
		m_qAbsRot = m_qRelRot;

		if( m_pParentBone != NULL )
		{
			Matrix3f matRot;
			m_pParentBone->m_qAbsRot.ToRotationMatrix( matRot );
			m_vAbsTrans = matRot * m_vAbsTrans;

			m_vAbsTrans += m_pParentBone->GetAbsTrans();
			m_qAbsRot = m_pParentBone->GetAbsRot() * m_qAbsRot;
		}

		Matrix3f matRot;
		m_qAbsRot.ToRotationMatrix( matRot );

		Matrix4f matTrans( matRot[0][0], matRot[0][1], matRot[0][2], m_vAbsTrans.X(),
			               matRot[1][0], matRot[1][1], matRot[1][2], m_vAbsTrans.Y(),
			               matRot[2][0], matRot[2][1], matRot[2][2], m_vAbsTrans.Z(),
			               0.0f, 0.0f, 0.0f, 1.0f );
		m_matTransform = matTrans;
	}

	Skeleton::Skeleton()
		: PSF_MAGIC( MAKEFOURCC('P', 'S', 'F', '\0') ), PSF_VERSION( 200 )
	{
		m_iClassType = Base::Resource_Skeleton;
	}

	Skeleton::~Skeleton()
	{
	
	}

	Skeleton&    Skeleton::operator = ( Skeleton& Right )
	{
		int iNum = Right.GetBoneNum();
		m_pBone.reserve( iNum );
		m_pBone.resize( iNum );
		for ( int i = 0; i < iNum; i++ )
		{
			m_pBone[i] = new Bone;
			*( m_pBone[i] ) = *( Right.GetBone( i ) );
		}

		for ( int i = 0; i < (int) m_pBone.size(); i++ )
		{
			if ( m_pBone[i]->m_iParentID < 0 )
			{
				m_pBone[i]->m_pParentBone = NULL;
			}
			else
			{
				m_pBone[i]->m_pParentBone = m_pBone[ m_pBone[i]->m_iParentID ];
			}

			int iChildNum = (int) m_pBone[i]->m_pChild.size();
			m_pBone[i]->m_pChildBone.resize(iChildNum);

			for ( int j = 0; j < iChildNum; j++ )
			{
				int iChildID = m_pBone[i]->m_pChild[j];
				m_pBone[i]->m_pChildBone[j] = m_pBone[iChildID];
			}
		}

		iNum = Right.GetRootBoneNum();
		m_pRootBoneID.reserve( iNum );
		m_pRootBoneID.resize( iNum );
		for ( int i = 0; i < iNum; i++ )
		{
			m_pRootBoneID[i] = Right.GetRootBoneID( i );
		}

		m_fScale = Right.GetScale();

		m_bReady = true;
		return *this;
	}

	void    Skeleton::UpdateAnimation( Animation * pAnimation, int iKeyFrame, float fDeltaTime, float fAlpha )
	{
		int NumTracks  = pAnimation->GetNumTracks();
		AnimationTrack * pTracks = pAnimation->GetAnimationTrack( 0 );
		Bone ** pParts = &m_pBone[0];
		for( int  i = 0; i < NumTracks; i++ )
		{
			AnimationTrack * pAnimTrack = pTracks + i;
			assert( pAnimTrack->m_iBoneID < (int) m_pBone.size() );

			Bone * pPart = pParts[pAnimTrack->m_iBoneID];
			assert( iKeyFrame < (int)pAnimTrack->m_iNumRotKey );

			// »ìºÏÐý×ª×ËÌ¬
			Quaternionf rot, rot0, rot1;
			pAnimTrack->m_pRotKey[iKeyFrame].ToQuat( rot0 );
			if ( iKeyFrame + 1 < (int) pAnimTrack->m_iNumRotKey )
			{
				pAnimTrack->m_pRotKey[iKeyFrame + 1].ToQuat( rot1 );
			}
			else
			{
				pAnimTrack->m_pRotKey[0].ToQuat(rot1);
			}

			rot.Slerp( fDeltaTime, rot0, rot1 );

			if ( fAlpha == 1.0f )
			{
				pPart->m_qRelRot = rot;
			}
			else
			{
				pPart->m_qRelRot.Slerp( fAlpha, pPart->m_qRelRot, rot );
			}

			Vector3f trans;
			//»ìºÏÆ½ÒÆ×ËÌ¬
			if ( pAnimTrack->m_iNumPosKey <= 1 )
			{
				trans = pAnimTrack->m_vOnePosKey;
			}
			else
			{
				int nk = 0;
				if ( iKeyFrame + 1 < (int)pAnimTrack->m_iNumPosKey )
				{
					nk = iKeyFrame + 1;
				}

				trans.Lerp( fDeltaTime, pAnimTrack->m_pTransKey[iKeyFrame], pAnimTrack->m_pTransKey[nk] );
			}

			if ( fAlpha == 1.0f )
			{
				pPart->m_vRelTrans = trans;
			}
			else
			{
				pPart->m_vRelTrans.Lerp( fAlpha, pPart->m_vRelTrans, trans );
			}
    	}	
	}

	void    Skeleton::UpdateBody()
	{
		for ( int i = 0; i < (int) m_pBone.size(); i++ )
		{
			m_pBone[i]->UpdateBone();
		}
	}

	int    Skeleton::GetBoneNum()
	{
		return (int) m_pBone.size();
	}

	Bone *    Skeleton::GetBone( int iBoneID )
	{
		if ( iBoneID >= (int) m_pBone.size() )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "Skeleton::GetBone iBoundID not exist! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return NULL;
		}

		return m_pBone[iBoneID];
	}

	int    Skeleton::GetRootBoneNum()
	{
		return (int) m_pRootBoneID.size();
	}

	int    Skeleton::GetRootBoneID( int iIndex )
	{
		if ( iIndex < (int) m_pRootBoneID.size() )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "Skeleton::GetRootBoneID Index not exist! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return NULL;

		}
		
		return m_pRootBoneID[iIndex];
	}

	bool    Skeleton::Create()
	{
//		char szFile[256];
//		wcstombs( szFile, m_szPathName.c_str(), 256 );
//		FILE * pFile = fopen( szFile, "rb" );

		char * pData = (char *) m_kFileBuffer.GetPointer();

		// Test magic number and version
		int magic;
		int version;
		magic = * ( (int *) pData );
		pData += sizeof(int);
		version = * ( (int *) pData );
		pData += sizeof(int);
//		fread( &magic, sizeof(int), 1, pFile );
//		fread( &version, sizeof(int), 1, pFile );
		
		if (magic != PSF_MAGIC || version != PSF_VERSION)
		{	
			//	WRITE_ERROR_LOG(  L"Bad file format or version : skeleton file < %s >." , Path.c_str() );
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, m_szPathName.c_str(), 256 );
			sprintf( szLog, "Skeleton::Load Version Error! File:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}
		
		// Read all root bones id
		DWORD dwLength;
		dwLength = * ( (DWORD *) pData );
		pData += sizeof(DWORD);
//		fread( &dwLength, sizeof(DWORD), 1, pFile );
		m_pRootBoneID.reserve(dwLength);
		m_pRootBoneID.resize(dwLength);

		for( DWORD i = 0; i < dwLength; i++ )
		{
			m_pRootBoneID[i] = * ( (int *) pData );
			pData += sizeof(int);
//			fread( &(m_pRootBoneID[i]), sizeof(int), 1, pFile );
		}

		unsigned int numBones = 0;

		// Read number of bones
		numBones = * ( (unsigned int *) pData );
		pData += sizeof(unsigned int);
//		fread( &numBones, sizeof(unsigned int), 1, pFile );
		m_pBone.resize( numBones );

		Vector3f InvTrans;

		const DWORD MAX_BONE_NAME_LEN = 512;
		wchar_t nameBuf[MAX_BONE_NAME_LEN];

		// Read all bones
		for (unsigned int i = 0; i < numBones; ++i)
		{
			Bone * bone = new Bone();

			// Read bone name
			DWORD nameLen;
			nameLen = * ( (DWORD *) pData );
			pData += sizeof(DWORD);
//			fread( &nameLen, sizeof(DWORD), 1, pFile );
			if (nameLen > MAX_BONE_NAME_LEN)
			{
				memcpy( nameBuf, pData, MAX_BONE_NAME_LEN * sizeof(wchar_t) );
				pData += MAX_BONE_NAME_LEN * sizeof(wchar_t);
//				fread( nameBuf, sizeof(wchar_t), MAX_BONE_NAME_LEN, pFile );
				nameBuf[MAX_BONE_NAME_LEN - 1] = 0;
			}else
			{
				memcpy( nameBuf, pData, nameLen * sizeof(wchar_t) );
				pData += nameLen * sizeof(wchar_t);
//				fread( nameBuf, sizeof(wchar_t), nameLen, pFile );
				nameBuf[nameLen] = 0;
			}
			bone->m_szName = nameBuf;
			bone->m_iParentID = * ( (int *) pData );
			pData += sizeof(int);
//			fread( &(bone->m_iParentID), sizeof(int), 1, pFile );

			float fVector[3];
			memcpy( fVector, pData, 3 * sizeof(float) );
			pData += 3 * sizeof(float);
//			fread( fVector, sizeof(float), 3, pFile );
			bone->m_vRelTrans = Vector3f( fVector );

			float fQuad[4];
			memcpy( fQuad, pData, 4 * sizeof(float) );
			pData += 4 * sizeof(float);
//			fread( fQuad, sizeof(float), 4, pFile );
			bone->m_qRelRot = Quaternionf( fQuad[3], fQuad[0], fQuad[1], fQuad[2] );

			memcpy( fVector, pData, 3 * sizeof(float) );
			pData += 3 * sizeof(float);
//			fread( fVector, sizeof(float), 3, pFile );
			InvTrans = Vector3f( fVector );

			memcpy( fQuad, pData, 4 * sizeof(float) );
			pData += 4 * sizeof(float);
//			fread( fQuad, sizeof(float), 4, pFile );
			bone->m_qInvRot = Quaternionf( fQuad[3], fQuad[0], fQuad[1], fQuad[2] );

			memcpy( fVector, pData, 3 * sizeof(float) );
			pData += 3 * sizeof(float);
//			fread( fVector, sizeof(float), 3, pFile );
			bone->m_vRotLimitMin = Vector3f( fVector );

			memcpy( fVector, pData, 3 * sizeof(float) );
			pData += 3 * sizeof(float);
//			fread( fVector, sizeof(float), 3, pFile );
			bone->m_vRotLimitMax = Vector3f( fVector );

			memcpy( bone->m_pAxisLocked, pData, 4 * sizeof(bool) );
			pData += 4 * sizeof(bool);
//			fread( bone->m_pAxisLocked, sizeof(bool), 4, pFile );

			// Read children ids
			dwLength = * ( (DWORD *) pData );
			pData += sizeof(DWORD);
//			fread( &dwLength, sizeof(DWORD), 1, pFile );
			bone->m_pChild.reserve( dwLength );
			bone->m_pChild.resize( dwLength );
			for( DWORD t = 0; t < dwLength; t++ )
			{
				bone->m_pChild[t] = * ( (int *) pData );
				pData += sizeof(int);
//				fread( &(bone->m_pChild[t]), sizeof(int), 1, pFile );
			}

			// Calculate the inverse matrix of this bone
			Matrix3f matRot;
			bone->m_qInvRot.ToRotationMatrix( matRot );
			Matrix4f matInv( matRot[0][0], matRot[0][1], matRot[0][2], InvTrans.X(),
				             matRot[1][0], matRot[1][1], matRot[1][2], InvTrans.Y(),
				             matRot[2][0], matRot[2][1], matRot[2][2], InvTrans.Z(),
				             0.0f, 0.0f, 0.0f, 1.0f );
			bone->m_matInv = matInv;
			
			// Set bone id
			bone->m_iBoneID = i;

			// Add to skeleton
			m_pBone[i] = bone;
		}

		for ( int i = 0; i < (int) m_pBone.size(); i++ )
		{
			if ( m_pBone[i]->m_iParentID < 0 )
			{
				m_pBone[i]->m_pParentBone = NULL;
			}
			else
			{
				m_pBone[i]->m_pParentBone = m_pBone[ m_pBone[i]->m_iParentID ];
			}

			int iChildNum = (int) m_pBone[i]->m_pChild.size();
			m_pBone[i]->m_pChildBone.resize(iChildNum);

			for ( int j = 0; j < iChildNum; j++ )
			{
				int iChildID = m_pBone[i]->m_pChild[j];
				m_pBone[i]->m_pChildBone[j] = m_pBone[iChildID];
			}
		}

//		fclose( pFile );
		return true;
	}

	void    Skeleton::Destory()
	{
		for ( int i = 0; i < (int) m_pBone.size(); i++ )
		{
			delete m_pBone[i];
		}

		m_pBone.clear();
		m_pRootBoneID.clear();
	}
}