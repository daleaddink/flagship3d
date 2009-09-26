#ifndef _D3D10RENDERER_H_
#define _D3D10RENDERER_H_

#include "D3D10Prerequisites.h"
#include "../Main/Renderer.h"

namespace Flagship
{
	class _DLL_Export D3D10Renderer : public Renderer
	{
	public:
		D3D10Renderer( BYTE byRenderType = RenderType_All );
		~D3D10Renderer();

	public:
		// ��ʼ��Ⱦ
		virtual bool                BeginScene();

		// ������Ⱦ
		virtual bool                EndScene();

		// ������
		virtual bool                Clear(  Vector4f& vColor = Vector4f( 0.0f, 0.0f, 0.0f, 1.0f ) );

		// ��ʾ����Ļ
		virtual bool                Present();

		// ���û��״̬
		virtual bool                SetBlendEnable( bool bBlend );

		// ��Ⱦ����
		virtual bool                RenderSingle( Material * pMaterial, Resource * pResource );

		// ��Ⱦ��Ļ�ı���
		virtual bool                RenderQuad( Effect * pEffect, float fLeftU, float fTopV, float fRightU, float fBottomV );

		// ��Ⱦ����
		virtual bool                RenderText( int iX, int iY, Vector4f& vColor, wstring& szText );

	protected:
        		
	private:
	};
}

#endif