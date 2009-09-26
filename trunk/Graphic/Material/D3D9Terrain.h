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

		// ���ص���
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

		// ���õ�������
		void                       SetTerrainInfo( TerrainInfo& kInfo ) { m_kTerrainInfo = kInfo; }

	public:
		// ��ʼ��
		virtual bool               Initialize();

		// ����������
		virtual void               BuildParamMap();

		// ���²��ʲ���
		virtual void               Update( Renderable * pParent );

		// ���²��ʲ���
		virtual void               Update( Resource * pParent );

		// �Ƿ����
		virtual bool               IsReady();

		// ��ȡ��Դ
		virtual bool               Load();

		// �ͷ���Դ
		virtual bool               Release();

	protected:
		// ��������
		TerrainInfo                m_kTerrainInfo;

		// �߳�ͼ
		D3D9Texture *              m_pHeightMap;

		// ��������ͼ
		D3D9Texture *              m_pTexcoordMap;

		// ����ͼ
		D3D9Texture *              m_pNormalMap;

		// ������ֵ
		Key                        m_kWorldMatrix;
		Key                        m_kHeightMap;
		Key                        m_kTexcoordMap;
		Key                        m_kNormalMap;

	private:

	};
}

#endif