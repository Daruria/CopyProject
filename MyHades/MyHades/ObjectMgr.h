#pragma once
class CGameObject;
class CPlayer;
class CTerrain;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	CObjectMgr();
	~CObjectMgr();

public:
	HRESULT AddObject(OBJECT_TYPE eType, CGameObject* pObject);
	HRESULT AddLayer(RENDER_TYPE eType, CGameObject* pObject);

public:
	void RenderListClear();
	void ObjectListClear(OBJECT_TYPE eType);

public:
	int Update();
	void LateUpdate();
	void Render();

private:
	void Release();

public:
	void SaveData(HANDLE hFile, const TCHAR* pFilePath);
	void LoadData(HANDLE hFile, const TCHAR* pFilePath);
	
public:
	CPlayer* GetPlayer();
	CTerrain* GetTerrain();
	list<CGameObject*>* GetObjectList(OBJECT_TYPE eObjType) { return &m_ObjectList[eObjType]; }

private:
	list<CGameObject*> m_ObjectList[OBJECT_END];
	list<CGameObject*> m_RenderList[RENDER_END];
};

