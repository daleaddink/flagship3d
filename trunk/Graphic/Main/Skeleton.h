#ifndef _SKELETON_H_
#define _SKELETON_H_

#include <Common/Prerequisites.h>
#include "Resource.h"

namespace Flagship
{
	class Animation;

	class _DLL_Export Bone
	{
	public:
		Bone();
		virtual ~Bone();

		// ���ص���
		Bone&                       operator = ( Bone& Right );

		// ���¹���
		void                        UpdateBone();

		// ��ȡʵ��λ��
		Vector3f&                   GetAbsTrans() { return m_vAbsTrans; }

		// ��ȡʵ����ת
		Quaternionf&                GetAbsRot() { return m_qAbsRot; }

		// ��ȡ�任����
		Matrix4f&                   GetTransformMatrix() { return m_matTransform; }

		// ��ȡ��任����
		Matrix4f&                   GetInverseMatrix() { return m_matInv; }

	public:
		// ������������
		wstring				        m_szName;
		int	        			    m_iBoneID;
		int							m_iParentID;
		vector< int >               m_pChild;

		// �����任����
		Vector3f					m_vRelTrans;
		Quaternionf				    m_qRelRot;
		Matrix4f					m_matInv;
		Quaternionf				    m_qInvRot;

		// ������������
		Vector3f					m_vRotLimitMin;
		Vector3f					m_vRotLimitMax;
		bool						m_pAxisLocked[4];

		// ʵ�ʱ任����
		Vector3f            		m_vAbsTrans;
		Quaternionf                 m_qAbsRot;
		Matrix4f                    m_matTransform;

		// ʵ�����ṹ
		Bone *                      m_pParentBone;
		vector< Bone * >            m_pChildBone;
	};

	class _DLL_Export Skeleton : public Resource
	{
	public:
		Skeleton();
		virtual ~Skeleton();

		// ���ص���
		Skeleton&         operator = ( Skeleton& Right );

		// ���¶���
		void              UpdateAnimation( Animation * pAnimation, int iKeyFrame, float fDeltaTime, float fAlpha = 1.0f );

		// ���¹���
		void              UpdateBody();

		// ��ȡ��ͷ����
		int               GetBoneNum();

		// ͨ��ID��ȡ��ͷ
		Bone *            GetBone( int iBoneID );

		// ��ȡ����ͷ����
		int               GetRootBoneNum();

		// ͨ��������ȡ����ͷ
		int               GetRootBoneID ( int iIndex );

		// �������ű���
		void              SetScale( float scaling ) { m_fScale = scaling; }

		// ��ȡ���ű���
		float             GetScale() { return m_fScale; }

	protected:
		// ��ͷ�б�
		vector<Bone *>    m_pBone;

		// ����ͷID�б�
		vector<int>       m_pRootBoneID;

		// ���ű���
		float             m_fScale;

	private:

	};
}

#endif
