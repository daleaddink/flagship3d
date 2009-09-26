#ifndef _GLOBALPARAM_H_
#define _GLOBALPARAM_H_

#include <Common/Prerequisites.h>
#include "DataDefine.h"

namespace Flagship
{
	class Material;
	class Light;
	class ShadowMap;
	class RenderPass;

	class _DLL_Export GlobalParam
	{
	public:
		virtual ~GlobalParam();

		// ���ȫ�ֲ���ָ��
		static GlobalParam *   GetSingleton();

	public:
		// ��ǰȫ�ֲ���
		Material *              m_pGlobalMaterial;

		// ��Ӱ����
		Material *              m_pMakeShadow;

		// ��Ӱ����
		Material *              m_pPostShadow;

		// ���������
		Material *              m_pAmbient;

		// ��ǰ��Դ
		Light *                 m_pCurLight;

		// ��ǰ��Ӱͼ
		ShadowMap *             m_pCurShadowMap;

		// ��Ⱦ����
		RenderPass *            m_pRenderPass;

	public:
		// ȫ�ֻ�����
		VectorData              m_vAmbientColor;

	protected:
		// ˽�й���
		GlobalParam();

		// ��̬����������ָ��
		static GlobalParam *    m_pGlobalParam;

	private:

	};
}

#endif
