#ifndef _D3D9SKYBOX_H_
#define _D3D9SKYBOX_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class D3D9CubeTexture;

	class _DLL_Export D3D9SkyBox : public Material
	{
	public:
		D3D9SkyBox();
		virtual ~D3D9SkyBox();

	public:
		// ��ʼ��
		virtual bool         Initialize();

		// ����������
		virtual void         BuildParamMap();

		// ���²��ʲ���
		virtual void         Update( Renderable * pParent );

		// ���²��ʲ���
		virtual void         Update( Resource * pParent );

	protected:
		// ������ֵ
		Key                  m_kViewMatrix;

	private:

	};
}

#endif