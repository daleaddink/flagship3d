#ifndef _GLOBALPARAM_H_
#define _GLOBALPARAM_H_

#include <Common/Prerequisites.h>
#include "DataDefine.h"

namespace Flagship
{
	class Material;
	class Light;
	class ShadowMap;
	class RenderPass;

	class _DLL_Export GlobalParam
	{
	public:
		virtual ~GlobalParam();

		// 获得全局参数指针
		static GlobalParam *   GetSingleton();

	public:
		// 当前全局材质
		Material *              m_pGlobalMaterial;

		// 阴影材质
		Material *              m_pMakeShadow;

		// 阴影材质
		Material *              m_pPostShadow;

		// 环境光材质
		Material *              m_pAmbient;

		// 当前光源
		Light *                 m_pCurLight;

		// 当前阴影图
		ShadowMap *             m_pCurShadowMap;

		// 渲染过程
		RenderPass *            m_pRenderPass;

	public:
		// 全局环境光
		VectorData              m_vAmbientColor;

	protected:
		// 私有构造
		GlobalParam();

		// 静态场景管理器指针
		static GlobalParam *    m_pGlobalParam;

	private:

	};
}

#endif
