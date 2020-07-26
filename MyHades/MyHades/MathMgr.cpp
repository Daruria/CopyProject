#include "stdafx.h"
#include "MathMgr.h"
#include "ScreenEffect.h"
#include "AnimImp.h"
#include "NonAnimImp.h"

CMathMgr::CMathMgr()
{
}


CMathMgr::~CMathMgr()
{
}

float CMathMgr::CalculateAngleMPIToPI(const D3DXVECTOR3& vDest, const D3DXVECTOR3& vSrc)
{
	D3DXVECTOR3 vCross;
	D3DXVECTOR3 vNorm1;
	D3DXVECTOR3 vNorm2;
	D3DXVec3Normalize(&vNorm1, &vSrc);
	D3DXVec3Normalize(&vNorm2, &vDest);

	D3DXVec3Cross(&vCross, &vNorm1, &vNorm2);

	float fDot = D3DXVec3Dot(&vNorm1, &vNorm2);

	return vCross.z < 0.f ? acosf(fDot) : -acosf(fDot);
}

float CMathMgr::CalculateAngle0To2PI(const D3DXVECTOR3 & vPivot, const D3DXVECTOR3 & vLook)
{
	D3DXVECTOR3 vCross;
	D3DXVECTOR3 vNorm1;
	D3DXVECTOR3 vNorm2;
	D3DXVec3Normalize(&vNorm1, &vPivot);
	D3DXVec3Normalize(&vNorm2, &vLook);

	D3DXVec3Cross(&vCross, &vNorm1, &vNorm2);
	
	float fDot = D3DXVec3Dot(&vNorm1, &vNorm2);
	
	if (vCross.z < 0.f)
		return acosf(fDot);
	else
		return 2 * PI - acosf(fDot);
}

int CMathMgr::CalculateInfoTo16Dir(const INFO & tInfo)
{
	return CalculateVecTo16Dir(tInfo.vDir, tInfo.vLook);
}

int CMathMgr::CalculateVecTo16Dir(const D3DXVECTOR3 & vPivot, const D3DXVECTOR3 & vLook)
{
	return CMathMgr::AngleTo16Dir(CMathMgr::CalculateAngle0To2PI(vPivot, vLook));
}

int CMathMgr::AngleTo16Dir(float fAngle)
{
	float fInterval = PI / 8.f;

	int iDir = static_cast<int>((fAngle + PI / 16.f) / fInterval);
	if (iDir < 0) iDir = 0;
	if (iDir > 15) iDir = 0;
	return iDir;
}

int CMathMgr::AngleTo16Dir(const D3DXVECTOR3 & vPivot, const D3DXVECTOR3 & vLook)
{
	D3DXVECTOR3 vCross;
	D3DXVECTOR3 vNorm1;
	D3DXVECTOR3 vNorm2;
	D3DXVec3Normalize(&vNorm1, &vPivot);
	D3DXVec3Normalize(&vNorm2, &vLook);

	D3DXVec3Cross(&vCross, &vNorm1, &vNorm2);

	float fDot = D3DXVec3Dot(&vNorm1, &vNorm2);
	float fAngle = vCross.z < 0.f ? acosf(fDot) : 2 * PI - acosf(fDot);
	
	float fInterval = PI / 8.f;

	int iDir = static_cast<int>((fAngle + PI / 16.f) / fInterval);
	if (iDir < 0) iDir = 0;
	if (iDir > 15) iDir = 0;
	
	return iDir;
}

int CMathMgr::AngleTo8Dir(const D3DXVECTOR3 & vPivot, const D3DXVECTOR3 & vLook)
{
	D3DXVECTOR3 vCross;
	D3DXVECTOR3 vNorm1;
	D3DXVECTOR3 vNorm2;
	D3DXVec3Normalize(&vNorm1, &vPivot);
	D3DXVec3Normalize(&vNorm2, &vLook);

	D3DXVec3Cross(&vCross, &vNorm1, &vNorm2);

	float fDot = D3DXVec3Dot(&vNorm1, &vNorm2);
	float fAngle = vCross.z < 0.f ? acosf(fDot) : 2 * PI - acosf(fDot);

	float fInterval = PI / 4.f;

	int iDir = static_cast<int>((fAngle + PI / 8.f) / fInterval);
	if (iDir < 0) iDir = 0;
	if (iDir > 7) iDir = 0;

	return iDir;
}

D3DXVECTOR3 CMathMgr::PushIsoTile(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	D3DXVECTOR3 vPoint[2];
	float fCrossX = 0.f;
	float fCrossY = 0.f;

	if (vObjPos.x < vTilePos.x)
		vPoint[0] = { vTilePos.x - TILECX * 0.5f, vTilePos.y, 0.f };
	else
		vPoint[0] = { vTilePos.x + TILECX * 0.5f, vTilePos.y, 0.f };

	if (vObjPos.y < vTilePos.y)
		vPoint[1] = { vTilePos.x, vTilePos.y - TILECY * 0.5f, 0.f };
	else
		vPoint[1] = { vTilePos.x, vTilePos.y + TILECY * 0.5f, 0.f };

	float fTileSlope = (vPoint[0].y - vPoint[1].y) / (vPoint[0].x - vPoint[1].x);
	float fTileWeight = vPoint[0].y - fTileSlope * vPoint[0].x;

	float fdx = (vObjPos.x - vTilePos.x);
	float fdy = (vObjPos.y - vTilePos.y);
	if (fdx != 0.f)
	{
		float fObjSlope = fdy / fdx;
		float fObjWeight = vObjPos.y - fObjSlope * vObjPos.x;

		fCrossX = -(fTileWeight - fObjWeight) / (fTileSlope - fObjSlope);
		fCrossY = fObjSlope * fCrossX + fObjWeight;
	}
	else
	{
		float fObjSlope = 0.f;
		float fObjWeight = vObjPos.x;

		fCrossX = vObjPos.x;
		fCrossY = vObjPos.x * fTileSlope + fTileWeight;
	}

	D3DXVECTOR3 vDir = vObjPos - vTilePos;
	D3DXVec3Normalize(&vDir, &vDir);
	fCrossX += vDir.x;
	fCrossY += vDir.y;

	return D3DXVECTOR3{fCrossX, fCrossY, 0.f};
}

D3DXVECTOR3 CMathMgr::PushTile(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	D3DXVECTOR3 vPoint[2];
	float fCrossX = 0.f;
	float fCrossY = 0.f;

	if (vObjPos.x < vTilePos.x)
		vPoint[0] = { vTilePos.x - TILECX * 0.5f, vTilePos.y, 0.f };	//	좌측
	else
		vPoint[0] = { vTilePos.x + TILECX * 0.5f, vTilePos.y, 0.f };	//	우측

	if (vObjPos.y < vTilePos.y)
		vPoint[1] = { vTilePos.x, vTilePos.y - TILECY * 0.5f, 0.f };	//	상단
	else
		vPoint[1] = { vTilePos.x, vTilePos.y + TILECY * 0.5f, 0.f };	//	하단

	D3DXVECTOR3 vDiagonal = vPoint[1] - vPoint[0];

	float fdx = vObjPos.x - vTilePos.x;
	float fdy = vObjPos.y - vTilePos.y;

	D3DXVECTOR3 vZ = fdx * fdy < 0.f ? D3DXVECTOR3{ 0.f, 0.f, 1.f } : D3DXVECTOR3{ 0.f, 0.f, -1.f };

	D3DXVECTOR3 vCross;
	D3DXVec3Cross(&vCross, &vZ, &vDiagonal);
	D3DXVec3Normalize(&vCross, &vCross);

	float fSlope = (vPoint[0].y - vPoint[1].y) / (vPoint[0].x - vPoint[0].y);
	float fWeight = vPoint[0].y - fSlope * vPoint[0].x;

	float fDist = fabs(fSlope * vObjPos.x - vObjPos.y + fWeight) / sqrtf(fSlope * fSlope + 1.f);

	return vObjPos + vCross * fDist;
}

D3DXVECTOR3 CMathMgr::PushTileLT(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	D3DXVECTOR3 vPoint[3] =
	{
		D3DXVECTOR3(vTilePos.x - TILECX * 0.5f, vTilePos.y, 0.f),	//	Left
		D3DXVECTOR3(vTilePos.x, vTilePos.y - TILECY * 0.5f, 0.f),	//	Top
		D3DXVECTOR3(vTilePos.x, vTilePos.y + TILECY * 0.5f, 0.f)	//	Bottom
	};

	////	경계 직선의 방정식
	////	y = ax + b
	//float fa = (vPoint[0].y - vPoint[1].y) / (vPoint[0].x - vPoint[1].x);
	////	b = y - ax
	//float fb = vPoint[0].y - fa * vPoint[0].x;

	////	밀어낼 방향 직선
	//float fDirA = (vPoint[0].y - vPoint[2].y) / (vPoint[0].x - vPoint[2].x);
	//float fDirB = vObjPos.y - fDirA * vObjPos.x;

	////	두직선의 교점.
	//float fCrossX = -(fDirB - fb) / (fDirA - fa);
	//float fCrossY = fa * fCrossX + fb;

	//D3DXVECTOR3 vDir = vPoint[0] - vPoint[2];
	//D3DXVec3Normalize(&vDir, &vDir);
	//vDir.x += fCrossX;
	//vDir.y += fCrossY;

	//return vDir;

	return PushTile(vObjPos, vPoint);
}

D3DXVECTOR3 CMathMgr::PushTileRT(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	D3DXVECTOR3 vPoint[3] =
	{
		D3DXVECTOR3(vTilePos.x + TILECX * 0.5f, vTilePos.y, 0.f),	//	Right
		D3DXVECTOR3(vTilePos.x, vTilePos.y - TILECY * 0.5f, 0.f),	//	Top
		D3DXVECTOR3(vTilePos.x, vTilePos.y + TILECY * 0.5f, 0.f)	//	Bottom
	};

	////	경계 직선의 방정식
	////	y = ax + b
	//float fa = (vPoint[0].y - vPoint[1].y) / (vPoint[0].x - vPoint[1].x);
	////	b = y - ax
	//float fb = vPoint[0].y - fa * vPoint[0].x;

	////	밀어낼 방향 직선
	//float fDirA = (vPoint[0].y - vPoint[2].y) / (vPoint[0].x - vPoint[2].x);
	//float fDirB = vObjPos.y - fDirA * vObjPos.x;

	////	두직선의 교점.
	//float fCrossX = -(fDirB - fb) / (fDirA - fa);
	//float fCrossY = fa * fCrossX + fb;

	//return D3DXVECTOR3(fCrossX, fCrossY, 0.f);

	return PushTile(vObjPos, vPoint);
}

D3DXVECTOR3 CMathMgr::PushTileLB(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	D3DXVECTOR3 vPoint[3] =
	{
		D3DXVECTOR3(vTilePos.x - TILECX * 0.5f, vTilePos.y, 0.f),	//	Left
		D3DXVECTOR3(vTilePos.x, vTilePos.y + TILECY * 0.5f, 0.f),	//	Bottom
		D3DXVECTOR3(vTilePos.x, vTilePos.y - TILECY * 0.5f, 0.f)	//	Top
	};

	////	경계 직선의 방정식
	////	y = ax + b
	//float fa = (vPoint[0].y - vPoint[1].y) / (vPoint[0].x - vPoint[1].x);
	////	b = y - ax
	//float fb = vPoint[0].y - fa * vPoint[0].x;

	////	밀어낼 방향 직선
	//float fDirA = (vPoint[0].y - vPoint[2].y) / (vPoint[0].x - vPoint[2].x);
	//float fDirB = vObjPos.y - fDirA * vObjPos.x;

	////	두직선의 교점.
	//float fCrossX = -(fDirB - fb) / (fDirA - fa);
	//float fCrossY = fa * fCrossX + fb;

	//return D3DXVECTOR3(fCrossX, fCrossY, 0.f);

	return PushTile(vObjPos, vPoint);
}

D3DXVECTOR3 CMathMgr::PushTileRB(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	D3DXVECTOR3 vPoint[3] =
	{
		D3DXVECTOR3(vTilePos.x + TILECX * 0.5f, vTilePos.y, 0.f),	//	Right
		D3DXVECTOR3(vTilePos.x, vTilePos.y + TILECY * 0.5f, 0.f),	//	Bottom
		D3DXVECTOR3(vTilePos.x, vTilePos.y - TILECY * 0.5f, 0.f)	//	Top
	};

	////	경계 직선의 방정식
	////	y = ax + b
	//float fa = (vPoint[0].y - vPoint[1].y) / (vPoint[0].x - vPoint[1].x);
	////	b = y - ax
	//float fb = vPoint[0].y - fa * vPoint[0].x;

	////	밀어낼 방향 직선
	//float fDirA = (vPoint[0].y - vPoint[2].y) / (vPoint[0].x - vPoint[2].x);
	//float fDirB = vObjPos.y - fDirA * vObjPos.x;

	////	두직선의 교점.
	//float fCrossX = -(fDirB - fb) / (fDirA - fa);
	//float fCrossY = fa * fCrossX + fb;

	//return D3DXVECTOR3(fCrossX, fCrossY, 0.f);

	return PushTile(vObjPos, vPoint);
}

D3DXVECTOR3 CMathMgr::PushTile(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3* pPoint)
{
	//	경계 직선의 방정식
	//	y = ax + b
	float fa = (pPoint[0].y - pPoint[1].y) / (pPoint[0].x - pPoint[1].x);
	//	b = y - ax
	float fb = pPoint[0].y - fa * pPoint[0].x;

	//	밀어낼 방향 직선
	float fDirA = (pPoint[0].y - pPoint[2].y) / (pPoint[0].x - pPoint[2].x);
	float fDirB = vObjPos.y - fDirA * vObjPos.x;

	//	두직선의 교점.
	float fCrossX = -(fDirB - fb) / (fDirA - fa);
	float fCrossY = fa * fCrossX + fb;

	D3DXVECTOR3 vDir = pPoint[0] - pPoint[2];
	D3DXVec3Normalize(&vDir, &vDir);
	vDir.x += fCrossX + 1.f;
	vDir.y += fCrossY + 1.f;

	return vDir;
}

D3DXVECTOR3 CMathMgr::PushTileUp(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	return D3DXVECTOR3(vObjPos.x, vTilePos.y - TILECY * 0.5f, 0.f);
}

D3DXVECTOR3 CMathMgr::PushTileRight(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	return D3DXVECTOR3(vTilePos.x + TILECX * 0.5f, vObjPos.y, 0.f);
}

D3DXVECTOR3 CMathMgr::PushTileDown(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	return D3DXVECTOR3(vObjPos.x, vTilePos.y + TILECY * 0.5f, 0.f);
}

D3DXVECTOR3 CMathMgr::PushTileLeft(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	return D3DXVECTOR3(vTilePos.x - TILECX * 0.5f, vObjPos.y, 0.f);
}

D3DXVECTOR3 CMathMgr::PushTileLTR(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	if (vObjPos.x < vTilePos.x)
		return PushTileLT(vObjPos, vTilePos);
	else
		return PushTileRT(vObjPos, vTilePos);
}

D3DXVECTOR3 CMathMgr::PushTileTRB(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	if (vObjPos.y < vTilePos.y)
		return PushTileRT(vObjPos, vTilePos);
	else
		return PushTileRB(vObjPos, vTilePos);
}

D3DXVECTOR3 CMathMgr::PushTileRBL(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	if (vObjPos.x < vTilePos.x)
		return PushTileLB(vObjPos, vTilePos);
	else
		return PushTileRB(vObjPos, vTilePos);
}

D3DXVECTOR3 CMathMgr::PushTileBLT(const D3DXVECTOR3 & vObjPos, const D3DXVECTOR3 & vTilePos)
{
	if (vObjPos.y < vTilePos.y)
		return PushTileLT(vObjPos, vTilePos);
	else
		return PushTileLB(vObjPos, vTilePos);
}

void CMathMgr::DrawLineBox(CDeviceMgr* pDeviceMgr, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vSize, float fLine, D3DCOLOR d3dColor)
{
	pDeviceMgr->GetSprite()->End();

	D3DXVECTOR2 vPoint[5]
	{
		D3DXVECTOR2(vPos.x - vSize.x * 0.5f, vPos.y - vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x + vSize.x * 0.5f, vPos.y + vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x - vSize.x * 0.5f, vPos.y + vSize.y * 0.5f),
		D3DXVECTOR2(vPos.x - vSize.x * 0.5f, vPos.y - vSize.y * 0.5f)
	};

	pDeviceMgr->GetLine()->SetWidth(fLine); // 선의 굵기
	pDeviceMgr->GetLine()->Draw(vPoint, 5, d3dColor);

	pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CMathMgr::DrawLine(CDeviceMgr * pDeviceMgr, const D3DXVECTOR3 & vPos, float fLength, float fAngle, float fThickness, D3DCOLOR d3dColor)
{
	
	D3DXMATRIX matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, fAngle);

	D3DXVECTOR3 vEndPos = {fLength, 0.f, 0.f};
	
	vEndPos = static_cast<D3DXVECTOR3>(static_cast<D3DXMATRIX>(vEndPos) * matRotateZ);
	vEndPos += vPos;

	D3DXVECTOR2 vPoint[2] = { vPos, vEndPos };

	pDeviceMgr->GetSprite()->End();

	pDeviceMgr->GetLine()->SetWidth(fThickness); // 선의 굵기
	pDeviceMgr->GetLine()->Draw(vPoint, 2, d3dColor);

	pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CMathMgr::DrawLine(CDeviceMgr * pDeviceMgr, const D3DXVECTOR3 & vStartPos, const D3DXVECTOR3 & vEndPos, float fThickness, D3DCOLOR d3dColor)
{
	D3DXVECTOR2 vPoint[2] = { vStartPos, vEndPos};

	pDeviceMgr->GetSprite()->End();

	pDeviceMgr->GetLine()->SetWidth(fThickness); // 선의 굵기
	pDeviceMgr->GetLine()->Draw(vPoint, 2, d3dColor);

	pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CMathMgr::AddTitleEffect(const wstring & wstrTextImgKey, float fSort, const D3DXVECTOR3 & vPos)
{
	FRAME tFrame;
	tFrame.eType = ANIM_ONCE;
	tFrame.fCurFrame = 0.f;
	tFrame.fMaxFrame = CTextureMgr::GetInstance()->GetTextureMaxFrame(L"UnlockTextBG", L"UnlockTextBG");
	tFrame.fFrameSpeed = 1 / 30.f;
	tFrame.fFrameTime = 0.f;

	CScreenEffect* pEffect = CScreenEffect::Create(CAnimImp::Create(L"UnlockTextBG", L"UnlockTextBG", tFrame, D3DCOLOR_ARGB(255, 255, 255, 255)),
		vPos, RENDER_FRONT_UI, fSort);
	CObjectMgr::GetInstance()->AddObject(OBJECT_FRONT_UI, pEffect);

	pEffect = CScreenEffect::Create(CNonAnimImp::Create(wstrTextImgKey, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255)), vPos, RENDER_FRONT_UI, fSort + 1.f);
	CObjectMgr::GetInstance()->AddObject(OBJECT_FRONT_UI, pEffect);

	CSoundMgr::GetInstance()->PlaySound(L"TextReveal2.wav", CSoundMgr::EFFECT);
}

void CMathMgr::DarkAmbienceRender()
{
	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"FullScreenDark");
	NULL_CHECK(pTexInfo);

	D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, 2.f * WINCX / vCenter.x, 2.f * WINCY / vCenter.y, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);


	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&(matScale * matTrans));
	CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(200, 255, 255, 255));
	
}
