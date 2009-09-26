#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <Common/Prerequisites.h>
#include "Renderable.h"
#include "BoundingBox.h"

namespace Flagship
{
	class Renderer;
	class Cell;
	class Light;
	class Resource;
	class Mesh;
	struct EntityData;
	
	class _DLL_Export Entity : public Renderable
	{
	public:
		Entity( wstring szName = L"" );
		virtual ~Entity();

		// 设置包围体
		void                       SetOuterBound( BoundingVolume * pBound );

		// 设置遮挡体
		void                       SetInnerBound( Mesh * pCullMesh );

		// 获取遮挡体
		Mesh *                     GetInnerBound() { return m_pCullMesh; }

    	// 更新场景位置
		void                       UpdateScene();

		// 更新最新帧
		void                       UpdateFrame();

		// 获取材质类型
		int                        GetMaterialType();

		// 设置父场景节点
		void                       SetParent( Cell * pParent );

		// 获取父场景节点
		Cell *                     GetParent();

		// 添加资源
		void                       AttachResource( Resource * pResource, Material * pMaterial );

		// 获取最后渲染帧
		DWORD                      GetLastFrame() { return m_dwLastFrame; }

		// 设置光照状态
		void                       SetAffectByLight( bool bAffect ) { m_bAffectByLight = bAffect; }

		// 获取光照状态
		bool                       IsAffectByLight() { return m_bAffectByLight; }

		// 设置阴影状态
		void                       SetCastShadow( bool bCast ) { m_bCastShadow = bCast; }

		// 获取阴影状态
		bool                       IsCastShadow() { return m_bCastShadow; }

		// 设置阴影状态
		void                       SetReceiveShadow( bool bCast ) { m_bReceiveShadow = bCast; }

		// 获取阴影状态
		bool                       IsReceiveShadow() { return m_bReceiveShadow; }

	public:
		// 设置世界矩阵
		virtual void               SetWorldMatrix( Matrix4f& matWorldMatrix );

		// 渲染实体
		virtual void               Render( Renderer * pRenderer, BYTE byTechType );

		// 是否就绪
		virtual bool               IsReady();

		// 从磁盘读取资源
		virtual bool               Load();

		// 释放资源
		virtual bool               Release();

	protected:
		// 遮挡剪裁模型
		Mesh *                     m_pCullMesh;

		// 父场景节点
		Cell *                     m_pParentCell;

		// 本地包围体
		BoundingVolume *           m_pLocalBound;

		// 最后渲染帧
		DWORD                      m_dwLastFrame;

		// 材质列表
		map<Key, Material *>       m_pMaterialMap;

		// 资源列表
		list< Resource * >         m_pResourceList;

		// 是否受光照影响
		bool                       m_bAffectByLight;

		// 是否产生阴影
		bool                       m_bCastShadow;

		// 是否接受阴影
		bool                       m_bReceiveShadow;

	private:
		// 共享数据
		EntityData *               m_pEntityData;
	};
}

#endif