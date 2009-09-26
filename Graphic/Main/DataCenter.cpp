#include "DataCenter.h"
#include "Base.h"
#include "Renderable.h"

namespace Flagship
{
	DataCenter * DataCenter::m_pDataCenter = NULL;

	DataCenter::DataCenter()
	{
	    m_mData.clear();
		InitializeCriticalSection( &m_csMutex );
	}

	DataCenter::~DataCenter()
	{
		map< Key, void * >::iterator it;
		for ( it = m_mData.begin(); it != m_mData.end(); ++it )
		{
			SAFE_DELETE( (*it).second );
		}

		m_mData.clear();
		SAFE_DELETE( m_pDataCenter );
	}

	DataCenter *    DataCenter::GetSingleton()
	{
		if ( m_pDataCenter == NULL )
		{
			m_pDataCenter = new DataCenter;
		}

		return m_pDataCenter;
	}

	bool    DataCenter::EnterMutex()
	{
		EnterCriticalSection( &m_csMutex );
		return true;
	}

	void    DataCenter::LeaveMutex()
	{
		LeaveCriticalSection( &m_csMutex );
	}

	void *    DataCenter::Register( Key& szKey, int iClassType )
	{
		void * pData = NULL;

		switch ( iClassType )
		{
		// 场景节点
		case Base::Class_SceneNode:
			{
    			pData = new SceneNodeData;
				m_mData[szKey] = (void *) pData;

				break;
			}
		// 室内节点
		case Base::SceneNode_Sector:
			{
				pData = new SectorNodeData;
				m_mData[szKey] = (void *) pData;

				break;
			}
		// 实体节点
		case Base::Class_Entity:
			{
				pData = new EntityData;
				m_mData[szKey] = (void *) pData;
				
				break;
			}
		// 动画实体节点
		case Base::Entity_AnimEntity:
			{
				pData = new AnimEntityData;
				m_mData[szKey] = (void *) pData;

				break;
			}
		// 光源
		case Base::Entity_Light:
			{
				pData = new LightData;
				m_mData[szKey] = (void *) pData;

				break;
			}
		// 入口
		case Base::Class_Portal:
			{
				pData = new PortalData;
				m_mData[szKey] = (void *) pData;

				break;
			}
		// 摄像机
		case Base::Class_Camera:
			{
				pData = new CameraData;
				m_mData[szKey] = (void *) pData;

				break;
			}
		default:
			break;
		}

		return pData;
	}

	void    DataCenter::UnRegister( Key& szKey )
	{
		map< Key, void * >::iterator it = m_mData.find( szKey );
		SAFE_DELETE( (*it).second );
		m_mData.erase( it );
	}

	void    DataCenter::SetSharedData( Renderable * pRenderable, int iClassType )
	{
		void * pData = pRenderable->GetSharedData();

		switch ( iClassType )
		{
			// 场景节点
		case Base::Class_SceneNode:
			{
				( (SceneNodeData *) pData )->SetData();
				break;
			}
			// 室内节点
		case Base::SceneNode_Sector:
			{
				( (SectorNodeData *) pData )->SetData();
				break;
			}
			// 实体节点
		case Base::Class_Entity:
			{
				( (EntityData *) pData )->SetData();
				break;
			}
			// 运动实体节点
		case Base::Entity_AnimEntity:
			{
				( (AnimEntityData *) pData )->SetData();
				break;
			}
			// 光源
		case Base::Entity_Light:
			{
				( (LightData *) pData )->SetData();
				break;
			}
			// 入口
		case Base::Class_Portal:
			{
				( (PortalData *) pData )->SetData();
				break;
			}
			// 摄像机
		case Base::Class_Camera:
			{
				( (CameraData *) pData )->SetData();
				break;
			}
		default:
			{
				break;
			}
		}
	}

	IntData *    DataCenter::GetIntData( Renderable * pRenderable, int iClassType, int iDataType )
	{
		IntData * pIntData;
		void * pData = pRenderable->GetSharedData();

		switch ( iClassType )
		{
			// 场景节点
		case Base::Class_SceneNode:
			{
				pIntData = (IntData *) ( (SceneNodeData *) pData )->m_pData[iDataType];
				break;
			}
			// 室内节点
		case Base::SceneNode_Sector:
			{
				pIntData = (IntData *) ( (SectorNodeData *) pData )->m_pData[iDataType];
				break;
			}
			// 实体节点
		case Base::Class_Entity:
			{
				pIntData = (IntData *) ( (EntityData *) pData )->m_pData[iDataType];
				break;
			}
			// 运动实体节点
		case Base::Entity_AnimEntity:
			{
				pIntData = (IntData *) ( (AnimEntityData *) pData )->m_pData[iDataType];
				break;
			}
			// 光源
		case Base::Entity_Light:
			{
				pIntData = (IntData *) ( (LightData *) pData )->m_pData[iDataType];
				break;
			}
			// 入口
		case Base::Class_Portal:
			{
				pIntData = (IntData *) ( (PortalData *) pData )->m_pData[iDataType];
				break;
			}
			// 摄像机
		case Base::Class_Camera:
			{
				pIntData = (IntData *) ( (CameraData *) pData )->m_pData[iDataType];
				break;
			}
		default:
			{
				return NULL;
				break;
			}
		}

		return pIntData;
	}

	FloatData *    DataCenter::GetFloatData( Renderable * pRenderable, int iClassType, int iDataType )
	{
		FloatData * pFloatData;
		void * pData = pRenderable->GetSharedData();

		switch ( iClassType )
		{
			// 场景节点
		case Base::Class_SceneNode:
			{
				pFloatData = (FloatData *) ( (SceneNodeData *) pData )->m_pData[iDataType];
				break;
			}
			// 室内节点
		case Base::SceneNode_Sector:
			{
				pFloatData = (FloatData *) ( (SectorNodeData *) pData )->m_pData[iDataType];
				break;
			}
			// 实体节点
		case Base::Class_Entity:
			{
				pFloatData = (FloatData *) ( (EntityData *) pData )->m_pData[iDataType];
				break;
			}
			// 运动实体节点
		case Base::Entity_AnimEntity:
			{
				pFloatData = (FloatData *) ( (AnimEntityData *) pData )->m_pData[iDataType];
				break;
			}
			// 光源
		case Base::Entity_Light:
			{
				pFloatData = (FloatData *) ( (LightData *) pData )->m_pData[iDataType];
				break;
			}
			// 入口
		case Base::Class_Portal:
			{
				pFloatData = (FloatData *) ( (PortalData *) pData )->m_pData[iDataType];
				break;
			}
			// 摄像机
		case Base::Class_Camera:
			{
				pFloatData = (FloatData *) ( (CameraData *) pData )->m_pData[iDataType];
				break;
			}
		default:
			{
				return NULL;
				break;
			}
		}

		return pFloatData;
	}

	VectorData *    DataCenter::GetVectorData( Renderable * pRenderable, int iClassType, int iDataType )
	{
		VectorData * pVectorData;
		void * pData = pRenderable->GetSharedData();

		switch ( iClassType )
		{
			// 场景节点
		case Base::Class_SceneNode:
			{
				pVectorData = (VectorData *) ( (SceneNodeData *) pData )->m_pData[iDataType];
				break;
			}
			// 室内节点
		case Base::SceneNode_Sector:
			{
				pVectorData = (VectorData *) ( (SectorNodeData *) pData )->m_pData[iDataType];
				break;
			}
			// 实体节点
		case Base::Class_Entity:
			{
				pVectorData = (VectorData *) ( (EntityData *) pData )->m_pData[iDataType];
				break;
			}
			// 运动实体节点
		case Base::Entity_AnimEntity:
			{
				pVectorData = (VectorData *) ( (AnimEntityData *) pData )->m_pData[iDataType];
				break;
			}
			// 光源
		case Base::Entity_Light:
			{
				pVectorData = (VectorData *) ( (LightData *) pData )->m_pData[iDataType];
				break;
			}
			// 入口
		case Base::Class_Portal:
			{
				pVectorData = (VectorData *) ( (PortalData *) pData )->m_pData[iDataType];
				break;
			}
			// 摄像机
		case Base::Class_Camera:
			{
				pVectorData = (VectorData *) ( (CameraData *) pData )->m_pData[iDataType];
				break;
			}
		default:
			{
				return NULL;
				break;
			}
		}

		return pVectorData;
	}

	MatrixData *    DataCenter::GetMatrixData( Renderable * pRenderable, int iClassType, int iDataType )
	{
		MatrixData * pMatrixData;
		void * pData = pRenderable->GetSharedData();

		switch ( iClassType )
		{
			// 场景节点
		case Base::Class_SceneNode:
			{
				pMatrixData = (MatrixData *) ( (SceneNodeData *) pData )->m_pData[iDataType];
				break;
			}
			// 室内节点
		case Base::SceneNode_Sector:
			{
				pMatrixData = (MatrixData *) ( (SectorNodeData *) pData )->m_pData[iDataType];
				break;
			}
			// 实体节点
		case Base::Class_Entity:
			{
				pMatrixData = (MatrixData *) ( (EntityData *) pData )->m_pData[iDataType];
				break;
			}
			// 运动实体节点
		case Base::Entity_AnimEntity:
			{
				pMatrixData = (MatrixData *) ( (AnimEntityData *) pData )->m_pData[iDataType];
				break;
			}
			// 环境光
		case Base::Entity_Light:
			{
				pMatrixData = (MatrixData *) ( (LightData *) pData )->m_pData[iDataType];
				break;
			}
			// 入口
		case Base::Class_Portal:
			{
				pMatrixData = (MatrixData *) ( (PortalData *) pData )->m_pData[iDataType];
				break;
			}
			// 摄像机
		case Base::Class_Camera:
			{
				pMatrixData = (MatrixData *) ( (CameraData *) pData )->m_pData[iDataType];
				break;
			}
		default:
			{
				return NULL;
				break;
			}
		}

		return pMatrixData;
	}

	MatrixArrayData *    DataCenter::GetMatrixArrayData( Renderable * pRenderable, int iClassType, int iDataType )
	{
		MatrixArrayData * pMatrixArrayData;
		void * pData = pRenderable->GetSharedData();

		switch ( iClassType )
		{
		case Base::Entity_AnimEntity:
			{
				pMatrixArrayData = (MatrixArrayData *) ( (AnimEntityData *) pData )->m_pData[iDataType];
				break;
			}
		}

		return pMatrixArrayData;
	}
}
