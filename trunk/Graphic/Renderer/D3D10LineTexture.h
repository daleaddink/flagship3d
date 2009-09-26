#ifndef _D3D10LINETEXTURE_H_
#define _D3D10LINETEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D10LineTexture : public Texture
	{
	public:
		D3D10LineTexture();
		virtual ~D3D10LineTexture();

		// ��ȡʵ������ָ��
		ID3D10Texture1D *             GetImpliment();

		// ��ȡ��Դ����
		ID3D10ShaderResourceView *    GetShaderResourceView();

	public:
		// ����Դ����Ч��
		virtual bool                  CreateFromMemory();

		// ������ͼ
		virtual bool                  CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// �����ͼ
		virtual bool                  ClearTexture();

		// ������ͼ
		virtual bool                  Lock( int& rPitch, void ** ppData );

		// ������ͼ
		virtual void                  UnLock();

	protected:
		// �ͷŻ���
		virtual void                  UnCache();

	protected:
		// D3D10��ͼ����
		ID3D10Texture1D *             m_pD3D10LineTexture;

		// Shader��Դ����
		ID3D10ShaderResourceView *    m_pD3D10ShaderResource;

	private:

	};

	typedef ResHandle<D3D10LineTexture> D3D10LineTextureHandle;
}

#endif