#include "D3D9Terrain.h"
#include "../Renderer/D3D9Effect.h"
#include "../Renderer/D3D9Texture.h"
#include "../Main/Camera.h"
#include "../Main/RenderWindow.h"
#include "../Main/DataCenter.h"
#include "../Main/ResourceManager.h"
#include "../Main/ResourceLoader.h"

namespace Flagship
{
	D3D9Terrain::D3D9Terrain()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();

		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D9Terrain::~D3D9Terrain()
	{
		SAFE_DELETE( m_pEffect );
	}

	bool    D3D9Terrain::Initialize()
	{
		// 创建Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D9Terrain.fx";

		D3D9EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();

		D3D9TextureHandle hHeightHandle;
		hHeightHandle.SetPath( m_kTerrainInfo.szHeightPath );
		m_pHeightMap = hHeightHandle.GetImpliment();

		D3D9TextureHandle hTexcoordHandle;
		hTexcoordHandle.SetPath( m_kTerrainInfo.szTexcoordPath );
		m_pTexcoordMap = hTexcoordHandle.GetImpliment();

		D3D9TextureHandle hNormalHandle;
		hNormalHandle.SetPath( m_kTerrainInfo.szNormalPath );
		m_pNormalMap = hNormalHandle.GetImpliment();

		return true;
	}

	void    D3D9Terrain::BuildParamMap()
	{
		// 建立参数表
		m_kWorldMatrix.SetName( wstring( L"g_matWorld" ) );
		Key kView( wstring( L"g_matView" ) );
		Key kProj( wstring( L"g_matProj" ) );
		Key kAmbientColor( wstring( L"g_vAmbientColor" ) );
		m_kHeightMap.SetName( wstring( L"g_HeightMap" ) );
		m_kTexcoordMap.SetName( wstring( L"g_TexcoordMap" ) );
		m_kNormalMap.SetName( wstring( L"g_NormalMap" ) );

		m_mParamMap[Material::Entity_WorldMatrix] = m_kWorldMatrix;
		m_mParamMap[Material::Entity_ViewMatrix] = kView;
		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::Light_Ambient] = kAmbientColor;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, m_kWorldMatrix );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kView );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Vector, kAmbientColor );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kHeightMap );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kTexcoordMap );
		m_pEffect->AddParamHandle( Effect::Param_Texture, m_kNormalMap );

		// 建立科技表
		Key kAmbient( wstring( L"Ambient" ) );
		Key kLight( wstring( L"Light" ) );
		Key kColor( wstring( L"Color" ) );
		m_mTechniqueMap[Material::Technique_Ambient] = kAmbient;
		m_mTechniqueMap[Material::Technique_Light] = kLight;
		m_mTechniqueMap[Material::Technique_Color] = kColor;
		m_pEffect->AddParamHandle( Effect::Param_Technique, kAmbient );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kLight );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kColor );
	}

	void    D3D9Terrain::Update( Renderable * pParent )
	{
		Material::Update( pParent );

		// 获取主摄像机
		Camera * pCamera = RenderWindow::GetActiveRenderWindow()->GetCamera();

		// 获取视矩阵
		Vector4f * pPos = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Position )->Get();
		
		Matrix4f matTerrain;
		matTerrain.MakeIdentity();
		matTerrain[0][3] = pPos->X();
		matTerrain[2][3] = pPos->Z();
//		m_pEffect->SetMatrix( m_kWorldMatrix, &matTerrain );

		m_pEffect->SetTexture( m_kHeightMap, m_pHeightMap );
		m_pEffect->SetTexture( m_kTexcoordMap, m_pTexcoordMap );
		m_pEffect->SetTexture( m_kNormalMap, m_pNormalMap );
	}

	void    D3D9Terrain::Update( Resource * pParent )
	{
		Material::Update( pParent );
	}

	bool    D3D9Terrain::IsReady()
	{
		if ( ! Material::IsReady() )
		{
			return false;
		}

/*		if ( ! m_pHeightMap->IsReady() )
		{
			return false;
		}

		if ( ! m_pTexcoordMap->IsReady() )
		{
			return false;
		}

		if ( ! m_pNormalMap->IsReady() )
		{
			return false;
		}
*/
		return true;
	}

	bool    D3D9Terrain::Load()
	{
		Material::Load();
		ResourceLoader::GetSingleton()->PushResource( m_pHeightMap );
		ResourceLoader::GetSingleton()->PushResource( m_pTexcoordMap );
		ResourceLoader::GetSingleton()->PushResource( m_pNormalMap );

		return true;
	}

	bool    D3D9Terrain::Release()
	{
		Material::Release();
		m_pHeightMap->Release();
		m_pTexcoordMap->Release();
		m_pNormalMap->Release();

		return true;
	}
}