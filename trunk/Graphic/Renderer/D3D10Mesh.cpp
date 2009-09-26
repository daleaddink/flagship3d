#include "D3D10Mesh.h"
#include "D3D10RenderWindow.h"

namespace Flagship
{
	D3D10Mesh::D3D10Mesh()
	{
		m_pD3D10VertexDecl.clear();
		m_pD3D10VertexBuffer = NULL;
		m_pD3D10IndexBuffer = NULL;
	}

	D3D10Mesh::~D3D10Mesh()
	{
		map< Key, ID3D10InputLayout * >::iterator it = m_pD3D10VertexDecl.begin();
		while ( it != m_pD3D10VertexDecl.end() )
		{
			SAFE_RELEASE( (*it).second );
			it++;
		}

		SAFE_RELEASE( m_pD3D10VertexBuffer );
		SAFE_RELEASE( m_pD3D10IndexBuffer );
	}

	ID3D10InputLayout *    D3D10Mesh::GetVertexDeclaration( Key& szTech, ID3D10EffectTechnique * pTech )
	{
		map< Key, ID3D10InputLayout * >::iterator it = m_pD3D10VertexDecl.find( szTech );
		if ( it != m_pD3D10VertexDecl.end() )
		{
			return (*it).second;
		}

		// 获取D3D10设备指针
		ID3D10Device * pD3D10Device = ( ( D3D10RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 获取描述
		D3D10_PASS_DESC PassDesc;
		pTech->GetPassByIndex( 0 )->GetDesc( &PassDesc );

		// 创建InputLayout
		ID3D10InputLayout * pInputLayout;
		HRESULT hr = pD3D10Device->CreateInputLayout( m_pInputElement, m_iElementNum, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout );
		if ( FAILED( hr ) )
		{
			char szLog[10240];
			char szFile[256];
			wcstombs( szFile, szTech.GetName().c_str(), 256 );
			sprintf( szLog, "D3D10Mesh::GetVertexDeclaration Fail! Tech:%s", szFile );
			LogManager::GetSingleton()->WriteLog( szLog );

			return NULL;
		}

		m_pD3D10VertexDecl[szTech] = pInputLayout;
		return pInputLayout;
	}
}