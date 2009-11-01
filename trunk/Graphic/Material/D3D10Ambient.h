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
		// 初始化
		virtual bool    Initialize();

		// 建立参数表
		virtual void    BuildParamMap();

	protected:

	private:
	};
}

#endif