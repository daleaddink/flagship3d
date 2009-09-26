#ifndef _D3D10LINETEXTURE_H_
#define _D3D10LINETEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D10LineTexture : public Texture
	{
	public:
		D3D10LineTexture();
		virtual ~D3D10LineTexture();

		// 获取实例对象指针
		ID3D10Texture1D *             GetImpliment();

		// 获取资源对象
		ID3D10ShaderResourceView *    GetShaderResourceView();

	public:
		// 从资源创建效果
		virtual bool                  CreateFromMemory();

		// 创建贴图
		virtual bool                  CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

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
		ID3D10Texture1D *             m_pD3D10LineTexture;

		// Shader资源对象
		ID3D10ShaderResourceView *    m_pD3D10ShaderResource;

	private:

	};

	typedef ResHandle<D3D10LineTexture> D3D10LineTextureHandle;
}

#endif