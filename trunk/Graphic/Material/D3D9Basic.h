#ifndef _D3D9BASIC_H_
#define _D3D9BASIC_H_

#include <Common/Prerequisites.h>
#include "../Renderer/D3D9Material.h"

namespace Flagship
{
	class _DLL_Export D3D9Basic : public D3D9Material
	{
	public:
		D3D9Basic();
		virtual ~D3D9Basic();
		
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