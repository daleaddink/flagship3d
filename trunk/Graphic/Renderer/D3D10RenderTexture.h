#ifndef _D3D10RENDERTEXTURE_H_
#define _D3D10RENDERTEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/RenderTexture.h"

namespace Flagship
{
	class _DLL_Export D3D10RenderTexture : public RenderTexture
	{
	public:
		D3D10RenderTexture();
		virtual ~D3D10RenderTexture();

		// 初始化渲染贴图
		bool                        Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

	public:
		// 存储渲染贴图
		virtual void                SaveRenderTexture( wstring szPath );

		// 更新渲染对象
		virtual void                Update();

	protected:
		// 渲染表面
		ID3D10RenderTargetView *    m_pRenderSurface;

		// 深度模版表面
		ID3D10DepthStencilView *    m_pDepthSurface;

	private:
	};
}

#endif