#ifndef _D3D9VOLUMETEXTURE_H_
#define _D3D9VOLUMETEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D9VolumeTexture : public Texture
	{
	public:
		D3D9VolumeTexture();
		virtual ~D3D9VolumeTexture();

		// 获取实例对象指针
		LPDIRECT3DVOLUMETEXTURE9    GetImpliment();

	public:
		// 从资源创建效果
		virtual bool                CreateFromMemory();

		// 清空贴图
		virtual bool                ClearTexture();

	protected:
		// 释放缓存
		virtual void                UnCache();

	protected:
		// D3D9贴图对象
		LPDIRECT3DVOLUMETEXTURE9    m_pD3D9VolumeTexture;

	private:
	};

	typedef ResHandle<D3D9VolumeTexture> D3D9VolumeTextureHandle;
}

#endif