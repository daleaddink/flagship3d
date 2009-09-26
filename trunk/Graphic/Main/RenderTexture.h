#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_

#include <Common/Prerequisites.h>
#include "RenderTarget.h"

namespace Flagship
{
	class Texture;

	class _DLL_Export RenderTexture : public RenderTarget
	{
	public:
		RenderTexture();
		virtual ~RenderTexture();

		// 获取渲染贴图
		Texture *       GetRenderTarget();

		// 获取深度缓冲贴图
		Texture *       GetDepthStencil();

	public:
		// 更新渲染对象
		virtual void    Update();

	protected:
		// 渲染贴图
		Texture *       m_pRenderTarget;

		// 深度贴图
		Texture *       m_pDepthStencil;

	private:

	};
}

#endif