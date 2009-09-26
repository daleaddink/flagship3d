#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <Common/Prerequisites.h>
#include <Common/Thread.h>
#include "RenderQueue.h"
#include "Entity.h"
#include "Effect.h"

namespace Flagship
{
	class Entity;
	class Effect;
	class Camera;
	
	class _DLL_Export Renderer : public Thread
	{
	public:
		Renderer( BYTE byRenderType = RenderType_All );
		virtual ~Renderer();

		enum
		{
			RenderType_NoReflect,
			RenderType_Shadow,
			RenderType_All,
		};

        enum
		{
			RenderQueue_General,
			RenderQueue_Visible,
			RenderQueue_Light,
			RenderQueue_Bright,
			
			RenderQueue_Max,
		};

		// 获取渲染队列
		RenderQueue *       GetRenderQueue( int iType );

		// 获取渲染器类型
		void                SetRenderType( BYTE byRenderType ) { m_byRenderType = byRenderType; }

		// 获取渲染器类型
		BYTE                GetRenderType() { return m_byRenderType; }

		// 渲染场景
		void                RenderScene();

	public:
		// 渲染循环
		virtual int         Run();

		// 开始渲染
		virtual bool        BeginScene();

		// 结束渲染
		virtual bool        EndScene();

		// 清理缓冲
		virtual bool        Clear( Vector4f& vColor = Vector4f( 0.0f, 0.0f, 0.0f, 1.0f ) );

		// 显示到屏幕
		virtual bool        Present();

		// 渲染对象
		virtual bool        RenderSingle( Material * pMaterial, Resource * pResource );

		// 渲染屏幕四边形
		virtual bool        RenderQuad( Effect * pEffect, float fLeftU, float fTopV, float fRightU, float fBottomV );

		// 渲染文字
		virtual bool        RenderText( int iX, int iY, Vector4f& vColor, wstring& szText );

	protected:
		// 渲染类型
		BYTE                m_byRenderType;

		// 渲染队列表
		RenderQueue *       m_pRenderQueue;
		
	private:
	};
}

#endif