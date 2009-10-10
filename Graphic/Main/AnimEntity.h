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
		
		// 创建动画任务
		void            Create( Animation * pAnimation, float fCurTime, float fAlpha, float fReplayRate, bool bLoop, bool bInvert );
		
		// 淡入
		void            FadeIn( float fRange );

		// 淡出
		void            FadeOut( float fRange );

		// 控制播放状态
		void            Play()          { m_byState = Task_Play; }
		void            Replay()        { m_byState = Task_Play; m_fCurTime = 0; }
		void            Pause()         { m_byState = Task_Pause; }
		void            Stop()          { m_byState = Task_Stop; m_fCurTime = 0; }

		// 获取播放状态
		BYTE            GetState()      { return m_byState; }

		// 获取动画指针
		Animation *     GetAnimation()  { return m_pAnimation; }

		// 获取动画任务数据
		float&          ReplayRate()    { return m_fReplayRate;	}
		float&          Alpha()		    { return m_fAlpha; }
		float&          FadeAlpha()     { return m_fFadeAlpha; }
		float&          FadeRange()     { return m_fFadeRange; }
		float&          CurTime()	    { return m_fCurTime; }
		Quaternionf&    RelRot()        { return m_qRelRot;  }

		// 获取动画任务状态
		bool            IsLoop()	    { return m_bLoop; }
		bool            IsInvert()      { return m_bInvert; }
		bool            IsPlaying()     { return ( m_byState == Task_Play || m_byState == Task_FadeIn || m_byState == Task_FadeOut ); }
		bool            IsFadeIn()      { return ( m_byState == Task_FadeIn ); }
		bool            IsFadeOut()     { return ( m_byState == Task_FadeOut );	}

	protected:
		// 动画指针
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

		// 设置骨骼
		void                           SetSkeleton( Skeleton * pSkeleton );

		// 设置动画
		void                           AddAnimation( Animation * pAnimation );

		// 绑定骨头
		void                           BindBone( Resource * pResource, int iBoneID );

		// 添加动画任务
		AnimationTask *                AddAnimationTask( Animation * pAnimation, float fCurTime, float fAlpha, float fReplayRate, bool bLoop, bool bInvert );

		// 删除动画任务
		bool                           DelAnimationTask( Key& szKey );

		// 更新动画
		void                           UpdateAnimation( float fElapsedTime );

		// 获取骨骼矩阵数
		int                            GetBoneMatrixNum() { return (int)m_pBoneMatrix.size(); }

		// 获取骨骼矩阵
		Matrix4f *                     GetBoneMatrixArray() { return &(m_pBoneMatrix[0]); }

	public:
		// 设置世界矩阵
		virtual void                   SetWorldMatrix( Matrix4f& matWorldMatrix );

		// 是否就绪
		virtual bool                   IsReady();

		// 从磁盘读取资源
		virtual bool                   Load();

		// 释放资源
		virtual bool                   Release();

	protected:
		// 更新骨骼数据
		void                           UpdatePose();

	protected:
		// 骨骼
		Skeleton *                     m_pSkeleton;
		Skeleton *                     m_pLocalSkeleton;

		// 骨骼世界矩阵列表
		vector< Matrix4f >             m_pBoneMatrix;

		// 动画列表
		map< Key, Animation * >        m_pAnimationMap;

		// 动画任务列表
		map< Key, AnimationTask * >    m_pAnimationTaskMap;

		// 绑定关系
		map< int, Resource * >         m_pBindMap;

		// 完成标志
		bool                           m_bFirst;
				
	private:
		// 共享数据
		AnimEntityData *               m_pAnimEntityData;

	};
}

#endif
