#ifndef _D3D9POSTSHADOW_H_
#define _D3D9POSTSHADOW_H_

#include <Common/Prerequisites.h>
#include "../Renderer/D3D9Material.h"

namespace Flagship
{
	class _DLL_Export D3D9PostShadow : public D3D9Material
	{
	public:
		D3D9PostShadow();
		virtual ~D3D9PostShadow();

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