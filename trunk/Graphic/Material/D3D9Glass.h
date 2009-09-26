#ifndef _D3D9GLASS_H_
#define _D3D9GLASS_H_

#include "../Renderer/D3D9Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class Camera;
	class D3D9Texture;
	class D3D9RenderCubeTexture;

	class _DLL_Export D3D9Glass : public Material
	{
	public:
		D3D9Glass();
		virtual ~D3D9Glass();

	public:
		// ��ʼ��
		virtual bool               Initialize();

		// ����������
		virtual void               BuildParamMap();

		// ���²��ʲ���
		virtual void               Update( Renderable * pParent );

		// ���²��ʲ���
		virtual void               Update( Resource * pParent );

		// �Ƿ����
		virtual bool               IsReady();

		// ��ȡ��Դ
		virtual bool               Load();

		// �ͷ���Դ
		virtual bool               Release();

	protected:
		// Fresnel��ͼ
		D3D9Texture *              m_pFresnelTexture;

		// ��Ⱦ��ͼ
		D3D9RenderCubeTexture *    m_pRenderCubeTexture;

		// ������ֵ
		Key                        m_kFresnelTexture;
		Key                        m_kRenderCubeTexture;

	private:

	};
}

#endif