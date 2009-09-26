#ifndef _D3D9SKYBOX_H_
#define _D3D9SKYBOX_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class D3D9CubeTexture;

	class _DLL_Export D3D9SkyBox : public Material
	{
	public:
		D3D9SkyBox();
		virtual ~D3D9SkyBox();

	public:
		// 初始化
		virtual bool         Initialize();

		// 建立参数表
		virtual void         BuildParamMap();

		// 更新材质参数
		virtual void         Update( Renderable * pParent );

		// 更新材质参数
		virtual void         Update( Resource * pParent );

	protected:
		// 参数键值
		Key                  m_kViewMatrix;

	private:

	};
}

#endif