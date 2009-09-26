#ifndef _D3D9RENDERTEXTURE_H_
#define _D3D9RENDERTEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/RenderTexture.h"

namespace Flagship
{
	class _DLL_Export D3D9RenderTexture : public RenderTexture
	{
	public:
		D3D9RenderTexture();
		virtual ~D3D9RenderTexture();

		// 初始化渲染贴图
		bool                  Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

	public:
		// 更新渲染对象
		virtual void          Update();

		// 存储渲染贴图
		virtual void          SaveRenderTexture( wstring szPath );

	protected:
		// 渲染表面
		LPDIRECT3DSURFACE9    m_pRenderSurface;

		// 深度模版表面
		LPDIRECT3DSURFACE9    m_pDepthSurface;

	private:
	};
}

#endif