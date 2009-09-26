#ifndef _RENDERTARGETQUEUE_H_
#define _RENDERTARGETQUEUE_H_

#include <Common/Prerequisites.h>
#include "RenderTarget.h"

namespace Flagship
{
	class _DLL_Export RenderTargetQueue
	{
	public:
		virtual ~RenderTargetQueue();

		static RenderTargetQueue *    GetSingleton();

		// 添加渲染目标
		void                          AddRenderTarget( RenderTarget * pTarget );

		// 更新所有渲染目标
		void                          UpdateAll();

	protected:
		// 渲染目标队列
		list< RenderTarget * >        m_pRenderTargetList;

	private:
		// 私有构造函数
		RenderTargetQueue();

		// 静态指针
		static RenderTargetQueue *    m_pRenderTargetQueue;
	};
}

#endif