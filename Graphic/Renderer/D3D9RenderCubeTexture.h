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

		// 初始化渲染贴图
		bool                      Initialize( UINT uiSize, DWORD dwFormat );

	public:
		// 存储渲染贴图
		virtual void              SaveRenderTexture( wstring szPath );

		// 更新渲染对象
		virtual void              Update();

	protected:
		// 渲染表面
		LPDIRECT3DSURFACE9        m_pRenderSurface[Face_Max];

		// 深度模版表面
		LPDIRECT3DSURFACE9        m_pDepthSurface;

	private:
	};
}

#endif