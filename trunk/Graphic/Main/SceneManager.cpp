#include "SceneManager.h"
#include "Camera.h"
#include "SceneNode.h"
#include "Cell.h"
#include "SectorNode.h"
#include "BoundingBox.h"
#include "Portal.h"
#include "SceneCuller.h"
#include "LightCuller.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "RenderTarget.h"
#include "RenderWindow.h"
#include "Light.h"
#include "AnimEntity.h"
#include "DataCenter.h"
#include "ResourceManager.h"

namespace Flagship
{
	SceneManager *    SceneManager::m_pSceneManager = NULL;

	SceneManager::SceneManager()
	{
		m_pRootNode = NULL;
		m_pSceneCuller = new SceneCuller;
		m_pLightCuller = new LightCuller;

		m_dwCurFrame = 0;
		m_szNodeName = L"SceneNode";

		m_iCurIndexX = 0;
		m_iCurIndexZ = 0;

		m_iHeight = 0;
		m_iSizeX = 0;
		m_iSizeZ = 0;
	}

	SceneManager::~SceneManager()
	{
		SAFE_DELETE( m_pRootNode );
		SAFE_DELETE( m_pSceneCuller );
		SAFE_DELETE( m_pLightCuller );
		SAFE_DELETE( m_pSceneManager );

		for ( int i = 0; i < m_iSizeX; i++ )
		{
			SAFE_DELETE_ARRAY( m_ppCell[i] );
		}
	}

	SceneManager *    SceneManager::GetSingleton()
	{
		if ( m_pSceneManager == NULL )
		{
			m_pSceneManager = new SceneManager;
		}

		return m_pSceneManager;
	}

	void    SceneManager::SetThreadInfo( HANDLE hThread, DWORD dwThreadID )
	{
		m_hThread = hThread;
		m_dwThreadID = dwThreadID;
	}

	void    SceneManager::BuildScene( int iSizeX, int iSizeZ, int iHeight )
	{
		m_iHeight = iHeight;
		m_iSizeX = iSizeX;
		m_iSizeZ = iSizeZ;
		m_ppCell = new Cell * [iSizeX];

		for ( int i = 0; i < iSizeX; i++ )
		{
			m_ppCell[i] = new Cell [iSizeZ];
		    for ( int j = 0; j < iSizeZ; j++ )
		    {
				Vector3f vCenter( ( i + 0.5f ) * CELL_SIZE, 0.0f, ( j + 0.5f ) * CELL_SIZE );
				Vector3f avAxis[3]   = { Vector3f::UNIT_X, Vector3f::UNIT_Y, Vector3f::UNIT_Z };
				float    afExtent[3] = { CELL_SIZE / 2.0f, (float) iHeight, CELL_SIZE / 2.0f };
				BoundingBox kBox( vCenter, avAxis, afExtent );

				wchar_t szI[8];
				wchar_t szJ[8];
				wsprintf( szI, L"%d", i );
				wsprintf( szJ, L"%d", j );
				wstring szCellName = L"Cell_";
				szCellName += szI;
				szCellName += L'_';
				szCellName += szJ;

				m_ppCell[i][j].SetName( szCellName );
				m_ppCell[i][j].SetIndex( i, j );
				m_ppCell[i][j].SetBound( &kBox );
		    }
		}

		m_pRootNode = _SplitScene( CELL_VIEW, CELL_SIZE * CELL_VIEW / 2, CELL_SIZE * CELL_VIEW / 2 );
	}

	bool    SceneManager::PreLoad( Camera * pCamera )
	{
		// ��ȡ��ǰ�����λ�õĵ�Ԫ
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Position )->Read();
		int iIndexX = ( (int) pPos->X() + CELL_SIZE / 2 ) / CELL_SIZE;
		int iIndexZ = ( (int) pPos->Z() + CELL_SIZE / 2 ) / CELL_SIZE;

		// �ж�λ���Ƿ�Ϸ�
		if ( iIndexX < CELL_VIEW / 2 )
		{
			iIndexX = CELL_VIEW / 2;
		}

		if ( iIndexX > m_iSizeX - CELL_VIEW / 2 )
		{
			iIndexX = m_iSizeX - CELL_VIEW / 2;
		}

		if ( iIndexZ < CELL_VIEW / 2 )
		{
			iIndexZ = CELL_VIEW / 2;
		}

		if ( iIndexZ > m_iSizeZ - CELL_VIEW / 2 )
		{
			iIndexZ = m_iSizeZ - CELL_VIEW / 2;
		}
		
		// ���õ�ǰ����
		m_iCurIndexX = iIndexX;
		m_iCurIndexZ = iIndexZ;

		// ������Դ
		for ( int i = 0; i < CELL_VIEW; i++ )
		{
			for ( int j = 0; j < CELL_VIEW; j++ )
			{
				int iOffsetX = iIndexX - CELL_VIEW / 2 + i;
				int iOffsetZ = iIndexZ - CELL_VIEW / 2 + j;
				m_ppCell[iOffsetX][iOffsetZ].Load();
			}
		}

		// �ؽ�������
		map< Vector2f, SceneNode * >::iterator LeafIt = m_mLeafNode.begin();
		while ( LeafIt != m_mLeafNode.end() )
		{
			(*LeafIt).second->ClearChild();
			LeafIt++;
		}

		// ���°�Χ��λ��
		m_pRootNode->UpdateBound( Vector2f( (float) ( iIndexX - CELL_VIEW / 2 ) * CELL_SIZE, (float) ( iIndexZ - CELL_VIEW / 2 ) * CELL_SIZE ) );

		for ( int i = 0; i < CELL_VIEW; i++ )
		{
			for ( int j = 0; j < CELL_VIEW; j++ )
			{
				int iOffsetX = iIndexX - CELL_VIEW / 2 + i;
				int iOffsetZ = iIndexZ - CELL_VIEW / 2 + j;
				m_mLeafNode[Vector2f( (float) i, (float) j )]->AttechChild( &( m_ppCell[iOffsetX][iOffsetZ] ) );
			}
		}

		return true;
	}

	void    SceneManager::SetCuller( SceneCuller * pSceneCuller, LightCuller * pLightCuller )
	{
		m_pSceneCuller = pSceneCuller;
		m_pLightCuller = pLightCuller;
	}

	void    SceneManager::_UpdateScene()
	{
		// ��ȡ��ǰ�����λ�õĵ�Ԫ
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( RenderTarget::GetActiveCamera()
			, RenderTarget::GetActiveCamera()->GetClassType(), CameraData::Vector4_Position )->Read();

		int iIndexX = ( (int) pPos->X() + CELL_SIZE / 2 ) / CELL_SIZE;
		int iIndexZ = ( (int) pPos->Z() + CELL_SIZE / 2 ) / CELL_SIZE;

		// �ж�λ���Ƿ�Ϸ�
		if ( iIndexX < CELL_VIEW / 2 )
		{
			iIndexX = CELL_VIEW / 2;
		}

		if ( iIndexX > m_iSizeX - CELL_VIEW / 2 )
		{
			iIndexX = m_iSizeX - CELL_VIEW / 2;
		}

		if ( iIndexZ < CELL_VIEW / 2 )
		{
			iIndexZ = CELL_VIEW / 2;
		}

		if ( iIndexZ > m_iSizeZ - CELL_VIEW / 2 )
		{
			iIndexZ = m_iSizeZ - CELL_VIEW / 2;
		}

		// ��ǰ��Ԫδ�䣬���ظ���
		if ( m_iCurIndexX == iIndexX && m_iCurIndexZ == iIndexZ )
		{
			return;
		}

		// ������Դ
		if ( ! _ResourceTravel( Vector2f( pPos->X(), pPos->Z() ) ) )
		{
			return;
		}

		// �ؽ�������
		map< Vector2f, SceneNode * >::iterator LeafIt = m_mLeafNode.begin();
		while ( LeafIt != m_mLeafNode.end() )
		{
			(*LeafIt).second->ClearChild();
			LeafIt++;
		}

		// ���°�Χ��λ��
		m_pRootNode->UpdateBound( Vector2f( (float) ( iIndexX - CELL_VIEW / 2 ) * CELL_SIZE, (float) ( iIndexZ - CELL_VIEW / 2 ) * CELL_SIZE ) );

		for ( int i = 0; i < CELL_VIEW; i++ )
		{
			for ( int j = 0; j < CELL_VIEW; j++ )
			{
				int iOffsetX = iIndexX - CELL_VIEW / 2 + i;
				int iOffsetZ = iIndexZ - CELL_VIEW / 2 + j;
				m_mLeafNode[Vector2f( (float) i, (float) j )]->AttechChild( &( m_ppCell[iOffsetX][iOffsetZ] ) );
			}
		}
	}

	void    SceneManager::TravelScene()
	{
		// ��ѯ��ǰ��Ⱦ��
		Renderer * pRenderer = RenderTarget::GetActiveRenderTarget()->GetRenderer();
		m_dwCurFrame ++;

		// ���³�����
		if ( RenderTarget::GetActiveRenderTarget()->GetClassType() == Base::RenderTarget_Window )
		{
			_UpdateScene();
		}

		// �Ӹ��ڵ㿪ʼ����
		if ( m_pRootNode != NULL && m_pSceneCuller != NULL && m_pLightCuller != NULL && pRenderer != NULL )
		{
			// ��ʼ���ü���
			m_pSceneCuller->Clear();
			m_pSceneCuller->Initialize( RenderTarget::GetActiveCamera() );

			// ����������
			_VisibleTravel( m_pRootNode, pRenderer );

			// �ڵ�����
			_CollideTravel( pRenderer );

			if ( pRenderer->GetRenderType() != Renderer::RenderType_Shadow )
			{
				multimap< float, Entity * > * pVisibleList = pRenderer->GetRenderQueue( Renderer::RenderQueue_Light )->GetEntityList();

				multimap< float, Entity * >::iterator it;
				int iTempCount = 0;

				for ( it = pVisibleList->begin(); it != pVisibleList->end(); it++ )
				{
					if ( iTempCount >= MAX_LIGHT )
					{
						break;
					}

					Light * pCurLight = (Light *) (*it).second;

					// ��ʼ��������
					m_pLightCuller->Clear();
					m_pLightCuller->Initialize( pCurLight );
					pRenderer->GetRenderQueue( Renderer::RenderQueue_Bright )->Clear();
					pCurLight->GetEntityList()->clear();					

					// ����������
					_LightTravel( m_pRootNode, pRenderer, (Light *) (*it).second );

					// �ڵ�����
					_BrightTravel( pRenderer, (Light *) (*it).second );

					iTempCount++;
				}
			}
		}
    }

	Cell *    SceneManager::_FindCell( Cell * pParent, Vector3f& vPos )
	{
		map<Key, SceneNode* > * pChildMap = pParent->GetChildMap();
		if ( pChildMap->size() == 0 )
		{
			return pParent;
		}

		map<Key, SceneNode* >::iterator ChildIt;
		for ( ChildIt = pChildMap->begin(); ChildIt != pChildMap->end(); ChildIt++ )
		{
			if ( (*ChildIt).second->GetBound()->IsInBound( vPos ) )
			{
				return _FindCell( (Cell *) (*ChildIt).second, vPos );
			}
		}

		return NULL;
	}

	Cell *    SceneManager::GetCell( Vector3f& vPos )
	{
		int iIndexX = (int) vPos.X() / CELL_SIZE;
		int iIndexZ = (int) vPos.Z() / CELL_SIZE;

		return _FindCell( &m_ppCell[iIndexX][iIndexZ], vPos );
	}

	void    SceneManager::AddEntity( Entity * pEntity, Matrix4f& matWorld, bool bInit )
	{
		switch ( pEntity->GetClassType() )
		{
		case Base::Class_Entity:
			{
				( (MatrixData *) ( (EntityData *) pEntity->GetSharedData() )->m_pData[EntityData::Matrix4_World] )->Write( matWorld );
				break;
			}
		case Base::Entity_AnimEntity:
			{
				( (MatrixData *) ( (AnimEntityData *) pEntity->GetSharedData() )->m_pData[AnimEntityData::Matrix4_World] )->Write( matWorld );
				break;
			}
		case Base::Entity_Light:
			{
				( (MatrixData *) ( (LightData *) pEntity->GetSharedData() )->m_pData[LightData::Matrix4_World] )->Write( matWorld );
				break;
			}
		}
		
		if ( bInit )
		{
			pEntity->UpdateScene();
		}
		else
		{
			PostThreadMessage( GetThreadID(), WM_ADDENTITY, (WPARAM) pEntity, 0 );
		}
	}

	void    SceneManager::DelEntity( Entity * pEntity )
	{
		PostThreadMessage( GetThreadID(), WM_DELENTITY, (WPARAM) pEntity, 0 );
	}

	bool    SceneManager::MessageProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch ( uMsg )
		{
		case WM_ADDENTITY:
			{
				( (Entity *) wParam )->Load();
				( (Entity *) wParam )->UpdateScene();
				break;
			}
		case WM_UPDATEENTITY:
			{
				( (Entity *) wParam )->UpdateScene();
				break;
			}
		case WM_DELENTITY:
			{
				Entity * pEntity = (Entity *) wParam;
				if ( ! pEntity->IsReady() )
				{
					return false;
				}
				else
				{
					Cell * pCell = ( pEntity )->GetParent();
					pCell->DetachEntity( pEntity );
					SAFE_DELETE( pEntity );
				}

				break;
			}
		default:
			{
				break;
			}
		}

		return true;
	}

	SceneNode *    SceneManager::_SplitScene( int iSize, int iPosX, int iPosZ )
	{
		if ( iSize == 2 )
		{
			int iIndexX = iPosX / CELL_SIZE;
			int iIndexZ = iPosZ / CELL_SIZE;

			SceneNode * pLeafNode = new SceneNode( m_szNodeName );
			( (BoundingBox *) ( pLeafNode->GetBound() ) )->SetCenter( Vector3f( (float) iPosX, 0.0f, (float) iPosZ ) );
			float fExtent[3] = { (float) CELL_SIZE, (float) m_iHeight, (float) CELL_SIZE };
			( (BoundingBox *) ( pLeafNode->GetBound() ) )->SetExtent( fExtent );

			m_mLeafNode[Vector2f( (float) ( iIndexX - 1 ), (float) ( iIndexZ - 1 ) )] = pLeafNode;
			m_mLeafNode[Vector2f( (float) ( iIndexX - 1 ), (float) iIndexZ )] = pLeafNode;
			m_mLeafNode[Vector2f( (float) iIndexX, (float) ( iIndexZ - 1 ) )] = pLeafNode;
			m_mLeafNode[Vector2f( (float) iIndexX, (float) iIndexZ )] = pLeafNode;

			return pLeafNode;
		}

		SceneNode * pNewNode = new SceneNode( m_szNodeName );
		SceneNode * pSplitNode[4];

		m_szNodeName += L"_0";
		pSplitNode[0] = _SplitScene( iSize / 2, iPosX / 2, iPosZ / 2 );
		m_szNodeName.erase( m_szNodeName.length() - 2, 2 );

		m_szNodeName += L"_1";
		pSplitNode[1] = _SplitScene( iSize / 2, iPosX / 2, iPosZ + iPosZ / 2 );
		m_szNodeName.erase( m_szNodeName.length() - 2, 2 );

		m_szNodeName += L"_2";
		pSplitNode[2] = _SplitScene( iSize / 2, iPosX + iPosX / 2, iPosZ / 2 );
		m_szNodeName.erase( m_szNodeName.length() - 2, 2 );

		m_szNodeName += L"_3";
		pSplitNode[3] = _SplitScene( iSize / 2, iPosX + iPosX / 2, iPosZ + iPosZ / 2 );
		m_szNodeName.erase( m_szNodeName.length() - 2, 2 );

		for ( int i = 0; i < 4; i++ )
		{
			pNewNode->AttechChild( pSplitNode[i] );
		}

		( (BoundingBox *) ( pNewNode->GetBound() ) )->SetCenter( Vector3f( (float) iPosX, 0.0f, (float) iPosZ ) );
		float fExtent[3] = { (float) iSize * CELL_SIZE / 2, (float) m_iHeight, (float) iSize * CELL_SIZE / 2 };
		( (BoundingBox *) ( pNewNode->GetBound() ) )->SetExtent( fExtent );

		return pNewNode;
	}

	bool    SceneManager::_ResourceTravel( Vector2f& vPos )
	{
		Vector2f vCurCenter( (float) m_iCurIndexX * CELL_SIZE, (float) m_iCurIndexZ * CELL_SIZE );
		Vector2f vOffset( vPos.X() - vCurCenter.X(), vPos.Y() - vCurCenter.Y() );

		vector< Cell * > pLoadCell;
		vector< Cell * > pDelCell;

		// ���Ͻǵ�Ԫ
		if ( (int) vOffset.X() > CELL_SIZE && (int) vOffset.Y() > CELL_SIZE )
		{
			for ( int i = 0; i < CELL_VIEW; i++ )
			{
				int iLoadZ = m_iCurIndexZ - CELL_VIEW / 2 + i;
				m_ppCell[m_iCurIndexX + CELL_VIEW / 2][iLoadZ + 1].Load();
				m_ppCell[m_iCurIndexX - CELL_VIEW / 2][iLoadZ].Release();
			}

			for ( int i = 0; i < CELL_VIEW - 1; i++ )
			{
				int iLoadX = m_iCurIndexX - CELL_VIEW / 2 + i;
				m_ppCell[iLoadX + 1][m_iCurIndexZ + CELL_VIEW / 2].Load();
				m_ppCell[iLoadX + 1][m_iCurIndexZ - CELL_VIEW / 2].Release();
			}

			m_iCurIndexX = ( (int) vPos.X() + CELL_SIZE / 2 ) / CELL_SIZE;
			m_iCurIndexZ = ( (int) vPos.Y() + CELL_SIZE / 2 ) / CELL_SIZE;
			return true;
		}

		// ���½ǵ�Ԫ
		if ( (int) vOffset.X() > CELL_SIZE && (int) vOffset.Y() < - CELL_SIZE )
		{
			for ( int i = 0; i < CELL_VIEW; i++ )
			{
				int iLoadZ = m_iCurIndexZ - CELL_VIEW / 2 + i;
				m_ppCell[m_iCurIndexX + CELL_VIEW / 2][iLoadZ - 1].Load();
				m_ppCell[m_iCurIndexX - CELL_VIEW / 2][iLoadZ].Release();
			}

			for ( int i = 0; i < CELL_VIEW - 1; i++ )
			{
				int iLoadX = m_iCurIndexX - CELL_VIEW / 2 + i;
				m_ppCell[iLoadX + 1][m_iCurIndexZ - CELL_VIEW / 2 - 1].Load();
				m_ppCell[iLoadX + 1][m_iCurIndexZ + CELL_VIEW / 2 - 1].Release();
			}

			m_iCurIndexX = ( (int) vPos.X() + CELL_SIZE / 2 ) / CELL_SIZE;
			m_iCurIndexZ = ( (int) vPos.Y() + CELL_SIZE / 2 ) / CELL_SIZE;
			return true;
		}

		// ���½ǵ�Ԫ
		if ( (int) vOffset.X() < - CELL_SIZE && (int) vOffset.Y() < - CELL_SIZE )
		{
			for ( int i = 0; i < CELL_VIEW; i++ )
			{
				int iLoadZ = m_iCurIndexZ - CELL_VIEW / 2 + i;
				m_ppCell[m_iCurIndexX - CELL_VIEW / 2 - 1][iLoadZ - 1].Load();
				m_ppCell[m_iCurIndexX + CELL_VIEW / 2 - 1][iLoadZ].Release();
			}

			for ( int i = 0; i < CELL_VIEW - 1; i++ )
			{
				int iLoadX = m_iCurIndexX - CELL_VIEW / 2 + i;
				m_ppCell[iLoadX][m_iCurIndexZ - CELL_VIEW / 2 - 1].Load();
				m_ppCell[iLoadX][m_iCurIndexZ + CELL_VIEW / 2 - 1].Release();
			}

			m_iCurIndexX = ( (int) vPos.X() + CELL_SIZE / 2 ) / CELL_SIZE;
			m_iCurIndexZ = ( (int) vPos.Y() + CELL_SIZE / 2 ) / CELL_SIZE;
			return true;
		}

		// ���Ͻǵ�Ԫ
		if ( (int) vOffset.X() < - CELL_SIZE && (int) vOffset.Y() > CELL_SIZE )
		{
			for ( int i = 0; i < CELL_VIEW; i++ )
			{
				int iLoadZ = m_iCurIndexZ - CELL_VIEW / 2 + i;
				m_ppCell[m_iCurIndexX - CELL_VIEW / 2][iLoadZ + 1].Load();
				m_ppCell[m_iCurIndexX + CELL_VIEW / 2][iLoadZ].Release();
			}

			for ( int i = 0; i < CELL_VIEW - 1; i++ )
			{
				int iLoadX = m_iCurIndexX - CELL_VIEW / 2 + i;
				m_ppCell[iLoadX][m_iCurIndexZ + CELL_VIEW / 2 - 1].Load();
				m_ppCell[iLoadX][m_iCurIndexZ - CELL_VIEW / 2 - 1].Release();
			}

			m_iCurIndexX = ( (int) vPos.X() + CELL_SIZE / 2 ) / CELL_SIZE;
			m_iCurIndexZ = ( (int) vPos.Y() + CELL_SIZE / 2 ) / CELL_SIZE;
			return true;
		}

		// �Ҳ൥Ԫ
		if ( (int) vOffset.X() > CELL_SIZE )
		{
			for ( int i = 0; i < CELL_VIEW; i++ )
			{
				int iLoadZ = m_iCurIndexZ - CELL_VIEW / 2 + i;
				m_ppCell[m_iCurIndexX + CELL_VIEW / 2][iLoadZ].Load();
				m_ppCell[m_iCurIndexX - CELL_VIEW / 2][iLoadZ].Release();
			}

			m_iCurIndexX = ( (int) vPos.X() + CELL_SIZE / 2 ) / CELL_SIZE;
			m_iCurIndexZ = ( (int) vPos.Y() + CELL_SIZE / 2 ) / CELL_SIZE;
			return true;
		}

		// ��൥Ԫ
		if ( (int) vOffset.X() < - CELL_SIZE )
		{
			for ( int i = 0; i < CELL_VIEW; i++ )
			{
				int iLoadZ = m_iCurIndexZ - CELL_VIEW / 2 + i;
				m_ppCell[m_iCurIndexX - CELL_VIEW / 2 - 1][iLoadZ].Load();
				m_ppCell[m_iCurIndexX + CELL_VIEW / 2 - 1][iLoadZ].Release();
			}

			m_iCurIndexX = ( (int) vPos.X() + CELL_SIZE / 2 ) / CELL_SIZE;
			m_iCurIndexZ = ( (int) vPos.Y() + CELL_SIZE / 2 ) / CELL_SIZE;
			return true;
		}

		// �Ϸ���Ԫ
		if ( (int) vOffset.Y() > CELL_SIZE )
		{
			for ( int i = 0; i < CELL_VIEW; i++ )
			{
				int iLoadX = m_iCurIndexX - CELL_VIEW / 2 + i;
				m_ppCell[iLoadX][m_iCurIndexZ + CELL_VIEW / 2].Load();
				m_ppCell[iLoadX][m_iCurIndexZ - CELL_VIEW / 2].Release();
			}

			m_iCurIndexX = ( (int) vPos.X() + CELL_SIZE / 2 ) / CELL_SIZE;
			m_iCurIndexZ = ( (int) vPos.Y() + CELL_SIZE / 2 ) / CELL_SIZE;
			return true;
		}

		// �·���Ԫ
		if ( (int) vOffset.Y() < - CELL_SIZE )
		{
			for ( int i = 0; i < CELL_VIEW; i++ )
			{
				int iLoadX = m_iCurIndexX - CELL_VIEW / 2 + i;
				m_ppCell[iLoadX][m_iCurIndexZ - CELL_VIEW / 2 - 1].Load();
				m_ppCell[iLoadX][m_iCurIndexZ + CELL_VIEW / 2 - 1].Release();
			}

			m_iCurIndexX = ( (int) vPos.X() + CELL_SIZE / 2 ) / CELL_SIZE;
			m_iCurIndexZ = ( (int) vPos.Y() + CELL_SIZE / 2 ) / CELL_SIZE;
			return true;
		}

		return false;
	}

	void    SceneManager::_VisibleTravel( SceneNode * pSceneNode, Renderer * pRenderer )
	{
		// ��ȡ��������
		int iSceneType = pSceneNode->GetClassType();

		if ( iSceneType == Base::Class_SceneNode )
		{
			// ��ѯ�ڵ�������ӽڵ�
			map< Key, SceneNode * >::iterator NodeIt;
			for ( NodeIt = pSceneNode->GetChildMap()->begin(); NodeIt != pSceneNode->GetChildMap()->end(); ++NodeIt )
			{
				if ( m_pSceneCuller->IsVisible( (*NodeIt).second->GetBound() ) )
				{
					_VisibleTravel( (*NodeIt).second, pRenderer );
				}
			}
		}

		if ( iSceneType == Base::SceneNode_Cell || iSceneType == Base::SceneNode_Sector )
		{
			// ʵ��ɼ����ж�
			map< Key, Entity * >::iterator EntityIt;

			for ( EntityIt = pSceneNode->GetEntityMap()->begin(); EntityIt != pSceneNode->GetEntityMap()->end(); ++EntityIt )
			{
				int iEntityType = (*EntityIt).second->GetClassType();
				if ( iEntityType == Base::Entity_Light )
				{
					Light * pLight = (Light *) ( (*EntityIt).second );
					if ( pLight->GetOn() && m_pSceneCuller->IsVisible( pLight->GetBound() ) )
					{
						pRenderer->GetRenderQueue( Renderer::RenderQueue_Light )->InsertEntity( pLight );
					}
				}

				if ( m_pSceneCuller->IsVisible( (*EntityIt).second->GetBound() ) )
				{
					// ��������Ⱦ����
					pRenderer->GetRenderQueue( Renderer::RenderQueue_General )->InsertEntity( (*EntityIt).second );
				}
			}

			// ��ѯ�ڵ�������ӽڵ�
			map< Key, SceneNode * >::iterator NodeIt;
			for ( NodeIt = pSceneNode->GetChildMap()->begin(); NodeIt != pSceneNode->GetChildMap()->end(); ++NodeIt )
			{
				if ( m_pSceneCuller->IsVisible( (*NodeIt).second->GetBound() ) )
				{
					_VisibleTravel( (*NodeIt).second, pRenderer );
				}
			}
		}

		if ( iSceneType == Base::SceneNode_Sector )
		{
			// ���ڽڵ�
			SectorNode * pSectorNode = (SectorNode *) pSceneNode;
			map< Key, Portal * >::iterator PortalIt;

			for ( PortalIt = pSectorNode->GetPortalMap()->begin(); PortalIt != pSectorNode->GetPortalMap()->end(); ++PortalIt )
			{
				if ( (*PortalIt).second->GetOpen() && m_pSceneCuller->IsVisible( (*PortalIt).second->GetBound() ) )
				{
					// ��ʼ��ڼ���
					m_pSceneCuller->PushPortal( (*PortalIt).second );

					_VisibleTravel( (*PortalIt).second->GetOppositeNode( pSceneNode ), pRenderer );

					// ������ڼ���
					m_pSceneCuller->PopPortal( 1 );
				}
			}
		}
	}

	void    SceneManager::_CollideTravel( Renderer * pRenderer )
	{
		RenderQueue * pQueue = pRenderer->GetRenderQueue( Renderer::RenderQueue_General );
		multimap< float, Entity * >::iterator GeneralIt = pQueue->GetEntityList()->begin();
		while ( GeneralIt != pQueue->GetEntityList()->end() )
		{
			if ( m_pSceneCuller->IsVisible( (*GeneralIt).second->GetBound() ) )
			{
				if ( (*GeneralIt).second->IsReady() )
				{
					// ��������Ⱦ����
					pRenderer->GetRenderQueue( Renderer::RenderQueue_Visible )->InsertEntity( (*GeneralIt).second );
					(*GeneralIt).second->UpdateFrame();

					// �ڵ�����
					if ( (*GeneralIt).second->GetInnerBound() != NULL )
					{
						m_pSceneCuller->PushEntity( (*GeneralIt).second );
					}
				}
				else
				{
					// �첽����
					(*GeneralIt).second->Load();
				}
			}
			
			GeneralIt++;
		}
	}

	void    SceneManager::_LightTravel( SceneNode * pSceneNode, Renderer * pRenderer, Light * pLight )
	{
		// ��ȡ��������
		int iSceneType = pSceneNode->GetClassType();

		if ( iSceneType == Base::Class_SceneNode )
		{
			// ��ѯ�ڵ�������ӽڵ�
			map< Key, SceneNode * >::iterator NodeIt;
			for ( NodeIt = pSceneNode->GetChildMap()->begin(); NodeIt != pSceneNode->GetChildMap()->end(); ++NodeIt )
			{
				if ( m_pLightCuller->IsVisible( (*NodeIt).second->GetBound() ) )
				{
					_LightTravel( (*NodeIt).second, pRenderer, pLight );
				}
			}
		}

		if ( iSceneType == Base::SceneNode_Cell || iSceneType == Base::SceneNode_Sector )
		{
			// ʵ��ɼ����ж�
			Cell * pCell = (Cell *) pSceneNode;
			map< Key, Entity * >::iterator EntityIt;

			for ( EntityIt = pCell->GetEntityMap()->begin(); EntityIt != pCell->GetEntityMap()->end(); ++EntityIt )
			{
				if ( (*EntityIt).second == pLight )
				{
					continue;
				}

				if ( (*EntityIt).second->IsReady() && (*EntityIt).second->IsAffectByLight()
					&& m_pLightCuller->IsVisible( (*EntityIt).second->GetBound() )
					&& (*EntityIt).second->GetLastFrame() == RenderWindow::GetActiveRenderWindow()->GetCurrentFrame() )
				{
					Matrix4f * pLightMatrix = DataCenter::GetSingleton()->GetMatrixData( pLight, pLight->GetClassType(), EntityData::Matrix4_World )->Read();
					Vector4f vLightPos = pLightMatrix->GetColumn( 3 );

					pRenderer->GetRenderQueue( Renderer::RenderQueue_Bright )->InsertEntity( vLightPos, (*EntityIt).second );
				}
			}

			// ��ѯ�ڵ�������ӽڵ�
			map< Key, SceneNode * >::iterator NodeIt;
			for ( NodeIt = pCell->GetChildMap()->begin(); NodeIt != pCell->GetChildMap()->end(); ++NodeIt )
			{
				if ( m_pLightCuller->IsVisible( (*NodeIt).second->GetBound() ) )
				{
					_LightTravel( (*NodeIt).second, pRenderer, pLight );
				}
			}
		}

		if ( iSceneType == Base::SceneNode_Sector )
		{
			// ���ڽڵ�
			SectorNode * pSectorNode = (SectorNode *) pSceneNode;
			map< Key, Portal * >::iterator PortalIt;

			for ( PortalIt = pSectorNode->GetPortalMap()->begin(); PortalIt != pSectorNode->GetPortalMap()->end(); ++PortalIt )
			{
				if ( (*PortalIt).second->GetOpen() && m_pSceneCuller->IsVisible( (*PortalIt).second->GetBound() ) )
				{
					// ��ʼ��ڼ���
					m_pLightCuller->PushPortal( (*PortalIt).second );

					_LightTravel( (*PortalIt).second->GetOppositeNode( pSceneNode ), pRenderer, pLight );

					// ������ڼ���
					m_pLightCuller->PopPortal( 1 );
				}
			}
		}
	}

	void    SceneManager::_BrightTravel( Renderer * pRenderer, Light * pLight )
	{
		RenderQueue * pQueue = pRenderer->GetRenderQueue( Renderer::RenderQueue_Bright );
		multimap< float, Entity * >::iterator BrightIt = pQueue->GetEntityList()->begin();
		while ( BrightIt != pQueue->GetEntityList()->end() )
		{
			if ( m_pLightCuller->IsVisible( (*BrightIt).second->GetBound() ) )
			{
				if ( (*BrightIt).second->IsReady() )
				{
					pLight->AttachEntity( (*BrightIt).second );

					// �ڵ�����
					if ( (*BrightIt).second->GetInnerBound() != NULL )
					{
						m_pLightCuller->PushEntity( (*BrightIt).second );
					}
				}
			}

			BrightIt++;
		}
	}
}
