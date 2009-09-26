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

		// ��Ӻ������
		void                         AddPostProcess( Material * pPostProcess );

		// �������к������
		void                         ProcessAll();

	protected:
		// ��ȾĿ�����
		list< Material * >           m_pPostProcessList;

	private:
		// ˽�й��캯��
		PostProcessQueue();

		// ��ָ̬��
		static PostProcessQueue *    m_pPostProcessQueue;
	};
}

#endif