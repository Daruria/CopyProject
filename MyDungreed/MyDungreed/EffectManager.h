#pragma once

class CEffect;
class CEffectManager
{
public:
	CEffectManager();
	~CEffectManager();

	DECLARE_SINGLE(CEffectManager)

public:
	void Initialize();
	int Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	void FirstRender(HDC hDC);

private:
	list<CEffect*> m_EffectList;
	list<CEffect*> m_PretEffectList;

public:
	list<CEffect*>* GetEffectList() { return &m_EffectList; }

public:
	void AddEffectToPret(const string& strKey, const wchar_t* pPath, float fx, float fy, int icx, int icy, int iMaxFrame, DWORD dwLifeTime, float fRatio = 1.f, float fAngle = 0.f, float fSpeed = 0.f);
	void AddEffect(const string& strKey, const wchar_t* pPath, float fx, float fy, int icx, int icy, int iMaxFrame, float fRatio = 1.f, float fAngle = 0.f, float fSpeed = 0.f);
	void AddRotateEffect(const string& strKey, const wchar_t* pPath, float fx, float fy, int icx, int icy, int iMaxFrame, float fAngle, float fRatio);
	void AddFontEffect(float fStartX, float fStartY, float fEndX, float fEndY, float fSpeed, float fRatio, const string& strSentence, FONT_TYPE eFontType);
};

