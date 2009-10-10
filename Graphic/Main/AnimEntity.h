#ifndef _ANIMENTITY_H_
#define _ANIMENTITY_H_

#include <Common/Prerequisites.h>
#include "Entity.h"

namespace Flagship
{
	class Resource;
	class Skeleton;
	class Animation;
	struct AnimEntityData;

	class AnimationTask : public Base
	{
	public:

		enum
		{
			Task_Play,
			Task_FadeIn, 
			Task_FadeOut,
			Task_Stop,
			Task_Pause,
			Task_None,
		};

		AnimationTask();
		virtual	~AnimationTask();
		
		// ������������
		void            Create( Animation * pAnimation, float fCurTime, float fAlpha, float fReplayRate, bool bLoop, bool bInvert );
		
		// ����
		void            FadeIn( float fRange );

		// ����
		void            FadeOut( float fRange );

		// ���Ʋ���״̬
		void            Play()          { m_byState = Task_Play; }
		void            Replay()        { m_byState = Task_Play; m_fCurTime = 0; }
		void            Pause()         { m_byState = Task_Pause; }
		void            Stop()          { m_byState = Task_Stop; m_fCurTime = 0; }

		// ��ȡ����״̬
		BYTE            GetState()      { return m_byState; }

		// ��ȡ����ָ��
		Animation *     GetAnimation()  { return m_pAnimation; }

		// ��ȡ������������
		float&          ReplayRate()    { return m_fReplayRate;	}
		float&          Alpha()		    { return m_fAlpha; }
		float&          FadeAlpha()     { return m_fFadeAlpha; }
		float&          FadeRange()     { return m_fFadeRange; }
		float&          CurTime()	    { return m_fCurTime; }
		Quaternionf&    RelRot()        { return m_qRelRot;  }

		// ��ȡ��������״̬
		bool            IsLoop()	    { return m_bLoop; }
		bool            IsInvert()      { return m_bInvert; }
		bool            IsPlaying()     { return ( m_byState == Task_Play || m_byState == Task_FadeIn || m_byState == Task_FadeOut ); }
		bool            IsFadeIn()      { return ( m_byState == Task_FadeIn ); }
		bool            IsFadeOut()     { return ( m_byState == Task_FadeOut );	}

	protected:
		// ����ָ��
		Animation *	    m_pAnimation;

		Quaternionf     m_qRelRot;
		bool	        m_bLoop;
		bool	        m_bInvert;

		float		    m_fFadeRange;
		float		    m_fFadeAlpha;

		float	        m_fReplayRate;
		float	        m_fAlpha;

		float	        m_fCurTime;
		BYTE    	    m_byState;
	};

	class _DLL_Export AnimEntity : public Entity
	{
	public:
		AnimEntity( wstring szName = L"" );
		virtual ~AnimEntity();

		// ���ù���
		void                           SetSkeleton( Skeleton * pSkeleton );

		// ���ö���
		void                           AddAnimation( Animation * pAnimation );

		// �󶨹�ͷ
		void                           BindBone( Resource * pResource, int iBoneID );

		// ��Ӷ�������
		AnimationTask *                AddAnimationTask( Animation * pAnimation, float fCurTime, float fAlpha, float fReplayRate, bool bLoop, bool bInvert );

		// ɾ����������
		bool                           DelAnimationTask( Key& szKey );

		// ���¶���
		void                           UpdateAnimation( float fElapsedTime );

		// ��ȡ����������
		int                            GetBoneMatrixNum() { return (int)m_pBoneMatrix.size(); }

		// ��ȡ��������
		Matrix4f *                     GetBoneMatrixArray() { return &(m_pBoneMatrix[0]); }

	public:
		// �����������
		virtual void                   SetWorldMatrix( Matrix4f& matWorldMatrix );

		// �Ƿ����
		virtual bool                   IsReady();

		// �Ӵ��̶�ȡ��Դ
		virtual bool                   Load();

		// �ͷ���Դ
		virtual bool                   Release();

	protected:
		// ���¹�������
		void                           UpdatePose();

	protected:
		// ����
		Skeleton *                     m_pSkeleton;
		Skeleton *                     m_pLocalSkeleton;

		// ������������б�
		vector< Matrix4f >             m_pBoneMatrix;

		// �����б�
		map< Key, Animation * >        m_pAnimationMap;

		// ���������б�
		map< Key, AnimationTask * >    m_pAnimationTaskMap;

		// �󶨹�ϵ
		map< int, Resource * >         m_pBindMap;

		// ��ɱ�־
		bool                           m_bFirst;
				
	private:
		// ��������
		AnimEntityData *               m_pAnimEntityData;

	};
}

#endif
