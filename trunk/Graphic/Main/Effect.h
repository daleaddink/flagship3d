#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <Common/Prerequisites.h>
#include "Resource.h"

namespace Flagship
{
	class Texture;
	class RenderTexture;

	class _DLL_Export Effect : public Resource
	{
	public:
		Effect();
		virtual ~Effect();

		enum
		{
			Param_Value,
			Param_Int,
			Param_Float,
			Param_Vector,
			Param_VectorArray,
			Param_Matrix,
			Param_MatrixArray,
			Param_Texture,
			Param_Technique,
			Param_Max,
		};
		
	public:
		// 从资源创建效果
		virtual bool    CreateFromMemory() = 0;

		// 添加句柄
		virtual void    AddParamHandle( BYTE byType, Key& szName ) = 0;

		// 设置数据
		virtual bool    SetValue( Key& szParam, void * pValue, UINT uiSize ) = 0;

		// 设置浮点数据
		virtual bool    SetInt( Key& szParam, int * piValue ) = 0;

		// 设置浮点数据
		virtual bool    SetFloat( Key& szParam, float * pfValue ) = 0;

		// 设置向量数据
		virtual bool    SetVector( Key& szParam, Vector4f * pvValue ) = 0;

		// 设置向量数组
		virtual bool    SetVectorArray( Key& szParam, Vector4f * pvValue, UINT uiNum ) = 0;

		// 设置矩阵数据
		virtual bool    SetMatrix( Key& szParam, Matrix4f * pmatValue ) = 0;

		// 设置矩阵数组
		virtual bool    SetMatrixArray( Key& szParam, Matrix4f * pmatValue, UINT uiNum ) = 0;

		// 设置贴图数据
		virtual bool    SetTexture( Key& szParam, Texture * pTexture ) = 0;

		// 设置技术
		virtual bool    SetTechnique( Key& szParam ) = 0;

		// 开始效果
		virtual bool    Begin( UINT * uiPassNum ) = 0;

		// 结束效果
		virtual bool    End() = 0;

		// 开始渲染
		virtual bool    BeginPass( UINT uiPass ) = 0;

		// 结束渲染
		virtual bool    EndPass() = 0;

	protected:

	private:

	};
}

#endif