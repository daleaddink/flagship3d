#ifndef _D3D10GLASS_H_
#define _D3D10GLASS_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class Camera;
	class D3D10Texture;
	class D3D10RenderCubeTexture;

	class _DLL_Export D3D10Glass : public D3D10Material
	{
	public:
		D3D10Glass();
		virtual ~D3D10Glass();

	public:
		// 初始化
		virtual bool                Initialize();

		// 建立参数表
		virtual void                BuildParamMap();

		// 更新材质参数
		virtual void                Update( Renderable * pParent );

		// 更新材质参数
		virtual void                Update( Resource * pParent );

		// 是否就绪
		virtual bool                IsReady();

		// 读取资源
		virtual bool                Load();

		// 释放资源
		virtual bool                Release();

	protected:
		// Fresnel贴图
		D3D10Texture *              m_pFresnelTexture;

		// 渲染贴图
		D3D10RenderCubeTexture *    m_pRenderCubeTexture;

		// 参数键值
		Key                         m_kFresnelTexture;
		Key                         m_kRenderCubeTexture;

	private:

	};
}

#endif