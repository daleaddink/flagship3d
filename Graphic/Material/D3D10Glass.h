#ifndef _D3D10GLASS_H_
#define _D3D10GLASS_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class Camera;
	class D3D10Texture;
	class D3D10RenderCubeTexture;

	class _DLL_Export D3D10Glass : public D3D10Material
	{
	public:
		D3D10Glass();
		virtual ~D3D10Glass();

	public:
		// ��ʼ��
		virtual bool                Initialize();

		// ����������
		virtual void                BuildParamMap();

		// ���²��ʲ���
		virtual void                Update( Renderable * pParent );

		// ���²��ʲ���
		virtual void                Update( Resource * pParent );

		// �Ƿ����
		virtual bool                IsReady();

		// ��ȡ��Դ
		virtual bool                Load();

		// �ͷ���Դ
		virtual bool                Release();

	protected:
		// Fresnel��ͼ
		D3D10Texture *              m_pFresnelTexture;

		// ��Ⱦ��ͼ
		D3D10RenderCubeTexture *    m_pRenderCubeTexture;

		// ������ֵ
		Key                         m_kFresnelTexture;
		Key                         m_kRenderCubeTexture;

	private:

	};
}

#endif