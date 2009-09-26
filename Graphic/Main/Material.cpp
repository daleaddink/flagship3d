#include "Material.h"
#include "Effect.h"
#include "Texture.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "ResourceLoader.h"
#include "Renderable.h"
#include "Resource.h"
#include "DataCenter.h"
#include "RenderTarget.h"
#include "Renderer.h"
#include "SceneNode.h"
#include "Cell.h"
#include "ShadowMap.h"
#include "Entity.h"
#include "AnimEntity.h"
#include "Light.h"
#include "GlobalParam.h"

#include <Common/LogManager.h>

namespace Flagship
{
	Material::Material()
	{
		m_iClassType = Base::Material_General;
		m_pEffect = NULL;
		m_pTexture = NULL;
		m_szTexturePath = L"";

		m_bFirst = true;
	}

	Material::~Material()
	{
		
	}

	bool    Material::Begin( UINT * uiPassNum )
	{
		return m_pEffect->Begin( uiPassNum );
	}

	bool    Material::End()
	{
		return m_pEffect->End();
	}

	bool    Material::BeginPass( UINT uiPass )
	{
		return m_pEffect->BeginPass( uiPass );
	}

	bool    Material::EndPass()
	{
		return m_pEffect->EndPass();
	}

	void    Material::SetTexturePath( wstring& szPath )
	{
		m_szTexturePath = szPath.c_str();
	}

	bool    Material::Initialize()
	{
		return true;
	}

	bool    Material::Reset()
	{
		return true;
	}

	void    Material::BuildParamMap()
	{
		map< int, Key >::iterator it;
		for ( it = m_mParamMap.begin(); it != m_mParamMap.end(); it++ )
		{
			switch ( (*it).first )
			{
			case Entity_WorldMatrix:
			case Entity_ViewMatrix:
    		case Entity_ProjMatrix:
			case Entity_MVPMatrix:
			case Entity_WorldIMatrix:
			case Entity_ViewIMatrix:
				{
					m_pEffect->AddParamHandle( Effect::Param_Matrix, (*it).second );
					break;
				}
			case AnimEntity_SkinnedMatrix:
				{
					m_pEffect->AddParamHandle( Effect::Param_MatrixArray, (*it).second );
					break;
				}
			case AnimEntity_SkinnedMap:
				{
					m_pEffect->AddParamHandle( Effect::Param_Texture, (*it).second );
					break;
				}
			case Camera_Position:
			case Camera_Direction:
			case Camera_Up:
			case Camera_Right:
			case Light_Ambient:
			case Light_Color:
			case Light_Position:
			case Light_Direction:
			case Light_Attenuation:
				{
					m_pEffect->AddParamHandle( Effect::Param_Vector, (*it).second );
					break;
				}
			default:
				break;
			}
		}

		for ( it = m_mTechniqueMap.begin(); it != m_mTechniqueMap.end(); it++ )
		{
			m_pEffect->AddParamHandle( Effect::Param_Technique, (*it).second );
		}
	}

	void    Material::ClearParamMap()
	{

	}

	bool    Material::SetTechnique( Renderable * pParent, BYTE byTechType )
	{
		if ( m_bFirst )
		{
			BuildParamMap();
			m_bFirst = false;
		}

		if ( pParent->GetClassType() == Entity_AnimEntity )
		{
			byTechType ++;
		}
		
		if ( m_mTechniqueMap.find( byTechType ) == m_mTechniqueMap.end() )
		{
			return false;
		}

		return m_pEffect->SetTechnique( m_mTechniqueMap[byTechType] );
	}

	void    Material::Update( Renderable * pParent )
	{
		m_iFlatShadowMap = 0;
		m_iCubeShadowMap = 0;

		// 获取当前摄像机
		Camera * pCamera = RenderTarget::GetActiveCamera();

		// 设置最近渲染时间
		m_pEffect->UpdateFrame();

		if ( m_pTexture != NULL )
		{
			m_pTexture->UpdateFrame();
		}

		// 更新材质参数
		map< int, Key >::iterator it;
		for ( it = m_mParamMap.begin(); it != m_mParamMap.end(); ++it )
		{
			switch ( (*it).first )
			{
			// 世界矩阵
			case Entity_WorldMatrix:
				{
					Matrix4f * pWorld = DataCenter::GetSingleton()->GetMatrixData( pParent, pParent->GetClassType(), RenderableData::Matrix4_World )->Get();
					m_pEffect->SetMatrix( (*it).second, pWorld );

					break;
				}
			// 视矩阵
			case Entity_ViewMatrix:
				{
					Matrix4f * pView = DataCenter::GetSingleton()->GetMatrixData( pCamera, pCamera->GetClassType(), CameraData::Matrix4_View )->Get();
					m_pEffect->SetMatrix( (*it).second, pView );

					break;
				}
			// 投影矩阵
			case Entity_ProjMatrix:
				{
					Matrix4f * pProj = DataCenter::GetSingleton()->GetMatrixData( pCamera, pCamera->GetClassType(), CameraData::Matrix4_Proj )->Get();
					m_pEffect->SetMatrix( (*it).second, pProj );

					break;
				}
			// 世界矩阵Inverse
			case Entity_WorldIMatrix:
				{
					Matrix4f * pWorld = DataCenter::GetSingleton()->GetMatrixData( pParent, pParent->GetClassType(), RenderableData::Matrix4_World )->Get();
					Matrix4f matWorldI = * pWorld;
					matWorldI = matWorldI.Inverse();
					m_pEffect->SetMatrix( (*it).second, &matWorldI );

					break;
				}
			// 视矩阵Inverse
			case Entity_ViewIMatrix:
				{
					Matrix4f * pView = DataCenter::GetSingleton()->GetMatrixData( pCamera, pCamera->GetClassType(), CameraData::Matrix4_View )->Get();
					Matrix4f matViewI = * pView;
					matViewI = matViewI.Inverse();
					m_pEffect->SetMatrix( (*it).second, &matViewI );

					break;
				}
			// 摄像机位置
			case Camera_Position:
				{
					Vector4f * pEyePos = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Position )->Get();
					m_pEffect->SetVector( (*it).second, pEyePos );

					break;
				}
			// 摄像机正方向
			case Camera_Direction:
				{
					Vector4f * pEyeDir = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Direction )->Get();
					m_pEffect->SetVector( (*it).second, pEyeDir );

					break;
				}
			// 摄像机上方向
			case Camera_Up:
				{
					Vector4f * pEyeUp = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Up )->Get();
					m_pEffect->SetVector( (*it).second, pEyeUp );

					break;
				}
			// 摄像机右方向
			case Camera_Right:
				{
					Vector4f * pEyeRight = DataCenter::GetSingleton()->GetVectorData( pCamera, pCamera->GetClassType(), CameraData::Vector4_Right )->Get();
					m_pEffect->SetVector( (*it).second, pEyeRight );

					break;
				}
			default:
				break;
			}

			// 实体参数
			if ( pParent->GetClassType() >= Base::Class_Entity && pParent->GetClassType() < Base::Entity_Max )
			{
				// 环境光
				if ( (*it).first == Light_Ambient )
				{
					Vector4f * pAmbientColor = GlobalParam::GetSingleton()->m_vAmbientColor.Get();
					m_pEffect->SetVector( (*it).second, pAmbientColor );
					continue;
				}

				// 当前光源
				Light * pLight = GlobalParam::GetSingleton()->m_pCurLight;
				switch ( (*it).first )
				{
				// 光源颜色
				case Light_Color:
					{
						if ( pLight != NULL )
						{
							Vector4f * pColor = DataCenter::GetSingleton()->GetVectorData( pLight, pLight->GetClassType(), LightData::Vector4_Color )->Get();
							m_pEffect->SetVector( (*it).second, pColor );
						}

						break;
					}
				// 光源位置
				case Light_Position:
					{
						if ( pLight != NULL )
						{
							Vector4f * pPosition = DataCenter::GetSingleton()->GetVectorData( pLight, pLight->GetClassType(), LightData::Vector4_Position )->Get();
							m_pEffect->SetVector( (*it).second, pPosition );
						}

						break;
					}
				// 光源方向
				case Light_Direction:
					{
						if ( pLight != NULL )
						{
							Vector4f * pDirection = DataCenter::GetSingleton()->GetVectorData( pLight, pLight->GetClassType(), LightData::Vector4_Direction )->Get();
							m_pEffect->SetVector( (*it).second, pDirection );
						}

						break;
					}
				case Light_Attenuation:
					{
						if ( pLight != NULL )
						{
							Vector4f * pAttenuation = DataCenter::GetSingleton()->GetVectorData( pLight, pLight->GetClassType(), LightData::Vector4_Attenuation )->Get();
							m_pEffect->SetVector( (*it).second, pAttenuation );
						}

						break;
					}
				// 骨骼矩阵数组
				case AnimEntity_SkinnedMatrix:
					{
						if ( pParent->GetClassType() == Base::Entity_AnimEntity )
						{
							Matrix4f * pSkinnedMatrix = DataCenter::GetSingleton()->GetMatrixArrayData( pParent, pParent->GetClassType(), AnimEntityData::Matrix4_Skinned )->Get();
							int * pBoneNum = DataCenter::GetSingleton()->GetIntData( pParent, pParent->GetClassType(), AnimEntityData::Int_BoneNum )->Get();
							m_pEffect->SetMatrixArray( (*it).second, pSkinnedMatrix, (UINT) (* pBoneNum) );
						}

						break;
					}
				// 贴图
				case Texture_Color:
					{
						m_pEffect->SetTexture( (*it).second, m_pTexture );
						break;
					}
				default:
					break;
				}
			}
		}
	}

	void    Material::Update( Resource * pParent )
	{

	}

	void    Material::PostProcess()
	{

	}

	bool    Material::IsReady()
	{
		if ( ! m_pEffect->IsReady() )
		{
			return false;
		}

		if ( m_szTexturePath != L"" )
		{
			if ( ! m_pTexture->IsReady() )
			{
				return false;
			}
		}

		return true;
	}

	bool    Material::Load()
	{
		ResourceLoader::GetSingleton()->PushResource( m_pEffect );
		if ( m_pTexture != NULL )
		{
			ResourceLoader::GetSingleton()->PushResource( m_pTexture );
		}

		return true;
	}

	bool    Material::Release()
	{
		m_pEffect->Release();
		m_pTexture->Release();

		return true;
	}
}