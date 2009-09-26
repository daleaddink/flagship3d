#include "RenderQueue.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "DataCenter.h"

namespace Flagship
{
	RenderQueue::RenderQueue()
	{
		m_iSortType = Sort_FrontToBack;
		m_pRenderList.clear();
	}

	RenderQueue::~RenderQueue()
	{

	}

	void    RenderQueue::SetSortType( int iType )
	{
		m_iSortType = iType;
	}

	void    RenderQueue::InsertEntity( Entity * pEntity )
	{
		switch ( m_iSortType )
		{
		// ������
		case Sort_None:
			{
				m_pRenderList.insert( make_pair( 0.0f, pEntity ) );
				break;
			}
		case Sort_FrontToBack:
			{
				// ��ȡ��ǰ�����
				Camera * pCamera = RenderTarget::GetActiveCamera();
				Vector4f * pEyePos = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Position )->Read();
				Matrix4f * pEntityMatrix = DataCenter::GetSingleton()->GetMatrixData( pEntity, pEntity->GetClassType(), EntityData::Matrix4_World )->Read();
				Vector4f vEntityPos = pEntityMatrix->GetColumn( 3 );

				Vector4f vDis = (*pEyePos) - vEntityPos;
				m_pRenderList.insert( make_pair( vDis.Length(), pEntity ) );
				break;
			}
		default:
			break;
		}
	}

	// ����ʵ��
	void    RenderQueue::InsertEntity( Vector4f& vRefer, Entity * pEntity )
	{
		Matrix4f * pEntityMatrix = DataCenter::GetSingleton()->GetMatrixData( pEntity, pEntity->GetClassType(), EntityData::Matrix4_World )->Read();
		Vector4f vEntityPos = pEntityMatrix->GetColumn( 3 );

		Vector4f vDis = vRefer - vEntityPos;
		m_pRenderList.insert( make_pair( vDis.Length(), pEntity ) );
	}

	multimap< float, Entity * > *    RenderQueue::GetEntityList()
	{
		return &m_pRenderList;
	}

	void    RenderQueue::Clear()
	{
		m_pRenderList.clear();
	}
}