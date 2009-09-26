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
		// ��������
		bool      Create( DWORD dwSize );

		// ��������
		bool      Copy( char * pBuffer, DWORD dwSize );

		// ���û���ָ��
		void      SetPointer( void * pData );

		// ��ȡ����ָ��
		void *    GetPointer();

		// ���û����С
		void      SetSize( DWORD dwSize );

		// ��ȡ�����С
		DWORD     GetSize();


		// �ͷŻ���
		void      Release();

	protected:
		// ����
		char *    m_pBuffer;

		// ���ɴ�С
		DWORD     m_dwSize;

	private:
	};
}

#endif