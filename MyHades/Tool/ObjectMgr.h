#pragma once
class CGameObject;
class CToolView;
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

public:
	int Update();
	void LateUpdate();
	void Render();

private:
	void Release();

public:
	void SaveData(HANDLE hFile, const TCHAR* pFilePath, CToolView* pView = nullptr);
	void LoadData(HANDLE hFile, const TCHAR* pFilePath, CToolView* pView = nullptr);

public:
	list<CGameObject*>* GetObjectList(OBJECT_TYPE eType) { return &m_ObjectList[eType]; }

private:
	list<CGameObject*> m_ObjectList[OBJECT_END];
	list<CGameObject*> m_RenderList[RENDER_END];
};

