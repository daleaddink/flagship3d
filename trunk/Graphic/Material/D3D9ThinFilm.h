#ifndef _D3D9THINFLIM_H_
#define _D3D9THINFLIM_H_

#include "../Renderer/D3D9Prerequisites.h"
#include "../Renderer/D3D9Material.h"

namespace Flagship
{
	class D3D9Texture;

	class _DLL_Export D3D9ThinFlim : public D3D9Material
	{
	public:
		D3D9ThinFlim();
		virtual ~D3D9ThinFlim();

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
		D3D9Texture *              m_pFriengeTexture;

		// ������ֵ
		Key                        m_kFriengeTexture;

	private:

	};
}

#endif