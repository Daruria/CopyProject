#pragma once

class CScene;
class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();

	DECLARE_SINGLE(CSceneManager)

private:
	CScene* m_pScene;

public:
	void Initialize();
	int Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	CScene* GetScene() { return m_pScene; }
};

