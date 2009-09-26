#ifndef _LIGHTCULLER_H_
#define _LIGHTCULLER_H_

#include <Common/Prerequisites.h>
#include "Culler.h"
#include "Light.h"

namespace Flagship
{
	class BoundingSphere;

	class _DLL_Export LightCuller : public Culler
	{
	public:
		LightCuller();
		virtual ~LightCuller();

		// 从摄像机初始化
		void                Initialize( Light * pLight );

	public:
		// 包围体测试
		virtual bool        IsVisible( BoundingVolume * pBound );

	protected:
		// 光源范围
		BoundingSphere *    m_pLightBound;

	private:
	};
}

#endif