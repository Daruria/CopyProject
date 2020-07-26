#include "stdafx.h"
#include "MainLogo.h"
#include "MyBitmap.h"
#include "MyButton.h"
#include "BGObject.h"
#include "ScrollManager.h"
#include "LogoObject.h"
#include "Mouse.h"

CMainLogo::CMainLogo()
{
}


CMainLogo::~CMainLogo()
{
	Release();
}

void CMainLogo::AddBird(float fx, float fy, float fSpeed)
{
	CLogoObject* pBird = new CLogoObject;
	pBird->Initialize();
	pBird->SetInfo(fx, fy, 48, 32);
	pBird->SetSpeed(fSpeed);
	pBird->SetKey("Bird");
	pBird->SetMaxFrame(8);
	m_pBirdList.push_back(pBird);
	m_iBirdCount++;
}

void CMainLogo::AddVBird(float fx, float fy, float fSpeed)
{
	float fIntervalX = 48;
	float fIntervalY = 32;
	AddBird(fx - (2 * fIntervalX), fy - (2 * fIntervalY), fSpeed);
	AddBird(fx - (fIntervalX), fy - (fIntervalY), fSpeed);
	AddBird(fx, fy, fSpeed);
	AddBird(fx - (fIntervalX), fy + (fIntervalY), fSpeed);
	AddBird(fx - (2 * fIntervalX), fy + (2 * fIntervalY), fSpeed);

}

void CMainLogo::AddCloud(float fx, float fy, float fSpeed)
{
	m_pCloud = new CLogoObject;
	m_pCloud->Initialize();
	m_pCloud->SetInfo(fx, fy, 180, 88);
	m_pCloud->SetSpeed(fSpeed);
	m_pCloud->SetMaxFrame(1);
	m_pCloud->SetKey("MidCloud");
}

void CMainLogo::BirdUpdate()
{
	float fScrollX = CScrollManager::GetScrollX();
	float fScrollY = CScrollManager::GetScrollY();

	if (!m_iBirdCount)
	{
		int iCase = rand() % 2;

		switch (iCase)
		{
		case 0:
			AddBird(-48, static_cast<float>(rand() % (WINCY - 160) + 80), 1.5f);
			break;
		case 1:
			AddVBird(-48, static_cast<float>(rand() % (WINCY - 160) + 80), 1.5f);
			break;
		}
	}
	else
	{
		list<CLogoObject*>::iterator iter;
		list<CLogoObject*>::iterator iterEnd = m_pBirdList.end();

		for (iter = m_pBirdList.begin(); iter != iterEnd;)
		{
			(*iter)->Update();
			INFO tInfo = (*iter)->GetInfo();
			if (fScrollX + WINCX / 2	< tInfo.fx - tInfo.icx)
			{
				SAFE_DELETE((*iter));
				iter = m_pBirdList.erase(iter);
				m_iBirdCount--;
			}
			else
				++iter;
		}
	}
}

void CMainLogo::Initialize()
{

	GET_SINGLE(CSoundMgr)->PlaySound(L"Title.wav", CSoundMgr::BGM);

	INSERT_BITMAP("MainLogoBG", L"Image/MainLogo/MainLogoBG.bmp");
	INSERT_BITMAP("MainLogoTitle", L"Image/MainLogo/MainLogoTitle(Big).bmp");
	INSERT_BITMAP("MidCloud", L"Image/MainLogo/MidCloud(Big).bmp");
	INSERT_BITMAP("Bird", L"Image/MainLogo/Bird/BirdSprite.bmp");

	m_pCloud = nullptr;
	m_iBirdCount = 0;

	CScrollManager::SetScrollX(WINCX / 2);
	CScrollManager::SetScrollY(WINCY / 2);
	CScrollManager::SetScrollSizeX(WINCX);
	CScrollManager::SetScrollSizeY(WINCY);

	GET_SINGLE(CMouse)->ChangeCursor(0);

	m_pCloud1 = new CBGObject;
	m_pCloud1->SetInfo(static_cast<float>(0), static_cast<float>(0));
	m_pCloud1->SetInterval(0);
	m_pCloud1->SetImage("BGCloud1", L"Image/MainLogo/BGCloud1.bmp");

	m_pCloud2 = new CBGObject;
	m_pCloud2->SetInfo(static_cast<float>(0), static_cast<float>(0));
	m_pCloud2->SetInterval(300);
	m_pCloud2->SetImage("BGCloud2", L"Image/MainLogo/BGCloud2.bmp");

	m_pPlayButton = new CMyButton;
	m_pPlayButton->Initialize("PlayButton", L"Image/MainLogo/PlayOff(1024x768).bmp", "PlayButtonOn", L"Image/MainLogo/PlayOn(1024x768).bmp");
	m_pPlayButton->SetPoint(WINCX / 2, WINCY - 250);

	m_pOptionButton = new CMyButton;
	m_pOptionButton->Initialize("OptionButton", L"Image/MainLogo/OptionOff(1024x768).bmp", "OptionButtonOn", L"Image/MainLogo/OptionOn(1024x768).bmp");
	m_pOptionButton->SetPoint(WINCX / 2, WINCY - 200);

	m_pExitButton = new CMyButton;
	m_pExitButton->Initialize("ExitButton", L"Image/MainLogo/ExitOff(1024x768).bmp", "ExitButtonOn", L"Image/MainLogo/ExitOn(1024x768).bmp");
	m_pExitButton->SetPoint(WINCX / 2, WINCY - 150);

#ifdef _DEBUG
	m_pEditButton = new CMyButton;
	m_pEditButton->Initialize("EditButton", L"Image/MainLogo/EditOff(1024x768).bmp", "EditButtonOn", L"Image/MainLogo/EditOn(1024x768).bmp");
	m_pEditButton->SetPoint(WINCX / 2, WINCY - 100);
#endif // _DEBUG

}

int CMainLogo::Update()
{
	float fScrollX = CScrollManager::GetScrollX();
	float fScreenLeft = CScrollManager::GetScreenLeft();
	float fScreenRight = CScrollManager::GetScreenRight();

	float fScrollY = CScrollManager::GetScrollY();
	float fScreenTop = CScrollManager::GetScreenTop();
	float fScreenBottom = CScrollManager::GetScreenBottom();

	INFO tInfo = m_pCloud1->GetInfo();
	float fx = tInfo.fx - 0.5f;
	if (fScreenRight >= fx + tInfo.icx / 2 + m_pCloud1->GetInterval())
		fx = fx + m_pCloud1->GetInterval() + tInfo.icx;
	m_pCloud1->SetInfo(fx, tInfo.fy);
	
	tInfo = m_pCloud2->GetInfo();
	fx = tInfo.fx - 1.f;
	if (fScreenRight >= fx + tInfo.icx / 2 + m_pCloud2->GetInterval())
		fx = fx + m_pCloud2->GetInterval() + tInfo.icx;
	m_pCloud2->SetInfo(fx, tInfo.fy);


	if (m_pPlayButton->Update())
		return SC_TOWN;
	if (m_pOptionButton->Update());
	if (m_pExitButton->Update())
		return SC_END;
#ifdef _DEBUG
	if (m_pEditButton->Update())
		return SC_EDIT;
#endif // _DEBUG


	BirdUpdate();

	if (!m_pCloud)
		AddCloud(WINCX + 180.f, static_cast<float>(rand() % (WINCY - 176) + 88), -1.5f);
	else
	{
		m_pCloud->Update();
		INFO tInfo = m_pCloud->GetInfo();
		if (fScrollX - WINCX / 2 > tInfo.fx + tInfo.icx)	SAFE_DELETE(m_pCloud);
	}

	return 0;
}

void CMainLogo::LateUpdate()
{
	m_pPlayButton->LateUpdate();
	m_pOptionButton->LateUpdate();
	m_pExitButton->LateUpdate();
#ifdef _DEBUG
	m_pEditButton->LateUpdate();
#endif // _DEBUG

}

void CMainLogo::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("MainLogoBG");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	m_pCloud1->Render(hDC);
	
	if(m_pCloud)
		m_pCloud->Render(hDC);
	
	for (auto pBird : m_pBirdList)
		pBird->Render(hDC);

	m_pCloud2->Render(hDC);

	hMemDC = FIND_BITMAP_DC("MainLogoTitle");
	SIZE tSize = GET_BITMAP_SIZE("MainLogoTitle");
	GdiTransparentBlt(hDC, WINCX/2 - tSize.cx / 2, WINCY/2 - tSize.cy / 2 - 100, tSize.cx, tSize.cy, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));

	m_pPlayButton->Render(hDC);
	m_pOptionButton->Render(hDC);
	m_pExitButton->Render(hDC);

#ifdef _DEBUG
	m_pEditButton->Render(hDC);
#endif // _DEBUG

}

void CMainLogo::Release()
{
	GET_SINGLE(CSoundMgr)->StopAll();

	SAFE_DELETE(m_pCloud);

	for (auto pObj : m_pBirdList)
		SAFE_DELETE(pObj);

	SAFE_DELETE(m_pCloud1);
	SAFE_DELETE(m_pCloud2);

	SAFE_DELETE(m_pPlayButton);
	SAFE_DELETE(m_pOptionButton);
	SAFE_DELETE(m_pExitButton);

#ifdef _DEBUG
	SAFE_DELETE(m_pEditButton);
#endif // _DEBUG
}
