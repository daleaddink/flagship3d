#ifndef _D3D10SHADOWMAP_H_
#define _D3D10SHADOWMAP_H_

#include "D3D10Prerequisites.h"
#include "../Main/ShadowMap.h"

namespace Flagship
{
	class _DLL_Export D3D10ShadowMap : public ShadowMap
	{
	public:
		D3D10ShadowMap();
		virtual ~D3D10ShadowMap();

	public:
		// 初始化渲染贴图
		virtual bool                Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 更新阴影图
		virtual void                UpdateShadow( Light * pLight, BYTE byShadowFace = Shadow_Max );

		// 存储渲染贴图
		virtual void                SaveRenderTexture( wstring szPath );

	protected:
		// 渲染表面
		ID3D10RenderTargetView *    m_pRenderSurface;

		// 深度模版表面
		ID3D10DepthStencilView *    m_pDepthSurface;

	private:
	};
}

#endif