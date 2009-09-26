#include "RenderPass.h"
#include "RenderQueue.h"
#include "Light.h"
#include "GlobalParam.h"
#include "RenderWindow.h"
#include "Entity.h"
#include "Material.h"
#include "Renderer.h"

namespace Flagship
{
	RenderPass::RenderPass()
	{

	}

	RenderPass::~RenderPass()
	{

	}

	void    RenderPass::Apply( BYTE byRenderPass, RenderQueue * pRenderQueue )
	{
		Renderer * pRenderer = RenderTarget::GetActiveRenderTarget()->GetRenderer();

		switch ( byRenderPass )
		{
		case RenderPass_Light:
			{
				multimap< float, Entity * > * pVisibleList = pRenderQueue->GetEntityList();

				multimap< float, Entity * >::iterator it;
				for ( it = pVisibleList->begin(); it != pVisibleList->end(); it++ )
				{
					Light * pCurLight = (Light *) (*it).second;
					GlobalParam::GetSingleton()->m_pCurLight = pCurLight;

					list< Entity * >::iterator EntityIt = pCurLight->GetEntityList()->begin();
					while ( EntityIt != pCurLight->GetEntityList()->end() )
					{
						(*EntityIt)->Render( pRenderer, Material::Technique_Light );
						EntityIt ++;
					}
				}

				GlobalParam::GetSingleton()->m_pCurLight = NULL;
				break;
			}
		case RenderPass_PostShadow:
			{
				GlobalParam::GetSingleton()->m_pGlobalMaterial = GlobalParam::GetSingleton()->m_pPostShadow;

				// �ɼ�ʵ���б�
				multimap< float, Entity * > * pVisibleList = pRenderQueue->GetEntityList();

				multimap< float, Entity * >::iterator it;
				for ( it = pVisibleList->begin(); it != pVisibleList->end(); it++ )
				{
					if ( (*it).second->IsReceiveShadow() )
					{
						(*it).second->Render( pRenderer, Material::Technique_PostShadow );
					}
				}

				GlobalParam::GetSingleton()->m_pGlobalMaterial = NULL;
				break;
			}
		case RenderPass_Color:
			{
				// �ɼ�ʵ���б�
				multimap< float, Entity * > * pVisibleList = pRenderQueue->GetEntityList();

				multimap< float, Entity * >::iterator it;
				for ( it = pVisibleList->begin(); it != pVisibleList->end(); it++ )
				{
					(*it).second->Render( pRenderer, Material::Technique_Color );
				}

				break;
			}
		case RenderPass_Amibent:
			{
				GlobalParam::GetSingleton()->m_pGlobalMaterial = GlobalParam::GetSingleton()->m_pAmbient;

				// �ɼ�ʵ���б�
				multimap< float, Entity * > * pVisibleList = pRenderQueue->GetEntityList();

				multimap< float, Entity * >::iterator it;
				for ( it = pVisibleList->begin(); it != pVisibleList->end(); it++ )
				{
					(*it).second->Render( pRenderer, Material::Technique_Ambient );
				}

				GlobalParam::GetSingleton()->m_pGlobalMaterial = NULL;
				break;
			}
		default:
			break;
		}
	}
}