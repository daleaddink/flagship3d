#ifndef _PORTAL_H_
#define _PORTAL_H_

#include <Common/Prerequisites.h>
#include "Renderable.h"

namespace Flagship
{
	class  SceneNode;
	struct PortalData;
	
	class _DLL_Export Portal : public Renderable
	{
	public:
		Portal( wstring& szName, BoundingVolume * pBound = NULL, Matrix4f * pWorld = NULL );
		virtual ~Portal();

		// ���ÿ���״̬
		void                           SetOpen( bool bOpen );

		// ��ȡ����״̬
		bool                           GetOpen();

		// ���ӳ����ڵ�
		void                           LinkSceneNode( SceneNode * pInnerNode, SceneNode * pOuterNode );

		// ��ȡ����ڵ�
		SceneNode *                    GetOppositeNode( SceneNode * pNode );

	public:
		// �����������
		virtual void                   SetWorldMatrix( Matrix4f& matWorldMatrix );

	protected:
		// ��������
		PortalData *                   m_pPortalData;

		//  ����״̬
		bool                           m_bOpen;

		// �ڲ��ڵ�
		SceneNode *                    m_pInnerNode;

		// �ⲿ�ڵ�
		SceneNode *                    m_pOuterNode;
		
	private:

	};
}

#endif