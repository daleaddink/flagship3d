#ifndef _D3D10AMBIENT_H_
#define _D3D10AMBIENT_H_

#include <Common/Prerequisites.h>
#include "../Renderer/D3D10Material.h"

namespace Flagship
{
	class _DLL_Export D3D10Ambient : public D3D10Material
	{
	public:
		D3D10Ambient();
		virtual ~D3D10Ambient();

	public:
		// ��ʼ��
		virtual bool    Initialize();

		// ����������
		virtual void    BuildParamMap();

	protected:

	private:
	};
}

#endif