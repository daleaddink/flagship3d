#ifndef _D3D10SKYBOX_H_
#define _D3D10SKYBOX_H_

#include <Common/Prerequisites.h>
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class D3D10CubeTexture;

	class _DLL_Export D3D10SkyBox : public D3D10Material
	{
	public:
		D3D10SkyBox();
		virtual ~D3D10SkyBox();

	public:
		// ��ʼ��
		virtual bool          Initialize();

		// ����������
		virtual void          BuildParamMap();

		// ���²��ʲ���
		virtual void          Update( Renderable * pParent );

		// ���²��ʲ���
		virtual void          Update( Resource * pParent );

		// �Ƿ����
		virtual bool          IsReady();

		// ��ȡ��Դ
		virtual bool          Load();

		// �ͷ���Դ
		virtual bool          Release();

	protected:
		// ������ͼ
		D3D10CubeTexture *    m_pSkyTexture;

		// ������ֵ
		Key                   m_kViewMatrix;
		Key                   m_kSkyTexture;

	private:

	};
}

#endif