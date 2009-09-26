#ifndef _D3D10METAL_H_
#define _D3D10METAL_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class Camera;
	class D3D10RenderCubeTexture;

	class _DLL_Export D3D10Metal : public D3D10Material
	{
	public:
		D3D10Metal();
		virtual ~D3D10Metal();

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
		D3D10RenderCubeTexture *    m_pRenderCubeTexture;

		// ������ֵ
		Key                         m_kRenderCubeTexture;

	private:

	};
}

#endif