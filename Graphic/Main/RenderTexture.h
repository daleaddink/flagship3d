#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_

#include <Common/Prerequisites.h>
#include "RenderTarget.h"

namespace Flagship
{
	class Texture;

	class _DLL_Export RenderTexture : public RenderTarget
	{
	public:
		RenderTexture();
		virtual ~RenderTexture();

		// ��ȡ��Ⱦ��ͼ
		Texture *       GetRenderTarget();

		// ��ȡ��Ȼ�����ͼ
		Texture *       GetDepthStencil();

	public:
		// ������Ⱦ����
		virtual void    Update();

	protected:
		// ��Ⱦ��ͼ
		Texture *       m_pRenderTarget;

		// �����ͼ
		Texture *       m_pDepthStencil;

	private:

	};
}

#endif