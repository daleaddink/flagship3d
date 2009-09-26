#ifndef _D3D10SKYBOX_H_
#define _D3D10SKYBOX_H_

#include <Common/Prerequisites.h>
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class D3D10CubeTexture;

	class _DLL_Export D3D10SkyBox : public D3D10Material
	{
	public:
		D3D10SkyBox();
		virtual ~D3D10SkyBox();

	public:
		// 初始化
		virtual bool          Initialize();

		// 建立参数表
		virtual void          BuildParamMap();

		// 更新材质参数
		virtual void          Update( Renderable * pParent );

		// 更新材质参数
		virtual void          Update( Resource * pParent );

		// 是否就绪
		virtual bool          IsReady();

		// 读取资源
		virtual bool          Load();

		// 释放资源
		virtual bool          Release();

	protected:
		// 环境贴图
		D3D10CubeTexture *    m_pSkyTexture;

		// 参数键值
		Key                   m_kViewMatrix;
		Key                   m_kSkyTexture;

	private:

	};
}

#endif