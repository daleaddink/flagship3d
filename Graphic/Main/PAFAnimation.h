#ifndef _PAFANIMATION_H_
#define _PAFANIMATION_H_

#include <Common/Prerequisites.h>
#include "Animation.h"

namespace Flagship
{
	class _DLL_Export PAFAnimation : public Animation
	{
	public:
		PAFAnimation();
		virtual ~PAFAnimation();

	protected:
		// 读取资源
		virtual bool                Create();

	protected:
		// 版本信息
		const int                   PAF_MAGIC;
		const int                   PAF_VERSION;
		
	private:

	};
}

#endif
