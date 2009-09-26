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

		// ���ð�Χ��
		void                       SetOuterBound( BoundingVolume * pBound );

		// �����ڵ���
		void                       SetInnerBound( Mesh * pCullMesh );

		// ��ȡ�ڵ���
		Mesh *                     GetInnerBound() { return m_pCullMesh; }

    	// ���³���λ��
		void                       UpdateScene();

		// ��������֡
		void                       UpdateFrame();

		// ��ȡ��������
		int                        GetMaterialType();

		// ���ø������ڵ�
		void                       SetParent( Cell * pParent );

		// ��ȡ�������ڵ�
		Cell *                     GetParent();

		// �����Դ
		void                       AttachResource( Resource * pResource, Material * pMaterial );

		// ��ȡ�����Ⱦ֡
		DWORD                      GetLastFrame() { return m_dwLastFrame; }

		// ���ù���״̬
		void                       SetAffectByLight( bool bAffect ) { m_bAffectByLight = bAffect; }

		// ��ȡ����״̬
		bool                       IsAffectByLight() { return m_bAffectByLight; }

		// ������Ӱ״̬
		void                       SetCastShadow( bool bCast ) { m_bCastShadow = bCast; }

		// ��ȡ��Ӱ״̬
		bool                       IsCastShadow() { return m_bCastShadow; }

		// ������Ӱ״̬
		void                       SetReceiveShadow( bool bCast ) { m_bReceiveShadow = bCast; }

		// ��ȡ��Ӱ״̬
		bool                       IsReceiveShadow() { return m_bReceiveShadow; }

	public:
		// �����������
		virtual void               SetWorldMatrix( Matrix4f& matWorldMatrix );

		// ��Ⱦʵ��
		virtual void               Render( Renderer * pRenderer, BYTE byTechType );

		// �Ƿ����
		virtual bool               IsReady();

		// �Ӵ��̶�ȡ��Դ
		virtual bool               Load();

		// �ͷ���Դ
		virtual bool               Release();

	protected:
		// �ڵ�����ģ��
		Mesh *                     m_pCullMesh;

		// �������ڵ�
		Cell *                     m_pParentCell;

		// ���ذ�Χ��
		BoundingVolume *           m_pLocalBound;

		// �����Ⱦ֡
		DWORD                      m_dwLastFrame;

		// �����б�
		map<Key, Material *>       m_pMaterialMap;

		// ��Դ�б�
		list< Resource * >         m_pResourceList;

		// �Ƿ��ܹ���Ӱ��
		bool                       m_bAffectByLight;

		// �Ƿ������Ӱ
		bool                       m_bCastShadow;

		// �Ƿ������Ӱ
		bool                       m_bReceiveShadow;

	private:
		// ��������
		EntityData *               m_pEntityData;
	};
}

#endif