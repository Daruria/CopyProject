#include "stdafx.h"
#include "EffectManager.h"
#include "NormalEffect.h"
#include "RotateEffect.h"
#include "Effect.h"
#include "FontEffect.h"

DEFINITION_SINGLE(CEffectManager);

CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{
	Release();
}

void CEffectManager::Initialize()
{

}

int CEffectManager::Update()
{
	for (auto& pEffect : m_PretEffectList)
		pEffect->Update();

	for (auto& pEffect : m_EffectList)
		pEffect->Update();
	
	return 0;
}

void CEffectManager::LateUpdate()
{
	list<CEffect*>::iterator iter;

	for (iter = m_PretEffectList.begin(); iter != m_PretEffectList.end(); )
	{
		if ((*iter)->IsDie())
		{
			SAFE_DELETE((*iter));
			iter = m_PretEffectList.erase(iter);
		}
		else
			++iter;
	}

	for (iter = m_EffectList.begin(); iter != m_EffectList.end(); )
	{
		if ((*iter)->IsDie())
		{
			SAFE_DELETE((*iter));
			iter = m_EffectList.erase(iter);
		}
		else
			++iter;
	}
}

void CEffectManager::Render(HDC hDC)
{
	for (auto& pEffect : m_EffectList)
		pEffect->Render(hDC);
	
}

void CEffectManager::Release()
{
	for (auto& pEffect : m_EffectList)
		SAFE_DELETE(pEffect);
	
	for (auto& pEffect : m_PretEffectList)
		SAFE_DELETE(pEffect);
}

void CEffectManager::FirstRender(HDC hDC)
{
	for (auto& pEffect : m_PretEffectList)
		pEffect->Render(hDC);
}

void CEffectManager::AddEffectToPret(const string & strKey, const wchar_t * pPath, float fx, float fy, int icx, int icy, int iMaxFrame, DWORD dwLifeTime, float fRatio, float fAngle, float fSpeed)
{
	CNormalEffect* pEffect = new CNormalEffect;
	pEffect->Initialize();
	pEffect->SetKey(strKey);
	INSERT_BITMAP(strKey, pPath);
	pEffect->SetInfo(fx, fy, icx, icy);
	pEffect->SetMaxFrame(iMaxFrame);
	pEffect->SetFrameMaxTime(dwLifeTime);
	pEffect->SetRatio(fRatio);
	pEffect->SetAngle(fAngle);
	pEffect->SetSpeed(fSpeed);
	m_PretEffectList.push_back(pEffect);
}

void CEffectManager::AddEffect(const string & strKey, const wchar_t * pPath, float fx, float fy, int icx, int icy, int iMaxFrame, float fRatio, float fAngle, float fSpeed)
{
	CNormalEffect* pEffect = new CNormalEffect;
	pEffect->Initialize();
	pEffect->SetKey(strKey);
	INSERT_BITMAP(strKey, pPath);
	pEffect->SetInfo(fx, fy, icx, icy);
	pEffect->SetMaxFrame(iMaxFrame);
	pEffect->SetRatio(fRatio);
	pEffect->SetAngle(fAngle);
	pEffect->SetSpeed(fSpeed);
	m_EffectList.push_back(pEffect);
}

void CEffectManager::AddRotateEffect(const string & strKey, const wchar_t * pPath, float fx, float fy, int icx, int icy, int iMaxFrame, float fAngle, float fRatio)
{
	CRotateEffect* pEffect = new CRotateEffect;
	pEffect->Initialize();
	pEffect->SetKey(strKey);
	for (int i = 0; i < iMaxFrame; ++i)
	{
		string FrameKey = strKey + to_string(i);
		wstring wstrPath = pPath + to_wstring(i) + L".bmp";
		INSERT_BITMAP(FrameKey, wstrPath.c_str());
		pEffect->AddFrameKey(FrameKey);
	}
	pEffect->SetInfo(fx, fy, icx, icy);
	pEffect->SetMaxFrame(iMaxFrame);
	pEffect->SetAngle(fAngle);
	pEffect->SetRatio(fRatio);
	m_EffectList.push_back(pEffect);
}

void CEffectManager::AddFontEffect(float fStartX, float fStartY, float fEndX, float fEndY, float fSpeed, float fRatio, const string& strSentence, FONT_TYPE eFontType)
{
	CFontEffect* pFontEffect = new CFontEffect;
	pFontEffect->Initialize();
	//pFontEffect->SetImage(strKey, pPath);
	pFontEffect->SetInfo(fStartX, fStartY);
	pFontEffect->MoveDirSetting(POSITION{ fStartX, fStartY }, POSITION{ fEndX, fEndY });
	pFontEffect->SetRatio(fRatio);
	pFontEffect->SetSpeed(fSpeed);
	pFontEffect->SetSentence(strSentence);
	pFontEffect->SetFontType(eFontType);
	m_EffectList.push_back(pFontEffect);
}
