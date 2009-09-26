#include "D3D9Effect.h"
#include "D3D9RenderWindow.h"
#include "D3D9Texture.h"
#include "D3D9VolumeTexture.h"
#include "D3D9CubeTexture.h"
#include "D3D9RenderTexture.h"
#include "D3D9RenderCubeTexture.h"

namespace Flagship
{
	extern HINSTANCE g_hInstance;

	D3D9Effect::D3D9Effect()
	{
		m_iClassType = Base::Resource_Effect;
		m_pD3D9Effect = NULL;
	}

	D3D9Effect::~D3D9Effect()
	{
		SAFE_RELEASE( m_pD3D9Effect );
	}

	bool    D3D9Effect::Cache()
	{
		if ( ! Effect::Cache() )
		{
			return false;
		}

		CreateFromMemory();

		m_bReady = true;
		return true;
	}

	void    D3D9Effect::UnCache()
	{
		Effect::UnCache();
		m_mParamHandleMap.clear();
		m_mTechHandleMap.clear();

		SAFE_RELEASE( m_pD3D9Effect );
	}

	bool    D3D9Effect::CreateFromMemory()
	{
		// 获取D3D9设备指针
		LPD3DXBUFFER pErrer;
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		HRESULT hr = D3DXCreateEffect( pD3D9Device, m_kFileBuffer.GetPointer(), m_kFileBuffer.GetSize()
			, NULL, NULL, 0, NULL, &m_pD3D9Effect, &pErrer );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, GetKey().GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::CreateEffect() Fail! File:%s, %s", szFile, (char *)pErrer->GetBufferPointer() );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	void    D3D9Effect::AddParamHandle( BYTE byType, Key& szName )
	{
		D3DXHANDLE hHandle;

		char szTemp[256];
		wcstombs( szTemp, szName.GetName().c_str(), 256 );

		if ( byType == Param_Technique )
		{
			hHandle = m_pD3D9Effect->GetTechniqueByName( szTemp );
			m_mTechHandleMap[ szName ] = hHandle;
		}
		else
		{
			hHandle = m_pD3D9Effect->GetParameterByName( NULL, szTemp );
			m_mParamHandleMap[ szName ] = hHandle;
		}
	}

	bool    D3D9Effect::SetValue( Key& szParam, void * pValue, UINT uiSize )
	{
		HRESULT hr = m_pD3D9Effect->SetValue( m_mParamHandleMap[szParam], pValue, uiSize );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetValue() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::SetInt( Key& szParam, int * piValue )
	{
		HRESULT hr = m_pD3D9Effect->SetInt( m_mParamHandleMap[szParam], *piValue );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetInt() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::SetFloat( Key& szParam, float * pfValue )
	{
		HRESULT hr = m_pD3D9Effect->SetFloat( m_mParamHandleMap[szParam], *pfValue );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetFloat() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );
			
			return false;
		}

		return true;
	}

	bool    D3D9Effect::SetVector( Key& szParam, Vector4f * pvValue )
	{
		D3DXVECTOR4 vVector( (*pvValue)[0], (*pvValue)[1], (*pvValue)[2], (*pvValue)[3] );
		HRESULT hr = m_pD3D9Effect->SetVector( m_mParamHandleMap[szParam], &vVector );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetVector() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::SetVectorArray( Key& szParam, Vector4f * pvValue, UINT uiNum )
	{
		D3DXVECTOR4 * pvVector;
		pvVector = new D3DXVECTOR4[uiNum];

		for ( UINT i = 0; i < uiNum; i++ )
		{
			pvVector[i] = D3DXVECTOR4( (pvValue[i])[0], (pvValue[i])[1], (pvValue[i])[2], (pvValue[i])[3] );
		}

		HRESULT hr = m_pD3D9Effect->SetVectorArray( m_mParamHandleMap[szParam], pvVector, uiNum );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetVectorArray() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}
		
		SAFE_DELETE_ARRAY( pvVector );
		return true;
	}

	bool    D3D9Effect::SetMatrix( Key& szParam, Matrix4f * pmatValue )
	{
		Matrix4f matTemp = pmatValue->Transpose();

		D3DXMATRIX matMatrix( matTemp[0][0], matTemp[0][1], matTemp[0][2], matTemp[0][3],
			                  matTemp[1][0], matTemp[1][1], matTemp[1][2], matTemp[1][3],
							  matTemp[2][0], matTemp[2][1], matTemp[2][2], matTemp[2][3],
							  matTemp[3][0], matTemp[3][1], matTemp[3][2], matTemp[3][3] );
		HRESULT hr = m_pD3D9Effect->SetMatrix( m_mParamHandleMap[szParam], &matMatrix );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetMatrix() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::SetMatrixArray( Key& szParam, Matrix4f * pmatValue, UINT uiNum )
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

		HRESULT hr = m_pD3D9Effect->SetMatrixArray( m_mParamHandleMap[szParam], pmatMatrix, uiNum );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetMatrixArray() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}
		
		SAFE_DELETE_ARRAY( pmatMatrix );
		return true;
	}

	bool    D3D9Effect::SetTexture( Key& szParam, Texture * pTexture )
	{
		HRESULT hr;
		int iTextureType = pTexture->GetClassType();

		switch ( iTextureType )
		{
		case Base::Texture_General:
			{
				LPDIRECT3DTEXTURE9 pD3D9Texture = ( (D3D9Texture *) pTexture )->GetImpliment();
				hr = m_pD3D9Effect->SetTexture( m_mParamHandleMap[szParam], pD3D9Texture );

				break;
			}

		case Base::Texture_Volume:
			{
				LPDIRECT3DVOLUMETEXTURE9 pD3D9Texture = ( (D3D9VolumeTexture *) pTexture )->GetImpliment();
				hr = m_pD3D9Effect->SetTexture( m_mParamHandleMap[szParam], pD3D9Texture );

				break;
			}

		case Base::Texture_Cube:
			{
				LPDIRECT3DCUBETEXTURE9 pD3D9Texture = ( (D3D9CubeTexture *) pTexture )->GetImpliment();
				hr = m_pD3D9Effect->SetTexture( m_mParamHandleMap[szParam], pD3D9Texture );

				break;
			}
		}

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetTexture() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::SetTechnique( Key& szParam )
	{
		HRESULT hr = m_pD3D9Effect->SetTechnique( m_mTechHandleMap[szParam] );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szTemp[256];
			wcstombs( szTemp, szParam.GetName().c_str(), 256 );
			sprintf( szLog, "D3D9Effect::SetTechnique() Fail! Param:%s", szTemp );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::Begin( UINT * uiPassNum )
	{
		HRESULT hr = m_pD3D9Effect->Begin( uiPassNum, 0 );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Effect::Begin() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::End()
	{
		HRESULT hr = m_pD3D9Effect->End();

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Effect::End() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::BeginPass( UINT uiPass )
	{
		HRESULT hr = m_pD3D9Effect->BeginPass( uiPass );

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Effect::BeginPass() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}

	bool    D3D9Effect::EndPass()
	{
		HRESULT hr = m_pD3D9Effect->EndPass();

		if ( FAILED( hr ) )
		{
			char szLog[10240];
			sprintf( szLog, "D3D9Effect::EndPass() Fail!" );
			LogManager::GetSingleton()->WriteLog( szLog );

			return false;
		}

		return true;
	}
}