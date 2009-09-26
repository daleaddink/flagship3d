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

		// ���������ʼ��
		void                Initialize( Light * pLight );

	public:
		// ��Χ�����
		virtual bool        IsVisible( BoundingVolume * pBound );

	protected:
		// ��Դ��Χ
		BoundingSphere *    m_pLightBound;

	private:
	};
}

#endif