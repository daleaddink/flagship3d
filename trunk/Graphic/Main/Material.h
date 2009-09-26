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

		// 材质参数类型
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

		// 材质版本
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

		// 开始材质
		bool                   Begin( UINT * uiPassNum );

		// 结束材质
		bool                   End();

		// 开始渲染
		bool                   BeginPass( UINT uiPass );

		// 结束渲染
		bool                   EndPass();

		// 设置贴图路径
		void                   SetTexturePath( wstring& szPath );

	public:
		// 初始化
		virtual bool           Initialize();

		// 重置
		virtual bool           Reset();

		// 建立参数表
		virtual void           BuildParamMap();

		// 建立参数表
		virtual void           ClearParamMap();

		// 设置材质版本
		virtual bool           SetTechnique( Renderable * pParent, BYTE byTechType );

		// 更新材质节点参数
		virtual void           Update( Renderable * pParent );

		// 更新材质资源数据
		virtual void           Update( Resource * pParent );

		// 后处理
		virtual void           PostProcess();

		// 是否就绪
		virtual bool           IsReady();

		// 读取资源
		virtual bool           Load();

		// 释放资源
		virtual bool           Release();

	protected:
		// 效果对象
		Effect *               m_pEffect;

		// 贴图路径
		Texture *              m_pTexture;

		// 贴图路径
		wstring                m_szTexturePath;

		// 材质参数表
		map< int, Key >        m_mParamMap;

		// 贴图参数表
		map< Key, Texture * >  m_mTextureMap;

		// 科技表
		map< int, Key >        m_mTechniqueMap;

		// 骨骼矩阵贴图
		Texture *              m_pSkinnedMap;

		// 临时阴影贴图计数
		int                    m_iFlatShadowMap;
		int                    m_iCubeShadowMap;
		Matrix4f               m_pLightMatrix[MAX_LIGHT];

		// 完成标志
		bool                   m_bFirst;
		
	private:

	};
}

#endif