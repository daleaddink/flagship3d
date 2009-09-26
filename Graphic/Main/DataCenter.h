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

		// 获得数据中心指针
		static DataCenter *    GetSingleton();

		// 进入临界区
		bool                   EnterMutex();

		// 离开临界区
		void                   LeaveMutex();

		// 注册数据对象
		void *                 Register( Key& szKey, int iClassType );

		// 取消注册数据对象
		void                   UnRegister( Key& szKey );

		// 设置当前帧数据
		void                   SetSharedData( Renderable * pRenderable, int iClassType );

		// 获取共享整数数据
		IntData *              GetIntData( Renderable * pRenderable, int iClassType, int iDataType );

		// 获取共享浮点数数据
		FloatData *            GetFloatData( Renderable * pRenderable, int iClassType, int iDataType );

		// 获取共享向量数据
		VectorData *           GetVectorData( Renderable * pRenderable, int iClassType, int iDataType );

		// 获取共享矩阵数据
		MatrixData *           GetMatrixData( Renderable * pRenderable, int iClassType, int iDataType );

		// 获取共享矩阵队列数据
		MatrixArrayData *      GetMatrixArrayData( Renderable * pRenderable, int iClassType, int iDataType );		

	protected:
		// 数据集
		map< Key, void * >     m_mData;

		// 临界区
		CRITICAL_SECTION       m_csMutex;
	
	private:
		DataCenter();

		static DataCenter *    m_pDataCenter;
	};
}

#endif