#include "Mesh.h"
#include "BoundingVolume.h"

namespace Flagship
{
	Mesh::Mesh()
	{
		m_iClassType = Base::Resource_Mesh;

		m_dwFVF = 0;
		m_dwNumTrangle = 0;
		m_dwNumVertex = 0;
		m_dwVertexSize = 0;
	}

	Mesh::~Mesh()
	{

	}
}