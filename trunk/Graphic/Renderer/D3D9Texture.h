#ifndef _D3D9TEXTURE_H_
#define _D3D9TEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D9Texture : public Texture
	{
	public:
		D3D9Texture();
		virtual ~D3D9Texture();

		// ��ȡʵ������ָ��
		LPDIRECT3DTEXTURE9    GetImpliment();

	public:
		// ����Դ����Ч��
		virtual bool          CreateFromMemory();

		// ������ͼ
		virtual bool          CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// ������Ⱦ��ͼ
		virtual bool          CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// ���������ͼ
		virtual bool          CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// �����ͼ
		virtual bool          ClearTexture();

		// ������ͼ
		virtual bool          Lock( int& rPitch, void ** ppData );

		// ������ͼ
		virtual void          UnLock();

	protected:
		// �ͷŻ���
		virtual void          UnCache();

	protected:
		// D3D9��ͼ����
		LPDIRECT3DTEXTURE9    m_pD3D9Texture;

	private:
	};

	typedef ResHandle<D3D9Texture> D3D9TextureHandle;
}

#endif