#pragma once

class CObj;
class CPlayer;
class CObjectManager
{
public:
	CObjectManager();
	~CObjectManager();

	DECLARE_SINGLE(CObjectManager)

public:
	void Initialize();
	int Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

private:
	list<CObj*>* m_ObjListArr[OT_END];

public:
	void AddObj(OBJECT_TYPE eType, CObj* pObj);
	void AddMonster(CObj* pObj, float fx, float fy);

public:
	list<CObj*>* GetObjList(OBJECT_TYPE eType) { return m_ObjListArr[eType]; }
	CPlayer*	GetPlayer();
};

