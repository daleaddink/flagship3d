#include "GlobalParam.h"
#include "Material.h"
#include "Light.h"
#include "ShadowMap.h"
#include "RenderPass.h"

namespace Flagship
{
	GlobalParam *    GlobalParam::m_pGlobalParam = NULL;
	
	GlobalParam::GlobalParam()
	{
		m_pGlobalMaterial = NULL;
		m_pMakeShadow = NULL;
		m_pPostShadow = NULL;
		m_pAmbient = NULL;
		m_pCurLight = NULL;
		m_pCurShadowMap = NULL;

		m_pRenderPass = new RenderPass;
		m_vAmbientColor.Write( Vector4f( 0.1f, 0.1f, 0.1f, 1.0f ) );
	}

	GlobalParam::~GlobalParam()
	{
		SAFE_DELETE( m_pGlobalParam );
		SAFE_DELETE( m_pRenderPass );
	}

	GlobalParam *    GlobalParam::GetSingleton()
	{
		if ( m_pGlobalParam == NULL )
		{
			m_pGlobalParam = new GlobalParam;
		}

		return m_pGlobalParam;
	}
}