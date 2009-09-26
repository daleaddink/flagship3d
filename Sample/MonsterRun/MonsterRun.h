#ifndef _MONSTERRUN_H_
#define _MONSTERRUN_H_

#include "../../FrameWork/DemoFrame/DemoApp.h"
#include "../../FrameWork/DemoFrame/FreeCamera.h"
#include "../../Framework/DemoFrame/Graphic.h"
#include "../../Graphic/Main/Logic.h"

using namespace Flagship;

class MonsterRun : public DemoApp
{
public:
	MonsterRun();
	virtual ~MonsterRun();

protected:
	
private:

};

class MonsterRunLogic : public Logic
{
public:
	MonsterRunLogic();
	virtual ~MonsterRunLogic();

public:
	// 初始化
	virtual void        Initialize();

	// 逻辑函数
	virtual bool        LogicTick();

	// 消息处理
	virtual bool        MessageProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
	// 摄像机
	FreeCamera *        m_pFreeCamera;

	Mesh *              m_pMonsterMesh;
	Mesh *              m_pStoneMesh;
	Mesh *              m_pBoxMesh;
	TerrainTile *       m_pTerrainTile;
	Material *          m_pSkyBox;
	Material *          m_pTerrain;
	Material *          m_pThinFlim;
	Material *          m_pBasic;
	Material *          m_pHDR;

	Animation           m_kRunAnim;
	Skeleton            m_kSkeleton;
	Entity *            m_pSkyEntity;
	Entity *            m_pStoneEntity;
	Entity *            m_pTerrainEntity;
	vector<AnimEntity *>    m_pThinFlimEntity;
	vector<AnimationTask *>    m_pRunTask;
	Light *                 m_pSunLight[2];

	UINT                m_uiKeyDown;
	Vector2f            m_kvPoint;
	bool                m_bMonsterRun;
	int                 m_iEntityCount;
	
private:

};

#endif