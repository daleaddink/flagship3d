#include "PostProcessQueue.h"
#include "Material.h"

namespace Flagship
{
	PostProcessQueue *    PostProcessQueue::m_pPostProcessQueue = NULL;

	PostProcessQueue::PostProcessQueue()
	{
		m_pPostProcessList.clear();
	}

	PostProcessQueue::~PostProcessQueue()
	{

	}

	PostProcessQueue *    PostProcessQueue::GetSingleton()
	{
		if ( m_pPostProcessQueue == NULL )
		{
			m_pPostProcessQueue = new PostProcessQueue;
		}

		return m_pPostProcessQueue;
	}

	void    PostProcessQueue::AddPostProcess( Material * pPostProcess )
	{
		m_pPostProcessList.push_back( pPostProcess );
	}

	void    PostProcessQueue::ProcessAll()
	{
		list< Material * >::iterator it;
		for ( it = m_pPostProcessList.begin(); it != m_pPostProcessList.end(); it++ )
		{
			// ºó´¦Àí
			(*it)->PostProcess();
		}
	}
}