#ifndef _D3D9METAL_H_
#define _D3D9METAL_H_

#include <Common/Prerequisites.h>
#include "../Main/Material.h"

namespace Flagship
{
	class Camera;
	class D3D9RenderCubeTexture;

	class _DLL_Export D3D9Metal : public Material
	{
	public:
		D3D9Metal();
		virtual ~D3D9Metal();

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
		// 渲染贴图
		D3D9RenderCubeTexture *    m_pRenderCubeTexture;

		// 参数键值
		Key                        m_kRenderCubeTexture;

	private:

	};
}

#endif