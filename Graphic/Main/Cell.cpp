#include "Cell.h"
#include "BoundingBox.h"
#include "Entity.h"

namespace Flagship
{
	Cell::Cell()
	{
		m_iClassType = SceneNode_Cell;

		// …Ë÷√∞¸ŒßÃÂ
		m_pBound = new BoundingBox;

	}

	Cell::~Cell()
	{
		Release();
	}

	void    Cell::SetIndex( int iIndexX, int iIndexZ )
	{
		m_iIndexX = iIndexX;
		m_iIndexZ = iIndexZ;
	}

	bool    Cell::Load()
	{
		map< Key, Entity * >::iterator it = m_pEntityMap.begin();
		while ( it != m_pEntityMap.end() )
		{
			(*it).second->Load();
			it++;
		}

		map< Key, SceneNode * >::iterator it2 = m_pChildMap.begin();
		while ( it2 != m_pChildMap.end() )
		{
			( (Cell *) (*it).second )->Load();
			it2++;
		}

		return true;
	}

	bool    Cell::Release()
	{
		map< Key, Entity * >::iterator it = m_pEntityMap.begin();
		while ( it != m_pEntityMap.end() )
		{
			(*it).second->Release();
			it++;
		}

		map< Key, SceneNode * >::iterator it2 = m_pChildMap.begin();
		while ( it2 != m_pChildMap.end() )
		{
			( (Cell *) (*it).second )->Release();
			it2++;
		}

		return true;
	}

	void    Cell::AttechEntity( Entity * pEntity )
	{
		m_pEntityMap[ pEntity->GetKey() ] = pEntity;
		pEntity->SetParent( this );
	}

	bool    Cell::DetachEntity( Entity * pEntity )
	{
		map< Key, Entity * >::iterator it = m_pEntityMap.find( pEntity->GetKey() );
		if ( it == m_pEntityMap.end() )
		{
			return false;
		}

		m_pEntityMap.erase( it );
		return true;
	}
}