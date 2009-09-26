#include "LightCuller.h"
#include "BoundingSphere.h"

namespace Flagship
{
	LightCuller::LightCuller()
	{
		m_pLightBound = new BoundingSphere;
	}

	LightCuller::~LightCuller()
	{
		SAFE_DELETE( m_pLightBound );
	}

	void    LightCuller::Initialize( Light * pLight )
	{
		* m_pLightBound = * ( (BoundingSphere *) pLight->GetBound() );
	}

	bool    LightCuller::IsVisible( BoundingVolume * pBound )
	{
		if ( ! m_pLightBound->IsIntersect( pBound ) )
		{
			return false;
		}

		return Culler::IsVisible( pBound );
	}
}