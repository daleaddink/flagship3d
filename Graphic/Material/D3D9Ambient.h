#ifndef _D3D9AMBIENT_H_
#define _D3D9AMBIENT_H_

#include <Common/Prerequisites.h>
#include "../Renderer/D3D9Material.h"

namespace Flagship
{
	class _DLL_Export D3D9Ambient : public D3D9Material
	{
	public:
		D3D9Ambient();
		virtual ~D3D9Ambient();

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