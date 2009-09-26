#ifndef _D3D9RENDERER_H_
#define _D3D9RENDERER_H_

#include "D3D9Prerequisites.h"
#include "../Main/Renderer.h"

namespace Flagship
{
	class _DLL_Export D3D9Renderer : public Renderer
	{
	public:
		D3D9Renderer( BYTE byRenderType = RenderType_All );
		~D3D9Renderer();

		struct ScreenVertex
		{
			D3DXVECTOR4 vPosition;
			D3DXVECTOR2 vTextureCoord;
			static const DWORD FVF;
		};

	public:
		// ��ʼ��Ⱦ
		virtual bool        BeginScene();

		// ������Ⱦ
		virtual bool        EndScene();

		// ������
		virtual bool        Clear( Vector4f& vColor = Vector4f( 1.0f, 0.0f, 0.0f, 0.0f ) );

		// ��ʾ����Ļ
		virtual bool        Present();

		// ��Ⱦ����
		virtual bool        RenderSingle( Material * pMaterial, Resource * pResource );

		// ��Ⱦ��Ļ�ı���
		virtual bool        RenderQuad( Effect * pEffect, float fLeftU, float fTopV, float fRightU, float fBottomV );

		// ��Ⱦ����
		virtual bool        RenderText( int iX, int iY, Vector4f& vColor, wstring& szText );

	protected:
		
	private:
	};
}

#endif