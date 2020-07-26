#include "stdafx.h"
#include "CollisionFunctions.h"

#include "Export_Function.h"
#include "Transform.h"

CCollisionFunctions::CCollisionFunctions()
{

}

CCollisionFunctions::~CCollisionFunctions()
{

}

HRESULT CCollisionFunctions::Ready_Functions()
{
	Engine::Add_CollisionFunction(L"Player_Attack", L"MapObject",	CCollisionFunctions::CollisionExample);
	Engine::Add_CollisionFunction(L"Player_Attack", L"Monster_Body", CCollisionFunctions::CollisionExample);
	//Engine::Add_CollisionFunction(L"Player_Body",	L"MapObject", CCollisionFunctions::CollisionExample);
	Engine::Add_CollisionFunction(L"Player_Body",	L"HealingObject", CCollisionFunctions::CollisionExample);
	Engine::Add_CollisionFunction(L"Monster_Attack", L"Player_Body", CCollisionFunctions::CollisionExample);

	Engine::Add_CollisionFunction(L"Player_Attack", L"WoodPlatform", CCollisionFunctions::CollisionExample);

	//Engine::Add_CollisionFunction(L"MapObject", L"Player_Body", CCollisionFunctions::CollisionMapObject);
	//Engine::Add_CollisionFunction(L"MapObject", L"Monster_Body", CCollisionFunctions::CollisionMapObject);

	return S_OK;
}

void CCollisionFunctions::CollisionExample(const wstring& wstrSrcTag, const wstring& wstrDestTag)
{
	list<Engine::CGameObject*>* pSrcList = Engine::Get_CollisionObjectList(wstrSrcTag);
	list<Engine::CGameObject*>* pDestList = Engine::Get_CollisionObjectList(wstrDestTag);

	if (nullptr == pSrcList || nullptr == pDestList)
		return;

	for (auto& pSrc : *pSrcList)
	{
		Engine::CCollider* pSrcCol = DC_ENG(CCollider, pSrc->Get_Component(Engine::ID_DYNAMIC, L"Com_Collider"));
		NULL_CHECK(pSrcCol);
		_bool	bSrcDie = false;
		for (auto& pDest : *pDestList)
		{
			Engine::CCollider* pDestCol = DC_ENG(CCollider, pDest->Get_Component(Engine::ID_DYNAMIC, L"Com_Collider"));
			NULL_CHECK(pDestCol);

			_vec3 vPush;
			Engine::COLLISIONTYPE eCollisionType = pSrcCol->Check_CollisionType(pDest, Engine::CCollisionMgr::Check_Collision(pSrcCol, pDestCol, &vPush, 0));

			switch (eCollisionType)
			{
			case Engine::COLL_ENTER:
#ifdef _DEBUG
				pSrcCol->Set_Color(Engine::CC_PURPLE);
				pDestCol->Set_Color(Engine::CC_PURPLE);
#endif // _DEBUG
				pSrc->Collision_Enter(pDest);
				pDest->Collision_Enter(pSrc);
				break;
			case Engine::COLL_STAY:
#ifdef _DEBUG
				pSrcCol->Set_Color(Engine::CC_RED);
				pDestCol->Set_Color(Engine::CC_RED);
#endif // _DEBUG
				break;
			case Engine::COLL_LEAVE:
#ifdef _DEBUG
				pSrcCol->Set_Color(Engine::CC_YELLOW);
				pDestCol->Set_Color(Engine::CC_YELLOW);
#endif // _DEBUG
				break;
			case Engine::COLL_AWAY:

				break;
			}

			//if (Engine::CCollisionMgr::Check_Collision(pSrcCol, pDestCol))
			//{
			//	pSrcCol->Set_Color(Engine::CC_RED);
			//	pDestCol->Set_Color(Engine::CC_RED);
			//}
		}
	}
}

void CCollisionFunctions::CollisionMapObject(const wstring & wstrSrcTag, const wstring & wstrDestTag)
{
	list<Engine::CGameObject*>* pSrcList = Engine::Get_CollisionObjectList(wstrSrcTag);
	list<Engine::CGameObject*>* pDestList = Engine::Get_CollisionObjectList(wstrDestTag);

	if (nullptr == pSrcList || nullptr == pDestList)
		return;

	for (auto& pSrc : *pSrcList)
	{
		Engine::CCollider* pSrcCol = DC_ENG(CCollider, pSrc->Get_Component(Engine::ID_DYNAMIC, L"Com_Collider"));
		NULL_CHECK(pSrcCol);
		_bool	bSrcDie = false;
		for (auto& pDest : *pDestList)
		{
			Engine::CCollider* pDestCol = DC_ENG(CCollider, pDest->Get_Component(Engine::ID_DYNAMIC, L"Com_Collider"));
			NULL_CHECK(pDestCol);

			_vec3 vPush;

			Engine::CTransform* pDestTrans = nullptr;
			Engine::COLLISIONTYPE eCollisionType = pSrcCol->Check_CollisionType(pDest, Engine::CCollisionMgr::Check_Collision(pSrcCol, pDestCol, &vPush, 0));

			switch (eCollisionType)
			{
			case Engine::COLL_ENTER:
#ifdef _DEBUG
				pSrcCol->Set_Color(Engine::CC_PURPLE);
				pDestCol->Set_Color(Engine::CC_PURPLE);
#endif // _DEBUG
				pDestTrans = DC_ENG(CTransform, pDest->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
				NULL_CHECK(pDestTrans);
				pDestTrans->Move_Pos(vPush);
				break;
			case Engine::COLL_STAY:
#ifdef _DEBUG
				pSrcCol->Set_Color(Engine::CC_RED);
				pDestCol->Set_Color(Engine::CC_RED);
#endif // _DEBUG
				pDestTrans = DC_ENG(CTransform, pDest->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
				NULL_CHECK(pDestTrans);
				pDestTrans->Move_Pos(vPush);
				break;
			case Engine::COLL_LEAVE:
#ifdef _DEBUG
				pSrcCol->Set_Color(Engine::CC_YELLOW);
				pDestCol->Set_Color(Engine::CC_YELLOW);
#endif // _DEBUG
				break;
			case Engine::COLL_AWAY:

				break;
			}

			//if (Engine::CCollisionMgr::Check_Collision(pSrcCol, pDestCol))
			//{
			//	pSrcCol->Set_Color(Engine::CC_RED);
			//	pDestCol->Set_Color(Engine::CC_RED);
			//}
		}
	}
}
