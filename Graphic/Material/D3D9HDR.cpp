#include "D3D9HDR.h"
#include "HDRGlareDef.h"
#include "../Main/DataCenter.h"
#include "../Main/ResourceManager.h"
#include "../Main/Camera.h"
#include "../Renderer/D3D9RenderWindow.h"
#include "../Renderer/D3D9Renderer.h"
#include "../Renderer/D3D9Texture.h"
#include "../Renderer/D3D9Effect.h"

namespace Flagship
{
	D3D9HDR::D3D9HDR()
	{
		m_mParamMap.clear();
		m_mTechniqueMap.clear();
		m_eGlareType = GLT_FILTER_CROSSSCREEN;

		// 材质类型
		m_iClassType = Base::Material_PostProcess;

		m_pSceneTexture = NULL;
		m_pSceneScaled = NULL;
		m_pBrightTexture = NULL;
		m_pStarSource = NULL;
		m_pBloomSource = NULL;
		m_pCurLuminance = NULL;
		m_pLastLuminance = NULL;
		m_pStarTexture = NULL;
		m_pBloomTexture = NULL;
		m_pToneMapTexture = NULL;
	}

	D3D9HDR::~D3D9HDR()
	{
		SAFE_DELETE( m_pEffect );
		SAFE_DELETE( m_pGlareDef );

		SAFE_DELETE( m_pSceneTexture );
		SAFE_DELETE( m_pSceneScaled );
		SAFE_DELETE( m_pBrightTexture );
		SAFE_DELETE( m_pStarSource );
		SAFE_DELETE( m_pBloomSource );
		SAFE_DELETE( m_pCurLuminance );
		SAFE_DELETE( m_pLastLuminance );

		SAFE_DELETE_ARRAY( m_pStarTexture );
		SAFE_DELETE_ARRAY( m_pBloomTexture );
		SAFE_DELETE_ARRAY( m_pToneMapTexture );
	}

	bool    D3D9HDR::Initialize()
	{
		// 创建D3D9Effect对象
		wchar_t szPath[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szPath );
		wstring szEffectPath = szPath;
		szEffectPath += L"\\Effect\\D3D9HDR.fx";

		D3D9EffectHandle hEffectHandle;
		hEffectHandle.SetPath( szEffectPath );
		m_pEffect = hEffectHandle.GetImpliment();
		m_pEffect->Load();
		BuildParamMap();

		// 初始化对象
		m_pSceneTexture = new D3D9Texture;
		m_pSceneScaled = new D3D9Texture;
		m_pBrightTexture = new D3D9Texture;
		m_pStarSource = new D3D9Texture;
		m_pBloomSource = new D3D9Texture;
		m_pCurLuminance = new D3D9Texture;
		m_pLastLuminance = new D3D9Texture;

		m_pStarTexture = new D3D9Texture[NUM_STAR_TEXTURES];
		m_pBloomTexture = new D3D9Texture[NUM_BLOOM_TEXTURES];
		m_pToneMapTexture = new D3D9Texture[NUM_TONEMAP_TEXTURES];

		m_pGlareDef = new CGlareDef;
		m_pGlareDef->Initialize( m_eGlareType );

		return Reset();
	}

	bool    D3D9HDR::Reset()
	{
		// 重置资源
		m_pSceneTexture->Release();
		m_pSceneScaled->Release();
		m_pBrightTexture->Release();
		m_pStarSource->Release();
		m_pBloomSource->Release();
		m_pCurLuminance->Release();
		m_pLastLuminance->Release();

		for ( int i = 0; i < NUM_STAR_TEXTURES; i++ )
		{
			m_pStarTexture[i].Release();
		}

		for ( int i = 0; i < NUM_BLOOM_TEXTURES; i++ )
		{
			m_pBloomTexture[i].Release();
		}

		for ( int i = 0; i < NUM_TONEMAP_TEXTURES; i++ )
		{
			m_pToneMapTexture[i].Release();
		}

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 获取后备缓冲格式
		LPDIRECT3DSURFACE9 pBackBuffer;
		pD3D9Device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
		D3DSURFACE_DESC kBackBufferDesc;
		pBackBuffer->GetDesc( &kBackBufferDesc );
		SAFE_RELEASE( pBackBuffer );

		m_dwCropWidth = kBackBufferDesc.Width - kBackBufferDesc.Width % 8;
		m_dwCropHeight = kBackBufferDesc.Height - kBackBufferDesc.Height % 8;

		// Create the HDR scene texture
		if ( ! m_pSceneTexture->CreateRenderTarget( kBackBufferDesc.Width, kBackBufferDesc.Height, D3DFMT_A16B16G16R16F ) )
		{
			return false;
		}

		// Scaled version of the HDR scene texture
		if ( ! m_pSceneScaled->CreateRenderTarget( m_dwCropWidth / 4, m_dwCropHeight / 4, D3DFMT_A16B16G16R16F ) )
		{
			return false;
		}

		// Create the bright-pass filter texture.
		if ( ! m_pBrightTexture->CreateRenderTarget( m_dwCropWidth / 4 + 2, m_dwCropHeight / 4 + 2, D3DFMT_A8R8G8B8 ) )
		{
			return false;
		}

		// Create a texture to be used as the source for the star effect
		if ( ! m_pStarSource->CreateRenderTarget( m_dwCropWidth / 4 + 2, m_dwCropHeight / 4 + 2, D3DFMT_A8R8G8B8 ) )
		{
			return false;
		}

		// Create a texture to be used as the source for the bloom effect
		if ( ! m_pBloomSource->CreateRenderTarget( m_dwCropWidth / 8 + 2, m_dwCropHeight / 8 + 2, D3DFMT_A8R8G8B8 ) )
		{
			return false;
		}

		// Create a 2 textures to hold the luminance that the user is currently adapted
		if ( ! m_pCurLuminance->CreateRenderTarget( 1, 1, D3DFMT_R32F ) )
		{
			return false;
		}

		if ( ! m_pLastLuminance->CreateRenderTarget( 1, 1, D3DFMT_R32F ) )
		{
			return false;
		}
		
		// For each scale stage, create a texture to hold the intermediate results of the luminance calculation
		for( int i = 0; i < NUM_TONEMAP_TEXTURES; i++ )
		{
			int iSampleLen = 1 << ( 2 * i );

			if ( ! m_pToneMapTexture[i].CreateRenderTarget( iSampleLen, iSampleLen, D3DFMT_R32F ) )
			{
				return false;
			}
		}


		// Create the temporary blooming effect textures
		for( int i = 1; i < NUM_BLOOM_TEXTURES; i++ )
		{
			if ( ! m_pBloomTexture[i].CreateRenderTarget( m_dwCropWidth / 8 + 2, m_dwCropHeight / 8 + 2, D3DFMT_A8R8G8B8 ) )
			{
				return false;
			}
		}

		// Create the final blooming effect texture
		if ( ! m_pBloomTexture[0].CreateRenderTarget( m_dwCropWidth / 8, m_dwCropHeight / 8, D3DFMT_A8R8G8B8 ) )
		{
			return false;
		}

		// Create the star effect textures
		for( int i = 0; i < NUM_STAR_TEXTURES; i++ )
		{
			if ( ! m_pStarTexture[i].CreateRenderTarget( m_dwCropWidth / 4, m_dwCropHeight / 4, D3DFMT_A8R8G8B8 ) )
			{
				return false;
			}
		}

		// Textures with borders must be cleared since scissor rect testing will be used to avoid rendering on top of the border
		m_pCurLuminance->ClearTexture();
		m_pLastLuminance->ClearTexture();
		m_pBloomSource->ClearTexture();
		m_pBrightTexture->ClearTexture();
		m_pStarSource->ClearTexture();

		for( int i = 0; i < NUM_BLOOM_TEXTURES; i++ )
		{
			m_pBloomTexture[i].ClearTexture();
		}

		return true;
	}

	void    D3D9HDR::BuildParamMap()
	{
		// 建立参数表
		m_kProj.SetName( wstring( L"g_mProjection" ) );
		m_kBloomScale.SetName( wstring( L"g_fBloomScale" ) );
		m_kStarScale.SetName( wstring( L"g_fStarScale" ) );
		m_kMiddleGray.SetName( wstring( L"g_fMiddleGray" ) );
		m_kSampleOffsets.SetName( wstring( L"g_avSampleOffsets" ) );
		m_kSampleWeights.SetName( wstring( L"g_avSampleWeights" ) );
		m_kElapsedTime.SetName( wstring( L"g_fElapsedTime" ) );

		m_pEffect->AddParamHandle( Effect::Param_Matrix, m_kProj );
		m_pEffect->AddParamHandle( Effect::Param_Float, m_kBloomScale );
		m_pEffect->AddParamHandle( Effect::Param_Float, m_kStarScale );
		m_pEffect->AddParamHandle( Effect::Param_Float, m_kMiddleGray );
		m_pEffect->AddParamHandle( Effect::Param_Value, m_kSampleOffsets );
		m_pEffect->AddParamHandle( Effect::Param_Value, m_kSampleWeights );
		m_pEffect->AddParamHandle( Effect::Param_Float, m_kElapsedTime );

		// 建立科技表
		m_kFinalScenePass.SetName( wstring( L"FinalScenePass" ) );
		m_kDownScale4x4.SetName( wstring( L"DownScale4x4" ) );
		m_kBrightPassFilter.SetName( wstring( L"BrightPassFilter" ) );
		m_kGaussBlur5x5.SetName( wstring( L"GaussBlur5x5" ) );
		m_kDownScale2x2.SetName( wstring( L"DownScale2x2" ) );
		m_kSampleAvgLum.SetName( wstring( L"SampleAvgLum" ) );
		m_kResampleAvgLum.SetName( wstring( L"ResampleAvgLum" ) );
		m_kResampleAvgLumExp.SetName( wstring( L"ResampleAvgLumExp" ) );
		m_kCalculateAdaptedLum.SetName( wstring( L"CalculateAdaptedLum" ) );
		m_kStar.SetName( wstring( L"Star" ) );
		m_kBloom.SetName( wstring( L"Bloom" ) );
		m_kMergeTextures[0].SetName( wstring( L"MergeTextures_1" ) );
		m_kMergeTextures[1].SetName( wstring( L"MergeTextures_2" ) );
		m_kMergeTextures[2].SetName( wstring( L"MergeTextures_3" ) );
		m_kMergeTextures[3].SetName( wstring( L"MergeTextures_4" ) );
		m_kMergeTextures[4].SetName( wstring( L"MergeTextures_5" ) );
		m_kMergeTextures[5].SetName( wstring( L"MergeTextures_6" ) );
		m_kMergeTextures[6].SetName( wstring( L"MergeTextures_7" ) );
		m_kMergeTextures[7].SetName( wstring( L"MergeTextures_8" ) );

		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kFinalScenePass );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kDownScale4x4 );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kBrightPassFilter );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kGaussBlur5x5 );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kDownScale2x2 );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kSampleAvgLum );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kResampleAvgLum );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kResampleAvgLumExp );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kCalculateAdaptedLum );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kStar );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kBloom );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kMergeTextures[0] );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kMergeTextures[1] );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kMergeTextures[2] );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kMergeTextures[3] );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kMergeTextures[4] );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kMergeTextures[5] );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kMergeTextures[6] );
		m_pEffect->AddParamHandle( Effect::Param_Technique, m_kMergeTextures[7] );
	}

	bool    D3D9HDR::BeginPass( UINT uiPass )
	{
		return true;
	}

	bool    D3D9HDR::EndPass()
	{
		return true;
	}

	void    D3D9HDR::PostProcess()
	{
		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();
		LPDIRECT3DSURFACE9 pHDRSurfaceRT; // High dynamic range render surface
		LPDIRECT3DSURFACE9 pHDRSurfaceTex; // High dynamic range surface to store

		// 设置HDR渲染表面
		HRESULT hr;
		pD3D9Device->GetRenderTarget( 0, &pHDRSurfaceRT );
		m_pSceneTexture->GetImpliment()->GetSurfaceLevel( 0, &pHDRSurfaceTex );
		hr = pD3D9Device->StretchRect( pHDRSurfaceRT, NULL, pHDRSurfaceTex, NULL, D3DTEXF_NONE );
		hr = pD3D9Device->SetRenderTarget( 0, pHDRSurfaceTex );

		// Create a scaled copy of the scene
		Scene_To_SceneScaled();
		// Setup tone mapping technique
		MeasureLuminance();
		// Calculate the current luminance adaptation level
		CalculateAdaptation();
		// Now that luminance information has been gathered, the scene can be bright-pass filtered
		// to remove everything except bright lights and reflections.
		SceneScaled_To_BrightPass();
		// Blur the bright-pass filtered image to create the source texture for the star effect
		BrightPass_To_StarSource();
		// Scale-down the source texture for the star effect to create the source texture
		// for the bloom effect
		StarSource_To_BloomSource();
		// Render post-process lighting effects
		RenderBloom();
		RenderStar();

		// Draw the high dynamic range scene texture to the low dynamic range
		// back buffer. As part of this final pass, the scene will be tone-mapped
		// using the user's current adapted luminance, blue shift will occur
		// if the scene is determined to be very dark, and the post-process lighting
		// effect textures will be added to the scene.
		m_pEffect->SetTechnique( m_kFinalScenePass );

		float fMiddleGray = 0.5f;
		float fBloomScale = 1.0f;
		float fStarScale = 0.5f;
		m_pEffect->SetFloat( m_kMiddleGray, &fMiddleGray );
		m_pEffect->SetFloat( m_kBloomScale, &fBloomScale );
		m_pEffect->SetFloat( m_kStarScale, &fStarScale );

		( (D3D9RenderWindow *) RenderWindow::GetActiveRenderWindow() )->RestoreRenderTarget();
		pD3D9Device->SetTexture( 0, m_pSceneTexture->GetImpliment() );
		pD3D9Device->SetTexture( 1, m_pBloomTexture[0].GetImpliment() );
		pD3D9Device->SetTexture( 2, m_pStarTexture[0].GetImpliment() );
		pD3D9Device->SetTexture( 3, m_pCurLuminance->GetImpliment() );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pD3D9Device->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		pD3D9Device->SetSamplerState( 2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pD3D9Device->SetSamplerState( 2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		pD3D9Device->SetSamplerState( 3, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 3, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

		pD3D9Device->SetTexture( 1, NULL );
		pD3D9Device->SetTexture( 2, NULL );
		pD3D9Device->SetTexture( 3, NULL );

		SAFE_RELEASE( pHDRSurfaceRT );
		SAFE_RELEASE( pHDRSurfaceTex );
	}

	bool    D3D9HDR::Scene_To_SceneScaled()
	{
		D3DXVECTOR2 avSampleOffsets[MAX_SAMPLES];

		// Get the new render target surface
		LPDIRECT3DSURFACE9 pSurfScaledScene = NULL;
		m_pSceneScaled->GetImpliment()->GetSurfaceLevel( 0, &pSurfScaledScene );

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// 获取后备缓冲格式
		LPDIRECT3DSURFACE9 pBackBuffer;
		pD3D9Device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
		D3DSURFACE_DESC kBackBufferDesc;
		pBackBuffer->GetDesc( &kBackBufferDesc );
		SAFE_RELEASE( pBackBuffer );

		// Create a 1/4 x 1/4 scale copy of the HDR texture. Since bloom textures
		// are 1/8 x 1/8 scale, border texels of the HDR texture will be discarded 
		// to keep the dimensions evenly divisible by 8; this allows for precise 
		// control over sampling inside pixel shaders.
		m_pEffect->SetTechnique( m_kDownScale4x4 );

		// Place the rectangle in the center of the back buffer surface
		RECT rectSrc;
		rectSrc.left = ( kBackBufferDesc.Width - m_dwCropWidth ) / 2;
		rectSrc.top = ( kBackBufferDesc.Height - m_dwCropHeight ) / 2;
		rectSrc.right = rectSrc.left + m_dwCropWidth;
		rectSrc.bottom = rectSrc.top + m_dwCropHeight;

		// Get the texture coordinates for the render target
		CoordRect coords;
		GetTextureCoords( m_pSceneTexture->GetImpliment(), &rectSrc, m_pSceneScaled->GetImpliment(), NULL, &coords );

		// Get the sample offsets used within the pixel shader
		GetSampleOffsets_DownScale4x4( kBackBufferDesc.Width, kBackBufferDesc.Height, avSampleOffsets );
		m_pEffect->SetValue( m_kSampleOffsets, (void *) avSampleOffsets, sizeof(avSampleOffsets) );

		pD3D9Device->SetRenderTarget( 0, pSurfScaledScene );
		pD3D9Device->SetTexture( 0, m_pSceneTexture->GetImpliment() );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, coords.fLeftU, coords.fTopV, coords.fRightU, coords.fBottomV );

		SAFE_RELEASE( pSurfScaledScene );
		return true;
	}

	bool    D3D9HDR::SceneScaled_To_BrightPass()
	{
		D3DXVECTOR2 avSampleOffsets[MAX_SAMPLES];
		D3DXVECTOR4 avSampleWeights[MAX_SAMPLES];

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// Get the new render target surface
		LPDIRECT3DSURFACE9 pSurfBrightPass;
		m_pBrightTexture->GetImpliment()->GetSurfaceLevel( 0, &pSurfBrightPass );

		D3DSURFACE_DESC desc;
		m_pSceneScaled->GetImpliment()->GetLevelDesc( 0, &desc );

		// Get the rectangle describing the sampled portion of the source texture.
		// Decrease the rectangle to adjust for the single pixel black border.
		RECT rectSrc;
		GetTextureRect( m_pSceneScaled->GetImpliment(), &rectSrc );
		InflateRect( &rectSrc, -1, -1 );

		// Get the destination rectangle.
		// Decrease the rectangle to adjust for the single pixel black border.
		RECT rectDest;
		GetTextureRect( m_pBrightTexture->GetImpliment(), &rectDest );
		InflateRect( &rectDest, -1, -1 );

		// Get the correct texture coordinates to apply to the rendered quad in order 
		// to sample from the source rectangle and render into the destination rectangle
		CoordRect coords;
		GetTextureCoords( m_pSceneScaled->GetImpliment(), &rectSrc, m_pBrightTexture->GetImpliment(), &rectDest, &coords );

		// The bright-pass filter removes everything from the scene except lights and
		// bright reflections
		m_pEffect->SetTechnique( m_kBrightPassFilter );

		pD3D9Device->SetRenderTarget( 0, pSurfBrightPass );
		pD3D9Device->SetTexture( 0, m_pSceneScaled->GetImpliment() );
		pD3D9Device->SetTexture( 1, m_pCurLuminance->GetImpliment() );
//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
//		pD3D9Device->SetScissorRect( &rectDest );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, coords.fLeftU, coords.fTopV, coords.fRightU, coords.fBottomV );

//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
		SAFE_RELEASE( pSurfBrightPass );
		return true;
	}

	bool    D3D9HDR::BrightPass_To_StarSource()
	{
		D3DXVECTOR2 avSampleOffsets[MAX_SAMPLES];
		D3DXVECTOR4 avSampleWeights[MAX_SAMPLES];

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// Get the new render target surface
		LPDIRECT3DSURFACE9 pSurfStarSource;
		m_pStarSource->GetImpliment()->GetSurfaceLevel( 0, &pSurfStarSource );

		// Get the destination rectangle.
		// Decrease the rectangle to adjust for the single pixel black border.
		RECT rectDest;
		GetTextureRect( m_pStarSource->GetImpliment(), &rectDest );
		InflateRect( &rectDest, -1, -1 );

		// Get the correct texture coordinates to apply to the rendered quad in order 
		// to sample from the source rectangle and render into the destination rectangle
		CoordRect coords;
		GetTextureCoords( m_pBrightTexture->GetImpliment(), NULL, m_pStarSource->GetImpliment(), &rectDest, &coords );

		// Get the sample offsets used within the pixel shader
		D3DSURFACE_DESC desc;
		m_pBrightTexture->GetImpliment()->GetLevelDesc( 0, &desc );

		GetSampleOffsets_GaussBlur5x5( desc.Width, desc.Height, avSampleOffsets, avSampleWeights );
		m_pEffect->SetValue( m_kSampleOffsets, (void *) avSampleOffsets, sizeof(avSampleOffsets) );
		m_pEffect->SetValue( m_kSampleWeights, (void *) avSampleWeights, sizeof(avSampleWeights) );

		// The gaussian blur smooths out rough edges to avoid aliasing effects
		// when the star effect is run
		m_pEffect->SetTechnique( m_kGaussBlur5x5 );

		pD3D9Device->SetRenderTarget( 0, pSurfStarSource );
		pD3D9Device->SetTexture( 0, m_pBrightTexture->GetImpliment() );
//		pD3D9Device->SetScissorRect( &rectDest );
//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, coords.fLeftU, coords.fTopV, coords.fRightU, coords.fBottomV );

//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
		SAFE_RELEASE( pSurfStarSource );
		return true;
	}

	bool    D3D9HDR::StarSource_To_BloomSource()
	{
		D3DXVECTOR2 avSampleOffsets[MAX_SAMPLES];

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// Get the new render target surface
		LPDIRECT3DSURFACE9 pSurfBloomSource;
		m_pBloomTexture->GetImpliment()->GetSurfaceLevel( 0, &pSurfBloomSource );

		// Get the rectangle describing the sampled portion of the source texture.
		// Decrease the rectangle to adjust for the single pixel black border.
		RECT rectSrc;
		GetTextureRect( m_pStarSource->GetImpliment(), &rectSrc );
		InflateRect( &rectSrc, -1, -1 );

		// Get the destination rectangle.
		// Decrease the rectangle to adjust for the single pixel black border.
		RECT rectDest;
		GetTextureRect( m_pBloomSource->GetImpliment(), &rectDest );
		InflateRect( &rectDest, -1, -1 );

		// Get the correct texture coordinates to apply to the rendered quad in order 
		// to sample from the source rectangle and render into the destination rectangle
		CoordRect coords;
		GetTextureCoords( m_pStarSource->GetImpliment(), &rectSrc, m_pBloomSource->GetImpliment(), &rectDest, &coords );

		// Get the sample offsets used within the pixel shader
		D3DSURFACE_DESC desc;
		m_pBrightTexture->GetImpliment()->GetLevelDesc( 0, &desc );

		GetSampleOffsets_DownScale2x2( desc.Width, desc.Height, avSampleOffsets );
		m_pEffect->SetValue( m_kSampleOffsets, (void *) avSampleOffsets, sizeof(avSampleOffsets) );

		// Create an exact 1/2 x 1/2 copy of the source texture
		m_pEffect->SetTechnique( m_kDownScale2x2 );

		pD3D9Device->SetRenderTarget( 0, pSurfBloomSource );
		pD3D9Device->SetTexture( 0, m_pStarSource->GetImpliment() );
//		pD3D9Device->SetScissorRect( &rectDest );
//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, coords.fLeftU, coords.fTopV, coords.fRightU, coords.fBottomV );

//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
		SAFE_RELEASE( pSurfBloomSource );
		return true;
	}

	bool    D3D9HDR::MeasureLuminance()
	{
		int i, x, y, index;
		D3DXVECTOR2 avSampleOffsets[MAX_SAMPLES];
		DWORD dwCurTexture = NUM_TONEMAP_TEXTURES - 1;

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// Sample log average luminance
		LPDIRECT3DSURFACE9 apSurfToneMap[NUM_TONEMAP_TEXTURES] = {0};

		// Retrieve the tonemap surfaces
		for( i = 0; i < NUM_TONEMAP_TEXTURES; i++ )
		{
			m_pToneMapTexture[i].GetImpliment()->GetSurfaceLevel( 0, &apSurfToneMap[i] );
		}

		D3DSURFACE_DESC desc;
		m_pToneMapTexture[dwCurTexture].GetImpliment()->GetLevelDesc( 0, &desc );

		// Initialize the sample offsets for the initial luminance pass.
		float tU, tV;
		tU = 1.0f / ( 3.0f * desc.Width );
		tV = 1.0f / ( 3.0f * desc.Height );

		index = 0;
		for( x = -1; x <= 1; x++ )
		{
			for( y = -1; y <= 1; y++ )
			{
				avSampleOffsets[index].x = x * tU;
				avSampleOffsets[index].y = y * tV;

				index++;
			}
		}

		// After this pass, the g_apTexToneMap[NUM_TONEMAP_TEXTURES-1] texture will contain
		// a scaled, grayscale copy of the HDR scene. Individual texels contain the log 
		// of average luminance values for points sampled on the HDR texture.
		m_pEffect->SetTechnique( m_kSampleAvgLum );
		m_pEffect->SetValue( m_kSampleOffsets, (void *) avSampleOffsets, sizeof(avSampleOffsets) );

		pD3D9Device->SetRenderTarget( 0, apSurfToneMap[dwCurTexture] );
		pD3D9Device->SetTexture( 0, m_pSceneScaled->GetImpliment() );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		pD3D9Device->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		pD3D9Device->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );
		dwCurTexture--;

		// Initialize the sample offsets for the iterative luminance passes
		while( dwCurTexture > 0 )
		{
			m_pToneMapTexture[dwCurTexture + 1].GetImpliment()->GetLevelDesc( 0, &desc );
			GetSampleOffsets_DownScale4x4( desc.Width, desc.Height, avSampleOffsets );

			// Each of these passes continue to scale down the log of average
			// luminance texture created above, storing intermediate results in 
			// g_apTexToneMap[1] through g_apTexToneMap[NUM_TONEMAP_TEXTURES-1].
			m_pEffect->SetTechnique( m_kResampleAvgLum );
			m_pEffect->SetValue( m_kSampleOffsets, (void *)avSampleOffsets, sizeof(avSampleOffsets) );

			pD3D9Device->SetRenderTarget( 0, apSurfToneMap[dwCurTexture] );
			pD3D9Device->SetTexture( 0, m_pToneMapTexture[dwCurTexture + 1].GetImpliment() );
			pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
			pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
			RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

			dwCurTexture--;
		}

		// Downsample to 1x1
		m_pToneMapTexture[1].GetImpliment()->GetLevelDesc( 0, &desc );
		GetSampleOffsets_DownScale4x4( desc.Width, desc.Height, avSampleOffsets );

		// Perform the final pass of the average luminance calculation. This pass
		// scales the 4x4 log of average luminance texture from above and performs
		// an exp() operation to return a single texel cooresponding to the average
		// luminance of the scene in g_apTexToneMap[0].
		m_pEffect->SetTechnique( m_kResampleAvgLumExp );
		m_pEffect->SetValue( m_kSampleOffsets, (void *)avSampleOffsets, sizeof(avSampleOffsets) );

		pD3D9Device->SetRenderTarget( 0, apSurfToneMap[0] );
		pD3D9Device->SetTexture( 0, m_pToneMapTexture[1].GetImpliment() );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

		for( i=0; i < NUM_TONEMAP_TEXTURES; i++ )
		{
			SAFE_RELEASE( apSurfToneMap[i] );
		}

		return true;
	}

	bool    D3D9HDR::CalculateAdaptation()
	{
		HRESULT hr = S_OK;

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		// Swap current & last luminance
		D3D9Texture * pTexSwap = m_pLastLuminance;
		m_pLastLuminance = m_pCurLuminance;
		m_pCurLuminance = pTexSwap;

		LPDIRECT3DSURFACE9 pSurfAdaptedLum = NULL;
		m_pCurLuminance->GetImpliment()->GetSurfaceLevel( 0, &pSurfAdaptedLum );

		// This simulates the light adaptation that occurs when moving from a 
		// dark area to a bright area, or vice versa. The g_pTexAdaptedLuminance
		// texture stores a single texel cooresponding to the user's adapted 
		// level.
		m_pEffect->SetTechnique( m_kCalculateAdaptedLum );
		float fElapsedTime = RenderWindow::GetActiveRenderWindow()->GetElapsedTime();
		m_pEffect->SetFloat( m_kElapsedTime, &fElapsedTime );

		pD3D9Device->SetRenderTarget( 0, pSurfAdaptedLum );
		pD3D9Device->SetTexture( 0, m_pLastLuminance->GetImpliment() );
		pD3D9Device->SetTexture( 1, m_pToneMapTexture[0].GetImpliment() );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

		SAFE_RELEASE( pSurfAdaptedLum );
		return true;
	}

	bool    D3D9HDR::RenderStar()
	{
		int i, d, p, s; // Loop variables

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		LPDIRECT3DSURFACE9 pSurfStar = NULL;
		m_pStarTexture[0].GetImpliment()->GetSurfaceLevel( 0, &pSurfStar );

		// Clear the star texture
		pD3D9Device->ColorFill( pSurfStar, NULL, D3DCOLOR_ARGB( 0, 0, 0, 0 ) );
		SAFE_RELEASE( pSurfStar );

		// Avoid rendering the star if it's not being used in the current glare
		if ( m_pGlareDef->m_fGlareLuminance <= 0.0f ||
			m_pGlareDef->m_fStarLuminance <= 0.0f )
		{
			return true;
		}

		// Initialize the constants used during the effect
		const CStarDef& starDef = m_pGlareDef->m_starDef ;
		const float fTanFoV = atanf(D3DX_PI/8) ;
		const D3DXVECTOR4 vWhite( 1.0f, 1.0f, 1.0f, 1.0f );
		static const int s_maxPasses = 3 ;
		static const int nSamples = 8 ;
		static D3DXVECTOR4 s_aaColor[s_maxPasses][8] ;
		static const D3DXCOLOR s_colorWhite(0.63f, 0.63f, 0.63f, 0.0f) ;

		D3DXVECTOR4 avSampleWeights[MAX_SAMPLES];
		D3DXVECTOR2 avSampleOffsets[MAX_SAMPLES];

		pD3D9Device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE ) ;
		pD3D9Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE ) ;

		LPDIRECT3DSURFACE9 pSurfSource = NULL;
		LPDIRECT3DSURFACE9 pSurfDest = NULL;

		// Set aside all the star texture surfaces as a convenience
		LPDIRECT3DSURFACE9 apSurfStar[NUM_STAR_TEXTURES] = {0};
		for( i=0; i < NUM_STAR_TEXTURES; i++ )
		{
			m_pStarTexture[i].GetImpliment()->GetSurfaceLevel( 0, &apSurfStar[i] );
		}

		// Get the source texture dimensions
		m_pStarSource->GetImpliment()->GetSurfaceLevel( 0, &pSurfSource );
		D3DSURFACE_DESC desc;
		pSurfSource->GetDesc( &desc );
		SAFE_RELEASE( pSurfSource );

		float srcW;
		srcW = (FLOAT) desc.Width;
		float srcH;
		srcH= (FLOAT) desc.Height;

		for ( p = 0; p < s_maxPasses; p++ )
		{
			float ratio;
			ratio = (float)(p + 1) / (float)s_maxPasses ;

			for (s = 0 ; s < nSamples ; s ++)
			{
				D3DXCOLOR chromaticAberrColor ;
				D3DXColorLerp( &chromaticAberrColor,
					           &( CStarDef::GetChromaticAberrationColor(s) ),
					           &s_colorWhite,
					           ratio );
				D3DXColorLerp( (D3DXCOLOR*)&( s_aaColor[p][s] ),
					           &s_colorWhite, &chromaticAberrColor,
					           m_pGlareDef->m_fChromaticAberration );
			}
		}

		float radOffset;
		radOffset = m_pGlareDef->m_fStarInclination + starDef.m_fInclination ;

		LPDIRECT3DTEXTURE9 pTexSource;
		// Direction loop
		for (d = 0 ; d < starDef.m_nStarLines ; d ++)
		{
			const STARLINE& starLine = starDef.m_pStarLine[d];
			pTexSource = m_pStarSource->GetImpliment();

			float rad;
			rad = radOffset + starLine.fInclination ;
			float sn, cs;
			sn = sinf(rad), cs = cosf(rad) ;
			D3DXVECTOR2 vtStepUV;
			vtStepUV.x = sn / srcW * starLine.fSampleLength ;
			vtStepUV.y = cs / srcH * starLine.fSampleLength ;

			float attnPowScale;
			attnPowScale = ( fTanFoV + 0.1f ) * 1.0f * (160.0f + 120.0f) / ( srcW + srcH ) * 1.2f ;

			// 1 direction expansion loop
			pD3D9Device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ) ;

			int iWorkTexture;
			iWorkTexture = 1 ;
			for ( p = 0; p < starLine.nPasses; p ++ )
			{
				if ( p == starLine.nPasses - 1 )
				{
					// Last pass move to other work buffer
					pSurfDest = apSurfStar[d + 4];
				}
				else 
				{
					pSurfDest = apSurfStar[iWorkTexture];
				}

				// Sampling configration for each stage
				for ( i = 0; i < nSamples; i++ )
				{
					float lum;
					lum = powf( starLine.fAttenuation, attnPowScale * i );
					avSampleWeights[i] = s_aaColor[starLine.nPasses - 1 - p][i] * lum * ( p + 1.0f ) * 0.5f ;

					// Offset of sampling coordinate
					avSampleOffsets[i].x = vtStepUV.x * i;
					avSampleOffsets[i].y = vtStepUV.y * i;
					if ( fabs( avSampleOffsets[i].x ) >= 0.9f ||
   						 fabs( avSampleOffsets[i].y ) >= 0.9f )
					{
						avSampleOffsets[i].x = 0.0f ;
						avSampleOffsets[i].y = 0.0f ;
						avSampleWeights[i] *= 0.0f ;
					}

				}

				m_pEffect->SetTechnique( m_kStar );
				m_pEffect->SetValue( m_kSampleOffsets, avSampleOffsets, sizeof(avSampleOffsets) );

				Vector4f pvWeights[nSamples];
				for ( i = 0; i < nSamples; i++ )
				{
					pvWeights[i] = Vector4f( avSampleWeights[i][0], avSampleWeights[i][1], avSampleWeights[i][2], avSampleWeights[i][3] );
				}
				m_pEffect->SetVectorArray( m_kSampleWeights, pvWeights, nSamples );

				pD3D9Device->SetRenderTarget( 0, pSurfDest );
				pD3D9Device->SetTexture( 0, pTexSource );
				pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
				pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
				RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

				// Setup next expansion
				vtStepUV *= nSamples ;
				attnPowScale *= nSamples ;

				// Set the work drawn just before to next texture source.
				pTexSource = m_pStarTexture[iWorkTexture].GetImpliment();

				iWorkTexture += 1 ;
				if ( iWorkTexture > 2 ) 
				{
					iWorkTexture = 1 ;
				}
			}
		}

		pSurfDest = apSurfStar[0];
		for( i = 0; i < starDef.m_nStarLines; i++ )
		{
			pD3D9Device->SetTexture( i, m_pStarTexture[i + 4].GetImpliment() );
			pD3D9Device->SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			pD3D9Device->SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

			avSampleWeights[i] = vWhite * 1.0f / (FLOAT) starDef.m_nStarLines;
		}

    	m_pEffect->SetTechnique( m_kMergeTextures[starDef.m_nStarLines - 1] );

		Vector4f * pvWeights2;
		pvWeights2 = new Vector4f[starDef.m_nStarLines];
		for ( i = 0; i < starDef.m_nStarLines; i++ )
		{
			pvWeights2[i] = Vector4f( avSampleWeights[i][0], avSampleWeights[i][1], avSampleWeights[i][2], avSampleWeights[i][3] );
		}
		m_pEffect->SetVectorArray( m_kSampleWeights, pvWeights2, starDef.m_nStarLines );
		SAFE_DELETE_ARRAY( pvWeights2 );

		pD3D9Device->SetRenderTarget( 0, pSurfDest );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

		for( i=0; i < starDef.m_nStarLines; i++ )
		{
			pD3D9Device->SetTexture( i, NULL );
		}

		for( i = 0; i < NUM_STAR_TEXTURES; i++ )
		{
			SAFE_RELEASE( apSurfStar[i] );
		}

		return true;
	}

	bool    D3D9HDR::RenderBloom()
	{
		int i = 0;

		D3DXVECTOR2 avSampleOffsets[MAX_SAMPLES];
		float       afSampleOffsets[MAX_SAMPLES];
		D3DXVECTOR4 avSampleWeights[MAX_SAMPLES];

		// 获取D3D9设备指针
		LPDIRECT3DDEVICE9 pD3D9Device = ( ( D3D9RenderWindow * ) RenderWindow::GetActiveRenderWindow() )->GetDevice();

		LPDIRECT3DSURFACE9 pSurfScaledHDR;
		m_pSceneScaled->GetImpliment()->GetSurfaceLevel( 0, &pSurfScaledHDR );

		LPDIRECT3DSURFACE9 pSurfBloom;
		m_pBloomTexture[0].GetImpliment()->GetSurfaceLevel( 0, &pSurfBloom );

		LPDIRECT3DSURFACE9 pSurfHDR;
		m_pSceneTexture->GetImpliment()->GetSurfaceLevel( 0, &pSurfHDR );  

		LPDIRECT3DSURFACE9 pSurfTempBloom;
		m_pBloomTexture[1].GetImpliment()->GetSurfaceLevel( 0, &pSurfTempBloom );

		LPDIRECT3DSURFACE9 pSurfBloomSource;
		m_pBloomTexture[2].GetImpliment()->GetSurfaceLevel(0, &pSurfBloomSource);

		// Clear the bloom texture
		pD3D9Device->ColorFill( pSurfBloom, NULL, D3DCOLOR_ARGB(0, 0, 0, 0) );

		if ( m_pGlareDef->m_fGlareLuminance <= 0.0f ||
			 m_pGlareDef->m_fBloomLuminance <= 0.0f)
		{
			SAFE_RELEASE( pSurfBloomSource );
			SAFE_RELEASE( pSurfTempBloom );
			SAFE_RELEASE( pSurfBloom );
			SAFE_RELEASE( pSurfHDR );
			SAFE_RELEASE( pSurfScaledHDR );

			return true;
		}

		RECT rectSrc;
		GetTextureRect( m_pBloomSource->GetImpliment(), &rectSrc );
		InflateRect( &rectSrc, -1, -1 );

		RECT rectDest;
		GetTextureRect( m_pBloomTexture[2].GetImpliment(), &rectDest );
		InflateRect( &rectDest, -1, -1 );

		CoordRect coords;
		GetTextureCoords( m_pBloomSource->GetImpliment(), &rectSrc, m_pBloomTexture[2].GetImpliment(), &rectDest, &coords );

		D3DSURFACE_DESC desc;
		m_pBloomSource->GetImpliment()->GetLevelDesc( 0, &desc );

		m_pEffect->SetTechnique( m_kGaussBlur5x5 );
		GetSampleOffsets_GaussBlur5x5( desc.Width, desc.Height, avSampleOffsets, avSampleWeights, 1.0f );

		m_pEffect->SetValue( m_kSampleOffsets, avSampleOffsets, sizeof(avSampleOffsets) );
		m_pEffect->SetValue( m_kSampleWeights, avSampleWeights, sizeof(avSampleWeights) );

		pD3D9Device->SetRenderTarget( 0, pSurfBloomSource );
		pD3D9Device->SetTexture( 0, m_pBloomSource->GetImpliment() );
//		pD3D9Device->SetScissorRect( &rectDest );
//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );

		m_pBloomTexture[2].GetImpliment()->GetLevelDesc( 0, &desc );
		GetSampleOffsets_Bloom( desc.Width, afSampleOffsets, avSampleWeights, 3.0f, 2.0f );
		for( i = 0; i < MAX_SAMPLES; i++ )
		{
			avSampleOffsets[i] = D3DXVECTOR2( afSampleOffsets[i], 0.0f );
		}

		m_pEffect->SetTechnique( m_kBloom );
		m_pEffect->SetValue( m_kSampleOffsets, avSampleOffsets, sizeof(avSampleOffsets) );
		m_pEffect->SetValue( m_kSampleWeights, avSampleWeights, sizeof(avSampleWeights) );

		pD3D9Device->SetRenderTarget( 0, pSurfTempBloom );
		pD3D9Device->SetTexture( 0, m_pBloomTexture[2].GetImpliment() );
//		pD3D9Device->SetScissorRect( &rectDest );
//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

//		pD3D9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );

		m_pBloomTexture[1].GetImpliment()->GetLevelDesc( 0, &desc );
		GetSampleOffsets_Bloom( desc.Height, afSampleOffsets, avSampleWeights, 3.0f, 2.0f );
		for( i = 0; i < MAX_SAMPLES; i++ )
		{
			avSampleOffsets[i] = D3DXVECTOR2( 0.0f, afSampleOffsets[i] );
		}

		GetTextureRect( m_pBloomTexture[1].GetImpliment(), &rectSrc );
		InflateRect( &rectSrc, -1, -1 );
		GetTextureCoords( m_pBloomTexture[1].GetImpliment(), &rectSrc, m_pBloomTexture[0].GetImpliment(), NULL, &coords );

		m_pEffect->SetTechnique( m_kBloom );
		m_pEffect->SetValue( m_kSampleOffsets, avSampleOffsets, sizeof(avSampleOffsets) );
		m_pEffect->SetValue( m_kSampleWeights, avSampleWeights, sizeof(avSampleWeights) );

		pD3D9Device->SetRenderTarget( 0, pSurfBloom );
		pD3D9Device->SetTexture( 0, m_pBloomTexture[1].GetImpliment() );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pD3D9Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		RenderWindow::GetActiveRenderWindow()->GetRenderer()->RenderQuad( m_pEffect, 0.0f, 0.0f, 1.0f, 1.0f );

		SAFE_RELEASE( pSurfBloomSource );
		SAFE_RELEASE( pSurfTempBloom );
		SAFE_RELEASE( pSurfBloom );
		SAFE_RELEASE( pSurfHDR );
		SAFE_RELEASE( pSurfScaledHDR );

		return true;
	}

	bool    D3D9HDR::GetTextureRect( LPDIRECT3DTEXTURE9 pTexture, RECT * pRect )
	{
		if( pTexture == NULL || pRect == NULL )
		{
			return false;
		}

		D3DSURFACE_DESC desc;
		HRESULT hr = pTexture->GetLevelDesc( 0, &desc );

		if( FAILED( hr ) )
		{
			return false;
		}

		pRect->left = 0;
		pRect->top = 0;
		pRect->right = desc.Width;
		pRect->bottom = desc.Height;

		return true;
	}

	bool    D3D9HDR::GetTextureCoords( LPDIRECT3DTEXTURE9 pTexSrc, RECT * pRectSrc, LPDIRECT3DTEXTURE9 pTexDest, RECT * pRectDest, CoordRect * pCoords )
	{
		HRESULT hr;
		D3DSURFACE_DESC desc;
		float tU, tV;

		// Validate arguments
		if( pTexSrc == NULL || pTexDest == NULL || pCoords == NULL )
		{
			return false;
		}

		// Start with a default mapping of the complete source surface to complete 
		// destination surface
		pCoords->fLeftU = 0.0f;
		pCoords->fTopV = 0.0f;
		pCoords->fRightU = 1.0f; 
		pCoords->fBottomV = 1.0f;

		// If not using the complete source surface, adjust the coordinates
		if( pRectSrc != NULL )
		{
			// Get destination texture description
			hr = pTexSrc->GetLevelDesc( 0, &desc );
			if( FAILED( hr ) )
			{
				return false;
			}

			// These delta values are the distance between source texel centers in 
			// texture address space
			tU = 1.0f / desc.Width;
			tV = 1.0f / desc.Height;

			pCoords->fLeftU += pRectSrc->left * tU;
			pCoords->fTopV += pRectSrc->top * tV;
			pCoords->fRightU -= (desc.Width - pRectSrc->right) * tU;
			pCoords->fBottomV -= (desc.Height - pRectSrc->bottom) * tV;
		}

		// If not drawing to the complete destination surface, adjust the coordinates
		if( pRectDest != NULL )
		{
			// Get source texture description
			hr = pTexDest->GetLevelDesc( 0, &desc );
			if( FAILED( hr ) )
			{
				return false;
			}

			// These delta values are the distance between source texel centers in 
			// texture address space
			tU = 1.0f / desc.Width;
			tV = 1.0f / desc.Height;

			pCoords->fLeftU -= pRectDest->left * tU;
			pCoords->fTopV -= pRectDest->top * tV;
			pCoords->fRightU += (desc.Width - pRectDest->right) * tU;
			pCoords->fBottomV += (desc.Height - pRectDest->bottom) * tV;
		}

		return true;
	}

	float    D3D9HDR::GaussianDistribution( float x, float y, float rho )
	{
		float g = 1.0f / sqrtf( 2.0f * D3DX_PI * rho * rho );
		g *= expf( -(x*x + y*y)/(2*rho*rho) );

		return g;
	}

	bool    D3D9HDR::GetSampleOffsets_GaussBlur5x5( DWORD dwD3DTexWidth, DWORD dwD3DTexHeight, D3DXVECTOR2 * avTexCoordOffset, D3DXVECTOR4 * avSampleWeights, float fMultiplier )
	{
		float tu = 1.0f / (float)dwD3DTexWidth ;
		float tv = 1.0f / (float)dwD3DTexHeight ;

		D3DXVECTOR4 vWhite( 1.0f, 1.0f, 1.0f, 1.0f );

		float totalWeight = 0.0f;
		int index=0;
		for( int x = -2; x <= 2; x++ )
		{
			for( int y = -2; y <= 2; y++ )
			{
				// Exclude pixels with a block distance greater than 2. This will
				// create a kernel which approximates a 5x5 kernel using only 13
				// sample points instead of 25; this is necessary since 2.0 shaders
				// only support 16 texture grabs.
				if( abs(x) + abs(y) > 2 )
					continue;

				// Get the unscaled Gaussian intensity for this offset
				avTexCoordOffset[index] = D3DXVECTOR2( x * tu, y * tv );
				avSampleWeights[index] = vWhite * GaussianDistribution( (float)x, (float)y, 1.0f );
				totalWeight += avSampleWeights[index].x;

				index++;
			}
		}

		// Divide the current weight by the total weight of all the samples; Gaussian
		// blur kernels add to 1.0f to ensure that the intensity of the image isn't
		// changed when the blur occurs. An optional multiplier variable is used to
		// add or remove image intensity during the blur.
		for( int i=0; i < index; i++ )
		{
			avSampleWeights[i] /= totalWeight;
			avSampleWeights[i] *= fMultiplier;
		}

		return true;
	}

	bool    D3D9HDR::GetSampleOffsets_Bloom( DWORD dwD3DTexSize, float afTexCoordOffset[15], D3DXVECTOR4 * avColorWeight, float fDeviation, float fMultiplier )
	{
		int i=0;
		float tu = 1.0f / (float)dwD3DTexSize;

		// Fill the center texel
		float weight = fMultiplier * GaussianDistribution( 0, 0, fDeviation );
		avColorWeight[0] = D3DXVECTOR4( weight, weight, weight, 1.0f );

		afTexCoordOffset[0] = 0.0f;

		// Fill the first half
		for( i=1; i < 8; i++ )
		{
			// Get the Gaussian intensity for this offset
			weight = fMultiplier * GaussianDistribution( (float)i, 0, fDeviation );
			afTexCoordOffset[i] = i * tu;

			avColorWeight[i] = D3DXVECTOR4( weight, weight, weight, 1.0f );
		}

		// Mirror to the second half
		for( i=8; i < 15; i++ )
		{
			avColorWeight[i] = avColorWeight[i-7];
			afTexCoordOffset[i] = -afTexCoordOffset[i-7];
		}

		return true;
	}

	bool    D3D9HDR::GetSampleOffsets_Star(DWORD dwD3DTexSize, float afTexCoordOffset[15], D3DXVECTOR4 * avColorWeight, float fDeviation)
	{
		int i=0;
		float tu = 1.0f / (float)dwD3DTexSize;

		// Fill the center texel
		float weight = 1.0f * GaussianDistribution( 0, 0, fDeviation );
		avColorWeight[0] = D3DXVECTOR4( weight, weight, weight, 1.0f );

		afTexCoordOffset[0] = 0.0f;

		// Fill the first half
		for( i=1; i < 8; i++ )
		{
			// Get the Gaussian intensity for this offset
			weight = 1.0f * GaussianDistribution( (float)i, 0, fDeviation );
			afTexCoordOffset[i] = i * tu;

			avColorWeight[i] = D3DXVECTOR4( weight, weight, weight, 1.0f );
		}

		// Mirror to the second half
		for( i=8; i < 15; i++ )
		{
			avColorWeight[i] = avColorWeight[i-7];
			afTexCoordOffset[i] = -afTexCoordOffset[i-7];
		}

		return true;
	}

	bool    D3D9HDR::GetSampleOffsets_DownScale4x4( DWORD dwWidth, DWORD dwHeight, D3DXVECTOR2 avSampleOffsets[] )
	{
		if( NULL == avSampleOffsets )
		{
			return false;
		}

		float tU = 1.0f / dwWidth;
		float tV = 1.0f / dwHeight;

		// Sample from the 16 surrounding points. Since the center point will be in
		// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
		// center.
		int index=0;
		for( int y=0; y < 4; y++ )
		{
			for( int x=0; x < 4; x++ )
			{
				avSampleOffsets[ index ].x = (x - 1.5f) * tU;
				avSampleOffsets[ index ].y = (y - 1.5f) * tV;

				index++;
			}
		}

		return true;
	}

	bool    D3D9HDR::GetSampleOffsets_DownScale2x2( DWORD dwWidth, DWORD dwHeight, D3DXVECTOR2 avSampleOffsets[] )
	{
		if( NULL == avSampleOffsets )
		{
			return false;
		}

		float tU = 1.0f / dwWidth;
		float tV = 1.0f / dwHeight;

		// Sample from the 4 surrounding points. Since the center point will be in
		// the exact center of 4 texels, a 0.5f offset is needed to specify a texel
		// center.
		int index=0;
		for( int y=0; y < 2; y++ )
		{
			for( int x=0; x < 2; x++ )
			{
				avSampleOffsets[ index ].x = (x - 0.5f) * tU;
				avSampleOffsets[ index ].y = (y - 0.5f) * tV;

				index++;
			}
		}

		return true;
	}
}