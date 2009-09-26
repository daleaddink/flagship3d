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

		// �Ƿ����
		bool            IsOutOfDate();

		// ����ʱ��
		void            UpdateFrame();

		// ������Դ·��
		void            SetPath( wstring szPath );

		// ��ȡ��Դ
		bool            Load();

		// �ͷ���Դ
		bool            Release();

		// �Ƿ����
		bool            IsReady();

	protected:
		// ������Դ
		virtual bool    Create() { return true; }

		// �ͷŻ���
		virtual void    Destory() {}

		// ������Դ
		virtual bool    Cache() { return true; }

		// �ͷŻ���
		virtual void    UnCache() {}

	protected:
		// ��ɱ�� 
		bool            m_bReady;

		// �����Ⱦ֡
		DWORD           m_dwLastFrame;

		// ���ô���
		int             m_iRefCount;

		// ��Դ·��
		wstring         m_szPathName;

		// �ļ�����
		Buffer          m_kFileBuffer;

	private:

	};

	template <class T>
	class _DLL_Export ResHandle
	{
	public:
		ResHandle() { m_pResource = NULL; }
		virtual ~ResHandle() {}

		// ������Դ·��
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

		// ģ��ʵ����ָ��
		T *             GetImpliment() { return (T *) m_pResource; }

		T *             operator-> () { return (T *) m_pResource; }

	protected:
		// ģ��ʵ����ָ��
		Resource *      m_pResource;

	private:
	};
}

#endif