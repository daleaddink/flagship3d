#include "Texture.h"
#include "Renderer.h"

namespace Flagship
{
	Texture::Texture()
	{
		m_iClassType = Base::Resource_Texture;
	}

	Texture::~Texture()
	{

	}

	bool    Texture::Cache()
	{
		if ( ! Resource::Cache() )
		{
			return false;
		}

		CreateFromMemory();
		return true;
	}

}