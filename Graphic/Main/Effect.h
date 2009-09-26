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
		// ����Դ����Ч��
		virtual bool    CreateFromMemory() = 0;

		// ��Ӿ��
		virtual void    AddParamHandle( BYTE byType, Key& szName ) = 0;

		// ��������
		virtual bool    SetValue( Key& szParam, void * pValue, UINT uiSize ) = 0;

		// ���ø�������
		virtual bool    SetInt( Key& szParam, int * piValue ) = 0;

		// ���ø�������
		virtual bool    SetFloat( Key& szParam, float * pfValue ) = 0;

		// ������������
		virtual bool    SetVector( Key& szParam, Vector4f * pvValue ) = 0;

		// ������������
		virtual bool    SetVectorArray( Key& szParam, Vector4f * pvValue, UINT uiNum ) = 0;

		// ���þ�������
		virtual bool    SetMatrix( Key& szParam, Matrix4f * pmatValue ) = 0;

		// ���þ�������
		virtual bool    SetMatrixArray( Key& szParam, Matrix4f * pmatValue, UINT uiNum ) = 0;

		// ������ͼ����
		virtual bool    SetTexture( Key& szParam, Texture * pTexture ) = 0;

		// ���ü���
		virtual bool    SetTechnique( Key& szParam ) = 0;

		// ��ʼЧ��
		virtual bool    Begin( UINT * uiPassNum ) = 0;

		// ����Ч��
		virtual bool    End() = 0;

		// ��ʼ��Ⱦ
		virtual bool    BeginPass( UINT uiPass ) = 0;

		// ������Ⱦ
		virtual bool    EndPass() = 0;

	protected:

	private:

	};
}

#endif