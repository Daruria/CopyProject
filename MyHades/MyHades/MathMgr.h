#pragma once
class CMathMgr
{
private:
	CMathMgr();
	~CMathMgr();

public:
	static float CalculateAngleMPIToPI(const D3DXVECTOR3& vDest, const D3DXVECTOR3& vSrc);
	static float CalculateAngle0To2PI(const D3DXVECTOR3& vPivot, const D3DXVECTOR3& vLook);
	static int	CalculateInfoTo16Dir(const INFO& tInfo);
	static int	CalculateVecTo16Dir(const D3DXVECTOR3& vPivot, const D3DXVECTOR3& vLook);
	static int	AngleTo16Dir(float fAngle);
	static int	AngleTo16Dir(const D3DXVECTOR3& vPivot, const D3DXVECTOR3& vLook);
	static int	AngleTo8Dir(const D3DXVECTOR3& vPivot, const D3DXVECTOR3& vLook);

public:	//	Collision PushTile Functions
	static D3DXVECTOR3 PushIsoTile(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTile(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileLT(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileRT(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileLB(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileRB(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTile(const D3DXVECTOR3& vObjPos,	const D3DXVECTOR3* pTilePos);

	static D3DXVECTOR3 PushTileUp(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileRight(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileDown(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileLeft(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);

	static D3DXVECTOR3 PushTileLTR(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileTRB(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileRBL(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);
	static D3DXVECTOR3 PushTileBLT(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);


public:
	static void DrawLineBox(CDeviceMgr* pDeviceMgr, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vSize, float fLine, D3DCOLOR d3dColor);
	static void DrawLine(CDeviceMgr* pDeviceMgr, const D3DXVECTOR3& vPos, float fLength, float fAngle, float fThickness, D3DCOLOR d3dColor);
	static void DrawLine(CDeviceMgr* pDeviceMgr, const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vEndPos, float fThickness, D3DCOLOR d3dColor);

public:
	static void AddTitleEffect(const wstring& wstrTextImgKey, float fSort = 0.f, const D3DXVECTOR3& vPos = D3DXVECTOR3(WINCX * 0.5f, 100.f, 0.f));
	static void DarkAmbienceRender();

};

