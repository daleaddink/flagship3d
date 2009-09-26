#ifndef _D3D9METAL_H_
#define _D3D9METAL_H_

#include <Common/Prerequisites.h>
#include "../Main/Material.h"

namespace Flagship
{
	class Camera;
	class D3D9RenderCubeTexture;

	class _DLL_Export D3D9Metal : public Material
	{
	public:
		D3D9Metal();
		virtual ~D3D9Metal();

	public:
		// ��ʼ��
		virtual bool               Initialize();

		// ����������
		virtual void               BuildParamMap();

		// ���²��ʲ���
		virtual void               Update( Renderable * pParent );

		// ���²��ʲ���
		virtual void               Update( Resource * pParent );

	protected:
		// ��Ⱦ��ͼ
		D3D9RenderCubeTexture *    m_pRenderCubeTexture;

		// ������ֵ
		Key                        m_kRenderCubeTexture;

	private:

	};
}

#endif