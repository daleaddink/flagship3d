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

		// ���������ʼ��
		void                Initialize( Camera * pCamera );

	protected:
		// ��׶ƽ��
		Plane3f             m_akViewFrustum[6];
		
	private:

	};
}

#endif