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

		// �����ȾĿ��
		void                          AddRenderTarget( RenderTarget * pTarget );

		// ����������ȾĿ��
		void                          UpdateAll();

	protected:
		// ��ȾĿ�����
		list< RenderTarget * >        m_pRenderTargetList;

	private:
		// ˽�й��캯��
		RenderTargetQueue();

		// ��ָ̬��
		static RenderTargetQueue *    m_pRenderTargetQueue;
	};
}

#endif