#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <Common/Prerequisites.h>
#include "Renderable.h"

namespace Flagship
{
	struct CameraData;

	class _DLL_Export Camera : public Renderable
	{
	public:
		Camera( wstring szName = L"" );
		virtual ~Camera();

		enum
		{
			CameraType_Perspect,
			CameraType_Orthogon,
		};

		// ��ȡ���������
		BYTE              GetCameraType() { return m_byCameraType; }

		// �����Ӿ�����Ϣ
		void              SetView( Vector4f& vPos, Vector4f& vDir, Vector4f& vUp, Vector4f& vRight );

		// ����ͶӰ������Ϣ
		void              SetProjection( float fFovY, float fAspect, float fNear, float fFar );

		// ����ͶӰ������Ϣ
		void              SetOrthProjection( float fWidth, float fHeight, float fNear, float fFar );

		// ����λ��
		void              SetPosition( Vector4f& vPos );

		// ���÷���
		void              SetDirection( Vector4f& vDir );

		// �����Ϸ���
		void              SetUp( Vector4f& vUp );

		// �����ҷ���
		void              SetRight( Vector4f& vRight );

	public:
		// ������߼�
		virtual void      Logic() {}

	protected:
		// ���������
		BYTE              m_byCameraType;

		// ��������
		CameraData *      m_pCameraData;
		
	private:

	};
}

#endif