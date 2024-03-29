#ifndef _D3D10CUBETEXTURE_H_
#define _D3D10CUBETEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D10CubeTexture : public Texture
	{
	public:
		D3D10CubeTexture();
		virtual ~D3D10CubeTexture();

		// 获取实例对象指针
		ID3D10Texture2D *             GetImpliment();

		// 获取资源对象
		ID3D10ShaderResourceView *    GetShaderResourceView();

	public:
		// 从资源创建效果
		virtual bool                  CreateFromMemory();

		// 创建贴图
		virtual bool                  CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 创建贴图
		virtual bool                  CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 清空贴图
		virtual bool                  ClearTexture();

	protected:
		// 释放缓存
		virtual void                  UnCache();

	protected:
		// D3D10贴图对象
		ID3D10Texture2D *             m_pD3D10CubeTexture;

		// Shader资源对象
		ID3D10ShaderResourceView *    m_pD3D10ShaderResource;

	private:

	};

	typedef ResHandle<D3D10CubeTexture> D3D10CubeTextureHandle;
}

#endif