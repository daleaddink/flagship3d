#ifndef _D3D9CUBETEXTURE_H_
#define _D3D9CUBETEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D9CubeTexture : public Texture
	{
	public:
		D3D9CubeTexture();
		virtual ~D3D9CubeTexture();

		// ��ȡʵ������ָ��
		LPDIRECT3DCUBETEXTURE9    GetImpliment();

	public:
		// ����Դ����Ч��
		virtual bool              CreateFromMemory();

		// ������ͼ
		virtual bool              CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// �����ͼ
		virtual bool              ClearTexture();

	protected:
		// �ͷŻ���
		virtual void              UnCache();

	protected:
		// D3D9��ͼ����
		LPDIRECT3DCUBETEXTURE9    m_pD3D9CubeTexture;

	private:

	};

	typedef ResHandle<D3D9CubeTexture> D3D9CubeTextureHandle;
}

#endif