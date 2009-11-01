#ifndef _D3D10POSTSHADOW_H_
#define _D3D10POSTSHADOW_H_

#include <Common/Prerequisites.h>
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class _DLL_Export D3D10PostShadow : public D3D10Material
	{
	public:
		D3D10PostShadow();
		virtual ~D3D10PostShadow();

	public:
		// ��ʼ��
		virtual bool                Initialize();

		// ����������
		virtual void                BuildParamMap();

		// ���²��ʽڵ����
		virtual void                Update( Renderable * pParent );

	protected:
		// ��Ӱ��ͼ
		Key                         m_kShadowMap;

		// ��Դ����
		Key                         m_kLightMatrix;

	private:

	};
}

#endif