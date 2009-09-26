#ifndef _D3D9RENDERCUBETEXTURE_H_
#define _D3D9RENDERCUBETEXTURE_H_

#include "D3D9Prerequisites.h"
#include "../Main/RenderTexture.h"

namespace Flagship
{
	class _DLL_Export D3D9RenderCubeTexture : public RenderTexture
	{
	public:
		D3D9RenderCubeTexture();
		virtual ~D3D9RenderCubeTexture();

		enum
		{
			Face_PositiveX,
			Face_NegativeX,
			Face_PositiveY,
			Face_NegativeY,
			Face_PositiveZ,
			Face_NegativeZ,

			Face_Max,
		};

		// ��ʼ����Ⱦ��ͼ
		bool                      Initialize( UINT uiSize, DWORD dwFormat );

	public:
		// �洢��Ⱦ��ͼ
		virtual void              SaveRenderTexture( wstring szPath );

		// ������Ⱦ����
		virtual void              Update();

	protected:
		// ��Ⱦ����
		LPDIRECT3DSURFACE9        m_pRenderSurface[Face_Max];

		// ���ģ�����
		LPDIRECT3DSURFACE9        m_pDepthSurface;

	private:
	};
}

#endif