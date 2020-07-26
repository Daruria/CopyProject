#include "stdafx.h"
#include "CollisionMgr.h"
#include "GameObject.h"
#include "MoveObject.h"
#include "Terrain.h"
#include "HitBox.h"
#include "LineHitBox.h"

D3DXVECTOR3(*PushFunc[CTerrain::OPTION_END])(const D3DXVECTOR3&, const D3DXVECTOR3&) =
{
	nullptr,
	CMathMgr::PushIsoTile,

	CMathMgr::PushTileLT,
	CMathMgr::PushTileRT,
	CMathMgr::PushTileLB,
	CMathMgr::PushTileRB,

	CMathMgr::PushTileUp,
	CMathMgr::PushTileRight,
	CMathMgr::PushTileDown,
	CMathMgr::PushTileLeft,

	CMathMgr::PushTileLTR,
	CMathMgr::PushTileTRB,
	CMathMgr::PushTileRBL,
	CMathMgr::PushTileBLT
};

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionObjectToTerrain(list<CGameObject*>* pObjList, vector<TILE_INFO*>* pvecTiles)
{
	for (auto& pObj : *pObjList)
	{
		if (!(dynamic_cast<CMoveObject*>(pObj)->GetColl()))
			continue;

		D3DXVECTOR3 vObjPos = dynamic_cast<CMoveObject*>(pObj)->GetFoot();

		int iIndexX = static_cast<int>(vObjPos.x / TILECX);
		int iIndexY = static_cast<int>(vObjPos.y / (TILECY * 0.5f));

		int iStartX = iIndexX - 1 < 0 ? 0 : iIndexX - 1;
		int iStartY = iIndexY - 1 < 0 ? 0 : iIndexY - 1;

		int iEndX = iIndexX + 1 > TILEX - 1 ? TILEX - 1 : iIndexX + 1;
		int iEndY = iIndexY + 1 > TILEY - 1 ? TILEY - 1 : iIndexY + 1;

		for (int y = iStartY; y <= iEndY; ++y)
		{
			for (int x = iStartX; x <= iEndX; ++x)
			{
				int iIndex = y * TILEX + x;

				TILE_INFO* pTileInfo = (*pvecTiles)[iIndex];

				if (CTerrain::NO_OPTION == pTileInfo->byOption)
					continue;
				else
				{
					if (CollisionObjectToTile(vObjPos, pTileInfo->vPos))
					{
						D3DXVECTOR3 vPushPos = PushFunc[pTileInfo->byOption](vObjPos, pTileInfo->vPos);
						dynamic_cast<CMoveObject*>(pObj)->SetFoot(vPushPos);
					}
				}
			}	//	x For ¹®
		}	//	y For ¹®
	}
}

void CCollisionMgr::CollisionObjectToTerrain(list<CGameObject*>* pObjList, list<CGameObject*>* pTerrainList)
{
	for (auto& pObj : *pObjList)
	{
		D3DXVECTOR3 vObjPos = dynamic_cast<CMoveObject*>(pObj)->GetFoot();

		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainList->front());
		vector<TILE_INFO*>* pvecTiles = pTerrain->GetVecTile();

		int iTileIndex = pTerrain->GetTileIndex(vObjPos);
		int iTileIndexY = iTileIndex / TILEX;
		list<int> CheckList;
		CheckList.push_back(iTileIndex);
		CheckList.push_back(iTileIndex - 100);
		CheckList.push_back(iTileIndex + 100);
		CheckList.push_back(iTileIndex - 1);
		CheckList.push_back(iTileIndex + 1);
		
		int iLeftTop = iTileIndexY % 2 ? iTileIndex - 50 : iTileIndex - 51;
		int iLeftBottom = iTileIndexY % 2 ? iTileIndex + 50 : iTileIndex + 49;
		CheckList.push_back(iLeftTop);
		CheckList.push_back(iLeftTop + 1);
		CheckList.push_back(iLeftBottom);
		CheckList.push_back(iLeftBottom + 1);

		for (auto& iIndex : CheckList)
		{
			if (iIndex < 0 || iIndex >= TILEX * TILEY)
				continue;

			TILE_INFO* pTileInfo = (*pvecTiles)[iIndex];

			switch (pTileInfo->byOption)
			{
			case CTerrain::NO_OPTION:
				continue;
			case CTerrain::NO_MOVE:
				if (CollisionObjectToTile(vObjPos, pTileInfo->vPos))
				{
					D3DXVECTOR3 vPushPos = CMathMgr::PushIsoTile(vObjPos, pTileInfo->vPos);
					dynamic_cast<CMoveObject*>(pObj)->SetFoot(vPushPos);
				}
				break;
			case CTerrain::NO_LT_PUSH:
				if (CollisionObjectToTile(vObjPos, pTileInfo->vPos))
				{
					D3DXVECTOR3 vPushPos = CMathMgr::PushTileLT(vObjPos, pTileInfo->vPos);
					dynamic_cast<CMoveObject*>(pObj)->SetFoot(vPushPos);
				}
				break;
			case CTerrain::NO_RT_PUSH:
				if (CollisionObjectToTile(vObjPos, pTileInfo->vPos))
				{
					D3DXVECTOR3 vPushPos = CMathMgr::PushTileRT(vObjPos, pTileInfo->vPos);
					dynamic_cast<CMoveObject*>(pObj)->SetFoot(vPushPos);
				}
				break;
			case CTerrain::NO_LB_PUSH:
				if (CollisionObjectToTile(vObjPos, pTileInfo->vPos))
				{
					D3DXVECTOR3 vPushPos = CMathMgr::PushTileLB(vObjPos, pTileInfo->vPos);
					dynamic_cast<CMoveObject*>(pObj)->SetFoot(vPushPos);
				}
				break;
			case CTerrain::NO_RB_PUSH:
				if (CollisionObjectToTile(vObjPos, pTileInfo->vPos))
				{
					D3DXVECTOR3 vPushPos = CMathMgr::PushTileRB(vObjPos, pTileInfo->vPos);
					dynamic_cast<CMoveObject*>(pObj)->SetFoot(vPushPos);
				}
				break;
			}	//	switch ¹®
		}	//	Terrain IndexList For

	
	}	//	ObjectList For


}

void CCollisionMgr::CollisionHitBoxes(list<CGameObject*>* pHitBoxList, list<CGameObject*>* pObjList)
{
	for (auto& pBox : *pHitBoxList)
	{
		CHitBox* pHitBox = dynamic_cast<CHitBox*>(pBox);
		NULL_CHECK_CONTINUE(pHitBox);

		RECT rcHitBox = pHitBox->GetRect();
		LINE tLine = {};
		CLineHitBox* pLineHitBox = dynamic_cast<CLineHitBox*>(pHitBox);
		if (pLineHitBox)
			tLine = pLineHitBox->GetLine();
		
		for (auto& pObj : *pObjList)
		{
			CMoveObject* pMoveObj = dynamic_cast<CMoveObject*>(pObj);

			RECT rcObjBox = pMoveObj->GetRect();
			RECT rc = {};
			
			CHitBox::HIT_BOX_TYPE eBoxType = pHitBox->GetBoxType();

			switch (eBoxType)
			{
			case CHitBox::HIT_BOX_RECT:
				if (IntersectRect(&rc, &rcHitBox, &rcObjBox))
				{
					pMoveObj->Hit(pHitBox->GetInfo().vPos, pHitBox->GetAttack());
					pHitBox->Hit();
				}
				break;
			case CHitBox::HIT_BOX_LINE:
			{
				if (CollisionLineToRect(tLine, rcObjBox))
				{
					pMoveObj->Hit(pHitBox->GetInfo().vPos, pHitBox->GetAttack());
					pLineHitBox->Hit(pObj->GetInfo().vPos);
				}
			}
				break;
			}

		}
	}
}


bool CCollisionMgr::CollisionRectToRect(const RECT & rcSrc, const RECT & rcDest)
{
	if (rcSrc.right < rcDest.left)	return false;
	if (rcSrc.bottom < rcDest.top)	return false;
	if (rcSrc.left > rcDest.right)	return false;
	if (rcSrc.top > rcDest.bottom)	return false;

	return true;
}

bool CCollisionMgr::CollisionSphereToSphere(const SPHERE & spSrc, const SPHERE & spDest)
{
	D3DXVECTOR3 vDistance = spSrc.vCenter - spDest.vCenter;
	float fDist = D3DXVec3Length(&vDistance);

	return fDist < spSrc.fRadius + spDest.fRadius;
}

bool CCollisionMgr::CollisionObjectToTile(const CGameObject* Obj, const TILE_INFO* pTile)
{
	D3DXVECTOR3 vPoint[5] = 
	{
		D3DXVECTOR3{ pTile->vPos.x, pTile->vPos.y - TILECY * 0.5f, 0.f },	//	top
		D3DXVECTOR3{ pTile->vPos.x + TILECX * 0.5f, pTile->vPos.y, 0.f },	//	right
		D3DXVECTOR3{ pTile->vPos.x, pTile->vPos.y + TILECY * 0.5f, 0.f },	//	bottom
		D3DXVECTOR3{ pTile->vPos.x - TILECX * 0.5f, pTile->vPos.y, 0.f },	//	left
		D3DXVECTOR3{ pTile->vPos.x, pTile->vPos.y - TILECY * 0.5f, 0.f },	//	top
	};

	D3DXVECTOR3 vDir[4];
	for (int i = 0; i < 4; ++i)
		vDir[i] = vPoint[i + 1] - vPoint[i];
	
	D3DXVECTOR3 vCross[4];
	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vCross[i], &D3DXVECTOR3{ 0.f, 0.f, 1.f }, &vDir[i]);

	D3DXVECTOR3 vObj = Obj->GetInfo().vPos;

	D3DXVECTOR3 vPointToObjDir[4];
	for (int i = 0; i < 4; ++i)
		vPointToObjDir[i] = vObj - vPoint[i];

	for (int i = 0; i < 4; ++i)
	{
		if (0 > D3DXVec3Dot(&vCross[i], &vPointToObjDir[i]))
			return false;
	}

	return true;
}

bool CCollisionMgr::CollisionObjectToTile(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	D3DXVECTOR3 vPoint[5] =
	{
		D3DXVECTOR3{ vTilePos.x, vTilePos.y - TILECY * 0.5f, 0.f },	//	top
		D3DXVECTOR3{ vTilePos.x + TILECX * 0.5f, vTilePos.y, 0.f },	//	right
		D3DXVECTOR3{ vTilePos.x, vTilePos.y + TILECY * 0.5f, 0.f },	//	bottom
		D3DXVECTOR3{ vTilePos.x - TILECX * 0.5f, vTilePos.y, 0.f },	//	left
		D3DXVECTOR3{ vTilePos.x, vTilePos.y - TILECY * 0.5f, 0.f },	//	top
	};

	D3DXVECTOR3 vDir[4];
	for (int i = 0; i < 4; ++i)
		vDir[i] = vPoint[i + 1] - vPoint[i];

	D3DXVECTOR3 vCross[4];
	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vCross[i], &D3DXVECTOR3{ 0.f, 0.f, 1.f }, &vDir[i]);

	D3DXVECTOR3 vPointToObjDir[4];
	for (int i = 0; i < 4; ++i)
		vPointToObjDir[i] = vObjPos - vPoint[i];

	for (int i = 0; i < 4; ++i)
	{
		if (0 > D3DXVec3Dot(&vCross[i], &vPointToObjDir[i]))
			return false;
	}

	return true;
}

bool CCollisionMgr::CollisionLineToLine(const LINE& lSrc, const LINE& lDest)
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


bool CCollisionMgr::CollisionLineToRect(LINE lSrc, RECT rcDest)
{
	if (lSrc.P1.fx < rcDest.left && lSrc.P2.fx < rcDest.left)		return false;
	if (lSrc.P1.fx > rcDest.right && lSrc.P2.fx > rcDest.right)		return false;
	if (lSrc.P1.fy < rcDest.top && lSrc.P2.fy < rcDest.top)			return false;
	if (lSrc.P1.fy > rcDest.bottom && lSrc.P2.fy > rcDest.bottom)	return false;


	if (lSrc.P1.fx <= rcDest.right && lSrc.P1.fx >= rcDest.left &&
		lSrc.P1.fy <= rcDest.bottom && lSrc.P1.fy >= rcDest.top)	return true;
	if (lSrc.P2.fx <= rcDest.right && lSrc.P2.fx >= rcDest.left &&
		lSrc.P2.fy <= rcDest.bottom && lSrc.P2.fy >= rcDest.top)	return true;

	LINE tDestTop = { _POSITION{ (float)rcDest.right, (float)rcDest.top }, _POSITION{ (float)rcDest.left, (float)rcDest.top } };
	if (CollisionLineToLine(lSrc, tDestTop)) return true;

	LINE tDestBottom = { _POSITION{ (float)rcDest.right, (float)rcDest.bottom }, _POSITION{ (float)rcDest.left, (float)rcDest.bottom } };
	if (CollisionLineToLine(lSrc, tDestBottom)) return true;

	LINE tDestLeft = { _POSITION{ (float)rcDest.left, (float)rcDest.top }, _POSITION{ (float)rcDest.left, (float)rcDest.bottom } };
	if (CollisionLineToLine(lSrc, tDestLeft)) return true;

	LINE tDestRight = { _POSITION{ (float)rcDest.right, (float)rcDest.top }, _POSITION{ (float)rcDest.right, (float)rcDest.bottom } };
	if (CollisionLineToLine(lSrc, tDestRight)) return true;

	return false;
}