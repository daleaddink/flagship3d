#ifndef _D3D9SATIN_H_
#define _D3D9SATIN_H_

#include <Common/Prerequisites.h>
#include "../Main/Material.h"

namespace Flagship
{
	class D3D9VolumeTexture;

	class _DLL_Export D3D9Satin : public Material
	{
	public:
		D3D9Satin();
		virtual ~D3D9Satin();

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
		// Noise贴图
		D3D9VolumeTexture *        m_pNoiseTexture;

		// 参数键值
		Key                        m_kNoiseTexture;

	private:

	};
}

#endif