#ifndef _D3D10SHADOW_H_
#define _D3D10SHADOW_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class _DLL_Export D3D10MakeShadow : public Material
	{
	public:
		D3D10MakeShadow();
		virtual ~D3D10MakeShadow();

	public:
		// ��ʼ��
		virtual bool                Initialize();

		// ����������
		virtual void                BuildParamMap();

	protected:

	private:

	};
}

#endif