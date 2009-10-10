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
}