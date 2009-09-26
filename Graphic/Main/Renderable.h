#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include <Common/Prerequisites.h>
#include "Base.h"

namespace Flagship
{
	class Material;
	class BoundingVolume;

	class _DLL_Export Renderable : public Base
	{
	public:
		Renderable();
		virtual ~Renderable();

		// 获取包围体
		BoundingVolume *    GetBound() { return m_pBound; }

		// 获取注册数据
		void *              GetSharedData() { return m_pSharedData; }

	public:
		// 设置名称
		virtual void        SetName( wstring& szName );

		// 自动设置名称
		virtual void        AutoName();

		// 设置世界矩阵
		virtual void        SetWorldMatrix( Matrix4f& matWorldMatrix ) {}

		// 是否就绪
		virtual bool        IsReady() { return false; }

		// 读取资源
		virtual bool        Load() { return true; }

		// 释放资源
		virtual bool        Release() { return true; }

	protected:
		// 包围体
		BoundingVolume *    m_pBound;

		// 共享数据
		void *              m_pSharedData;

	private:
	};
}

#endif