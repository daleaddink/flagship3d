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

		// 重载等于
		Bone&                       operator = ( Bone& Right );

		// 更新骨骼
		void                        UpdateBone();

		// 获取实际位移
		Vector3f&                   GetAbsTrans() { return m_vAbsTrans; }

		// 获取实际旋转
		Quaternionf&                GetAbsRot() { return m_qAbsRot; }

		// 获取变换矩阵
		Matrix4f&                   GetTransformMatrix() { return m_matTransform; }

		// 获取逆变换矩阵
		Matrix4f&                   GetInverseMatrix() { return m_matInv; }

	public:
		// 骨骼基本数据
		wstring				        m_szName;
		int	        			    m_iBoneID;
		int							m_iParentID;
		vector< int >               m_pChild;

		// 骨骼变换数据
		Vector3f					m_vRelTrans;
		Quaternionf				    m_qRelRot;
		Matrix4f					m_matInv;
		Quaternionf				    m_qInvRot;

		// 骨骼限制数据
		Vector3f					m_vRotLimitMin;
		Vector3f					m_vRotLimitMax;
		bool						m_pAxisLocked[4];

		// 实际变换数据
		Vector3f            		m_vAbsTrans;
		Quaternionf                 m_qAbsRot;
		Matrix4f                    m_matTransform;

		// 实际树结构
		Bone *                      m_pParentBone;
		vector< Bone * >            m_pChildBone;
	};

	class _DLL_Export Skeleton : public Resource
	{
	public:
		Skeleton();
		virtual ~Skeleton();

		// 重载等于
		Skeleton&         operator = ( Skeleton& Right );

		// 更新动画
		void              UpdateAnimation( Animation * pAnimation, int iKeyFrame, float fDeltaTime, float fAlpha = 1.0f );

		// 更新骨骼
		void              UpdateBody();

		// 获取骨头总数
		int               GetBoneNum();

		// 通过ID获取骨头
		Bone *            GetBone( int iBoneID );

		// 获取根骨头总数
		int               GetRootBoneNum();

		// 通过索引获取根骨头
		int               GetRootBoneID ( int iIndex );

		// 设置缩放比例
		void              SetScale( float scaling ) { m_fScale = scaling; }

		// 获取缩放比例
		float             GetScale() { return m_fScale; }

	protected:
		// 骨头列表
		vector<Bone *>    m_pBone;

		// 根骨头ID列表
		vector<int>       m_pRootBoneID;

		// 缩放比例
		float             m_fScale;

	private:

	};
}

#endif
