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
		// ��ȡ��Դ
		virtual bool      Create();

		// �ͷ���Դ
		virtual void      Destory();

	protected:
		// �汾��Ϣ
		const int         PSF_MAGIC;
		const int         PSF_VERSION;
		
	private:

	};
}

#endif
