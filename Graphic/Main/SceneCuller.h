#ifndef _PLANECULLER_H_
#define _PLANECULLER_H_

#include <Common/Prerequisites.h>
#include "Culler.h"

namespace Flagship
{
	class Camera;

	class _DLL_Export SceneCuller : public Culler
	{
	public:
		SceneCuller();
		virtual ~SceneCuller();

		// 从摄像机初始化
		void                Initialize( Camera * pCamera );

	protected:
		// 视锥平面
		Plane3f             m_akViewFrustum[6];
		
	private:

	};
}

#endif