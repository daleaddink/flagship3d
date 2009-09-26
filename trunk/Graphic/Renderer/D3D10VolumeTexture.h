#ifndef _D3D10VOLUMETEXTURE_H_
#define _D3D10VOLUMETEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D10VolumeTexture : public Texture
	{
	public:
		D3D10VolumeTexture();
		virtual ~D3D10VolumeTexture();

		// ��ȡʵ������ָ��
		ID3D10Texture3D *             GetImpliment();

		// ��ȡ��Դ����
		ID3D10ShaderResourceView *    GetShaderResourceView();

	public:
		// ����Դ����Ч��
		virtual bool                  CreateFromMemory();

		// �����ͼ
		virtual bool                  ClearTexture();

	protected:
		// �ͷŻ���
		virtual void                  UnCache();

	protected:
		// D3D10��ͼ����
		ID3D10Texture3D *             m_pD3D10VolumeTexture;

		// Shader��Դ����
		ID3D10ShaderResourceView *    m_pD3D10ShaderResource;

	private:

	};

	typedef ResHandle<D3D10VolumeTexture> D3D10VolumeTextureHandle;
}

#endif