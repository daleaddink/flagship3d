#include "AnimEntity.h"
#include "Cell.h"
#include "SceneManager.h"
#include "ResourceLoader.h"
#include "Resource.h"
#include "Skeleton.h"
#include "Animation.h"
#include "DataCenter.h"

#define m_pAnimEntityData ( (AnimEntityData *) m_pSharedData )

namespace Flagship
{
	AnimationTask::AnimationTask()
	{
		m_byState = Task_None;
	}

	AnimationTask::~AnimationTask()
	{
		
	}

	void    AnimationTask::Create( Animation * pAnimation, float fCurTime, float fAlpha, float fReplayRate, bool bLoop, bool bInvert )
	{
		m_pAnimation = pAnimation;
		m_fCurTime = fCurTime;
		m_fAlpha = fAlpha;	
		m_fReplayRate = fReplayRate;
		m_bLoop = bLoop;
		m_bInvert = bInvert;
		m_byState = Task_None;
		m_qRelRot = Quaternionf::IDENTITY;
	}

	void    AnimationTask::FadeIn( float fRange )
	{
		m_byState = Task_FadeIn;
		m_fFadeAlpha = 0.0f;
		m_fFadeRange = fRange;
	}

	void    AnimationTask::FadeOut( float fRange )
	{
		m_byState = Task_FadeOut;
		m_fFadeAlpha = 0.0f;
		m_fFadeRange = fRange;
	}

	AnimEntity::AnimEntity( wstring szName )
	{
		m_iClassType = Entity_AnimEntity;
		m_dwLastFrame = 0;
		m_pLocalSkeleton = new Skeleton;
		m_bFirst = true;

		m_pAnimationMap.clear();
		m_pBindMap.clear();

		// 设置名称
		if ( szName != L"" )
		{
			SetName( szName );
		}

		// 设置包围体
		m_pBound = new BoundingBox;
		m_pLocalBound = new BoundingBox;
		* m_pLocalBound = * ( (BoundingBox *) m_pBound );
	}

	AnimEntity::~AnimEntity()
	{
		map< Key, AnimationTask * >::iterator it = m_pAnimationTaskMap.begin();
		while( it != m_pAnimationTaskMap.end() ) 
		{
			SAFE_DELETE( (*it).second );
			it++;
		}

		m_pAnimationTaskMap.clear();
		SAFE_DELETE( m_pLocalSkeleton );
	}

	void    AnimEntity::SetSkeleton( Skeleton * pSkeleton )
	{
		m_pSkeleton = pSkeleton;
	}

	void    AnimEntity::AddAnimation( Animation * pAnimation )
	{
		m_pAnimationMap[pAnimation->GetKey()] = pAnimation;
	}

	void    AnimEntity::BindBone( Resource * pResource, int iBoneID )
	{
		m_pBindMap[iBoneID] = pResource;
	}

	AnimationTask *    AnimEntity::AddAnimationTask( Animation * pAnimation, float fCurTime, float fAlpha, float fReplayRate, bool bLoop, bool bInvert )
	{
		AnimationTask * pTask = new AnimationTask;
		pTask->SetName( pAnimation->GetKey().GetName() );

		pTask->Create( pAnimation, fCurTime, fAlpha, fReplayRate, bLoop, bInvert );
		m_pAnimationTaskMap[pTask->GetKey()] = pTask;
		
		return pTask;
	}

	bool    AnimEntity::DelAnimationTask( Key& szKey )
	{
		map< Key, AnimationTask * >::iterator it = m_pAnimationTaskMap.find( szKey );
		if ( it != m_pAnimationTaskMap.end() )
		{
			SAFE_DELETE( (*it).second );
			m_pAnimationTaskMap.erase( it );
			return true;
		}

		return false;
	}

	void    AnimEntity::UpdateAnimation( float fElapsedTime )
	{
		if ( m_pAnimationTaskMap.size() == 0 )
		{
			return;
		}

		map< Key, AnimationTask * >::iterator it = m_pAnimationTaskMap.begin();
		while( it != m_pAnimationTaskMap.end() ) 
		{
			AnimationTask * pAnimTask = (*it).second;

			bool bTaskOver          = false;
			Animation * pAnim       = pAnimTask->GetAnimation();
			int sampleRate          = pAnim->GetSampleRate();
			float duration			= pAnim->GetDuration();
			float timePerFrame		= 1.0f/sampleRate;
			float startTime			= 0.0f;
			float endTime			= duration;
			float dt				= fElapsedTime * pAnimTask->ReplayRate();

			// Swap start time and end time if this animation task is invert
			if (pAnimTask->IsInvert())
			{
				swap(startTime, endTime);
			}

			if ( pAnimTask->IsPlaying() )
			{
				// The animation task is playing
				if (pAnimTask->IsInvert())
					pAnimTask->CurTime() -= dt;
				else
					pAnimTask->CurTime() += dt;
			}else
			{
				// This task not need update
				it++;
				continue;
			}

			if ( (!pAnimTask->IsInvert() && pAnimTask->CurTime() > endTime) || 
				( pAnimTask->IsInvert() && pAnimTask->CurTime() < endTime)  )
			{	
				if (!pAnimTask->IsInvert())
				{
					// Playing forward and current time great than duration
					if (pAnimTask->IsLoop())
					{
						if ( pAnimTask->CurTime() < endTime-0.001f )
							pAnimTask->CurTime() = endTime;
						else
							pAnimTask->CurTime() = startTime;
					}
					else
					{
						pAnimTask->CurTime() = endTime;
						bTaskOver = true;
					}
				}
				else
				{
					// Playing backward and current time less than 0
					if (pAnimTask->IsLoop())
					{				
						pAnimTask->CurTime() = startTime;
					}
					else
					{
						pAnimTask->CurTime() = endTime;
						bTaskOver = true;
					}
				}
			}

			float alpha = pAnimTask->Alpha();
			{		
				float curTime = pAnimTask->CurTime();
				if( pAnimTask->IsFadeIn() )
				{
					if ( pAnimTask->FadeAlpha() < pAnimTask->FadeRange()  )
					{
						alpha *= pAnimTask->FadeAlpha()/pAnimTask->FadeRange();
						pAnimTask->FadeAlpha() += fElapsedTime;
					}else
					{
						pAnimTask->Play();
					}
				}else if( pAnimTask->IsFadeOut() )
				{
					if ( pAnimTask->FadeAlpha() < pAnimTask->FadeRange() - timePerFrame )
					{	
						pAnimTask->FadeAlpha() += fElapsedTime;
					}else
					{
						bTaskOver = true;
					}
				}
			}

			float t   = ( pAnimTask->CurTime() * sampleRate );
			int   kf  = (int)t;

			// 更新身体
			if ( alpha > 0.99f )
			{
				m_pLocalSkeleton->UpdateAnimation( pAnim, kf, t-kf );
			}
			else
			{
				m_pLocalSkeleton->UpdateAnimation( pAnim, kf, t-kf, alpha );
			}

			if ( bTaskOver )
			{
				map< Key, AnimationTask * >::iterator it2 = it--;
				m_pAnimationTaskMap.erase( it );
				it = it2;
			}
			
			it++;
		}

		m_pLocalSkeleton->UpdateBody();
		UpdatePose();
	}

	void    AnimEntity::UpdatePose()
	{
		Matrix4f matTemp;
		Matrix4f * pWorld = DataCenter::GetSingleton()->GetMatrixData( this, GetClassType(), RenderableData::Matrix4_World )->Read();
		
		for ( int i = 0; i < (int) m_pLocalSkeleton->GetBoneNum(); i++ )
		{
			Bone * pPart = m_pLocalSkeleton->GetBone( i);
			Matrix4f& matBone = m_pBoneMatrix[i];

			matTemp = (* pWorld) * pPart->GetTransformMatrix();
			matBone = matTemp * pPart->GetInverseMatrix();
    	}

		( (MatrixArrayData *) m_pAnimEntityData->m_pData[AnimEntityData::Matrix4_Skinned] )->Write( GetBoneMatrixArray(), m_pLocalSkeleton->GetBoneNum() );
	}

	void    AnimEntity::SetWorldMatrix( Matrix4f& matWorldMatrix )
	{
		( (MatrixData *) m_pAnimEntityData->m_pData[AnimEntityData::Matrix4_World] )->Write( matWorldMatrix );
		PostThreadMessage( SceneManager::GetSingleton()->GetThreadID(), WM_UPDATEENTITY, (WPARAM) this, 0 );
	}

	bool    AnimEntity::IsReady()
	{
		if ( ! Entity::IsReady() )
		{
			return false;
		}

		if ( ! m_pSkeleton->IsReady() )
		{
			return false;
		}

		if ( ! m_pLocalSkeleton->IsReady() )
		{
			return false;
		}

		map< Key, Animation * >::iterator it = m_pAnimationMap.begin();
		for ( it = m_pAnimationMap.begin(); it != m_pAnimationMap.end(); it++ )
		{
			if ( ! (*it).second->IsReady() )
			{
				return false;
			}
		}

		if ( m_bFirst )
		{
			m_pLocalSkeleton->UpdateBody();
			UpdatePose();
			m_bFirst = false;
		}

		return true;
	}

	bool    AnimEntity::Load()
	{
		if ( ! Entity::Load() )
		{
			return false;
		}

		m_pSkeleton->Load();
		* m_pLocalSkeleton = * m_pSkeleton;
		
		int iBoneNum = m_pSkeleton->GetBoneNum();
		m_pBoneMatrix.reserve( iBoneNum );
		m_pBoneMatrix.resize( iBoneNum );
		( (IntData *) m_pAnimEntityData->m_pData[AnimEntityData::Int_BoneNum] )->Write( iBoneNum );

		map< Key, Animation * >::iterator it = m_pAnimationMap.begin();
		for ( it = m_pAnimationMap.begin(); it != m_pAnimationMap.end(); it++ )
		{
			ResourceLoader::GetSingleton()->PushResource( (*it).second );
		}

		return true;
	}

	bool    AnimEntity::Release()
	{
		if ( ! Entity::Release() )
		{
			return false;
		}

		m_pSkeleton->Release();
		
		map< Key, Animation * >::iterator it = m_pAnimationMap.begin();
		for ( it = m_pAnimationMap.begin(); it != m_pAnimationMap.end(); it++ )
		{
			(*it).second->Release();
		}

		return true;
	}
}