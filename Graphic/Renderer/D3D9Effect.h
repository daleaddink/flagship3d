#ifndef _D3D9EFFECT_H_
#define _D3D9EFFECT_H_

#include "D3D9Prerequisites.h"
#include "../Main/Effect.h"

namespace Flagship
{
	class _DLL_Export D3D9Effect : public Effect
	{
	public:
		D3D9Effect();
		virtual ~D3D9Effect();

	public:
		// ����Դ����Ч��
		virtual bool              CreateFromMemory();

		// ��Ӿ��
		virtual void              AddParamHandle( BYTE byType, Key& szName );

		// ��������
		virtual bool              SetValue( Key& szParam, void * pValue, UINT uiSize );

		// ���ø�������
		virtual bool              SetInt( Key& szParam, int * piValue );

		// ���ø�������
		virtual bool              SetFloat( Key& szParam, float * pfValue );

		// ������������
		virtual bool              SetVector( Key& szParam, Vector4f * pvValue );

		// ������������
		virtual bool              SetVectorArray( Key& szParam, Vector4f * pvValue, UINT uiNum );

		// ���þ�������
		virtual bool              SetMatrix( Key& szParam, Matrix4f * pmatValue );

		// ���þ�������
		virtual bool              SetMatrixArray( Key& szParam, Matrix4f * pmatValue, UINT uiNum );

		// ������ͼ����
		virtual bool              SetTexture( Key& szParam, Texture * pTexture );

		// ���ü���
		virtual bool              SetTechnique( Key& szParam );

		// ��ʼЧ��
		virtual bool              Begin( UINT * uiPassNum );

		// ����Ч��
		virtual bool              End();

		// ��ʼ��Ⱦ
		virtual bool              BeginPass( UINT uiPass );

		// ������Ⱦ
		virtual bool              EndPass();

	protected:
		// ������Դ
		virtual bool              Cache();

		// �ͷŻ���
		virtual void              UnCache();

	protected:
		// D3D9Ч������
		LPD3DXEFFECT              m_pD3D9Effect;

		// ��������б�
		map< Key, D3DXHANDLE >    m_mParamHandleMap;

		// �Ƽ�����б�
		map< Key, D3DXHANDLE >    m_mTechHandleMap;

	private:

	};

	typedef ResHandle<D3D9Effect> D3D9EffectHandle;
}

#endif