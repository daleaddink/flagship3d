#ifndef _D3D9SATIN_H_
#define _D3D9SATIN_H_

#include <Common/Prerequisites.h>
#include "../Main/Material.h"

namespace Flagship
{
	class D3D9VolumeTexture;

	class _DLL_Export D3D9Satin : public Material
	{
	public:
		D3D9Satin();
		virtual ~D3D9Satin();

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
		D3D9VolumeTexture *        m_pNoiseTexture;

		// ������ֵ
		Key                        m_kNoiseTexture;

	private:

	};
}

#endif