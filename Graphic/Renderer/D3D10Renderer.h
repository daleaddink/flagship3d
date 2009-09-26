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
		// 开始渲染
		virtual bool                BeginScene();

		// 结束渲染
		virtual bool                EndScene();

		// 清理缓冲
		virtual bool                Clear(  Vector4f& vColor = Vector4f( 0.0f, 0.0f, 0.0f, 1.0f ) );

		// 显示到屏幕
		virtual bool                Present();

		// 设置混合状态
		virtual bool                SetBlendEnable( bool bBlend );

		// 渲染对象
		virtual bool                RenderSingle( Material * pMaterial, Resource * pResource );

		// 渲染屏幕四边形
		virtual bool                RenderQuad( Effect * pEffect, float fLeftU, float fTopV, float fRightU, float fBottomV );

		// 渲染文字
		virtual bool                RenderText( int iX, int iY, Vector4f& vColor, wstring& szText );

	protected:
        		
	private:
	};
}

#endif