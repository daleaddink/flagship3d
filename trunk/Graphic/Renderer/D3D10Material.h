#ifndef _D3D10MATERIAL_H_
#define _D3D10MATERIAL_H_

#include "D3D10Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class _DLL_Export D3D10Material : public Material
	{
	public:
		D3D10Material();
		~D3D10Material();

		// 获取当前科技
		ID3D10EffectTechnique *    GetTechnique();

		// 获取当前科技键值
		Key&                       GetTechKey();

	public:
		// 设置材质版本
		virtual bool           SetTechnique( Renderable * pParent, BYTE byTechType );

		// 更新材质节点参数
		virtual void               Update( Renderable * pParent );

		// 更新材质资源数据
		virtual void               Update( Resource * pParent );

		// 读取资源
		virtual bool               Load();

		// 释放资源
		virtual bool               Release();

	private:
		// 当前科技键值
		Key                        m_kTechKey;

	};
}

#endif