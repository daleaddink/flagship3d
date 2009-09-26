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

		// ���������
		void                     SetCamera( Camera * pCamera );

		// ��ȡ�����
		Camera *                 GetCamera() { return m_pCamera; }

		// ��ȡ��Ⱦ��
		Renderer *               GetRenderer();

		// ��ȡ��Ӱ��ͼ
		ShadowMap **             GetShadowMap() { return m_pShadowMap; }

		// ��ȡ��Ӱͼ����
		int                      GetShadowMapCount() { return m_iSMCount; }

		// ���õ�ǰ��Ⱦ����
		static void              SetActiveRenderTarget( RenderTarget * pTarget );

		// ��ȡ��ǰ��Ⱦ����
		static RenderTarget *    GetActiveRenderTarget();

		// ���õ�ǰ�����
		static void              SetActiveCamera( Camera * pCamera );

		// ��ȡ��ǰ�����
		static Camera *          GetActiveCamera();

	public:
		// ������Ⱦ����
		virtual void             Update();

		// ׼����Ӱͼ
		virtual void             PrepareShadowMap();

		// �洢��Ⱦ��ͼ
		virtual void             SaveRenderTexture( wstring szPath );

	protected:
		// �����ָ��
		Camera *                 m_pCamera;

		// ��Ⱦ��ָ��
		Renderer *               m_pRenderer;

		// ��Ӱ��ͼ
		ShadowMap *              m_pShadowMap[MAX_LIGHT * 4];

		// ��Ӱͼ����
		int                      m_iSMCount;

        // ��ǰ��Ⱦ����
		static RenderTarget *    m_pActiveRenderTarget;

		// ��ǰ�����
		static Camera *          m_pActiveCamera;

	private:
	};
}

#endif