#ifndef _D3D9TERRAIN_H_
#define _D3D9TERRAIN_H_

#include "../Renderer/D3D9Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class D3D9Texture;

	struct TerrainInfo
	{
		wstring    szHeightPath;
		wstring    szTexcoordPath;
		wstring    szNormalPath;

		TerrainInfo::TerrainInfo()
		{
			szHeightPath = L"";
			szTexcoordPath = L"";
			szNormalPath = L"";
		}

		// 重载等于
		TerrainInfo&    operator = ( const TerrainInfo& Right )
		{
			szHeightPath = Right.szHeightPath;
			szTexcoordPath = Right.szTexcoordPath;
			szNormalPath = Right.szNormalPath;

			return *this;
		}
	};

	class _DLL_Export D3D9Terrain : public Material
	{
	public:
		D3D9Terrain();
		virtual ~D3D9Terrain();

		// 设置地形数据
		void                       SetTerrainInfo( TerrainInfo& kInfo ) { m_kTerrainInfo = kInfo; }

	public:
		// 初始化
		virtual bool               Initialize();

		// 建立参数表
		virtual void               BuildParamMap();

		// 更新材质参数
		virtual void               Update( Renderable * pParent );

		// 更新材质参数
		virtual void               Update( Resource * pParent );

		// 是否就绪
		virtual bool               IsReady();

		// 读取资源
		virtual bool               Load();

		// 释放资源
		virtual bool               Release();

	protected:
		// 地形数据
		TerrainInfo                m_kTerrainInfo;

		// 高程图
		D3D9Texture *              m_pHeightMap;

		// 纹理坐标图
		D3D9Texture *              m_pTexcoordMap;

		// 法线图
		D3D9Texture *              m_pNormalMap;

		// 参数键值
		Key                        m_kWorldMatrix;
		Key                        m_kHeightMap;
		Key                        m_kTexcoordMap;
		Key                        m_kNormalMap;

	private:

	};
}

#endif