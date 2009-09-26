#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include <Common/Prerequisites.h>
#include "Base.h"
#include "Camera.h"

namespace Flagship
{
	class Light;
	class ShadowMap;
	class Renderer;
	class Material;

	struct ShadowInfo
	{
		int iSMCount;
		int iCubeSMCount;

		ShadowInfo::ShadowInfo()
		{
			iSMCount = 0;
			iCubeSMCount = 0;
		}
	};

	class _DLL_Export RenderTarget : public Base
	{
	public:
		RenderTarget();
		virtual ~RenderTarget();

		// 设置摄像机
		void                     SetCamera( Camera * pCamera );

		// 获取摄像机
		Camera *                 GetCamera() { return m_pCamera; }

		// 获取渲染器
		Renderer *               GetRenderer();

		// 获取阴影贴图
		ShadowMap **             GetShadowMap() { return m_pShadowMap; }

		// 获取阴影图数量
		int                      GetShadowMapCount() { return m_iSMCount; }

		// 设置当前渲染对象
		static void              SetActiveRenderTarget( RenderTarget * pTarget );

		// 获取当前渲染对象
		static RenderTarget *    GetActiveRenderTarget();

		// 设置当前摄像机
		static void              SetActiveCamera( Camera * pCamera );

		// 获取当前摄像机
		static Camera *          GetActiveCamera();

	public:
		// 更新渲染对象
		virtual void             Update();

		// 准备阴影图
		virtual void             PrepareShadowMap();

		// 存储渲染贴图
		virtual void             SaveRenderTexture( wstring szPath );

	protected:
		// 摄像机指针
		Camera *                 m_pCamera;

		// 渲染器指针
		Renderer *               m_pRenderer;

		// 阴影贴图
		ShadowMap *              m_pShadowMap[MAX_LIGHT * 4];

		// 阴影图数量
		int                      m_iSMCount;

        // 当前渲染对象
		static RenderTarget *    m_pActiveRenderTarget;

		// 当前摄像机
		static Camera *          m_pActiveCamera;

	private:
	};
}

#endif