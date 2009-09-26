#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <Common/Prerequisites.h>
#include "Resource.h"

namespace Flagship
{
	class _DLL_Export Texture : public Resource
	{
	public:
		Texture();
		virtual ~Texture();

	public:
		// 从资源创建效果
		virtual bool    CreateFromMemory() { return true; }

		// 创建贴图
		virtual bool    CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat ) { return true; }

		// 创建渲染贴图
		virtual bool    CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat ) { return true; }

		// 创建深度贴图
		virtual bool    CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat ) { return true; }

		// 清空贴图
		virtual bool    ClearTexture() { return true; }

		// 锁定贴图
		virtual bool    Lock( int& rPitch, void ** ppData ) { return true; }

		// 解锁贴图
		virtual void    UnLock() {}

	protected:
		// 缓存资源
		virtual bool    Cache();
		
	private:

	};
}

#endif