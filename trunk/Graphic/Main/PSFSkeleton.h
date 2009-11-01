#ifndef _PSFSKELETON_H_
#define _PSFSKELETON_H_

#include <Common/Prerequisites.h>
#include "Skeleton.h"

namespace Flagship
{
	class _DLL_Export PSFSkeleton : public Skeleton
	{
	public:
		PSFSkeleton();
		virtual ~PSFSkeleton();

	protected:
		// 读取资源
		virtual bool      Create();

		// 释放资源
		virtual void      Destory();

	protected:
		// 版本信息
		const int         PSF_MAGIC;
		const int         PSF_VERSION;
		
	private:

	};
}

#endif
