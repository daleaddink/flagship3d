#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <Common/Prerequisites.h>
#include "Resource.h"

namespace Flagship
{
	inline short    QuatComponentCompress(float v)
	{
		return short( v * (float)(SHRT_MAX) );
	}	
	inline float    QuatComponentDecompress(short v)
	{
		return (float)(v)/(float)(SHRT_MAX);
	}

	struct ROTATION_KEY
	{
		short x;
		short y;
		short z;
		short w;
		void FromQuat( const Quaternionf& rot )
		{
			x = QuatComponentCompress( rot.X() );
			y = QuatComponentCompress( rot.Y() );
			z = QuatComponentCompress( rot.Z() );
			w = QuatComponentCompress( rot.W() );
		}
		void ToQuat( Quaternionf& rot )
		{
			rot[1] = QuatComponentDecompress( x );
			rot[2] = QuatComponentDecompress( y );
			rot[3] = QuatComponentDecompress( z );
			rot[0] = QuatComponentDecompress( w );
		}
	};

	struct AnimationTrack
	{
		AnimationTrack() : m_pRotKey( NULL )
			, m_pTransKey( NULL )
			, m_iNumRotKey(0)
			, m_iNumPosKey(0)
			, m_iBoneID(-1)
		{
		}
		~AnimationTrack()
		{
			delete [] m_pRotKey;
			delete [] m_pTransKey;
		}
		ROTATION_KEY *				   m_pRotKey;
		Vector3f *  				   m_pTransKey;
		Vector3f					   m_vOnePosKey;
		int         				   m_iNumRotKey;
		int			            	   m_iNumPosKey;
		int							   m_iBoneID;
	};

	class _DLL_Export Animation : public Resource
	{
	public:
		Animation();
		virtual ~Animation();

		// 查找动画轨
		AnimationTrack *            FindAnimTrack( int iBoneID );

		// 获取动画总帧数
		int                         GetNumFrames() { return m_iNumFrame; }

		// 获取动画总时间
		float		                GetDuration() { return m_fDuration; }

		// 获取动画采样率
		int                         GetSampleRate() { return m_iSampleFps; }

		// 获取动画优先级
		int			                GetPriority() { return m_iPriority; }

		// 获取动画轨总数
		int                         GetNumTracks() { return  (int) m_pAnimTrack.size(); }

		// 根据索引获取动画轨
		AnimationTrack *            GetAnimationTrack( int iIndex ) { return &( m_pAnimTrack[iIndex] ); }

		// 设置动画优先级
		void                        SetPriority( int iPriority ) { m_iPriority = iPriority; }

	protected:
		// 读取资源
		virtual bool                Create();

	protected:
		// 动画帧数
		int                         m_iNumFrame;

		// 动画时间
		float			            m_fDuration;

		// 采样时的帧率
		int                         m_iSampleFps;

		// 优先级
		int				            m_iPriority;

		// 动画轨列表
		vector< AnimationTrack >    m_pAnimTrack;

		// 版本信息
		const int                   PAF_MAGIC;
		const int                   PAF_VERSION;
		
	private:

	};
}

#endif
