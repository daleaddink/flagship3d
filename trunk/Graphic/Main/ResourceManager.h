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

		// 获得资源加载器指针
		static ResourceManager *        GetSingleton();

		// 查找资源
		Resource *                      GetResource( Key& kKey );

	    // 添加资源
		void                            AddResource( Resource * pResource );

		// 删除资源
		void                            DelResource( Resource * pResource );

		// 更新资源管理
		void                            Update();

	protected:

	private:
		// 私有构造函数
		ResourceManager();

		// 静态资源管理器指针
		static ResourceManager *        m_pResourceManager;

		// 资源列表
		map< Key, Resource *>           m_pResourceMap;

		// 当前缓存资源列表
		list<Resource *>                m_pCacheList;

		// 当前缓存指针
		list< Resource * >::iterator    m_pCacheIterator;
	};
}

#endif