#ifndef _RESOURCELOADER_H_
#define _RESOURCELOADER_H_

#include <Common/Prerequisites.h>
#include <Common/Thread.h>
#include "Resource.h"

namespace Flagship
{
	class Resource;
	class _DLL_Export ResourceLoader : public Thread
	{
	public:
		virtual ~ResourceLoader();

		// �����Դ������ָ��
		static ResourceLoader *    GetSingleton();

		// �����Ҫ���ص���Դ
		void                       PushResource( Resource * pResource );

	public:
        // ��ʼ������Դ
		virtual int                Run();

	protected:

	private:
		// ˽�й��캯��
		ResourceLoader();

		// ��̬��Դ������ָ��
		static ResourceLoader *    m_pResourceLoader;

		// ���ȡ����Դ�б�
	    map< Key, Resource * >     m_pResourceList;

	};
}

#endif