#ifndef _D3D9TEXTURE_H_
#define _D3D9TEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D9Texture : public Texture
	{
	public:
		D3D9Texture();
		virtual ~D3D9Texture();

		// 获取实例对象指针
		LPDIRECT3DTEXTURE9    GetImpliment();

	public:
		// 从资源创建效果
		virtual bool          CreateFromMemory();

		// 创建贴图
		virtual bool          CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 创建渲染贴图
		virtual bool          CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 创建深度贴图
		virtual bool          CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 清空贴图
		virtual bool          ClearTexture();

		// 锁定贴图
		virtual bool          Lock( int& rPitch, void ** ppData );

		// 解锁贴图
		virtual void          UnLock();

	protected:
		// 释放缓存
		virtual void          UnCache();

	protected:
		// D3D9贴图对象
		LPDIRECT3DTEXTURE9    m_pD3D9Texture;

	private:
	};

	typedef ResHandle<D3D9Texture> D3D9TextureHandle;
}

#endif