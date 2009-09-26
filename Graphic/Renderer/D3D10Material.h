#ifndef _D3D10MATERIAL_H_
#define _D3D10MATERIAL_H_

#include "D3D10Prerequisites.h"
#include "../Main/Material.h"

namespace Flagship
{
	class _DLL_Export D3D10Material : public Material
	{
	public:
		D3D10Material();
		~D3D10Material();

		// ��ȡ��ǰ�Ƽ�
		ID3D10EffectTechnique *    GetTechnique();

		// ��ȡ��ǰ�Ƽ���ֵ
		Key&                       GetTechKey();

	public:
		// ���ò��ʰ汾
		virtual bool           SetTechnique( Renderable * pParent, BYTE byTechType );

		// ���²��ʽڵ����
		virtual void               Update( Renderable * pParent );

		// ���²�����Դ����
		virtual void               Update( Resource * pParent );

		// ��ȡ��Դ
		virtual bool               Load();

		// �ͷ���Դ
		virtual bool               Release();

	private:
		// ��ǰ�Ƽ���ֵ
		Key                        m_kTechKey;

	};
}

#endif