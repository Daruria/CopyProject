#include "stdafx.h"
#include "ObjectManager.h"
#include "Obj.h"
#include "MoveObj.h"
#include "Player.h"
#include "PlayerUI.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "TileManager.h"

DEFINITION_SINGLE(CObjectManager);

CObjectManager::CObjectManager()
{
}


CObjectManager::~CObjectManager()
{
	Release();
}

void CObjectManager::Initialize()
{
	for (int i = 0; i < OT_END;++i)
	{
		m_ObjListArr[i] = new list<CObj*>;
	}
}

int CObjectManager::Update()
{
	for (int i = 0; i < OT_END; ++i)
	{
		auto iter = (*m_ObjListArr[i]).begin();

		for (; iter != (*m_ObjListArr[i]).end();)
		{
			if (!((*iter)->IsDie()))
			{
				(*iter)->Update();
				++iter;
			}
			else
			{
				SAFE_DELETE((*iter));
				iter = (*m_ObjListArr[i]).erase(iter);
			}
		}
	}
	CCollisionManager::CollisionPlayerToMonster(m_ObjListArr[OT_PLAYER], m_ObjListArr[OT_MONSTER]);
	CCollisionManager::CollisionPlayerToMonster(m_ObjListArr[OT_PLAYER], m_ObjListArr[OT_MONSTER_BOSS]);

	CCollisionManager::CollisionBulletToPlayer(m_ObjListArr[OT_MONSTER_BULLET], m_ObjListArr[OT_PLAYER]);
	CCollisionManager::CollisionBulletToMonster(m_ObjListArr[OT_PLAYER_BULLET], m_ObjListArr[OT_MONSTER]);
	CCollisionManager::CollisionBulletToMonster(m_ObjListArr[OT_PLAYER_BULLET], m_ObjListArr[OT_MONSTER_BOSS]);
	CCollisionManager::CollisionBulletToTile(m_ObjListArr[OT_MONSTER_BULLET], GET_SINGLE(CTileManager)->GetpVecTile());
	CCollisionManager::CollisionBulletToTile(m_ObjListArr[OT_PLAYER_BULLET], GET_SINGLE(CTileManager)->GetpVecTile());

	CCollisionManager::CollisionObjectToTile(m_ObjListArr[OT_PLAYER], GET_SINGLE(CTileManager)->GetpVecTile());
	CCollisionManager::CollisionObjectToTile(m_ObjListArr[OT_MONSTER], GET_SINGLE(CTileManager)->GetpVecTile());
	CCollisionManager::CollisionObjectToTile(m_ObjListArr[OT_MONSTER_PARTICLE], GET_SINGLE(CTileManager)->GetpVecTile());
	CCollisionManager::CollisionObjectToTile(m_ObjListArr[OT_FIELD_ITEM], GET_SINGLE(CTileManager)->GetpVecTile());
	
	CCollisionManager::CollisionPlayerToFieldObject(m_ObjListArr[OT_PLAYER], m_ObjListArr[OT_FIELD_OBJECT]);
	CCollisionManager::CollisionPlayerToTresureBox(m_ObjListArr[OT_PLAYER], m_ObjListArr[OT_TRESURE_BOX]);
	CCollisionManager::CollisionPlayerToItem(m_ObjListArr[OT_PLAYER], m_ObjListArr[OT_FIELD_ITEM]);
	CCollisionManager::CollisionPlayerToNPC(m_ObjListArr[OT_PLAYER], m_ObjListArr[OT_NPC]);

	GET_SINGLE(CPlayerUI)->ValueUpdate((*m_ObjListArr[OT_PLAYER]).front());

	return 0;
}

void CObjectManager::LateUpdate()
{
	for (int i = 0; i < OT_END; ++i)
	{
		for (auto& pObj : *m_ObjListArr[i])
		{
			if(!(pObj->IsDie()))
				pObj->LateUpdate();
		}
	}
}

void CObjectManager::Render(HDC hDC)
{
	for (int i = 0; i < OT_END; ++i)
	{
		for (auto& pObj : *m_ObjListArr[i])
		{
			if (!(pObj->IsDie()))
				pObj->Render(hDC);
		}
	}
}

void CObjectManager::Release()
{
	for (int i = 0; i < OT_END; ++i)
	{
		for (auto& pObj : *m_ObjListArr[i])
		{
			SAFE_DELETE(pObj);
		}
		SAFE_DELETE(m_ObjListArr[i]);
	}
}

void CObjectManager::AddObj(OBJECT_TYPE eType, CObj * pObj)
{
	(*m_ObjListArr[eType]).push_back(pObj);
}

void CObjectManager::AddMonster(CObj * pObj, float fx, float fy)
{
	pObj->Initialize();
	pObj->SetInfo(fx, fy);

	GET_SINGLE(CEffectManager)->AddEffect("MagicCircle", L"Image/Effect/MagicCircle/MagicCircleFX.bmp", fx, fy, 124, 124, 15);
	(*m_ObjListArr[OT_MONSTER]).push_back(pObj);
}

CPlayer * CObjectManager::GetPlayer()
{
	return dynamic_cast<CPlayer*>((*m_ObjListArr[OT_PLAYER]).front());
}
