#ifndef _D3D10VOLUMETEXTURE_H_
#define _D3D10VOLUMETEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D10VolumeTexture : public Texture
	{
	public:
		D3D10VolumeTexture();
		virtual ~D3D10VolumeTexture();

		// 获取实例对象指针
		ID3D10Texture3D *             GetImpliment();

		// 获取资源对象
		ID3D10ShaderResourceView *    GetShaderResourceView();

	public:
		// 从资源创建效果
		virtual bool                  CreateFromMemory();

		// 清空贴图
		virtual bool                  ClearTexture();

	protected:
		// 释放缓存
		virtual void                  UnCache();

	protected:
		// D3D10贴图对象
		ID3D10Texture3D *             m_pD3D10VolumeTexture;

		// Shader资源对象
		ID3D10ShaderResourceView *    m_pD3D10ShaderResource;

	private:

	};

	typedef ResHandle<D3D10VolumeTexture> D3D10VolumeTextureHandle;
}

#endif