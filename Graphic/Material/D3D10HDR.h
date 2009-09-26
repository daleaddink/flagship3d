#ifndef _D3D10HDR_H_
#define _D3D10HDR_H_

#include "../Renderer/D3D10Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class D3D10Texture;
	class CGlareDef;
	enum  EGLARELIBTYPE;

	class _DLL_Export D3D10HDR : public Material
	{
	public:
		D3D10HDR();
		virtual ~D3D10HDR();

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
		// ��ʼ��
		virtual bool               Initialize();

		// ����
		virtual bool               Reset();

		// ����������
		virtual void               BuildParamMap();

		// ��ʼ��Ⱦ
		virtual bool               BeginPass( UINT uiPass );

		// ������Ⱦ
		virtual bool               EndPass();

		// ����
		virtual void               PostProcess();

	protected:
		// �������
		bool                       Scene_To_SceneScaled();
		bool                       SceneScaled_To_BrightPass();
		bool                       BrightPass_To_StarSource();
		bool                       StarSource_To_BloomSource();
		bool                       MeasureLuminance();
		bool                       CalculateAdaptation();
		bool                       RenderStar();
		bool                       RenderBloom();

		// ��������
		bool                       GetTextureRect( ID3D10Texture2D * pTexture, RECT * pRect );
		bool                       GetTextureCoords( ID3D10Texture2D * pTexSrc, RECT * pRectSrc, ID3D10Texture2D * pTexDest, RECT * pRectDest, CoordRect * pCoords );
		bool                       ClearTexture( ID3D10Texture2D * pTexture );

		// λ�Ƽ���
		float                      GaussianDistribution( float x, float y, float rho );
		bool                       GetSampleOffsets_GaussBlur5x5(DWORD dwD3DTexWidth, DWORD dwD3DTexHeight, D3DXVECTOR4 * avTexCoordOffset, D3DXVECTOR4 * avSampleWeights, float fMultiplier = 1.0f );
		bool                       GetSampleOffsets_Bloom(DWORD dwD3DTexSize, float afTexCoordOffset[15], D3DXVECTOR4 * avColorWeight, float fDeviation, float fMultiplier=1.0f);    
		bool                       GetSampleOffsets_Star(DWORD dwD3DTexSize, float afTexCoordOffset[15], D3DXVECTOR4 * avColorWeight, float fDeviation);    
		bool                       GetSampleOffsets_DownScale4x4( DWORD dwWidth, DWORD dwHeight, D3DXVECTOR2 avSampleOffsets[] );
		bool                       GetSampleOffsets_DownScale2x2( DWORD dwWidth, DWORD dwHeight, D3DXVECTOR2 avSampleOffsets[] );

		// HDR������Ⱦ��ͼ
		D3D10Texture *             m_pSceneTexture;

		// ����������ͼ
		D3D10Texture *             m_pSceneScaled;

		// ������ͼ
		D3D10Texture *             m_pBrightTexture;

		// �ǹ�Դ��ͼ
		D3D10Texture *             m_pStarSource;

		// ����Դ��ͼ
		D3D10Texture *             m_pBloomSource;

		// ��ǰ����
		D3D10Texture *             m_pCurLuminance;

		// ��������
		D3D10Texture *             m_pLastLuminance;

		// �ǹ���ͼ
		D3D10Texture *             m_pStarTexture;

		// ������ͼ
		D3D10Texture *             m_pBloomTexture;

		// Tone��ͼ
		D3D10Texture *             m_pToneMapTexture;

		// ������ͼ��С
		DWORD                      m_dwCropWidth;
		DWORD                      m_dwCropHeight;

		// ѣ�ⶨ��
		CGlareDef *                m_pGlareDef;
		EGLARELIBTYPE              m_eGlareType;

		// ������ֵ
		Key                        m_kProj;
		Key                        m_kBloomScale;
		Key                        m_kStarScale;
		Key                        m_kMiddleGray;
		Key                        m_kSampleOffsets;
		Key                        m_kSampleWeights;
		Key                        m_kElapsedTime;
		Key                        m_kTexture[8];
		
		// �Ƽ���ֵ
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