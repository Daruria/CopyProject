#pragma once
class CMainGame
{
private:
	CMainGame();

public:
	~CMainGame();

public:
	void Update();
	void LateUpdate();
	void Render();

private:
	HRESULT Initialize();
	void Release();

public:
	static CMainGame* Create();

private:
	int	m_iEvent;

private:
	CKeyMgr*			m_pKeyMgr;
	CDeviceMgr*			m_pDeviceMgr;
	CTextureMgr*		m_pTextureMgr;
	CObjectMgr*			m_pObjectMgr;
	CTimeMgr*			m_pTimeMgr;
	CSceneMgr*			m_pSceneMgr;
	CFrameMgr*			m_pFrameMgr;
	class CMouse*		m_pMouse;
	CFontMgr*			m_pFontMgr;
	CSubjectMgr*		m_pSubjectMgr;
	CSoundMgr*			m_pSoundMgr;
};

