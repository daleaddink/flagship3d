#ifndef _D3D10TEXTURE_H_
#define _D3D10TEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D10Texture : public Texture
	{
	public:
		D3D10Texture();
		virtual ~D3D10Texture();

		// 获取实例对象指针
		ID3D10Texture2D *             GetImpliment();

		// 获取资源对象
		ID3D10ShaderResourceView *    GetShaderResourceView();

		// 获取渲染对象
		ID3D10RenderTargetView *      GetRenderTargetView();

		// 获取深度对象
		ID3D10DepthStencilView *      GetDepthStencilView();

	public:
		// 从资源创建效果
		virtual bool                  CreateFromMemory();

		// 创建贴图
		virtual bool                  CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 创建渲染贴图
		virtual bool                  CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 创建深度贴图
		virtual bool                  CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 清空贴图
		virtual bool                  ClearTexture();

		// 锁定贴图
		virtual bool                  Lock( int& rPitch, void ** ppData );

		// 解锁贴图
		virtual void                  UnLock();

	protected:
		// 释放缓存
		virtual void                  UnCache();

	protected:
		// D3D10贴图对象
		ID3D10Texture2D *             m_pD3D10Texture;

		// Shader资源对象
		ID3D10ShaderResourceView *    m_pD3D10ShaderResource;

		// 渲染对象
		ID3D10RenderTargetView *      m_pD3D10RenderTarget;

		// 深度对象
		ID3D10DepthStencilView *      m_pD3D10DepthStencil;

	private:

	};

	typedef ResHandle<D3D10Texture> D3D10TextureHandle;
}

#endif