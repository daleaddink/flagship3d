#ifndef _D3D9MATERIAL_H_
#define _D3D9MATERIAL_H_

#include <Common/Prerequisites.h>
#include "../Main/Material.h"

namespace Flagship
{
	class Renderable;
	class Resource;

	class _DLL_Export D3D9Material : public Material
	{
	public:
		D3D9Material();
		~D3D9Material();

	public:
		// 更新材质节点参数
		virtual void           Update( Renderable * pParent );

		// 更新材质资源数据
		virtual void           Update( Resource * pParent );

		// 读取资源
		virtual bool           Load();

		// 释放资源
		virtual bool           Release();

	private:

	};
}

#endif