#include "D3D9Mesh.h"

namespace Flagship
{
	D3D9Mesh::D3D9Mesh()
	{
		m_pD3D9VertexDecl = NULL;
		m_pD3D9VertexBuffer = NULL;
		m_pD3D9IndexBuffer = NULL;
	}

	D3D9Mesh::~D3D9Mesh()
	{
		SAFE_RELEASE( m_pD3D9VertexDecl );
		SAFE_RELEASE( m_pD3D9VertexBuffer );
		SAFE_RELEASE( m_pD3D9IndexBuffer );
	}
}