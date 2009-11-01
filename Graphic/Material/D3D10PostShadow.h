#ifndef _D3D10POSTSHADOW_H_
#define _D3D10POSTSHADOW_H_

#include <Common/Prerequisites.h>
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class _DLL_Export D3D10PostShadow : public D3D10Material
	{
	public:
		D3D10PostShadow();
		virtual ~D3D10PostShadow();

	public:
		// 初始化
		virtual bool                Initialize();

		// 建立参数表
		virtual void                BuildParamMap();

		// 更新材质节点参数
		virtual void                Update( Renderable * pParent );

	protected:
		// 阴影贴图
		Key                         m_kShadowMap;

		// 光源矩阵
		Key                         m_kLightMatrix;

	private:

	};
}

#endif