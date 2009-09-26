#ifndef _POSTPROCESSQUEUE_H_
#define _POSTPROCESSQUEUE_H_

#include <Common/Prerequisites.h>

namespace Flagship
{
	class Material;

	class _DLL_Export PostProcessQueue
	{
	public:
		virtual ~PostProcessQueue();

		static PostProcessQueue *    GetSingleton();

		// 添加后处理材质
		void                         AddPostProcess( Material * pPostProcess );

		// 更新所有后处理材质
		void                         ProcessAll();

	protected:
		// 渲染目标队列
		list< Material * >           m_pPostProcessList;

	private:
		// 私有构造函数
		PostProcessQueue();

		// 静态指针
		static PostProcessQueue *    m_pPostProcessQueue;
	};
}

#endif