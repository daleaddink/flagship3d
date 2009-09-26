#include "D3D10Material.h"
#include "D3D10Effect.h"
#include "D3D10LineTexture.h"
#include "../Main/DataCenter.h"
#include "../Main/RenderTarget.h"
#include "../Main/Renderer.h"

namespace Flagship
{
	D3D10Material::D3D10Material()
	{
		m_iClassType = Class_Material;
		m_pSkinnedMap = new D3D10LineTexture;
	}

	D3D10Material::~D3D10Material()
	{
		SAFE_DELETE( m_pSkinnedMap );
	}

	ID3D10EffectTechnique *    D3D10Material::GetTechnique()
	{
		return ( (D3D10Effect *) m_pEffect )->GetTechnique();
	}

	Key&    D3D10Material::GetTechKey()
	{
		return m_kTechKey;
	}

	bool    D3D10Material::SetTechnique( Renderable * pParent, BYTE byTechType )
	{
		if ( ! Material::SetTechnique( pParent, byTechType ) )
		{
			return false;
		}

		wstring szName = m_pEffect->GetKey().GetName();

		Renderer * pRenderer = RenderTarget::GetActiveRenderTarget()->GetRenderer();
		if ( pRenderer->GetRenderType() == Renderer::RenderType_Shadow )
		{
			szName += m_mTechniqueMap[byTechType].GetName();
		}
		else
		{
			switch ( pParent->GetClassType() )
			{
			case Class_Entity:
				{
					szName += m_mTechniqueMap[byTechType].GetName();
				}
				break;
			case Entity_AnimEntity:
				{
					if ( byTechType == Technique_Light )
					{
						byTechType = Technique_LightS;
					}

					if ( byTechType == Technique_Color )
					{
						byTechType = Technique_ColorS;
					}

					szName += m_mTechniqueMap[byTechType].GetName();
				}
				break;
			default:
				break;
			}
		}

		m_kTechKey.SetName( szName );
		return true;
	}

	void    D3D10Material::Update( Renderable * pParent )
	{
		Material::Update( pParent );

		if ( pParent->GetClassType() == Base::Entity_AnimEntity )
		{
			map< int, Key >::iterator it = m_mParamMap.find( AnimEntity_SkinnedMap );
			if ( it != m_mParamMap.end() )
			{
				// »ñÈ¡¹Ç÷À¾ØÕó
				Matrix4f * pSkinnedMatrix = DataCenter::GetSingleton()->GetMatrixArrayData( pParent, pParent->GetClassType(), AnimEntityData::Matrix4_Skinned )->Get();
				int * pBoneNum = DataCenter::GetSingleton()->GetIntData( pParent, pParent->GetClassType(), AnimEntityData::Int_BoneNum )->Get();

				// ¿½±´¾ØÕóÖÁÌùÍ¼
				int iPitch = 0;
				DWORD dwOffset = 0;
				BYTE * pData = NULL;
				m_pSkinnedMap->Lock( iPitch, (void **) &pData );

				for ( int i = 0; i < * pBoneNum; i++ )
				{
					for ( int j = 0; j < 4; j++ )
					{
						Vector4f * pVector = (Vector4f *) ( pData + dwOffset );
						* pVector = pSkinnedMatrix[i].GetColumn( j );
						dwOffset += sizeof(Vector4f);
					}
				}

				m_pSkinnedMap->UnLock();
				m_pEffect->SetTexture( (*it).second, m_pSkinnedMap );
			}
		}
	}

	void    D3D10Material::Update( Resource * pParent )
	{

	}

	bool    D3D10Material::Load()
	{
		if ( ! Material::Load() )
		{
			return false;
		}

		m_pSkinnedMap->CreateDynamic( 1024, 1, DXGI_FORMAT_R32G32B32A32_FLOAT );
		return true;
	}

	bool    D3D10Material::Release()
	{
		if ( ! Material::Release() )
		{
			return false;
		}

		m_pSkinnedMap->Release();
		return true;
	}
}