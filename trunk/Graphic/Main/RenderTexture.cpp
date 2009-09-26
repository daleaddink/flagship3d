#include "RenderTexture.h"
#include "Renderer.h"
#include "Texture.h"

namespace Flagship
{
	RenderTexture::RenderTexture()
	{
		m_iClassType = Base::RenderTarget_Texture;
		m_pCamera = new Camera( L"RenderCamera" );
	}

	RenderTexture::~RenderTexture()
	{
		SAFE_DELETE( m_pCamera );
	}

	Texture *    RenderTexture::GetRenderTarget()
	{
		return m_pRenderTarget;
	}

	Texture *    RenderTexture::GetDepthStencil()
	{
		return m_pDepthStencil;
	}

	void    RenderTexture::Update()
	{
		RenderTarget::Update();
	}
}