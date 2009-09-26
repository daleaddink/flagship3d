#ifndef _D3D10EFFECT_H_
#define _D3D10EFFECT_H_

#include "D3D10Prerequisites.h"
#include "../Main/Effect.h"

namespace Flagship
{
	class _DLL_Export D3D10Effect : public Effect
	{
	public:
		D3D10Effect();
		virtual ~D3D10Effect();

		// 获取技术
		ID3D10EffectTechnique *                GetTechnique() { return m_pCurTechnique; }

	public:
		// 从资源创建效果
		virtual bool                           CreateFromMemory();

		// 添加句柄
		virtual void                           AddParamHandle( BYTE byType, Key& szName );

		// 设置数据
		virtual bool                           SetValue( Key& szParam, void * pValue, UINT uiSize );

		// 设置浮点数据
		virtual bool                           SetInt( Key& szParam, int * piValue );

		// 设置浮点数据
		virtual bool                           SetFloat( Key& szParam, float * pfValue );

		// 设置向量数据
		virtual bool                           SetVector( Key& szParam, Vector4f * pvValue );

		// 设置向量数组
		virtual bool                           SetVectorArray( Key& szParam, Vector4f * pvValue, UINT uiNum );

		// 设置矩阵数据
		virtual bool                           SetMatrix( Key& szParam, Matrix4f * pmatValue );

		// 设置矩阵数组
		virtual bool                           SetMatrixArray( Key& szParam, Matrix4f * pmatValue, UINT uiNum );

		// 设置贴图数据
		virtual bool                           SetTexture( Key& szParam, Texture * pTexture );

		// 设置技术
		virtual bool                           SetTechnique( Key& szParam );

		// 开始效果
		virtual bool                           Begin( UINT * uiPassNum );

		// 结束效果
		virtual bool                           End();

		// 开始渲染
		virtual bool                           BeginPass( UINT uiPass );

		// 结束渲染
		virtual bool                           EndPass();

	protected:
		// 缓存资源
		virtual bool                           Cache();

		// 释放缓存
		virtual void                           UnCache();

	protected:
		// D3D9效果对象
		ID3D10Effect *                         m_pD3D10Effect;

		// 参数句柄列表
		map< Key, ID3D10EffectVariable * >     m_mParamHandleMap;

		// 科技句柄列表
		map< Key, ID3D10EffectTechnique * >    m_mTechHandleMap;

	private:
		//  当前科技
		ID3D10EffectTechnique *                m_pCurTechnique;

	};

	typedef ResHandle<D3D10Effect> D3D10EffectHandle;
}

#endif