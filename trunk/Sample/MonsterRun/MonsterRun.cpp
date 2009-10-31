#include "MonsterRun.h"

MonsterRun g_MonsterRunApp;

MonsterRun::MonsterRun()
{
	m_pLogic = new MonsterRunLogic;
}

MonsterRun::~MonsterRun()
{
	SAFE_DELETE( m_pLogic );
}

MonsterRunLogic::MonsterRunLogic()
{
	m_pCamera = new FreeCamera( wstring( L"FreeCamera" ) );
	m_pFreeCamera = (FreeCamera *) m_pCamera;

#ifdef FLAGSHIP_DX10
	m_pMonsterMesh = new D3D10PMFMesh;
	m_pStoneMesh = new D3D10PMFMesh;
	m_pBoxMesh = new D3D10SDKMesh;
	m_pSkyBox = new D3D10SkyBox;
	m_pThinFlim = new D3D10ThinFilm;
	m_pBasic = new D3D9Basic;
	m_pHDR = new D3D10HDR;
#else
	m_pMonsterMesh = new D3D9PMFMesh;
	m_pStoneMesh = new D3D9OgreMesh;
	m_pBoxMesh = new D3D9XMesh;
	m_pTerrainTile = new D3D9TerrainTile;
	m_pSkyBox = new D3D9SkyBox;
	m_pThinFlim = new D3D9ThinFlim;
	m_pBasic = new D3D9Basic;
	m_pTerrain = new D3D9Terrain;
	m_pHDR = new D3D9HDR;
#endif

	m_bMonsterRun = false;
	m_iEntityCount = 0;
}

MonsterRunLogic::~MonsterRunLogic()
{
	SAFE_DELETE( m_pCamera );
	SAFE_DELETE( m_pMonsterMesh );
	SAFE_DELETE( m_pStoneMesh );
	SAFE_DELETE( m_pBoxMesh );
	SAFE_DELETE( m_pSkyBox );
	SAFE_DELETE( m_pThinFlim );
	SAFE_DELETE( m_pHDR );
}

void    MonsterRunLogic::Initialize()
{
	Logic::Initialize();

	RECT rc;
	GetWindowRect( g_MonsterRunApp.GetWindowHandle(), &rc );
	float fAspect = (float)( rc.right - rc.left ) / (float)( rc.bottom - rc.top );

	m_uiKeyDown = 0;
	Vector4f vPos( 300, 1, 292.0f, 1 );
	Vector4f vDir( 0, 0, 1, 0 );
	Vector4f vUp( 0, 1, 0, 0 );
	Vector4f vRight( 1, 0, 0, 0 );
	m_pFreeCamera->SetView( vPos, vDir, vUp, vRight );
	m_pFreeCamera->SetProjection( 3.14f / 4.0f, fAspect, 0.1f, 100.0f );

	wchar_t szPath[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, szPath );

	wstring szTexturePath = szPath;
	szTexturePath += L"\\Resource\\HDRCubeMap.dds";
	wstring szMeshPath = szPath;
	szMeshPath += L"\\Resource\\Monster.pmf";

	m_pSkyBox->SetTexturePath( szTexturePath );
	m_pSkyBox->Initialize();
	m_pTerrain->Initialize();
	m_pThinFlim->Initialize();
	m_pHDR->Initialize();

	wstring szColorPath = szPath;
	szColorPath += L"\\Resource\\Monster.dds";
	m_pBasic->SetTexturePath( szColorPath );
	m_pBasic->Initialize();
	PostProcessQueue::GetSingleton()->AddPostProcess( m_pHDR );

	SceneManager::GetSingleton()->BuildScene( 16, 16, 512 );

	m_pMonsterMesh->SetPath( szMeshPath.c_str() );

	szMeshPath = szPath;
	szMeshPath += L"\\Resource\\cube.mesh";
	m_pStoneMesh->SetPath( szMeshPath.c_str() );

	szMeshPath = szPath;
	szMeshPath += L"\\Resource\\Monster.psf";
	m_kSkeleton.SetPath( szMeshPath.c_str() );

	szMeshPath = szPath;
	szMeshPath += L"\\Resource\\Monster.paf";
	m_kRunAnim.SetPath( szMeshPath.c_str() );

	szMeshPath = szPath;
#ifdef FLAGSHIP_DX10
	szMeshPath += L"\\Resource\\SkyBox.sdkmesh";
#else
	szMeshPath += L"\\Resource\\SkyBox.x";
#endif
	m_pBoxMesh->SetPath( szMeshPath.c_str() );

	float pSkyData[16] = { 1, 0, 0, 300,
		0, 1, 0, 0,
		0, 0, 1, 300,
		0, 0, 0, 1 };
	Matrix4f matSky( pSkyData, true );

	m_pSkyEntity = new Entity( wstring( L"SkyEntity" ) );
	m_pSkyEntity->AttachResource( m_pBoxMesh, m_pSkyBox );
	m_pSkyEntity->SetAffectByLight( false );
	m_pSkyEntity->SetCastShadow( false );
	m_pSkyEntity->SetReceiveShadow( false );
	
	float afTemp[3] = { FLT_MAX, FLT_MAX, FLT_MAX };
	BoundingBox kSkyBox( Vector3f( 0.0f, 0.0f, 0.0f ), NULL, afTemp );
	m_pSkyEntity->SetOuterBound( &kSkyBox );
	SceneManager::GetSingleton()->AddEntity( m_pSkyEntity, matSky, true );

	BoundingSphere kSphere( Vector3f(0.0f, 0.0f, 0.0f), FLT_MAX );

	m_pSunLight[0] = new Light( wstring( L"SunLight_0" ) );
	m_pSunLight[0]->SetLightType( Light::LightType_Directional );
	m_pSunLight[0]->SetOuterBound( &kSphere );
	m_pSunLight[0]->SetLightDirection( Vector4f( 1.0f, -1.0f, 0.0f, 0.0f ) );
	SceneManager::GetSingleton()->AddEntity( m_pSunLight[0], matSky, true );

	m_pSunLight[1] = new Light( wstring( L"SunLight_1" ) );
	m_pSunLight[1]->SetLightType( Light::LightType_Directional );
	m_pSunLight[1]->SetOuterBound( &kSphere );
	m_pSunLight[1]->SetLightDirection( Vector4f( -1.0f, -1.0f, 0.0f, 0.0f ) );
	SceneManager::GetSingleton()->AddEntity( m_pSunLight[1], matSky, true );

	float pTerrainData[16] = { 1, 0, 0, 300,
		0, 1, 0, 0,
		0, 0, 1, 300,
		0, 0, 0, 1 };
	Matrix4f matTerrain( pTerrainData, true );

	m_pTerrainEntity = new Entity( wstring( L"TerrainEntity" ) );
	m_pTerrainEntity->AttachResource( m_pTerrainTile, m_pTerrain );
	m_pTerrainTile->SetConfig( 256, 256, 0 );
	m_pTerrainEntity->SetCastShadow( false );

	float afTemp2[3] = { 256.0f, 256.0f, 256.0f };
	BoundingBox kTerrainBox( Vector3f( 0.0f, 0.0f, 0.0f ), NULL, afTemp2 );
	m_pTerrainEntity->SetOuterBound( &kTerrainBox );
	SceneManager::GetSingleton()->AddEntity( m_pTerrainEntity, matTerrain, true );

	float pStoneData[16] = { 0.1f, 0, 0, 300,
		0, 0.1f, 0, 0,
		0, 0, 0.1f, 295,
		0, 0, 0, 1 };
	Matrix4f matStone( pStoneData, true );

	float afTemp3[3] = { 1.0f, 1.0f, 1.0f };
	BoundingBox kStoneBox( Vector3f( 0.5f, 0.5f, 0.5f ), NULL, afTemp3 );
	
	m_pStoneEntity = new Entity( wstring( L"StoneEntity" ) );
	m_pStoneEntity->AttachResource( m_pStoneMesh, m_pThinFlim );
//	m_pStoneEntity->SetInnerBound( m_pStoneMesh );
	m_pStoneEntity->SetOuterBound( &kStoneBox );
	m_pStoneEntity->SetCastShadow( false );
	m_pStoneEntity->SetReceiveShadow( false );
	SceneManager::GetSingleton()->AddEntity( m_pStoneEntity, matStone, true );

	// Ô¤¼ÓÔØ
	SceneManager::GetSingleton()->PreLoad( m_pCamera );
}

bool    MonsterRunLogic::LogicTick()
{
	if ( ! Logic::LogicTick() )
	{
		return false;
	}

	if ( m_bMonsterRun )
	{
		DWORD dwAngle = GetTickCount() / 50 % 360;
		float sinth = sin( (float) dwAngle / 180.0f * 3.14f );
		float costh = cos( (float) dwAngle / 180.0f * 3.14f );
		m_pSunLight[0]->SetLightDirection( Vector4f( costh, -1.0f, sinth, 0.0f ) );
		m_pSunLight[1]->SetLightDirection( Vector4f( -costh, -1.0f, -sinth, 0.0f ) );

		for ( int i = 0; i < (int)m_pThinFlimEntity.size(); i++ )
		{
			if ( m_pThinFlimEntity[i]->IsReady() )
			{
				m_pThinFlimEntity[i]->UpdateAnimation( GetElapsedTime() );
			}
		}
	}

	switch ( m_uiKeyDown )
	{
	case 0x57:
		{
			m_pFreeCamera->MoveForward( GetElapsedTime() );
		}
		break;
	case 0x53:
		{
			m_pFreeCamera->MoveBackward( GetElapsedTime() );
		}
		break;
	case 0x41:
		{
			m_pFreeCamera->MoveLeft( GetElapsedTime() );
		}
		break;
	case 0x44:
		{
			m_pFreeCamera->MoveRight( GetElapsedTime() );
		}
		break;
	case 0x45:
		{
			m_pFreeCamera->MoveUp( GetElapsedTime() );
		}
		break;
	case 0x51:
		{
			m_pFreeCamera->MoveDown( GetElapsedTime() );
		}
		break;
	default:
		break;
	}

	return true;
}

bool    MonsterRunLogic::MessageProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_KEYDOWN:
		{
			m_uiKeyDown = (UINT)wParam;
			if ( m_uiKeyDown == 0x52 )
		    {
				m_bMonsterRun = true;
				for ( int i = 0; i < (int)m_pRunTask.size(); i++ )
				{
					m_pRunTask[i]->Play();
				}
			}

			switch ( m_uiKeyDown )
			{
			case 0x5A:
				{
					wchar_t szNum[16];
					_itow_s( m_iEntityCount, szNum, 10 );
					wstring szEntityName = L"MonsterEntity_";
					szEntityName += szNum;

					float pThinFlimData[16] = { 1, 0, 0, 300 + (float) ( rand() % 100 ) / 20.0f - 2.0f,
						0, 1, 0, 0,
						0, 0, 1, 300 + (float) ( rand() % 100 ) / 20.0f - 2.0f,
						0, 0, 0, 1 };
					Matrix4f matThinFlim( pThinFlimData, true );
					
					AnimEntity * pNewEntity = new AnimEntity( szEntityName );
					pNewEntity->AttachResource( m_pMonsterMesh, m_pBasic );
					pNewEntity->SetSkeleton( &m_kSkeleton );
					pNewEntity->AddAnimation( &m_kRunAnim );
					AnimationTask * pNewTask = pNewEntity->AddAnimationTask( &m_kRunAnim, 0.0f, 1.0f, 1.0f, true, false );

					float afTemp[3] = { 0.2f, 0.6f, 0.2f };
					BoundingBox kEntityBox( Vector3f( 0.0f, 0.3f, 0.0f ), NULL, afTemp );
					pNewEntity->SetOuterBound( &kEntityBox );
					pNewEntity->SetReceiveShadow( false );

					SceneManager::GetSingleton()->AddEntity( pNewEntity, matThinFlim );
					m_pRunTask.push_back( pNewTask );
					m_pThinFlimEntity.push_back( pNewEntity );
					m_iEntityCount ++;
				}
				break;
			case 0x43:
				{
					if ( m_pThinFlimEntity.size() > 0 )
					{
						AnimEntity * pEntity = m_pThinFlimEntity.back();
					    SceneManager::GetSingleton()->DelEntity( pEntity );
						m_pThinFlimEntity.pop_back();
						m_pRunTask.pop_back();
						m_iEntityCount --;
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_KEYUP:
		{
			if ( m_uiKeyDown == 0x52 )
			{
				m_bMonsterRun = false;
				for ( int i = 0; i < (int)m_pRunTask.size(); i++ )
				{
					m_pRunTask[i]->Stop();
				}
			}

			m_uiKeyDown = 0;
		}
		break;
	case WM_RBUTTONDOWN:
		{
			ShowCursor( FALSE );
			m_kvPoint[0] =	(float)LOWORD( lParam );
			m_kvPoint[1] =	(float)HIWORD( lParam );
		}
		break;
	case WM_RBUTTONUP:
		{
			POINT pos;
			pos.x = (LONG)m_kvPoint[0];
			pos.y = (LONG)m_kvPoint[1];
			ClientToScreen( g_MonsterRunApp.GetWindowHandle(), &pos );
			SetCursorPos( pos.x, pos.y );
			ShowCursor( TRUE );
		}
		break;
	case WM_MOUSEMOVE:
		{
			if ( wParam & MK_RBUTTON )
			{
				float fX = (float)LOWORD( lParam );
				float fY = (float)HIWORD( lParam );
				Vector2f kRotate( ( fX - m_kvPoint[0] ) * 0.01f, ( fY - m_kvPoint[1] ) * 0.01f );
				m_pFreeCamera->Rotate( kRotate[0], kRotate[1], 0.0f );

				POINT pos;
				pos.x = (LONG)m_kvPoint[0];
				pos.y = (LONG)m_kvPoint[1];
				ClientToScreen( g_MonsterRunApp.GetWindowHandle(), &pos );
				SetCursorPos( pos.x, pos.y );
			}
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return true;
}

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	hPrevInstance;
	lpCmdLine;

	return g_MonsterRunApp.Run( hInstance, nCmdShow );
}
