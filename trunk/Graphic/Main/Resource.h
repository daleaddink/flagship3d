#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <Common/Prerequisites.h>
#include "Base.h"
#include "ResourceManager.h"
#include "Buffer.h"

namespace Flagship
{
	class _DLL_Export Resource : public Base
	{
	public:
		Resource();
		virtual ~Resource();

		// 是否过期
		bool            IsOutOfDate();

		// 更新时间
		void            UpdateFrame();

		// 设置资源路径
		void            SetPath( wstring szPath );

		// 读取资源
		bool            Load();

		// 释放资源
		bool            Release();

		// 是否就绪
		bool            IsReady();

	protected:
		// 缓存资源
		virtual bool    Create() { return true; }

		// 释放缓存
		virtual void    Destory() {}

		// 缓存资源
		virtual bool    Cache() { return true; }

		// 释放缓存
		virtual void    UnCache() {}

	protected:
		// 完成标记 
		bool            m_bReady;

		// 最后渲染帧
		DWORD           m_dwLastFrame;

		// 引用次数
		int             m_iRefCount;

		// 资源路径
		wstring         m_szPathName;

		// 文件缓存
		Buffer          m_kFileBuffer;

	private:

	};

	template <class T>
	class _DLL_Export ResHandle
	{
	public:
		ResHandle() { m_pResource = NULL; }
		virtual ~ResHandle() {}

		// 设置资源路径
		void            SetPath( wstring szPath )
		{
			Resource * pResource = ResourceManager::GetSingleton()->GetResource( Key( szPath ) );
			if ( pResource != NULL )
			{
				m_pResource = (T *) pResource;
			}
			else
			{
				m_pResource = new T;
				m_pResource->SetPath( szPath );
				ResourceManager::GetSingleton()->AddResource( m_pResource );
			}
		}

		// 模板实体类指针
		T *             GetImpliment() { return (T *) m_pResource; }

		T *             operator-> () { return (T *) m_pResource; }

	protected:
		// 模板实体类指针
		Resource *      m_pResource;

	private:
	};
}

#endif