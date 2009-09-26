#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <Common/Prerequisites.h>
#include "Base.h"

namespace Flagship
{
	class Effect;
	class Renderable;
	class Resource;
	class Texture;

	class _DLL_Export Material : public Base
	{
	public:
		Material();
		~Material();

		// ���ʲ�������
		enum
		{
			Entity_WorldMatrix,
			Entity_ViewMatrix,
			Entity_ProjMatrix,
			Entity_MVPMatrix,
			Entity_WorldIMatrix,
			Entity_ViewIMatrix,
			AnimEntity_SkinnedMatrix,
			AnimEntity_SkinnedMap,

			Camera_Position,
			Camera_Direction,
			Camera_Up,
			Camera_Right,

			Light_Ambient,
			Light_Color,
			Light_Position,
			Light_Direction,
			Light_Attenuation,

			Texture_Color,
						
			Param_Max,
		};

		// ���ʰ汾
		enum
		{
			Technique_Ambient,
			Technique_AmbientS,
			Technique_Light,
			Technique_LightS,
			Technique_MakeShadow,
			Technique_MakeShadowS,
			Technique_PostShadow,
			Technique_PostShadowS,
			Technique_Color,
			Technique_ColorS,
			Technique_Max,
		};

		// ��ʼ����
		bool                   Begin( UINT * uiPassNum );

		// ��������
		bool                   End();

		// ��ʼ��Ⱦ
		bool                   BeginPass( UINT uiPass );

		// ������Ⱦ
		bool                   EndPass();

		// ������ͼ·��
		void                   SetTexturePath( wstring& szPath );

	public:
		// ��ʼ��
		virtual bool           Initialize();

		// ����
		virtual bool           Reset();

		// ����������
		virtual void           BuildParamMap();

		// ����������
		virtual void           ClearParamMap();

		// ���ò��ʰ汾
		virtual bool           SetTechnique( Renderable * pParent, BYTE byTechType );

		// ���²��ʽڵ����
		virtual void           Update( Renderable * pParent );

		// ���²�����Դ����
		virtual void           Update( Resource * pParent );

		// ����
		virtual void           PostProcess();

		// �Ƿ����
		virtual bool           IsReady();

		// ��ȡ��Դ
		virtual bool           Load();

		// �ͷ���Դ
		virtual bool           Release();

	protected:
		// Ч������
		Effect *               m_pEffect;

		// ��ͼ·��
		Texture *              m_pTexture;

		// ��ͼ·��
		wstring                m_szTexturePath;

		// ���ʲ�����
		map< int, Key >        m_mParamMap;

		// ��ͼ������
		map< Key, Texture * >  m_mTextureMap;

		// �Ƽ���
		map< int, Key >        m_mTechniqueMap;

		// ����������ͼ
		Texture *              m_pSkinnedMap;

		// ��ʱ��Ӱ��ͼ����
		int                    m_iFlatShadowMap;
		int                    m_iCubeShadowMap;
		Matrix4f               m_pLightMatrix[MAX_LIGHT];

		// ��ɱ�־
		bool                   m_bFirst;
		
	private:

	};
}

#endif