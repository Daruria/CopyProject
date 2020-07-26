#include "stdafx.h"
#include "Scene.h"
#include "Obj.h"

CScene::CScene()
{
}


CScene::~CScene()
{
}

void CScene::ObjectListClear()
{

	list<CObj*>* pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}

	pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER_BOSS);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}

	pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER_BULLET);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}

	pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_MONSTER_PARTICLE);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}

	pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_FIELD_ITEM);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}

	pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_PLAYER_BULLET);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}

	pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_TRESURE_BOX);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}

	pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_FIELD_OBJECT);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}

	pObjList = GET_SINGLE(CObjectManager)->GetObjList(OT_NPC);
	if (!(pObjList->empty()))
	{
		for (auto& pObj : *pObjList)
			pObj->SetLife(false);
	}
}
