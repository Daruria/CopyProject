#pragma once

#include "Terrain.h"

class CGameObject;
class CMoveObject;
class CCollisionMgr
{
private:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void CollisionObjectToTerrain(list<CGameObject*>* pObjList, vector<TILE_INFO*>* pvecTiles);
	static void CollisionObjectToTerrain(list<CGameObject*>* pObjList, list<CGameObject*>* pTerrainList);
	static void CollisionHitBoxes(list<CGameObject*>* pHitBoxList, list<CGameObject*>* pObjList);
	
public:
	static bool CollisionRectToRect(const RECT& rcSrc, const RECT& rcDest);
	static bool	CollisionSphereToSphere(const SPHERE& spSrc, const SPHERE& spDest);
	static bool CollisionObjectToTile(const CGameObject* Obj, const TILE_INFO* Tile);
	static bool CollisionObjectToTile(const D3DXVECTOR3& vObjPos, const D3DXVECTOR3& vTilePos);

	static bool CollisionLineToLine(const LINE& lSrc, const LINE& lDest);
	static bool CollisionLineToRect(LINE lSrc, RECT rcDest);

};