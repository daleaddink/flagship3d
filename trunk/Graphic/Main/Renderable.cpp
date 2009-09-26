#include "Renderable.h"
#include "Material.h"
#include "BoundingVolume.h"
#include "DataDefine.h"
#include "DataCenter.h"

namespace Flagship
{
	Renderable::Renderable()
	{
		m_iClassType = Base::Class_Renderable;
		m_pSharedData = NULL;
		m_pBound = NULL;
	}

	Renderable::~Renderable()
	{
		
	}

	void    Renderable::SetName( wstring& szName )
	{
		Base::SetName( szName );
		m_pSharedData = DataCenter::GetSingleton()->Register( m_szKey, m_iClassType );
	}

	void    Renderable::AutoName()
	{
		Base::AutoName();
		m_pSharedData = DataCenter::GetSingleton()->Register( m_szKey, m_iClassType );
	}
}