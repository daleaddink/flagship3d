#ifndef _ResourceManager_H_
#define _ResourceManager_H_

#include <Common/Prerequisites.h>
#include "Entity.h"

namespace Flagship
{
	class Resource;

	class _DLL_Export ResourceManager
	{
	public:
		virtual ~ResourceManager();

		// �����Դ������ָ��
		static ResourceManager *        GetSingleton();

		// ������Դ
		Resource *                      GetResource( Key& kKey );

	    // �����Դ
		void                            AddResource( Resource * pResource );

		// ɾ����Դ
		void                            DelResource( Resource * pResource );

		// ������Դ����
		void                            Update();

	protected:

	private:
		// ˽�й��캯��
		ResourceManager();

		// ��̬��Դ������ָ��
		static ResourceManager *        m_pResourceManager;

		// ��Դ�б�
		map< Key, Resource *>           m_pResourceMap;

		// ��ǰ������Դ�б�
		list<Resource *>                m_pCacheList;

		// ��ǰ����ָ��
		list< Resource * >::iterator    m_pCacheIterator;
	};
}

#endif