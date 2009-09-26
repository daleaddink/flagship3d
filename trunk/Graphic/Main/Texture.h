#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <Common/Prerequisites.h>
#include "Resource.h"

namespace Flagship
{
	class _DLL_Export Texture : public Resource
	{
	public:
		Texture();
		virtual ~Texture();

	public:
		// ����Դ����Ч��
		virtual bool    CreateFromMemory() { return true; }

		// ������ͼ
		virtual bool    CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat ) { return true; }

		// ������Ⱦ��ͼ
		virtual bool    CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat ) { return true; }

		// ���������ͼ
		virtual bool    CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat ) { return true; }

		// �����ͼ
		virtual bool    ClearTexture() { return true; }

		// ������ͼ
		virtual bool    Lock( int& rPitch, void ** ppData ) { return true; }

		// ������ͼ
		virtual void    UnLock() {}

	protected:
		// ������Դ
		virtual bool    Cache();
		
	private:

	};
}

#endif