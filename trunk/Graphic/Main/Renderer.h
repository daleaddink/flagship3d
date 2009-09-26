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

		// ��ȡ��Ⱦ����
		RenderQueue *       GetRenderQueue( int iType );

		// ��ȡ��Ⱦ������
		void                SetRenderType( BYTE byRenderType ) { m_byRenderType = byRenderType; }

		// ��ȡ��Ⱦ������
		BYTE                GetRenderType() { return m_byRenderType; }

		// ��Ⱦ����
		void                RenderScene();

	public:
		// ��Ⱦѭ��
		virtual int         Run();

		// ��ʼ��Ⱦ
		virtual bool        BeginScene();

		// ������Ⱦ
		virtual bool        EndScene();

		// ������
		virtual bool        Clear( Vector4f& vColor = Vector4f( 0.0f, 0.0f, 0.0f, 1.0f ) );

		// ��ʾ����Ļ
		virtual bool        Present();

		// ��Ⱦ����
		virtual bool        RenderSingle( Material * pMaterial, Resource * pResource );

		// ��Ⱦ��Ļ�ı���
		virtual bool        RenderQuad( Effect * pEffect, float fLeftU, float fTopV, float fRightU, float fBottomV );

		// ��Ⱦ����
		virtual bool        RenderText( int iX, int iY, Vector4f& vColor, wstring& szText );

	protected:
		// ��Ⱦ����
		BYTE                m_byRenderType;

		// ��Ⱦ���б�
		RenderQueue *       m_pRenderQueue;
		
	private:
	};
}

#endif