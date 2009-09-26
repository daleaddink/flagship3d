#ifndef _D3D10THINFLIM_H_
#define _D3D10THINFLIM_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class D3D10Texture;

	class _DLL_Export D3D10ThinFilm : public D3D10Material
	{
	public:
		D3D10ThinFilm();
		virtual ~D3D10ThinFilm();

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
		D3D10Texture *             m_pFriengeTexture;

		// ������ֵ
		Key                        m_kFriengeTexture;

	private:

	};
}

#endif