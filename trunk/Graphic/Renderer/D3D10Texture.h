#ifndef _D3D10TEXTURE_H_
#define _D3D10TEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D10Texture : public Texture
	{
	public:
		D3D10Texture();
		virtual ~D3D10Texture();

		// ��ȡʵ������ָ��
		ID3D10Texture2D *             GetImpliment();

		// ��ȡ��Դ����
		ID3D10ShaderResourceView *    GetShaderResourceView();

		// ��ȡ��Ⱦ����
		ID3D10RenderTargetView *      GetRenderTargetView();

		// ��ȡ��ȶ���
		ID3D10DepthStencilView *      GetDepthStencilView();

	public:
		// ����Դ����Ч��
		virtual bool                  CreateFromMemory();

		// ������ͼ
		virtual bool                  CreateDynamic( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// ������Ⱦ��ͼ
		virtual bool                  CreateRenderTarget( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

		// ���������ͼ
		virtual bool                  CreateDepthStencil( UINT uiWidth, UINT uiHeight, DWORD dwFormat );

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
		ID3D10Texture2D *             m_pD3D10Texture;

		// Shader��Դ����
		ID3D10ShaderResourceView *    m_pD3D10ShaderResource;

		// ��Ⱦ����
		ID3D10RenderTargetView *      m_pD3D10RenderTarget;

		// ��ȶ���
		ID3D10DepthStencilView *      m_pD3D10DepthStencil;

	private:

	};

	typedef ResHandle<D3D10Texture> D3D10TextureHandle;
}

#endif