#pragma once
class CThreadMgr
{
	DECLARE_SINGLETON(CThreadMgr)

private:
	CThreadMgr();
	~CThreadMgr();

public:
	bool LoadingThread(const wstring& strPath);
	bool AstarThread(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos);
	void WaitForThread();
	void WaitForAstarThread();
	const wstring& GetPath() const { return m_wstrPath; }
	const bool GetAstarThreading() const { return m_bAstarThreading; }

public:
	void ClaerPath() { m_wstrPath.clear(); }

private:
	static unsigned __stdcall LoadingThreadFunc(void* pParam);
	static unsigned __stdcall AstarThreadFunc(void* pParam);

private:
	float		m_fLoadingRatio;
	HANDLE		m_hLoadingThread;
	HANDLE		m_hAstarThread;
	bool		m_bAstarThreading;
	wstring		m_wstrPath;
	D3DXVECTOR3 m_vStartPos;
	D3DXVECTOR3 m_vGoalPos;
};

