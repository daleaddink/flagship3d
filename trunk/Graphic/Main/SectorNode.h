#ifndef _SECTORNODE_H_
#define _SECTORNODE_H_

#include <Common/Prerequisites.h>
#include "Cell.h"

namespace Flagship
{
	class Portal;
	class Entity;
	class Culler;
	struct SectorNodeData;

	class _DLL_Export SectorNode : public Cell
	{
	public:
		SectorNode( wstring szName = L"" );
		virtual ~SectorNode();

		// ����BSPƽ����
		void                      SetBSPlane( Plane3f& rkBSPlane );

		// ��ȡBSPƽ����
		Plane3f *                 GetBSPlane() { return &m_kBSPlane; }

		// ��ȡ����б�
		map< Key, Portal * > *    GetPortalMap();

	public:
		// �����������
		virtual void              SetWorldMatrix( Matrix4f& matWorldMatrix );

		// ���ʵ��
		virtual void              AttechEntity( Entity * pEntity );

		// ɾ��ʵ��
		virtual bool              DetachEntity( Entity * pEntity );

	protected:
		// ��������
		SectorNodeData *          m_pSectorNodeData;

		// ����б�
		map< Key, Portal * >      m_pPortalMap;

		// BSPƽ��
		Plane3f                   m_kBSPlane;

	private:
	};
}

#endif