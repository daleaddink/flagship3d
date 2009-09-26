#ifndef _D3D9VOLUMETEXTURE_H_
#define _D3D9VOLUMETEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/Texture.h"

namespace Flagship
{
	class _DLL_Export D3D9VolumeTexture : public Texture
	{
	public:
		D3D9VolumeTexture();
		virtual ~D3D9VolumeTexture();

		// ��ȡʵ������ָ��
		LPDIRECT3DVOLUMETEXTURE9    GetImpliment();

	public:
		// ����Դ����Ч��
		virtual bool                CreateFromMemory();

		// �����ͼ
		virtual bool                ClearTexture();

	protected:
		// �ͷŻ���
		virtual void                UnCache();

	protected:
		// D3D9��ͼ����
		LPDIRECT3DVOLUMETEXTURE9    m_pD3D9VolumeTexture;

	private:
	};

	typedef ResHandle<D3D9VolumeTexture> D3D9VolumeTextureHandle;
}

#endif