#ifndef _D3D9CUBETEXTURE_H_
#define _D3D9CUBETEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D9CubeTexture : public Texture
	{
	public:
		D3D9CubeTexture();
		virtual ~D3D9CubeTexture();

		// 获取实例对象指针
		LPDIRECT3DCUBETEXTURE9    GetImpliment();

	public:
		// 从资源创建效果
		virtual bool              CreateFromMemory();

		// 创建贴图
		virtual bool              CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 清空贴图
		virtual bool              ClearTexture();

	protected:
		// 释放缓存
		virtual void              UnCache();

	protected:
		// D3D9贴图对象
		LPDIRECT3DCUBETEXTURE9    m_pD3D9CubeTexture;

	private:

	};

	typedef ResHandle<D3D9CubeTexture> D3D9CubeTextureHandle;
}

#endif