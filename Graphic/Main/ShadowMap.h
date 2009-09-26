#ifndef _SHADOWMAP_H_
#define _SHADOWMAP_H_

#include <Common/Prerequisites.h>
#include "RenderTexture.h"

namespace Flagship
{
	class Light;
	class Material;

	class _DLL_Export ShadowMap : public RenderTexture
	{
	public:
		ShadowMap();
		virtual ~ShadowMap();

		enum
		{
			Shadow_PositiveX,
			Shadow_NegativeX,
			Shadow_PositiveY,
			Shadow_NegativeY,
			Shadow_PositiveZ,
			Shadow_NegativeZ,

			Shadow_Max,
		};

		// 获取光源矩阵
		Matrix4f *      GetLightMatrix();

	public:
		// 初始化渲染贴图
		virtual bool    Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// 更新渲染对象
		virtual void    UpdateShadow( Light * pLight, BYTE byShadowFace = Shadow_Max );

	protected:
		// 光源矩阵
		Matrix4f        m_matLight;

	private:

	};
}

#endif