#ifndef _DATACENTER_H_
#define _DATACENTER_H_

#include <Common/Prerequisites.h>
#include <Common/Thread.h>
#include "DataDefine.h"
#include "Base.h"

namespace Flagship
{
	class Renderable;

	class _DLL_Export DataCenter
	{
	public:
		virtual ~DataCenter();

		// �����������ָ��
		static DataCenter *    GetSingleton();

		// �����ٽ���
		bool                   EnterMutex();

		// �뿪�ٽ���
		void                   LeaveMutex();

		// ע�����ݶ���
		void *                 Register( Key& szKey, int iClassType );

		// ȡ��ע�����ݶ���
		void                   UnRegister( Key& szKey );

		// ���õ�ǰ֡����
		void                   SetSharedData( Renderable * pRenderable, int iClassType );

		// ��ȡ������������
		IntData *              GetIntData( Renderable * pRenderable, int iClassType, int iDataType );

		// ��ȡ������������
		FloatData *            GetFloatData( Renderable * pRenderable, int iClassType, int iDataType );

		// ��ȡ������������
		VectorData *           GetVectorData( Renderable * pRenderable, int iClassType, int iDataType );

		// ��ȡ�����������
		MatrixData *           GetMatrixData( Renderable * pRenderable, int iClassType, int iDataType );

		// ��ȡ��������������
		MatrixArrayData *      GetMatrixArrayData( Renderable * pRenderable, int iClassType, int iDataType );		

	protected:
		// ���ݼ�
		map< Key, void * >     m_mData;

		// �ٽ���
		CRITICAL_SECTION       m_csMutex;
	
	private:
		DataCenter();

		static DataCenter *    m_pDataCenter;
	};
}

#endif