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

		// 获得场景管理器指针
		static SceneManager *   GetSingleton();

		// 设置线程数据
		void                    SetThreadInfo( HANDLE hThread, DWORD dwThreadID );

		// 获取线程句柄
		HANDLE                  GetThreadHandle() { return m_hThread; }

		// 获取线程ID
		DWORD                   GetThreadID() { return m_dwThreadID; }

		// 建立场景
		void                    BuildScene( int iSizeX, int iSizeZ, int iHeight );

		// 预加载当前单元资源
		bool                    PreLoad( Camera * pCamera );

		// 设置剪裁器
		void                    SetCuller( SceneCuller * pSceneCuller, LightCuller * pLightCuller );

		// 遍历场景树
		void                    TravelScene();

		// 通过坐标获取单元
		Cell *                  GetCell( Vector3f& vPos );

		// 添加实体
		void                    AddEntity( Entity * pEntity, Matrix4f& matWorld, bool bInit = false );

		// 删除实体
		void                    DelEntity( Entity * pEntity );

		// 消息处理
		virtual bool            MessageProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		// 分割场景递归函数
		SceneNode *             _SplitScene( int iSize, int iPosX, int iPosZ );

		// 更新场景
		void                    _UpdateScene();

		// 查找最佳单元
		Cell *                  _FindCell( Cell * pParent, Vector3f& vPos );

		// 加载新单元函数
		bool                    _ResourceTravel( Vector2f& vPos );

		// 场景树递归函数
		void                    _VisibleTravel( SceneNode * pSceneNode, Renderer * pRenderer );

		// 遮挡剪裁函数
		void                    _CollideTravel( Renderer * pRenderer );

		// 光源递归函数
		void                    _LightTravel( SceneNode * pSceneNode, Renderer * pRenderer, Light * pLight );

		// 光照遮挡剪裁函数
		void                    _BrightTravel( Renderer * pRenderer, Light * pLight );

	protected:
		// 私有构造函数
		SceneManager();

		// 静态场景管理器指针
		static SceneManager *   m_pSceneManager;

		// 线程信息
		HANDLE                  m_hThread;
		DWORD                   m_dwThreadID;

		// 场景单元
		Cell **                 m_ppCell;

		// 场景树根节点
		SceneNode *             m_pRootNode;

		// 当前摄像机节点
		map< Vector2f, SceneNode * >    m_mLeafNode;

		// 场景剪裁器指针
		SceneCuller *           m_pSceneCuller;

		// 光源剪裁器指针
		LightCuller *           m_pLightCuller;

		// 当前帧
		DWORD                   m_dwCurFrame;

		// 高度限制
		int                     m_iHeight;

		// 当前位置索引
		int                     m_iCurIndexX;
		int                     m_iCurIndexZ;

		// 场景大小
		int                     m_iSizeX;
		int                     m_iSizeZ;

	private:
		// 节点名
		wstring                 m_szNodeName;
	};
}

#endif