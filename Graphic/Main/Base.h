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

		// 设置名称
		void           SetName( wstring& szName );

		// 获取名称
		wstring        GetName() { return m_szKey; }

		// 重载小于
		friend bool    _DLL_Export operator < ( const Key& Left, const Key& Right );

		// 重载相等
		bool           operator == ( const Key& Right );

		// 重载等于
		Key&           operator = ( const Key& Right );

	public:
		// 字符串
		wstring        m_szKey;

		// Hash值
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

		// 获取名称
		Key&            GetKey();

		// 获取对象类型
		int             GetClassType();

	public:
		// 设置名称
		virtual void    SetName( wstring& szName );

		// 自动设置名称
		virtual void    AutoName();

	protected:

		// 对象键
		Key             m_szKey;

		// 对象类型
		int             m_iClassType;

		// 名称表
		static map< Key, Base * >    m_mKeyMap;
		
	private:

	};
}

#endif