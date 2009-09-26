#include "RenderTargetQueue.h"

namespace Flagship
{
	RenderTargetQueue *    RenderTargetQueue::m_pRenderTargetQueue = NULL;

	RenderTargetQueue::RenderTargetQueue()
	{
		m_pRenderTargetList.clear();
	}

	RenderTargetQueue::~RenderTargetQueue()
	{

	}

	RenderTargetQueue *    RenderTargetQueue::GetSingleton()
	{
		if ( m_pRenderTargetQueue == NULL )
		{
			m_pRenderTargetQueue = new RenderTargetQueue;
		}

		return m_pRenderTargetQueue;
	}

	void    RenderTargetQueue::AddRenderTarget( RenderTarget * pTarget )
	{
		m_pRenderTargetList.push_back( pTarget );
	}

	void    RenderTargetQueue::UpdateAll()
	{
		list< RenderTarget * >::iterator it;
		for ( it = m_pRenderTargetList.begin(); it != m_pRenderTargetList.end(); it++ )
		{
			// 更新渲染对象
			(*it)->Update();
		}
	}
}