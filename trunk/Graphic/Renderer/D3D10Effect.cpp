#include "D3D10Effect.h"
#include "D3D10Renderer.h"
#include "D3D10RenderWindow.h"
#include "D3D10Texture.h"
#include "D3D10LineTexture.h"
#include "D3D10VolumeTexture.h"
#include "D3D10CubeTexture.h"
#include "D3D10RenderTexture.h"
#include "D3D10RenderCubeTexture.h"

namespace Flagship
{
	extern HINSTANCE g_hInstance;

	D3D10Effect::D3D10Effect()
	{
		m_iClassType = Base::Resource_Effect;
		m_pD3D10Effect = NULL;
	}

	D3D10Effect::~D3D10Effect()
	{
		SAFE_RELEASE( m_pD3D10Effect );
	}

	bool    D3D10Effect::Cache()
	{
		if ( ! Effect::Cache() )
		{
			return false;
		}

		CreateFromMemory();

		m_bReady = true;
		return true;
	}

	void    D3D10Effect::UnCache()
	{
		Effect::UnCache();
		m_mParamHandleMap.clear();
		m_mTechHandleMap.clear();

		SAFE_RELEASE( m_pD3D10Effect );
	}

	bool    D3D10Effect::CreateFromMemory()
	{
		char szPathName[256];
		wcstombs( szPathName, m_szPathName.c_str(), 256 );

		// 获取D3D10设备指针
		ID3D10Blob * pErrer;
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		HRESULT hr = D3DX10CreateEffectFromMemory( m_kFileBuffer.GetPointer(), m_kFileBuffer.GetSize(), NULL
			, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pD3D10Device, NULL, NULL, &m_pD3D10Effect, &pErrer, NULL );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, GetKey().GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::CreateFromResource() Fail! File:%s, %s", szFile, (char *)pErrer->GetBufferPointer() );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	void    D3D10Effect::AddParamHandle( BYTE byType, Key& szName )
	{
		char szTemp[256];
		wcstombs( szTemp, szName.GetName().c_str(), 256 );

		if ( byType == Param_Technique )
		{
			ID3D10EffectTechnique * pTechnique;
			pTechnique = m_pD3D10Effect->GetTechniqueByName( szTemp );
			m_mTechHandleMap[ szName ] = pTechnique;
		}
		else
		{
			ID3D10EffectVariable * pVariable;
			pVariable = m_pD3D10Effect->GetVariableByName( szTemp );
			m_mParamHandleMap[ szName ] = pVariable;
		}
	}

	bool    D3D10Effect::SetValue( Key& szParam, void * pValue, UINT uiSize )
	{
		HRESULT hr = m_mParamHandleMap[szParam]->SetRawValue( pValue, 0, uiSize );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::SetValue() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Effect::SetInt( Key& szParam, int * piValue )
	{
		HRESULT hr = m_mParamHandleMap[szParam]->AsScalar()->SetInt( * piValue );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::SetInt() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Effect::SetFloat( Key& szParam, float * pfValue )
	{
		HRESULT hr = m_mParamHandleMap[szParam]->AsScalar()->SetFloat( * pfValue );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::SetFloat() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Effect::SetVector( Key& szParam, Vector4f * pvValue )
	{
		D3DXVECTOR4 vVector( (*pvValue)[0], (*pvValue)[1], (*pvValue)[2], (*pvValue)[3] );
		HRESULT hr = m_mParamHandleMap[szParam]->AsVector()->SetFloatVector( (float *) &vVector );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::SetVector() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Effect::SetVectorArray( Key& szParam, Vector4f * pvValue, UINT uiNum )
	{
		D3DXVECTOR4 * pvVector;
		pvVector = new D3DXVECTOR4[uiNum];

		for ( UINT i = 0; i < uiNum; i++ )
		{
			pvVector[i] = D3DXVECTOR4( (pvValue[i])[0], (pvValue[i])[1], (pvValue[i])[2], (pvValue[i])[3] );
		}

		HRESULT hr = m_mParamHandleMap[szParam]->AsVector()->SetFloatVectorArray( (float *) pvVector, 0, uiNum );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::SetVectorArray() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		SAFE_DELETE_ARRAY( pvVector );
		return true;
	}

	bool    D3D10Effect::SetMatrix( Key& szParam, Matrix4f * pmatValue )
	{
		Matrix4f matTemp = pmatValue->Transpose();

		D3DXMATRIX matMatrix( matTemp[0][0], matTemp[0][1], matTemp[0][2], matTemp[0][3],
			                  matTemp[1][0], matTemp[1][1], matTemp[1][2], matTemp[1][3],
			                  matTemp[2][0], matTemp[2][1], matTemp[2][2], matTemp[2][3],
			                  matTemp[3][0], matTemp[3][1], matTemp[3][2], matTemp[3][3] );
		HRESULT hr = m_mParamHandleMap[szParam]->AsMatrix()->SetMatrix( (float *) &matMatrix );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::SetMatrix() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Effect::SetMatrixArray( Key& szParam, Matrix4f * pmatValue, UINT uiNum )
	{
		D3DXMATRIX * pmatMatrix;
		pmatMatrix = new D3DXMATRIX[uiNum];

		for ( UINT i = 0; i < uiNum; i++ )
		{
			Matrix4f matTemp = pmatValue[i].Transpose();
			pmatMatrix[i] = D3DXMATRIX( matTemp[0][0], matTemp[0][1], matTemp[0][2], matTemp[0][3],
				                        matTemp[1][0], matTemp[1][1], matTemp[1][2], matTemp[1][3],
				                        matTemp[2][0], matTemp[2][1], matTemp[2][2], matTemp[2][3],
				                        matTemp[3][0], matTemp[3][1], matTemp[3][2], matTemp[3][3] );
		}

		HRESULT hr = m_mParamHandleMap[szParam]->AsMatrix()->SetMatrixTransposeArray( (float *) pmatMatrix, 0, uiNum );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::SetMatrixArray() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		SAFE_DELETE_ARRAY( pmatMatrix );
		return true;
	}

	bool    D3D10Effect::SetTexture( Key& szParam, Texture * pTexture )
	{
		HRESULT hr;
		int iTextureType = pTexture->GetClassType();

		switch ( iTextureType )
		{
		case Base::Texture_General:
			{
				ID3D10ShaderResourceView * pD3D10Texture = ( (D3D10Texture *) pTexture )->GetShaderResourceView();
				hr = m_mParamHandleMap[szParam]->AsShaderResource()->SetResource( pD3D10Texture );

				break;
			}

		case Base::Texture_Line:
			{
				ID3D10ShaderResourceView * pD3D10Texture = ( (D3D10LineTexture *) pTexture )->GetShaderResourceView();
				hr = m_mParamHandleMap[szParam]->AsShaderResource()->SetResource( pD3D10Texture );

				break;
			}

		case Base::Texture_Volume:
			{
				ID3D10ShaderResourceView * pD3D10Texture = ( (D3D10VolumeTexture *) pTexture )->GetShaderResourceView();
				hr = m_mParamHandleMap[szParam]->AsShaderResource()->SetResource( pD3D10Texture );

				break;
			}

		case Base::Texture_Cube:
			{
				ID3D10ShaderResourceView * pD3D10Texture = ( (D3D10CubeTexture *) pTexture )->GetShaderResourceView();
				hr = m_mParamHandleMap[szParam]->AsShaderResource()->SetResource( pD3D10Texture );

				break;
			}
		}

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Effect::SetTexture() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Effect::SetTechnique( Key& szParam )
	{
		m_pCurTechnique = m_mTechHandleMap[szParam];
		return true;
	}

	bool    D3D10Effect::Begin( UINT * uiPassNum )
	{
		D3D10_TECHNIQUE_DESC techDesc;
		m_pCurTechnique->GetDesc( &techDesc );
		* uiPassNum = techDesc.Passes;

		return true;
	}

	bool    D3D10Effect::End()
	{
		return true;
	}

	bool    D3D10Effect::BeginPass( UINT uiPass )
	{
		HRESULT hr = m_pCurTechnique->GetPassByIndex( uiPass )->Apply( uiPass );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D10Effect::BeginPass() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D10Effect::EndPass()
	{
		return true;
	}
}