#include "D3D9SkyBox.h"

#include "../Renderer/D3D9Effect.h"
#include "../Renderer/D3D9CubeTexture.h"
#include "../Main/ResourceManager.h"
#include "../Main/ResourceLoader.h"
#include "../Main/RenderWindow.h"
#include "../Main/Camera.h"
#include "../Main/Mesh.h"

#include "../Main/DataCenter.h"
#include "../Main/DataDefine.h"

namespace Flagship
{
	D3D9SkyBox::D3D9SkyBox()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();
		
		// 材质类型
		m_iClassType = Base::Material_General;
	}

	D3D9SkyBox::~D3D9SkyBox()
	{
		SAFE_DELETE( m_pEffect );
	}

	bool    D3D9SkyBox::Initialize()
	{
		// 创建D3D9Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D9SkyBox.fx";

		D3D9EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();

		// 建立贴图
		D3D9CubeTextureHandle hCubeTextureHandle;
		hCubeTextureHandle.SetPath( m_szTexturePath );
		m_pTexture = hCubeTextureHandle.GetImpliment();

		return true;
	}

	void    D3D9SkyBox::BuildParamMap()
	{
		// 建立参数表
		Key kProj( wstring( L"g_matProj" ) );
		m_kViewMatrix.SetName( wstring( L"g_matView" ) );
		Key kSkyTexture( wstring(L"g_Texture") );
		Key kAmbientColor( wstring( L"g_vAmbientColor" ) );

		m_mParamMap[Material::Entity_ProjMatrix] = kProj;
		m_mParamMap[Material::Texture_Color] = kSkyTexture;
		m_mParamMap[Material::Light_Ambient] = kAmbientColor;

		m_pEffect->AddParamHandle( Effect::Param_Matrix, kProj );
		m_pEffect->AddParamHandle( Effect::Param_Matrix, m_kViewMatrix );
		m_pEffect->AddParamHandle( Effect::Param_Texture, kSkyTexture );
		m_pEffect->AddParamHandle( Effect::Param_Vector, kAmbientColor );

		// 建立科技表
		Key kAmbient( wstring( L"Ambient" ) );
		Key kColor( wstring( L"Color" ) );
		m_mTechniqueMap[Material::Technique_Ambient] = kAmbient;
		m_mTechniqueMap[Material::Technique_Color] = kColor;
		m_pEffect->AddParamHandle( Effect::Param_Technique, kAmbient );
		m_pEffect->AddParamHandle( Effect::Param_Technique, kColor );
	}

	void    D3D9SkyBox::Update( Renderable * pParent )
	{
		Material::Update( pParent );

		// 获取主摄像机
		Camera * pCamera = RenderWindow::GetActiveRenderWindow()->GetCamera();

		// 更新天空盒视矩阵
		Matrix4f * pView = DataCenter::GetSingleton()->GetMatrixData( pCamera, pCamera->GetClassType(), CameraData::Matrix4_View )->Get();
		float pData[16] = { (*pView)[0][0], (*pView)[0][1], (*pView)[0][2], 0,
			                (*pView)[1][0], (*pView)[1][1], (*pView)[1][2], 0,
			                (*pView)[2][0], (*pView)[2][1], (*pView)[2][2], 0,
			                (*pView)[3][0], (*pView)[3][1], (*pView)[3][2], (*pView)[3][3] };
		Matrix4f matSkyBox( pData, true );
		m_pEffect->SetMatrix( m_kViewMatrix, &matSkyBox );
	}

	void    D3D9SkyBox::Update( Resource * pParent )
	{
		Material::Update( pParent );
	}
}