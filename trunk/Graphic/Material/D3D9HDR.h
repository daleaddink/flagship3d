#ifndef _D3D9HDR_H_
#define _D3D9HDR_H_

#include "../Renderer/D3D9Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class D3D9Texture;
	class CGlareDef;
	enum  EGLARELIBTYPE;

	class _DLL_Export D3D9HDR : public Material
	{
	public:
		D3D9HDR();
		virtual ~D3D9HDR();

		struct CoordRect
		{
			float fLeftU;
			float fTopV;
			float fRightU;
			float fBottomV;
		};

		enum
		{
			MAX_SAMPLES            = 16,
			NUM_TONEMAP_TEXTURES   = 4,
			NUM_STAR_TEXTURES      = 12,
			NUM_BLOOM_TEXTURES     = 3,
		};

	public:
		// 初始化
		virtual bool               Initialize();

		// 重置
		virtual bool               Reset();

		// 建立参数表
		virtual void               BuildParamMap();

		// 开始渲染
		virtual bool               BeginPass( UINT uiPass );

		// 结束渲染
		virtual bool               EndPass();

		// 后处理
		virtual void               PostProcess();

	protected:
		// 后处理过程
		bool                       Scene_To_SceneScaled();
		bool                       SceneScaled_To_BrightPass();
		bool                       BrightPass_To_StarSource();
		bool                       StarSource_To_BloomSource();
		bool                       MeasureLuminance();
		bool                       CalculateAdaptation();
		bool                       RenderStar();
		bool                       RenderBloom();

		// 辅助函数
		bool                       GetTextureRect( LPDIRECT3DTEXTURE9 pTexture, RECT * pRect );
		bool                       GetTextureCoords( LPDIRECT3DTEXTURE9 pTexSrc, RECT * pRectSrc, LPDIRECT3DTEXTURE9 pTexDest, RECT * pRectDest, CoordRect * pCoords );
		bool                       ClearTexture( LPDIRECT3DTEXTURE9 pTexture );

		// 位移计算
		float                      GaussianDistribution( float x, float y, float rho );
		bool                       GetSampleOffsets_GaussBlur5x5(DWORD dwD3DTexWidth, DWORD dwD3DTexHeight, D3DXVECTOR2 * avTexCoordOffset, D3DXVECTOR4 * avSampleWeights, float fMultiplier = 1.0f );
		bool                       GetSampleOffsets_Bloom(DWORD dwD3DTexSize, float afTexCoordOffset[15], D3DXVECTOR4 * avColorWeight, float fDeviation, float fMultiplier=1.0f);    
		bool                       GetSampleOffsets_Star(DWORD dwD3DTexSize, float afTexCoordOffset[15], D3DXVECTOR4 * avColorWeight, float fDeviation);    
		bool                       GetSampleOffsets_DownScale4x4( DWORD dwWidth, DWORD dwHeight, D3DXVECTOR2 avSampleOffsets[] );
		bool                       GetSampleOffsets_DownScale2x2( DWORD dwWidth, DWORD dwHeight, D3DXVECTOR2 avSampleOffsets[] );

		// HDR场景渲染贴图
		D3D9Texture *              m_pSceneTexture;

		// 场景缩放贴图
		D3D9Texture *              m_pSceneScaled;

		// 高亮贴图
		D3D9Texture *              m_pBrightTexture;

		// 星光源贴图
		D3D9Texture *              m_pStarSource;

		// 泛光源贴图
		D3D9Texture *              m_pBloomSource;

		// 当前流明
		D3D9Texture *              m_pCurLuminance;

		// 最终流明
		D3D9Texture *              m_pLastLuminance;

		// 星光贴图
		D3D9Texture *              m_pStarTexture;

		// 泛光贴图
		D3D9Texture *              m_pBloomTexture;

		// Tone贴图
		D3D9Texture *              m_pToneMapTexture;

		// 缩放贴图大小
		DWORD                      m_dwCropWidth;
		DWORD                      m_dwCropHeight;

		// 眩光定义
		CGlareDef *                m_pGlareDef;
		EGLARELIBTYPE              m_eGlareType;

		// 参数键值
		Key                        m_kProj;
		Key                        m_kBloomScale;
		Key                        m_kStarScale;
		Key                        m_kMiddleGray;
		Key                        m_kSampleOffsets;
		Key                        m_kSampleWeights;
		Key                        m_kElapsedTime;
		
		// 科技键值
		Key                        m_kFinalScenePass;
		Key                        m_kDownScale4x4;
		Key                        m_kBrightPassFilter;
		Key                        m_kGaussBlur5x5;
		Key                        m_kDownScale2x2;
		Key                        m_kSampleAvgLum;
		Key                        m_kResampleAvgLum;
		Key                        m_kResampleAvgLumExp;
		Key                        m_kCalculateAdaptedLum;
		Key                        m_kStar;
		Key                        m_kBloom;
		Key                        m_kMergeTextures[8];

	private:

	};
}

#endif