#ifndef _D3D10RENDERCUBETEXTURE_H_
#define _D3D10RENDERCUBETEXTURE_H_

#include "D3D10Prerequisites.h"
#include "../Main/RenderTexture.h"

namespace Flagship
{
	class _DLL_Export D3D10RenderCubeTexture : public RenderTexture
	{
	public:
		D3D10RenderCubeTexture();
		virtual ~D3D10RenderCubeTexture();

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

		// 初始化渲染贴图
		bool                        Initialize( UINT uiSize, DWORD dwFormat );

	public:
		// 存储渲染贴图
		virtual void                SaveRenderTexture( wstring szPath );

		// 更新渲染对象
		virtual void                Update();

	protected:
		// 渲染表面
		ID3D10RenderTargetView *    m_pRenderSurface[Face_Max];

		// 深度模版表面
		ID3D10DepthStencilView *    m_pDepthSurface;

	private:
	};
}

#endif