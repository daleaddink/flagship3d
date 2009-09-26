#ifndef _D3D10CUBETEXTURE_H_
#define _D3D10CUBETEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D10CubeTexture : public Texture
	{
	public:
		D3D10CubeTexture();
		virtual ~D3D10CubeTexture();

		// ��ȡʵ������ָ��
		ID3D10Texture2D *             GetImpliment();

		// ��ȡ��Դ����
		ID3D10ShaderResourceView *    GetShaderResourceView();

	public:
		// ����Դ����Ч��
		virtual bool                  CreateFromMemory();

		// ������ͼ
		virtual bool                  CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// ������ͼ
		virtual bool                  CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// �����ͼ
		virtual bool                  ClearTexture();

	protected:
		// �ͷŻ���
		virtual void                  UnCache();

	protected:
		// D3D10��ͼ����
		ID3D10Texture2D *             m_pD3D10CubeTexture;

		// Shader��Դ����
		ID3D10ShaderResourceView *    m_pD3D10ShaderResource;

	private:

	};

	typedef ResHandle<D3D10CubeTexture> D3D10CubeTextureHandle;
}

#endif