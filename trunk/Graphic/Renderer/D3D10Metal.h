#ifndef _D3D10METAL_H_
#define _D3D10METAL_H_

#include <Common/Prerequisites.h>
#include "../Main/Material.h"

namespace Flagship
{
	class Camera;
	class D3D10Renderer;
	class D3D10RenderCubeTexture;

	class _DLL_Export D3D10Metal : public Material
	{
	public:
		D3D10Metal();
		virtual ~D3D10Metal();

	public:
		// 初始化
		virtual bool               Initialize();

		// 建立参数表
		virtual void               BuildParamMap();

		// 更新材质参数
		virtual void               Update( Renderable * pParent );

		// 更新材质参数
		virtual void               Update( Resource * pParent );

	protected:
		// 渲染贴图摄像机
		Camera *                   m_pRenderCamera;

		// 贴图渲染器
		D3D10Renderer *             m_pCubeTextureRenderer;

		// 渲染贴图
		D3D10RenderCubeTexture *    m_pRenderCubeTexture;

		// 参数键值
		Key                        m_kRenderCubeTexture;

	private:

	};
}

#endif