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
		// ��ȡ��Դ
		virtual bool                Create();

	protected:
		// �汾��Ϣ
		const int                   PAF_MAGIC;
		const int                   PAF_VERSION;
		
	private:

	};
}

#endif
