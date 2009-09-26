#ifndef _FREECAMERA_H_
#define _FREECAMERA_H_

#include <Common/Prerequisites.h>
#include "../../Graphic/Main/Camera.h"

namespace Flagship
{
	class _DLL_Export FreeCamera : public Camera
	{
	public:
		FreeCamera( wstring szName = L"" );
		virtual ~FreeCamera();

		// 向前移动
		void    MoveForward( float fDist );

		// 向后移动
		void    MoveBackward( float fDist );

		// 向左平移
		void    MoveLeft( float fDist );

		// 向右平移
		void    MoveRight( float fDist );

		// 上升
		void    MoveUp( float fDist );

		// 下降
		void    MoveDown( float fDist );

		// 旋转
		void    Rotate( float fYaw, float fPitch, float fRoll );

	protected:
		
	private:
	};
}

#endif