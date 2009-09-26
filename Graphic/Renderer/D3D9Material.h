#ifndef _D3D9MATERIAL_H_
#define _D3D9MATERIAL_H_

#include <Common/Prerequisites.h>
#include "../Main/Material.h"

namespace Flagship
{
	class Renderable;
	class Resource;

	class _DLL_Export D3D9Material : public Material
	{
	public:
		D3D9Material();
		~D3D9Material();

	public:
		// ���²��ʽڵ����
		virtual void           Update( Renderable * pParent );

		// ���²�����Դ����
		virtual void           Update( Resource * pParent );

		// ��ȡ��Դ
		virtual bool           Load();

		// �ͷ���Դ
		virtual bool           Release();

	private:

	};
}

#endif