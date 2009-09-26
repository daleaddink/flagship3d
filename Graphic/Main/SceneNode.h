#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include <Common/Prerequisites.h>
#include "Renderable.h"
#include "BoundingBox.h"

namespace Flagship
{
	class Entity;
	class Culler;
	struct SceneNodeData;

	class _DLL_Export SceneNode : public Renderable
	{
	public:
		SceneNode( wstring szName = L"" );
		virtual ~SceneNode();

		// ���ð�Χ��
		void                      SetBound( BoundingBox * pBound );

		// ���ø��ڵ�
		void                      SetParent( SceneNode * pParent );

		// ��ȡ���ڵ�
		SceneNode *               GetParent();

		// ��ȡ�ӽڵ��б�
		map< Key, SceneNode * > * GetChildMap();

		// ����ӽڵ�
		void                      AttechChild( SceneNode * pChild );

		// ����ӽڵ�
		void                      ClearChild();

		// ���°�Χ��
		void                      UpdateBound( Vector2f& vOffset );

		// ��ȡʵ���б�
		map< Key, Entity * > *    GetEntityMap() { return &m_pEntityMap; }

	public:
		// ���ʵ��
		virtual void              AttechEntity( Entity * pEntity ) {}

		// ɾ��ʵ��
		virtual bool              DetachEntity( Entity * pEntity ) { return true; }

	protected:
		// ��������
		SceneNodeData *           m_pSceneNodeData;

		// ���ڵ�
		SceneNode *               m_pParentNode;

		// �ӽڵ��б�
		map< Key, SceneNode * >   m_pChildMap;

		// ʵ���б�
		map< Key, Entity * >      m_pEntityMap;

		// ���������
		int                       m_iCullNum;

	private:
	};
}

#endif