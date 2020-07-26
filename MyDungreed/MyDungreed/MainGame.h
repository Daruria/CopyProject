#pragma once
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

	DECLARE_SINGLE(CMainGame)

private:
	int		m_iFrame;
	int		m_iMaxFrame;
	DWORD	m_dwFrameTime;

public:
	const int GetFrame() const { return m_iMaxFrame; }

public:
	void Initialize();
	int Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();
};

