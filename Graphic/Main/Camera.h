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

		// 获取摄像机类型
		BYTE              GetCameraType() { return m_byCameraType; }

		// 设置视矩阵信息
		void              SetView( Vector4f& vPos, Vector4f& vDir, Vector4f& vUp, Vector4f& vRight );

		// 设置投影矩阵信息
		void              SetProjection( float fFovY, float fAspect, float fNear, float fFar );

		// 设置投影矩阵信息
		void              SetOrthProjection( float fWidth, float fHeight, float fNear, float fFar );

		// 设置位置
		void              SetPosition( Vector4f& vPos );

		// 设置方向
		void              SetDirection( Vector4f& vDir );

		// 设置上方向
		void              SetUp( Vector4f& vUp );

		// 设置右方向
		void              SetRight( Vector4f& vRight );

	public:
		// 摄像机逻辑
		virtual void      Logic() {}

	protected:
		// 摄像机类型
		BYTE              m_byCameraType;

		// 共享数据
		CameraData *      m_pCameraData;
		
	private:

	};
}

#endif