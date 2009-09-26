#include "D3D9Material.h"
#include "D3D9Effect.h"
#include "D3D9Texture.h"
#include "../Main/DataCenter.h"

namespace Flagship
{
	D3D9Material::D3D9Material()
	{
		m_iClassType = Base::Material_General;
		m_pSkinnedMap = new D3D9Texture;
	}

	D3D9Material::~D3D9Material()
	{
		SAFE_DELETE( m_pSkinnedMap );
	}

	void    D3D9Material::Update( Renderable * pParent )
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

				ZeroMemory( pData, iPitch );
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

	void    D3D9Material::Update( Resource * pParent )
	{

	}

	bool    D3D9Material::Load()
	{
		if ( ! Material::Load() )
		{
			return false;
		}

		m_pSkinnedMap->CreateDynamic( 1024, 1, D3DFMT_A32B32G32R32F );
		return true;
	}

	bool    D3D9Material::Release()
	{
		if ( ! Material::Release() )
		{
			return false;
		}

		m_pSkinnedMap->Release();
		return true;
	}
}
