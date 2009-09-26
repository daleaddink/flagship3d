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

		// ��ǰ�ƶ�
		void    MoveForward( float fDist );

		// ����ƶ�
		void    MoveBackward( float fDist );

		// ����ƽ��
		void    MoveLeft( float fDist );

		// ����ƽ��
		void    MoveRight( float fDist );

		// ����
		void    MoveUp( float fDist );

		// �½�
		void    MoveDown( float fDist );

		// ��ת
		void    Rotate( float fYaw, float fPitch, float fRoll );

	protected:
		
	private:
	};
}

#endif