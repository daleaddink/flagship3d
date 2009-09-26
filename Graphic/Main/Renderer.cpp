#include "Renderer.h"
#include "RenderTargetQueue.h"
#include "RenderTexture.h"
#include "SceneManager.h"
#include "Material.h"
#include "Camera.h"
#include "ResourceLoader.h"
#include "DataCenter.h"
#include "GlobalParam.h"
#include "RenderPass.h"
#include "RenderWindow.h"
#include "ShadowMap.h"

namespace Flagship
{
	Renderer::Renderer( BYTE byRenderType )
	{
		m_byRenderType = byRenderType;
		m_pRenderQueue = new RenderQueue[ RenderQueue_Max ];
	}

	Renderer::~Renderer()
	{
		SAFE_DELETE_ARRAY( m_pRenderQueue );
	}

	RenderQueue *    Renderer::GetRenderQueue( int iType )
	{
		if ( iType < RenderQueue_Max )
		{
			return &m_pRenderQueue[ iType ];
		}
		else
		{
			return NULL;
		}
	}

	int    Renderer::Run()
	{
		while ( m_bRunning )
		{
			RenderTargetQueue::GetSingleton()->UpdateAll();
		}

		return 0;
	}

	bool    Renderer::BeginScene()
	{
		return true;
	}

	bool    Renderer::EndScene()
	{
		return true;
	}

	bool    Renderer::Clear( Vector4f& vColor )
	{
		return true;
	}

	bool    Renderer::Present()
	{
		return true;
	}

	bool    Renderer::RenderSingle( Material * pMaterial, Resource * pResource )
	{
		return true;	
	}

	bool    Renderer::RenderQuad( Effect * pEffect, float fLeftU, float fTopV, float fRightU, float fBottomV )
	{
		return true;
	}

	bool    Renderer::RenderText( int iX, int iY, Vector4f& vColor, wstring& szText )
	{
		return true;
	}

	void    Renderer::RenderScene()
	{
		// ��տ�����Ⱦ����
		GetRenderQueue( Renderer::RenderQueue_General )->Clear();
		GetRenderQueue( Renderer::RenderQueue_Visible )->Clear();
		GetRenderQueue( Renderer::RenderQueue_Light )->Clear();
		
		// ����������
		SceneManager::GetSingleton()->TravelScene();

		// ʵ���б�
		multimap< float, Entity * > * pVisibleList = GetRenderQueue( RenderQueue_Visible )->GetEntityList();
		multimap< float, Entity * > * pLightList = GetRenderQueue( RenderQueue_Light )->GetEntityList();

		if ( m_byRenderType == RenderType_Shadow )
		{
			// ������
			Clear( Vector4f( 1.0f, 1.0f, 1.0f, 1.0f ) );

			GlobalParam::GetSingleton()->m_pGlobalMaterial = GlobalParam::GetSingleton()->m_pMakeShadow;

			multimap< float, Entity * >::iterator it;
			for ( it = pVisibleList->begin(); it != pVisibleList->end(); it++ )
			{
				if ( (*it).second->IsCastShadow() )
				{
				    // ��Ⱦʵ��
				    (*it).second->Render( this, Material::Technique_MakeShadow );
				}
			}

			GlobalParam::GetSingleton()->m_pGlobalMaterial = NULL;
			return;
		}

		if ( m_byRenderType == RenderType_NoReflect )
		{
			multimap< float, Entity * >::iterator it = pVisibleList->begin();
			while ( it != pVisibleList->end() )
			{
				if ( (*it).second->GetMaterialType() != Base::Material_RenderTexture )
				{
					it = pVisibleList->erase( it );
				}
				else
				{
					it ++;
				}
			}
		}

		// ����ʵ�干������
		multimap< float, Entity * >::iterator EntityIt;
		for ( EntityIt = pVisibleList->begin(); EntityIt != pVisibleList->end(); EntityIt++ )
		{
			DataCenter::GetSingleton()->SetSharedData( (*EntityIt).second, (*EntityIt).second->GetClassType() );
		}

		// ���ù�Դ��������
		GlobalParam::GetSingleton()->m_vAmbientColor.Set();
		multimap< float, Entity * >::iterator LightIt;
		for ( LightIt = pLightList->begin(); LightIt != pLightList->end(); LightIt++ )
		{
			DataCenter::GetSingleton()->SetSharedData( (*LightIt).second, (*LightIt).second->GetClassType() );
		}
		
		// ׼����Ӱͼ
		RenderTarget::GetActiveRenderTarget()->PrepareShadowMap();

		// ������
		Clear();

		// AmbientPass
		GlobalParam::GetSingleton()->m_pRenderPass->Apply( RenderPass::RenderPass_Amibent, GetRenderQueue( RenderQueue_Visible ) );

		// LightPass
		GlobalParam::GetSingleton()->m_pRenderPass->Apply( RenderPass::RenderPass_Light, GetRenderQueue( RenderQueue_Light ) );

		// ShadowPass
		for ( int i = 0; i < RenderTarget::GetActiveRenderTarget()->GetShadowMapCount(); i++ )
		{
			GlobalParam::GetSingleton()->m_pCurShadowMap = RenderTarget::GetActiveRenderTarget()->GetShadowMap()[i];
			GlobalParam::GetSingleton()->m_pRenderPass->Apply( RenderPass::RenderPass_PostShadow, GetRenderQueue( RenderQueue_Visible ) );
		}
		GlobalParam::GetSingleton()->m_pCurShadowMap = NULL;

		// ColorPass
		GlobalParam::GetSingleton()->m_pRenderPass->Apply( RenderPass::RenderPass_Color, GetRenderQueue( RenderQueue_Visible ) );
	}
}