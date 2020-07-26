#pragma once
class CScene
{
protected:
	CScene();

public:
	virtual ~CScene();

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

protected:
	void LoadMap(const TCHAR* pFullPath);

protected:
	CObjectMgr*		m_pObjectMgr;
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CTimeMgr*		m_pTimeMgr;
	CSceneMgr*		m_pSceneMgr;
	CKeyMgr*		m_pKeyMgr;
};

