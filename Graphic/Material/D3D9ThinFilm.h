#ifndef _D3D9THINFLIM_H_
#define _D3D9THINFLIM_H_

#include "../Renderer/D3D9Prerequisites.h"
#include "../Renderer/D3D9Material.h"

namespace Flagship
{
	class D3D9Texture;

	class _DLL_Export D3D9ThinFlim : public D3D9Material
	{
	public:
		D3D9ThinFlim();
		virtual ~D3D9ThinFlim();

	public:
		// 初始化
		virtual bool               Initialize();

		// 建立参数表
		virtual void               BuildParamMap();

		// 更新材质参数
		virtual void               Update( Renderable * pParent );

		// 更新材质参数
		virtual void               Update( Resource * pParent );

		// 是否就绪
		virtual bool               IsReady();

		// 读取资源
		virtual bool               Load();

		// 释放资源
		virtual bool               Release();

	protected:
		// Fresnel贴图
		D3D9Texture *              m_pFriengeTexture;

		// 参数键值
		Key                        m_kFriengeTexture;

	private:

	};
}

#endif