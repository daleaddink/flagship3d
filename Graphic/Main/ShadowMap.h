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

		// ��ȡ��Դ����
		Matrix4f *      GetLightMatrix();

	public:
		// ��ʼ����Ⱦ��ͼ
		virtual bool    Initialize( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// ������Ⱦ����
		virtual void    UpdateShadow( Light * pLight, BYTE byShadowFace = Shadow_Max );

	protected:
		// ��Դ����
		Matrix4f        m_matLight;

	private:

	};
}

#endif