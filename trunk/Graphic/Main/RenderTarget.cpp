#include "RenderTarget.h"
#include "Renderer.h"
#include "RenderQueue.h"
#include "SceneManager.h"
#include "ShadowMap.h"
#include "Light.h"
#include "DataCenter.h"

namespace Flagship
{
	RenderTarget *    RenderTarget::m_pActiveRenderTarget = NULL;
	Camera *    RenderTarget::m_pActiveCamera = NULL;

	RenderTarget::RenderTarget()
	{

	}

	RenderTarget::~RenderTarget()
	{
		
	}

	void    RenderTarget::SetCamera( Camera* pCamera )
	{
		m_pCamera = pCamera;
	}

	Renderer *    RenderTarget::GetRenderer()
	{
		return m_pRenderer;
	}

	void    RenderTarget::SetActiveRenderTarget( RenderTarget * pTarget )
	{
		m_pActiveRenderTarget = pTarget;
	}

	RenderTarget *    RenderTarget::GetActiveRenderTarget()
	{
		return m_pActiveRenderTarget;
	}

	void    RenderTarget::SetActiveCamera( Camera * pCamera )
	{
		m_pActiveCamera = pCamera;
	}

	Camera *    RenderTarget::GetActiveCamera()
	{
		return m_pActiveCamera;
	}

	void    RenderTarget::Update()
	{
		// 设置当前渲染对象
		m_pActiveRenderTarget = this;
		// 设置当前摄像机
		m_pActiveCamera = m_pCamera;

		// 设置摄像机共享数据
		DataCenter::GetSingleton()->SetSharedData( m_pActiveCamera, m_pActiveCamera->GetClassType() );

		// 渲染场景
		m_pRenderer->RenderScene();
	}

	void    RenderTarget::PrepareShadowMap()
	{
		m_iSMCount = 0;

		// 可见实体列表
		multimap< float, Entity * > * pLightList = m_pRenderer->GetRenderQueue( Renderer::RenderQueue_Light )->GetEntityList();

		multimap< float, Entity * >::iterator it;
		for ( it = pLightList->begin(); it != pLightList->end(); it++ )
		{
			Light * pLight = (Light *) (*it).second;
			switch ( pLight->GetLightType() )
			{
			case Light::LightType_Directional:
			case Light::LightType_Spot:
				{
					m_pShadowMap[m_iSMCount]->UpdateShadow( (Light *) (*it).second );
					m_iSMCount ++;
					break;
				}
			case Light::LightType_Point:
				{
					for ( int i = ShadowMap::Shadow_PositiveX; i < ShadowMap::Shadow_Max; i++ )
					{
						m_pShadowMap[m_iSMCount]->UpdateShadow( (Light *) (*it).second, i );
						m_iSMCount ++;
					}
					
					break;
				}
			default:
				break;
			}
		}
	}

	void    RenderTarget::SaveRenderTexture( wstring szPath )
	{

	}
}