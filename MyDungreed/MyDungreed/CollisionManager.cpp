#include "stdafx.h"
#include "CollisionManager.h"
#include "Obj.h"
#include "Tile.h"
#include "MoveObj.h"
#include "Player.h"
#include "Monster.h"
#include "Bullet.h"
#include "FieldItem.h"
#include "Item.h"
#include "GoldCoin.h"
#include "TresureBox.h"
#include "FieldObject.h"
#include "Thorn.h"
#include "NPC.h"
#include "Merchant.h"
#include "Weapon.h"

CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::CollisionObjectToTile(list<CObj*>* pSrcList, vector<CTile*>* pDestVector)
{
	for (auto& pObjSrc : *pSrcList)
	{
		CMoveObj* pMoveObjSrc = dynamic_cast<CMoveObj*>(pObjSrc);
		int iTileIndexOfObj = CTileManager::GetInst()->GetTileIndex(pMoveObjSrc->GetHitBoxWorldInfo());
		int iIndex = iTileIndexOfObj / TILEX;
		int jIndex = iTileIndexOfObj % TILEX;

		if (iIndex < 0 || iIndex >= TILEY)
			continue;
		if (jIndex < 0 || jIndex >= TILEX)
			continue;

		list<int> iIndexList;
		list<int> jIndexList;

		int iTileCountX = int((pMoveObjSrc->GetInfo().icx * pMoveObjSrc->GetRatio() / TILECX) / 2 + 2);
		int iTileCountY = int((pMoveObjSrc->GetInfo().icy * pMoveObjSrc->GetRatio() / TILECY) / 2 + 2);

		if (iIndex >= 0 && iIndex < TILEY)
			iIndexList.push_back(iIndex);
		
		for (int i = 1; i < iTileCountY; ++i)
		{
			if (iIndex - i >= 0)
				iIndexList.push_back(iIndex - i);
			if (iIndex + i < TILEY)
				iIndexList.push_back(iIndex + i);
		}

		if (jIndex >= 0 && jIndex < TILEX)
			jIndexList.push_back(jIndex);
	
		for (int i = 1; i < iTileCountX; ++i)
		{
			if (jIndex - i >= 0)
				jIndexList.push_back(jIndex - i);
			if (jIndex + i < TILEX)
				jIndexList.push_back(jIndex + i);
		}

		for (auto& i : iIndexList)
		{
			for (auto& j : jIndexList)
			{
				POSITION PrevPos = pMoveObjSrc->GetPrevPos();

				INFO tInfo = pMoveObjSrc->GetHitBoxWorldInfo();
				INFO tPrevInfo = pMoveObjSrc->GetHitBoxPrevIWorldInfo();

				RECT tObjRect = CMath::InfoToRect(tInfo);
				RECT tObjPrevRect = CMath::InfoToRect(tPrevInfo);

				int iTileIndex = i * TILEX + j;
				INFO tTileInfo = (*pDestVector)[iTileIndex]->GetInfo();
				RECT tTileRC = CMath::InfoToRect(tTileInfo);
				switch ((*pDestVector)[iTileIndex]->GetOptionID())
				{
				case TO_NODOWN_RECT:
					if (tObjPrevRect.bottom < tTileRC.top &&
						CollisionRectToRect(pMoveObjSrc->GetHitBoxWorldInfo(), (*pDestVector)[iTileIndex]))
					{
						//bisCollide = true;
						if (!pMoveObjSrc->GetPassible())
						{
							//int iy = tTileRC.top - pMoveObjSrc->GetInfo().icy / 2;
							int iy = tTileRC.top - pMoveObjSrc->GetPushDistBottom();
							pMoveObjSrc->Stop();
							pMoveObjSrc->SetInfofy(iy - 1.f);
						}
					}
					break;
				case TO_NODOWN_RD:
					if ((tObjPrevRect.bottom < CMath::RDTriangleTop((float)tObjPrevRect.right, tTileRC)) &&
						CollisionRectToRDTriangle(tObjRect, tTileRC))
					{
						if (!pMoveObjSrc->GetPassible())
						{
							float fRatio = float(tObjRect.right - tTileRC.left) / (tTileRC.right - tTileRC.left);
							float fy = 0.f;
							if (fRatio >= 1.f)
								fy = tTileRC.top - pMoveObjSrc->GetInfo().icy * 0.5f;
							else if (fRatio <= 0.f)
								fy = tTileRC.bottom - pMoveObjSrc->GetInfo().icy * 0.5f;
							else
								fy = tTileRC.bottom - fRatio * (tTileRC.bottom - tTileRC.top) - pMoveObjSrc->GetInfo().icy / 2;

							pMoveObjSrc->Stop();
							pMoveObjSrc->SetInfofy(fy - 0.1f);
						}
					}
					break;
				case TO_NODOWN_LD:
					if ((tObjPrevRect.bottom < CMath::LDTriangleTop((float)tObjPrevRect.left, tTileRC)) &&
						CollisionRectToLDTriangle(tObjRect, tTileRC))
					{
						if (!pMoveObjSrc->GetPassible())
						{
							//bisCollide = true;
							float fRatio = float(tTileRC.right - tObjRect.left) / (tTileRC.right - tTileRC.left);
							float fy = 0.f;
							if (fRatio >= 1.f)
								fy = tTileRC.top - pMoveObjSrc->GetInfo().icy * 0.5f;
							else if (fRatio <= 0.f)
								fy = tTileRC.bottom - pMoveObjSrc->GetInfo().icy * 0.5f;
							else
								fy = tTileRC.bottom - fRatio * (tTileRC.bottom - tTileRC.top) - pMoveObjSrc->GetInfo().icy / 2;

							pMoveObjSrc->Stop();
							pMoveObjSrc->SetInfofy(fy - 0.1f);
						}
					}
					break;


				case TO_NOMOVE_RECT:
					if (tPrevInfo.fy < tInfo.fy &&			//	아래로 이동 중
						tObjPrevRect.bottom < tTileRC.top &&		//	전 프레임에는 충돌하지 않음.
						CollisionRectToRect(pMoveObjSrc->GetHitBoxWorldInfo(), (*pDestVector)[iTileIndex]))		//	하지만 이번 프레임에는 충돌함.
					{
						int iy = tTileRC.top - pMoveObjSrc->GetPushDistBottom();
						pMoveObjSrc->Stop();
						pMoveObjSrc->SetInfofy(iy - 1.f);
					}
					  
					if (tPrevInfo.fx <= tInfo.fx &&			//	우로 이동 중
						tObjPrevRect.right < tTileRC.left &&		//	전 프레임에는 충돌하지 않음.
						CollisionRectToRect(pMoveObjSrc->GetHitBoxWorldInfo(), (*pDestVector)[iTileIndex]))	//	하지만 이번 프레임에는 충돌함.
					{
						int ix = tTileRC.left - pMoveObjSrc->GetPushDistRight();
						pMoveObjSrc->SetInfofx(ix - 1.f);
					}

					if (tPrevInfo.fx > tInfo.fx &&				//	좌로 이동 중
						tObjPrevRect.left > tTileRC.right &&	//	전 프레임에는 충돌하지 않음.
						CollisionRectToRect(pMoveObjSrc->GetHitBoxWorldInfo(), (*pDestVector)[iTileIndex]))	//	하지만 이번 프레임에는 충돌함.
					{
						int ix = tTileRC.right + pMoveObjSrc->GetPushDistLeft();
						pMoveObjSrc->SetInfofx(ix + 1.f);
					}

					if (tPrevInfo.fy > tInfo.fy &&		//	위로 이동 중
						//tObjPrevRect.top > tTileRC.bottom &&	//	전 프레임에는 충돌하지 않음.
						CollisionRectToRect(pMoveObjSrc->GetHitBoxWorldInfo(), (*pDestVector)[iTileIndex]))	//	하지만 이번 프레임에는 충돌함.
					{
						int iy = tTileRC.bottom + pMoveObjSrc->GetPushDistTop();
						pMoveObjSrc->BangingHead();
						pMoveObjSrc->SetInfofy(iy + 1.f);
					}

					break;

				case TO_NOMOVE_RD:
					if (CollisionRectToRDTriangle(tObjRect, tTileRC))
					{
						//bisCollide = true;
						float fRatio = float(tObjRect.right - tTileRC.left) / (tTileRC.right - tTileRC.left);
						float fy = 0.f;
						if (fRatio >= 1.f)
							fy = tTileRC.top - pMoveObjSrc->GetInfo().icy * 0.5f;
						else if (fRatio <= 0.f)
							fy = tTileRC.bottom - pMoveObjSrc->GetInfo().icy * 0.5f;
						else
							fy = tTileRC.bottom - fRatio * (tTileRC.bottom - tTileRC.top) - pMoveObjSrc->GetInfo().icy / 2;

						pMoveObjSrc->Stop();
						pMoveObjSrc->SetInfofy(fy - 0.01f);
					}
					break;
				case TO_NOMOVE_LD:
					if (CollisionRectToLDTriangle(tObjRect, tTileRC))
					{
						//bisCollide = true;
						float fRatio = float(tTileRC.right - tObjRect.left) / (tTileRC.right - tTileRC.left);
						float fy = 0.f;
						if (fRatio >= 1.f)
							fy = tTileRC.top - pMoveObjSrc->GetInfo().icy * 0.5f;
						else if (fRatio <= 0.f)
							fy = tTileRC.bottom - pMoveObjSrc->GetInfo().icy * 0.5f;
						else
							fy = tTileRC.bottom - fRatio * (tTileRC.bottom - tTileRC.top) - pMoveObjSrc->GetInfo().icy / 2;

						pMoveObjSrc->Stop();
						pMoveObjSrc->SetInfofy(fy - 0.1f);
					}
					break;
				case TO_NOMOVE_RU:
					break;
				case TO_NOMOVE_LU:
					break;
				}
			}
		}
	}
}

void CCollisionManager::CollisionPlayerToMonster(list<CObj*>* pSrcList, list<CObj*>* pDestList)
{
	for (auto& pObjSrc : *pSrcList)
	{
		bool bSrcHit = false;
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObjSrc);
		CWeapon* pWeapon = pPlayer->GetEquipWeapon();
		for (auto& pObjDest : *pDestList)
		{
			bool bDestHit = false;
			CMonster* pMonster = dynamic_cast<CMonster*>(pObjDest);

			RECT rcSrc;
			RECT rcDest;

			RECT rc = {};
			if (pWeapon && pWeapon->GetAttacking())
			{
				rcSrc = CMath::InfoToRect(pWeapon->GetAttackBox());
				rcDest = CMath::InfoToRect(pMonster->GetHitBoxWorldInfo());
				if (IntersectRect(&rc, &rcSrc, &rcDest))
				{
					bSrcHit = true;
					//pMonster->Hit(static_cast<float>(pWeapon->GetAttack()), pPlayer->GetInfo());
					pMonster->Hit(static_cast<float>(pPlayer->GetAttack()), pPlayer->GetInfo());

					float fx = (rc.right + rc.left) * 0.5f;
					float fy = (rc.bottom + rc.top) * 0.5f;
					pPlayer->AddAttackEffect(fx, fy);
					if (pMonster->GetCharInfo().iHP <= 0)
						pMonster->Dying(pPlayer->GetInfo().fx, pPlayer->GetInfo().fy);
				}
			}

			if (pMonster->GetAttacking() && pPlayer->GetHittable())
			{
				rcSrc = CMath::InfoToRect(pPlayer->GetHitBoxWorldInfo());
				rcDest = CMath::InfoToRect(pMonster->GetAttackBox());

				if (IntersectRect(&rc, &rcSrc, &rcDest))
				{
					bDestHit = true;
					pPlayer->Hit(pMonster->GetCharInfo().fAttack, pMonster->GetInfo());
					pPlayer->SetAttackMonster(pMonster->GetName());

					float fx = (rc.right + rc.left) * 0.5f;
					float fy = (rc.bottom + rc.top) * 0.5f;
					pMonster->AddAttackEffect(fx, fy);
				}
			}

			if (bDestHit)
				pMonster->QuitAttack();

			int iA = 0;
		}
		if (bSrcHit)
		{
			pPlayer->QuitAttack();
			pWeapon->SetAttacking(false);
		}
	}
}

void CCollisionManager::CollisionPlayerToItem(list<CObj*>* pSrcList, list<CObj*>* pDestList)
{
	for (auto& pObjSrc : *pSrcList)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObjSrc);
		for (auto& pObjDest : *pDestList)
		{
			CFieldItem* pFieldItem = dynamic_cast<CFieldItem*>(pObjDest);

			if (CollisionRectToRect(pPlayer->GetHitBoxWorldInfo(), pFieldItem->GetHitBoxPrevIWorldInfo()))
			{
				bool bAddSuccess = false;
				switch (pFieldItem->GetItem()->GetType())
				{
				case IT_WEAPON:
					if (pFieldItem->GetIsStop() && GET_SINGLE(CInventory)->AddItem(pFieldItem->GetItem()))
						bAddSuccess = true;


					break;
				case IT_SHIELD:
					break;
				case IT_ACCESSORIES:
					break;
				case IT_GOLD_COIN:
				{
					CGoldCoin* pCoin = dynamic_cast<CGoldCoin*>(pFieldItem->GetItem());
					GET_SINGLE(CInventory)->AddGold(pCoin->GetGold());

					const INFO& tInfo = pFieldItem->GetInfo();
					string strGold = to_string(pCoin->GetGold()) + "G";
					GET_SINGLE(CEffectManager)->AddFontEffect(tInfo.fx, tInfo.fy, tInfo.fx + 10, tInfo.fy - 90, 2.f, 3.f, strGold, FT_FIELD_GOLD);
					bAddSuccess = true;
					GET_SINGLE(CSoundMgr)->PlaySound(L"EC_Collect.wav", CSoundMgr::EFFECT);
				}
					break;
				case IT_END:
					break;
				default:
					break;
				}
				if (bAddSuccess)
					pFieldItem->SetLife(false);
			}
		}
	}
}

void CCollisionManager::CollisionPlayerToTresureBox(list<CObj*>* pSrcList, list<CObj*>* pDestList)
{
	for (auto& pSrcObj : *pSrcList)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pSrcObj);
		for (auto& pDestObj : *pDestList)
		{
			CTresureBox* pBox = dynamic_cast<CTresureBox*>(pDestObj);

			RECT PlayerRect = CMath::InfoToRect(pPlayer->GetHitBoxWorldInfo());
			RECT BoxRect = CMath::InfoToRect(pBox->GetHitBoxWorldInfo());

			if (CollisionRectToRect(PlayerRect, BoxRect))
				pBox->SetCanOpen(true);
			else
				pBox->SetCanOpen(false);
		}
	}

}

void CCollisionManager::CollisionPlayerToFieldObject(list<CObj*>* pSrcList, list<CObj*>* pDestList)
{
	for (auto& pSrcObj : *pSrcList)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pSrcObj);
		for (auto& pDestObj : *pDestList)
		{
			CFieldObject* pFieldObj = dynamic_cast<CFieldObject*>(pDestObj);

			POSITION PrevPos = pPlayer->GetPrevPos();

			INFO tInfo = pPlayer->GetHitBoxWorldInfo();
			INFO tPrevInfo = pPlayer->GetHitBoxPrevIWorldInfo();

			RECT tObjRect = CMath::InfoToRect(tInfo);
			RECT tObjPrevRect = CMath::InfoToRect(tPrevInfo);

			INFO tFieldObjInfo = pFieldObj->GetHitBoxWorldInfo();
			RECT tFieldObjRect = CMath::InfoToRect(tFieldObjInfo);

			switch (pFieldObj->GetFieldObjectType())
			{
			case FOT_STELE:
			{
				if (tPrevInfo.fy < tInfo.fy &&			//	아래로 이동 중
					tObjPrevRect.bottom < tFieldObjRect.top &&		//	전 프레임에는 충돌하지 않음.
					CollisionRectToRect(pPlayer->GetHitBoxWorldInfo(), tFieldObjInfo))		//	하지만 이번 프레임에는 충돌함.
				{
					int iy = tFieldObjRect.top - pPlayer->GetPushDistBottom();
					pPlayer->Stop();
					pPlayer->SetInfofy(iy - 1.f);
				}

				if (tPrevInfo.fx <= tInfo.fx &&			//	우로 이동 중
					tObjPrevRect.right < tFieldObjRect.left &&		//	전 프레임에는 충돌하지 않음.
					CollisionRectToRect(pPlayer->GetHitBoxWorldInfo(), tFieldObjInfo))	//	하지만 이번 프레임에는 충돌함.
				{
					int ix = tFieldObjRect.left - pPlayer->GetPushDistRight();
					pPlayer->SetInfofx(ix - 1.f);
				}

				if (tPrevInfo.fx > tInfo.fx &&				//	좌로 이동 중
					tObjPrevRect.left > tFieldObjRect.right &&	//	전 프레임에는 충돌하지 않음.
					CollisionRectToRect(pPlayer->GetHitBoxWorldInfo(), tFieldObjInfo))	//	하지만 이번 프레임에는 충돌함.
				{
					int ix = tFieldObjRect.right + pPlayer->GetPushDistLeft();
					pPlayer->SetInfofx(ix + 1.f);
				}

				if (tPrevInfo.fy > tInfo.fy &&		//	위로 이동 중
													//tObjPrevRect.top > tTileRC.bottom &&	//	전 프레임에는 충돌하지 않음.
					CollisionRectToRect(pPlayer->GetHitBoxWorldInfo(), tFieldObjInfo))	//	하지만 이번 프레임에는 충돌함.
				{
					int iy = tFieldObjRect.bottom + pPlayer->GetPushDistTop();
					pPlayer->BangingHead();
					pPlayer->SetInfofy(iy + 1.f);
				}
			}
				break;
			case FOT_THORN:
			{
				CThorn* pThorn = dynamic_cast<CThorn*>(pFieldObj);
				if (CollisionRectToRect(tObjRect, tFieldObjRect) && pPlayer->GetHittable())
				{
					pPlayer->Hit(pThorn->GetAttack(), pThorn->GetInfo());
					
				}
			}
				break;
			case FOT_END:
				break;
			default:
				break;
			}

		}
	}
}

void CCollisionManager::CollisionPlayerToNPC(list<CObj*>* pSrcList, list<CObj*>* pDestList)
{
	for (auto& pSrcObj : *pSrcList)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pSrcObj);
		RECT PlayerRect = CMath::InfoToRect(pPlayer->GetHitBoxWorldInfo());

		for (auto& pDestObj : *pDestList)
		{
			CNPC *pNPC = dynamic_cast<CNPC*>(pDestObj);
			RECT NPCRect = CMath::InfoToRect(pNPC->GetHitBoxWorldInfo());

			if (CollisionRectToRect(PlayerRect, NPCRect))
				pNPC->SetCanInteract(true);
			else
				pNPC->SetCanInteract(false);
		}
	}
}

void CCollisionManager::CollisionBulletToPlayer(list<CObj*>* pSrcList, list<CObj*>* pDestList)
{
	for (auto& pSrcObj : *pSrcList)	//	bullet
	{
		bool bSrcHit = false;

		CBullet* pBullet = dynamic_cast<CBullet*>(pSrcObj);
		SPHERE tSphere = {pBullet->GetInfo().fx, pBullet->GetInfo().fy, pBullet->GetInfo().icx * 0.5f };
		for (auto& pDestObj : *pDestList)	//	Object
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pDestObj);
			RECT tRect = CMath::InfoToRect(pPlayer->GetHitBoxWorldInfo());
			if (!bSrcHit && CollisionRectToSphere(tRect, tSphere) && pPlayer->GetHittable())
			{
				bSrcHit = true;
				pPlayer->Hit(pBullet->GetAttack(), pBullet->GetInfo());
				pBullet->SetLife(false);
				
				float fx = (tSphere.fCenterX + pPlayer->GetInfo().fx) * 0.5f;
				float fy = (tSphere.fCenterY + pPlayer->GetInfo().fy) * 0.5f;

				pBullet->Dying((tRect.left + tRect.right) * 0.5f, (tRect.top + tRect.bottom) * 0.5f);
				
				pPlayer->SetAttackMonster(pBullet->GetName());

				if (pPlayer->GetCharInfo().iHP <= 0)
					pPlayer->Dying(fx, fy);
			}
		}
	}
}

void CCollisionManager::CollisionBulletToMonster(list<CObj*>* pSrcList, list<CObj*>* pDestList)
{
	for (auto& pSrcObj : *pSrcList)	//	bullet
	{
		bool bSrcHit = false;

		CBullet* pBullet = dynamic_cast<CBullet*>(pSrcObj);
		SPHERE tSphere = { pBullet->GetInfo().fx, pBullet->GetInfo().fy, pBullet->GetInfo().icx * 0.5f };
		for (auto& pDestObj : *pDestList)	//	Object
		{
			CMonster* pMonster = dynamic_cast<CMonster*>(pDestObj);
			RECT tRect = CMath::InfoToRect(pMonster->GetHitBoxWorldInfo());
			if (!bSrcHit && CollisionRectToSphere(tRect, tSphere))
			{
				bSrcHit = true;
				pMonster->Hit(pBullet->GetAttack(), pBullet->GetInfo());
				pBullet->SetLife(false);

				float fx = (tSphere.fCenterX + pMonster->GetInfo().fx) * 0.5f;
				float fy = (tSphere.fCenterY + pMonster->GetInfo().fy) * 0.5f;

				pBullet->Dying((tRect.left + tRect.right) * 0.5f, (tRect.top + tRect.bottom) * 0.5f);

				if (pMonster->GetCharInfo().iHP <= 0)
					pMonster->Dying(fx, fy);
			}
		}
	}
}

void CCollisionManager::CollisionBulletToTile(list<CObj*>* pSrcList, vector<CTile*>* pDestVector)
{
	for (auto& pObjSrc : *pSrcList)
	{
		CMoveObj* pMoveObjSrc = dynamic_cast<CMoveObj*>(pObjSrc);
		if (dynamic_cast<CBullet*>(pMoveObjSrc)->GetBulletType() == BT_GUIDED_MISSILE)
			continue;

		int iTileIndexOfObj = CTileManager::GetInst()->GetTileIndex(pMoveObjSrc->GetHitBoxWorldInfo());
		int iIndex = iTileIndexOfObj / TILEX;
		int jIndex = iTileIndexOfObj % TILEX;

		int iStartIndex = iIndex - 1 >= 0 ? iIndex - 1 : 0;
		int iEndIndex = iIndex + 1 < TILEY ? iIndex + 1 : TILEY - 1;

		int jStartIndex = jIndex - 1 >= 0 ? jIndex - 1 : 0;
		int jEndIndex = jIndex + 1 < TILEX ? jIndex + 1 : TILEX - 1;

		SPHERE tSphere = {pMoveObjSrc->GetInfo().fx, pMoveObjSrc->GetInfo().fy, pMoveObjSrc->GetInfo().icx * 0.5f};
		bool bSrcHit = false;
		for (int i = iStartIndex; i < iEndIndex; ++i)
		{
			if (bSrcHit)
				break;
			for (int j = jStartIndex; j < jEndIndex; ++j)
			{
				int iTileIndex = i * TILEX + j;
				INFO tTileInfo = (*pDestVector)[iTileIndex]->GetInfo();
				RECT tTileRC = CMath::InfoToRect(tTileInfo);

				switch ((*pDestVector)[iTileIndex]->GetOptionID())
				{
				case TO_NOMOVE_RECT:
					if (CollisionRectToSphere(tTileRC, tSphere))
					{
						bSrcHit = true;
						pMoveObjSrc->SetLife(false);
						pMoveObjSrc->Dying(tTileInfo.fx, tTileInfo.fy);
					}
					break;
				case TO_NOMOVE_RD:
					if (CollisionSphereToRDTriangle(tSphere, tTileRC))
					{
						bSrcHit = true;
						pMoveObjSrc->SetLife(false);
						pMoveObjSrc->Dying(tTileInfo.fx, tTileInfo.fy);
					}
					break;
				case TO_NOMOVE_LD:
					if (CollisionSphereToLDTriangle(tSphere, tTileRC))
					{
						bSrcHit = true;
						pMoveObjSrc->SetLife(false);
						pMoveObjSrc->Dying(tTileInfo.fx, tTileInfo.fy);
					}
					break;
				case TO_NOMOVE_RU:
					break;
				case TO_NOMOVE_LU:
					break;
				}
				if (bSrcHit)
					break;
			}
		}
	}
}

bool CCollisionManager::CollisionLineToLine(const LINE& lSrc, const LINE& lDest)
{
	float fCrossX = 0.f, fCrossY = 0.f;
	if (lSrc.P1.fx == lSrc.P2.fx)
	{
		if (lDest.P1.fx == lDest.P2.fx)	return false;

		float fDestM = (lDest.P1.fy - lDest.P2.fy) / (lDest.P1.fx - lDest.P2.fx);
		float fDestB = lDest.P1.fy - fDestM * lDest.P1.fx;

		fCrossX = lSrc.P1.fx;
		fCrossY = fDestM * fCrossX + fDestB;
	}

	else if (lDest.P1.fx == lDest.P2.fx)
	{
		float fSrcM = (lSrc.P1.fy - lSrc.P2.fy) / (lSrc.P1.fx - lSrc.P2.fx);
		float fSrcB = lSrc.P1.fy - fSrcM * lSrc.P1.fx;

		fCrossX = lDest.P1.fx;
		fCrossY = fSrcM * fCrossX + fSrcB;
	}
	else
	{
		float fSrcM = (lSrc.P1.fy - lSrc.P2.fy) / (lSrc.P1.fx - lSrc.P2.fx);
		float fDestM = (lDest.P1.fy - lDest.P2.fy) / (lDest.P1.fx - lDest.P2.fx);

		if (fSrcM == fDestM) return false;

		float fSrcB = lSrc.P1.fy - fSrcM * lSrc.P1.fx;
		float fDestB = lDest.P1.fy - fDestM * lDest.P1.fx;

		fCrossX = (fDestB - fSrcB) / (fSrcM - fDestM);
		fCrossY = fSrcM * fCrossX + fSrcB;
	}

	float fSrcLeft = lSrc.P1.fx < lSrc.P2.fx ? lSrc.P1.fx : lSrc.P2.fx;
	float fSrcRight = lSrc.P1.fx > lSrc.P2.fx ? lSrc.P1.fx : lSrc.P2.fx;
	float fSrcTop = lSrc.P1.fy < lSrc.P2.fy ? lSrc.P1.fy : lSrc.P2.fy;
	float fSrcBottom = lSrc.P1.fy > lSrc.P2.fy ? lSrc.P1.fy : lSrc.P2.fy;

	if (fCrossX < fSrcLeft)		return false;
	if (fCrossX > fSrcRight)	return false;
	if (fCrossY < fSrcTop)		return false;
	if (fCrossY > fSrcBottom)	return false;

	float fDestLeft = lDest.P1.fx < lDest.P2.fx ? lDest.P1.fx : lDest.P2.fx;
	float fDestRight = lDest.P1.fx > lDest.P2.fx ? lDest.P1.fx : lDest.P2.fx;
	float fDestTop = lDest.P1.fy < lDest.P2.fy ? lDest.P1.fy : lDest.P2.fy;
	float fDestBottom = lDest.P1.fy > lDest.P2.fy ? lDest.P1.fy : lDest.P2.fy;

	if (fCrossX < fDestLeft)	return false;
	if (fCrossX > fDestRight)	return false;
	if (fCrossY < fDestTop)		return false;
	if (fCrossY > fDestBottom)	return false;

	return true;
}

bool CCollisionManager::CollisionLineToRect(LINE lSrc, RECT rcDest)
{
	if (lSrc.P1.fx < rcDest.left && lSrc.P2.fx < rcDest.left)		return false;
	if (lSrc.P1.fx > rcDest.right && lSrc.P2.fx > rcDest.right)		return false;
	if (lSrc.P1.fy < rcDest.top && lSrc.P2.fy < rcDest.top)			return false;
	if (lSrc.P1.fy > rcDest.bottom && lSrc.P2.fy > rcDest.bottom)	return false;


	if (lSrc.P1.fx <= rcDest.right && lSrc.P1.fx >= rcDest.left &&
		lSrc.P1.fy <= rcDest.bottom && lSrc.P1.fy >= rcDest.top)	return true;
	if (lSrc.P2.fx <= rcDest.right && lSrc.P2.fx >= rcDest.left &&
		lSrc.P2.fy <= rcDest.bottom && lSrc.P2.fy >= rcDest.top)	return true;

	LINE tDestTop = { POSITION{ (float)rcDest.right, (float)rcDest.top }, POSITION{ (float)rcDest.left, (float)rcDest.top } };
	if (CollisionLineToLine(lSrc, tDestTop)) return true;

	LINE tDestBottom = { POSITION{ (float)rcDest.right, (float)rcDest.bottom }, POSITION{ (float)rcDest.left, (float)rcDest.bottom } };
	if (CollisionLineToLine(lSrc, tDestBottom)) return true;

	LINE tDestLeft = { POSITION{ (float)rcDest.left, (float)rcDest.top }, POSITION{ (float)rcDest.left, (float)rcDest.bottom } };
	if (CollisionLineToLine(lSrc, tDestLeft)) return true;

	LINE tDestRight = { POSITION{ (float)rcDest.right, (float)rcDest.top }, POSITION{ (float)rcDest.right, (float)rcDest.bottom } };
	if (CollisionLineToLine(lSrc, tDestRight)) return true;

	return false;
}

bool CCollisionManager::CollisionLineToRect(RECT rcSrc, RECT rcDest)
{

	return false;
}

bool CCollisionManager::CollisionLineToSphere(LINE lSrc, SPHERE spDest)
{
	float fdx = lSrc.P1.fx - lSrc.P2.fx;
	float fdy = lSrc.P1.fy - lSrc.P2.fy;

	float fA = fdy / fdx;
	float fB = lSrc.P1.fy - fA * lSrc.P1.fx;

	float fx = spDest.fCenterX;
	float fy = spDest.fCenterY;

	float fDist = abs(fA * fx - fy + fB) / sqrtf(fx * fx + fy * fy);
	
	if (fDist <= spDest.fRadius)
		return true;

	return false;
}

bool CCollisionManager::CollisionLineToRDTriangle(LINE lSrc, RECT rcDest)
{
	LINE lDestDiagonal = { POSITION{ (float)rcDest.right, (float)rcDest.top }, POSITION{ (float)rcDest.left, (float)rcDest.bottom } };
	if (CollisionLineToLine(lSrc, lDestDiagonal)) return true;

	return false;
}

bool CCollisionManager::CollisionLineToLDTriangle(LINE lSrc, RECT rcDest)
{
	return false;
}

bool CCollisionManager::CollisionLineToRUTriangle(LINE lSrc, RECT rcDest)
{
	return false;
}

bool CCollisionManager::CollisionLineToLUTriangle(LINE lSrc, RECT rcDest)
{
	return false;
}

bool CCollisionManager::CollisionRectToRDTriangle(const RECT& rcSrc, const RECT& rcDest)
{
	LINE lDestDiagonal = { POSITION{ (float)rcDest.right, (float)rcDest.top }, POSITION{ (float)rcDest.left, (float)rcDest.bottom } };

	if (CollisionLineToRect(lDestDiagonal, rcSrc)) return true;

	return false;
}

bool CCollisionManager::CollisionRectToLDTriangle(const RECT & rcSrc, const RECT & rcDest)
{
	LINE lDestDiagonal = { POSITION{ (float)rcDest.left, (float)rcDest.top }, POSITION{ (float)rcDest.right, (float)rcDest.bottom } };

	if (CollisionLineToRect(lDestDiagonal, rcSrc)) return true;

	return false;
}

bool CCollisionManager::CollisionRectToRect(CObj * pSrc, CObj * pDest)
{
	RECT rcSrc = CMath::InfoToRect(pSrc->GetInfo());
	RECT rcDest = CMath::InfoToRect(pDest->GetInfo());

	return CollisionRectToRect(rcSrc, rcDest);
}

bool CCollisionManager::CollisionRectToRect(CObj * pSrc, CTile * pDest)
{
	RECT rcSrc = CMath::InfoToRect(pSrc->GetInfo());
	RECT rcDest = CMath::InfoToRect(pDest->GetInfo());

	return CollisionRectToRect(rcSrc, rcDest);
}

bool CCollisionManager::CollisionRectToRect(const INFO & tInfoSrc, CTile * pDest)
{
	RECT rcSrc = CMath::InfoToRect(tInfoSrc);
	RECT rcDest = CMath::InfoToRect(pDest->GetInfo());

	return CollisionRectToRect(rcSrc, rcDest);
}

bool CCollisionManager::CollisionRectToRect(const INFO & tInfoSrc, const INFO & tInfoDest)
{
	RECT rcSrc = CMath::InfoToRect(tInfoSrc);
	RECT rcDest = CMath::InfoToRect(tInfoDest);

	return CollisionRectToRect(rcSrc, rcDest);
}

bool CCollisionManager::CollisionRectToRect(const RECT & rcSrc, const RECT & rcDest)
{
	if (rcSrc.left > rcDest.right)	return false;
	if (rcSrc.right < rcDest.left)	return false;
	if (rcSrc.top > rcDest.bottom)	return false;
	if (rcSrc.bottom < rcDest.top)	return false;

	return true;
}

bool CCollisionManager::CollisionRectToRect(const RECT & rcSrc, CTile * pDest)
{
	RECT rcDest = CMath::InfoToRect(pDest->GetInfo());

	return CollisionRectToRect(rcSrc, rcDest);
}

bool CCollisionManager::CollisionRectToSphere(CObj * pSrc, CObj * pDest)
{
	RECT rcSrc = CMath::InfoToRect(pSrc->GetInfo());
	SPHERE spDest = CMath::InfoToSphere(pDest->GetInfo());

	if ((rcSrc.left <= spDest.fCenterX && spDest.fCenterX <= rcSrc.right) ||
		(rcSrc.top <= spDest.fCenterY && spDest.fCenterY <= rcSrc.bottom))
	{
		if (spDest.fCenterX <= rcSrc.left - spDest.fRadius)		return false;
		if (spDest.fCenterX >= rcSrc.right + spDest.fRadius)	return false;
		if (spDest.fCenterY <= rcSrc.top - spDest.fRadius)		return false;
		if (spDest.fCenterY >= rcSrc.bottom + spDest.fRadius)	return false;

		return true;
	}

	if (CMath::Distance(spDest.fCenterX, spDest.fCenterY, (float)rcSrc.left, (float)rcSrc.top) <= spDest.fRadius)	return true;
	if (CMath::Distance(spDest.fCenterX, spDest.fCenterY, (float)rcSrc.right, (float)rcSrc.top) <= spDest.fRadius)	return true;
	if (CMath::Distance(spDest.fCenterX, spDest.fCenterY, (float)rcSrc.left, (float)rcSrc.bottom) <= spDest.fRadius)	return true;
	if (CMath::Distance(spDest.fCenterX, spDest.fCenterY, (float)rcSrc.right, (float)rcSrc.bottom) <= spDest.fRadius)return true;

	return false;
}

bool CCollisionManager::CollisionRectToSphere(RECT rcSrc, SPHERE spDest)
{
	if ((rcSrc.left <= spDest.fCenterX && spDest.fCenterX <= rcSrc.right) ||
		(rcSrc.top <= spDest.fCenterY && spDest.fCenterY <= rcSrc.bottom))
	{
		if (spDest.fCenterX <= rcSrc.left - spDest.fRadius)		return false;
		if (spDest.fCenterX >= rcSrc.right + spDest.fRadius)	return false;
		if (spDest.fCenterY <= rcSrc.top - spDest.fRadius)		return false;
		if (spDest.fCenterY >= rcSrc.bottom + spDest.fRadius)	return false;

		return true;
	}

	if (CMath::Distance(spDest.fCenterX, spDest.fCenterY, (float)rcSrc.left, (float)rcSrc.top) <= spDest.fRadius)	return true;
	if (CMath::Distance(spDest.fCenterX, spDest.fCenterY, (float)rcSrc.right, (float)rcSrc.top) <= spDest.fRadius)	return true;
	if (CMath::Distance(spDest.fCenterX, spDest.fCenterY, (float)rcSrc.left, (float)rcSrc.bottom) <= spDest.fRadius)	return true;
	if (CMath::Distance(spDest.fCenterX, spDest.fCenterY, (float)rcSrc.right, (float)rcSrc.bottom) <= spDest.fRadius)return true;

	return false;
}

bool CCollisionManager::CollisionSphereToSphere(SPHERE spSrc, SPHERE spDest)
{
	if (CMath::Distance(spSrc.fCenterX, spSrc.fCenterY, spDest.fCenterX, spDest.fCenterY) < spSrc.fRadius + spDest.fRadius)
		return true;

	return false;
}

bool CCollisionManager::CollisionSphereToRDTriangle(const SPHERE & spSrc, const RECT & rcDest)
{
	LINE tLine = { POSITION{ (float)rcDest.left, (float)rcDest.bottom }, POSITION{ (float)rcDest.right, (float)rcDest.top } };

	if (CollisionLineToSphere(tLine, spSrc))
		return true;

	return false;
}

bool CCollisionManager::CollisionSphereToLDTriangle(const SPHERE & spSrc, const RECT & rcDest)
{
	LINE tLine = { POSITION{ (float)rcDest.left, (float)rcDest.top }, POSITION{ (float)rcDest.right, (float)rcDest.bottom } };

	if (CollisionLineToSphere(tLine, spSrc))
		return true;

	return false;
}
