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

		// 获得资源加载器指针
		static ResourceLoader *    GetSingleton();

		// 添加需要加载的资源
		void                       PushResource( Resource * pResource );

	public:
        // 开始加载资源
		virtual int                Run();

	protected:

	private:
		// 私有构造函数
		ResourceLoader();

		// 静态资源加载器指针
		static ResourceLoader *    m_pResourceLoader;

		// 需读取的资源列表
	    map< Key, Resource * >     m_pResourceList;

	};
}

#endif