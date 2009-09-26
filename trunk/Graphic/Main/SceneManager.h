#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include <Common/Prerequisites.h>

namespace Flagship
{
	class Camera;
	class SceneNode;
	class Cell;
	class SectorNode;
	class SceneCuller;
	class Entity;
	class Light;
	class AmbientLight;
	class DirectionalLight;
	class LightCuller;
	class RenderQueue;
	class Renderer;

	class _DLL_Export SceneManager
	{
	public:
		virtual ~SceneManager();

		// ��ó���������ָ��
		static SceneManager *   GetSingleton();

		// �����߳�����
		void                    SetThreadInfo( HANDLE hThread, DWORD dwThreadID );

		// ��ȡ�߳̾��
		HANDLE                  GetThreadHandle() { return m_hThread; }

		// ��ȡ�߳�ID
		DWORD                   GetThreadID() { return m_dwThreadID; }

		// ��������
		void                    BuildScene( int iSizeX, int iSizeZ, int iHeight );

		// Ԥ���ص�ǰ��Ԫ��Դ
		bool                    PreLoad( Camera * pCamera );

		// ���ü�����
		void                    SetCuller( SceneCuller * pSceneCuller, LightCuller * pLightCuller );

		// ����������
		void                    TravelScene();

		// ͨ�������ȡ��Ԫ
		Cell *                  GetCell( Vector3f& vPos );

		// ���ʵ��
		void                    AddEntity( Entity * pEntity, Matrix4f& matWorld, bool bInit = false );

		// ɾ��ʵ��
		void                    DelEntity( Entity * pEntity );

		// ��Ϣ����
		virtual bool            MessageProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		// �ָ���ݹ麯��
		SceneNode *             _SplitScene( int iSize, int iPosX, int iPosZ );

		// ���³���
		void                    _UpdateScene();

		// ������ѵ�Ԫ
		Cell *                  _FindCell( Cell * pParent, Vector3f& vPos );

		// �����µ�Ԫ����
		bool                    _ResourceTravel( Vector2f& vPos );

		// �������ݹ麯��
		void                    _VisibleTravel( SceneNode * pSceneNode, Renderer * pRenderer );

		// �ڵ����ú���
		void                    _CollideTravel( Renderer * pRenderer );

		// ��Դ�ݹ麯��
		void                    _LightTravel( SceneNode * pSceneNode, Renderer * pRenderer, Light * pLight );

		// �����ڵ����ú���
		void                    _BrightTravel( Renderer * pRenderer, Light * pLight );

	protected:
		// ˽�й��캯��
		SceneManager();

		// ��̬����������ָ��
		static SceneManager *   m_pSceneManager;

		// �߳���Ϣ
		HANDLE                  m_hThread;
		DWORD                   m_dwThreadID;

		// ������Ԫ
		Cell **                 m_ppCell;

		// ���������ڵ�
		SceneNode *             m_pRootNode;

		// ��ǰ������ڵ�
		map< Vector2f, SceneNode * >    m_mLeafNode;

		// ����������ָ��
		SceneCuller *           m_pSceneCuller;

		// ��Դ������ָ��
		LightCuller *           m_pLightCuller;

		// ��ǰ֡
		DWORD                   m_dwCurFrame;

		// �߶�����
		int                     m_iHeight;

		// ��ǰλ������
		int                     m_iCurIndexX;
		int                     m_iCurIndexZ;

		// ������С
		int                     m_iSizeX;
		int                     m_iSizeZ;

	private:
		// �ڵ���
		wstring                 m_szNodeName;
	};
}

#endif