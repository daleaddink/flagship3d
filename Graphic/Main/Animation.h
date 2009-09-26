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

		// ���Ҷ�����
		AnimationTrack *            FindAnimTrack( int iBoneID );

		// ��ȡ������֡��
		int                         GetNumFrames() { return m_iNumFrame; }

		// ��ȡ������ʱ��
		float		                GetDuration() { return m_fDuration; }

		// ��ȡ����������
		int                         GetSampleRate() { return m_iSampleFps; }

		// ��ȡ�������ȼ�
		int			                GetPriority() { return m_iPriority; }

		// ��ȡ����������
		int                         GetNumTracks() { return  (int) m_pAnimTrack.size(); }

		// ����������ȡ������
		AnimationTrack *            GetAnimationTrack( int iIndex ) { return &( m_pAnimTrack[iIndex] ); }

		// ���ö������ȼ�
		void                        SetPriority( int iPriority ) { m_iPriority = iPriority; }

	protected:
		// ��ȡ��Դ
		virtual bool                Create();

	protected:
		// ����֡��
		int                         m_iNumFrame;

		// ����ʱ��
		float			            m_fDuration;

		// ����ʱ��֡��
		int                         m_iSampleFps;

		// ���ȼ�
		int				            m_iPriority;

		// �������б�
		vector< AnimationTrack >    m_pAnimTrack;

		// �汾��Ϣ
		const int                   PAF_MAGIC;
		const int                   PAF_VERSION;
		
	private:

	};
}

#endif
