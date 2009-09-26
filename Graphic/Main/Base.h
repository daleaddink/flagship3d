#ifndef _BASE_H_
#define _BASE_H_

#include <Common/Prerequisites.h>

namespace Flagship
{
	class _DLL_Export Key
	{
	public:
		Key();
		Key( wstring& szName );
		virtual ~Key();

		// ��������
		void           SetName( wstring& szName );

		// ��ȡ����
		wstring        GetName() { return m_szKey; }

		// ����С��
		friend bool    _DLL_Export operator < ( const Key& Left, const Key& Right );

		// �������
		bool           operator == ( const Key& Right );

		// ���ص���
		Key&           operator = ( const Key& Right );

	public:
		// �ַ���
		wstring        m_szKey;

		// Hashֵ
		DWORD          m_dwHash;
	};

	class _DLL_Export Base
	{
	public:
		Base();
		virtual ~Base();

		enum
		{
			Class_Renderable,

			Class_Material,
			Material_General,
			Material_RenderTexture,
			Material_PostProcess,

			Class_Entity,
			Entity_Light,
			Entity_AnimEntity,
			Entity_Max,

			Class_Portal,

			Class_Resource,
			Resource_Mesh,
			Mesh_XMesh,
			Mesh_PMFMesh,
			Mesh_SDKMesh,
			Mesh_Terrain,
			Resource_Texture,
			Texture_General,
			Texture_Line,
			Texture_Volume,
			Texture_Cube,
			Texture_Max,
			Resource_Effect,
			Resource_Skeleton,
			Resource_Animation,
			Resource_Max,

			Class_Camera,
			Camera_Max,

			Class_RenderTarget,
			RenderTarget_Window,
			RenderTarget_Texture,
			RenderTarget_Max,

			Class_SceneNode,
			SceneNode_Cell,
			SceneNode_Sector,
			SceneNode_Max,

			Class_Max,
		};

		// ��ȡ����
		Key&            GetKey();

		// ��ȡ��������
		int             GetClassType();

	public:
		// ��������
		virtual void    SetName( wstring& szName );

		// �Զ���������
		virtual void    AutoName();

	protected:

		// �����
		Key             m_szKey;

		// ��������
		int             m_iClassType;

		// ���Ʊ�
		static map< Key, Base * >    m_mKeyMap;
		
	private:

	};
}

#endif