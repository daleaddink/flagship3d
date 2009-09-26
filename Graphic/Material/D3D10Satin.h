#ifndef _D3D10SATIN_H_
#define _D3D10SATIN_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class D3D10VolumeTexture;

	class _DLL_Export D3D10Satin : public D3D10Material
	{
	public:
		D3D10Satin();
		virtual ~D3D10Satin();

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
		// Noise��ͼ
		D3D10VolumeTexture *       m_pNoiseTexture;

		// ������ֵ
		Key                        m_kNoiseTexture;

	private:

	};
}

#endif