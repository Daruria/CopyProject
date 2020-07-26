#pragma once

class CObj;
class CTile;
class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

public:
	static void CollisionObjectToTile(list<CObj*>* pSrcList, vector<CTile*>* pDestVector);

	static void CollisionPlayerToMonster(list<CObj*>* pSrcList, list<CObj*>* pDestList);
	static void CollisionPlayerToItem(list<CObj*>* pSrcList, list<CObj*>* pDestList);
	static void CollisionPlayerToTresureBox(list<CObj*>* pSrcList, list<CObj*>* pDestList);
	static void CollisionPlayerToFieldObject(list<CObj*>* pSrcList, list<CObj*>* pDestList);
	static void CollisionPlayerToNPC(list<CObj*>* pSrcList, list<CObj*>* pDestList);

	static void CollisionBulletToPlayer(list<CObj*>* pSrcList, list<CObj*>* pDestList);
	static void CollisionBulletToMonster(list<CObj*>* pSrcList, list<CObj*>* pDestList);
	static void CollisionBulletToTile(list<CObj*>* pSrcList, vector<CTile*>* pDestVector);

public:
	static bool CollisionLineToLine(const LINE& lSrc, const LINE& lDest);
	static bool CollisionLineToRect(LINE lSrc, RECT rcDest);
	static bool CollisionLineToRect(RECT rcSrc, RECT rcDest);

	static bool CollisionLineToSphere(LINE lSrc, SPHERE spDest);

	static bool CollisionLineToRDTriangle(LINE lSrc, RECT rcDest);
	static bool CollisionLineToLDTriangle(LINE lSrc, RECT rcDest);
	static bool CollisionLineToRUTriangle(LINE lSrc, RECT rcDest);
	static bool CollisionLineToLUTriangle(LINE lSrc, RECT rcDest);

	static bool CollisionRectToRDTriangle(const RECT& rcSrc, const RECT& rcDest);
	static bool CollisionRectToLDTriangle(const RECT& rcSrc, const RECT& rcDest);
	static bool CollisionRectToRect(CObj* pSrc, CObj* pDest);
	static bool CollisionRectToRect(CObj* pSrc, CTile* pDest);
	static bool CollisionRectToRect(const INFO& tInfoSrc, CTile*pDest);
	static bool CollisionRectToRect(const INFO& tInfoSrc, const INFO& tInfoDest);
	static bool CollisionRectToRect(const RECT& rcSrc, const RECT& rcDest);
	static bool CollisionRectToRect(const RECT& rcSrc, CTile* pDest);
	static bool CollisionRectToSphere(CObj* pSrc, CObj* pDest);
	static bool CollisionRectToSphere(RECT rcSrc, SPHERE spDest);
	
	static bool	CollisionSphereToSphere(SPHERE spSrc, SPHERE spDest);
	static bool CollisionSphereToRDTriangle(const SPHERE& spSrc, const RECT& rcDest);
	static bool CollisionSphereToLDTriangle(const SPHERE& spSrc, const RECT& rcDest);

};

