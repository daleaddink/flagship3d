#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <Common/Prerequisites.h>

namespace Flagship
{
	class _DLL_Export Buffer
	{
	public:
		Buffer();
		virtual ~Buffer();

	public:
		// 创建缓存
		bool      Create( DWORD dwSize );

		// 拷贝数据
		bool      Copy( char * pBuffer, DWORD dwSize );

		// 设置缓存指针
		void      SetPointer( void * pData );

		// 获取缓存指针
		void *    GetPointer();

		// 设置缓存大小
		void      SetSize( DWORD dwSize );

		// 获取缓存大小
		DWORD     GetSize();


		// 释放缓存
		void      Release();

	protected:
		// 缓存
		char *    m_pBuffer;

		// 换成大小
		DWORD     m_dwSize;

	private:
	};
}

#endif